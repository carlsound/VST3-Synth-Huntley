
#include "../include/noteExpressionSynthProcessor.h"
#include "../include/noteExpressionSynthIDs.h"
#include "../include/noteExpressionSynthVoice.h"

#include "public.sdk/samples/vst/common/voiceprocessor.h"
#include "pluginterfaces/base/ustring.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include "pluginterfaces/vst/ivstparameterchanges.h"
#include <public.sdk/source/vst/vstaudioprocessoralgo.h>
#include "pluginterfaces/vst/vsttypes.h"

#include <algorithm>

namespace Carlsound {
namespace Huntley {

//-----------------------------------------------------------------------------
Processor::Processor () : voiceProcessor(0)
{
	// register its editor class
	setControllerClass (MyControllerUID);

	memset(&paramState, 0, sizeof(paramState));

	paramState.masterVolume = 0.5;
	paramState.masterTuning = 0;
	paramState.velToLevel = 1.;

	paramState.noiseVolume = 0.1;
	paramState.sinusVolume = 1.;
	paramState.triangleVolume = 1.;
	paramState.releaseTime = 0.;
	paramState.sinusDetune = 0;
	paramState.triangleSlop = 0.5;
	paramState.filterFreq = 1.;
	paramState.filterQ = 0.;
	paramState.filterType = 0;
	paramState.freqModDepth = 1.;
	paramState.bypassSNA = 0;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::initialize (FUnknown* context)
{
	//---always initialize the parent-------
	Steinberg::tresult result = AudioEffect::initialize (context);
	if (result != Steinberg::kResultTrue)
		return Steinberg::kResultFalse;

	//---create Audio Out bus------
	addAudioOutput (STR16 ("AudioOutput"), Steinberg::Vst::SpeakerArr::kStereo);
	//
	addEventInput(STR16("Event Input"), 1);
	//
	/*
	m_oscillator[0] = std::make_shared<maxiOsc>();
	m_oscillator[1] = std::make_shared<maxiOsc>();
	//
	m_oscillatorSettings = std::make_shared<maxiSettings>();
	//
	m_speedRangeParameter = std::make_shared<Steinberg::Vst::RangeParameter>(STR16("Speed"), // title
		HuntleyParams::kParamSpeedId, // ParamID
		STR16("sec"), // units
		0.1, // minPlain
		10.0, // maxPlain
		1.0, // defaultValuePlain
		99, // stepCount
		Steinberg::Vst::ParameterInfo::kCanAutomate, // flags
		0, // unitID
		STR16("Speed")); // shortTitle
	*/
	return Steinberg::kResultTrue;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::setBusArrangements (Steinberg::Vst::SpeakerArrangement* inputs,
                                                            Steinberg::int32 numIns,
                                                            Steinberg::Vst::SpeakerArrangement* outputs,
                                                            Steinberg::int32 numOuts)
{
	// we only support one stereo output bus
	if (numIns == 0 && numOuts == 1 && outputs[0] == Steinberg::Vst::SpeakerArr::kStereo)
	{
		return AudioEffect::setBusArrangements(inputs, numIns, outputs, numOuts);
	}
	return Steinberg::kResultFalse;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::setState(Steinberg::IBStream* state)
{
	return paramState.setState(state);
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::getState(Steinberg::IBStream* state)
{
	return paramState.getState(state);
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::canProcessSampleSize(Steinberg::int32 symbolicSampleSize)
{
	if (symbolicSampleSize == Steinberg::Vst::kSample32 || symbolicSampleSize == Steinberg::Vst::kSample64)
	{
		return Steinberg::kResultTrue;
	}
	return Steinberg::kResultFalse;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::setActive (Steinberg::TBool state)
{
	if (state) // Initialize
	{
		// Allocate Memory Here
		// Ex: algo.create ();
		if (paramState.noiseBuffer == 0)
			paramState.noiseBuffer = new BrownNoise<float>((Steinberg::int32)processSetup.sampleRate, 
			                                               (float)processSetup.sampleRate);
		if (voiceProcessor == 0)
		{
			if (processSetup.symbolicSampleSize == Steinberg::Vst::kSample32)
			{
				voiceProcessor = new Steinberg::Vst::VoiceProcessorImplementation<float,
				                                                                  Voice<float>, 
				                                                                  2,
				                                                                  MAX_VOICES,
				                                                                  GlobalParameterState>((float)processSetup.sampleRate,
																				  &paramState);
			}
			else if (processSetup.symbolicSampleSize == Steinberg::Vst::kSample64)
			{
				voiceProcessor = new Steinberg::Vst::VoiceProcessorImplementation<double, 
				                                                                  Voice<double>, 
				                                                                  2,
				                                                                  MAX_VOICES, 
				                                                                  GlobalParameterState>((float)processSetup.sampleRate, 
																				  &paramState);
			}
			else
			{
				return Steinberg::kInvalidArgument;
			}
		}
	}
	else // Release
	{
		// Free Memory if still allocated
		// Ex: if(algo.isCreated ()) { algo.destroy (); }
		if (voiceProcessor)
		{
			delete voiceProcessor;
		}
		voiceProcessor = 0;
		if (paramState.noiseBuffer)
		{
			delete paramState.noiseBuffer;
		}
		paramState.noiseBuffer = 0;
	}
	return AudioEffect::setActive (state);
}

//-----------------------------------------------------------------------------
/*
template<class T>
inline void Carlsound::Huntley::Processor::bufferSampleGain(T inBuffer, T outBuffer, const int sampleLocation, const double gainValue)
{
	inBuffer = inBuffer + sampleLocation;  // pointer arithmetic
	outBuffer = outBuffer + sampleLocation; // pointer arithmetic
	//
	*outBuffer = *inBuffer * gainValue;
}
*/

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Processor::process(Steinberg::Vst::ProcessData& data)
{
	//--- Read inputs parameter changes-----------
	if (data.inputParameterChanges)
	{
		Steinberg::int32 count = data.inputParameterChanges->getParameterCount();
		for (Steinberg::int32 i = 0; i < count; i++)
		{
			Steinberg::Vst::IParamValueQueue* queue = data.inputParameterChanges->getParameterData(i);
			if (queue)
			{
				Steinberg::int32 sampleOffset;
				Steinberg::Vst::ParamValue value;
				Steinberg::Vst::ParamID pid = queue->getParameterId();
				if (queue->getPoint(queue->getPointCount() - 1, sampleOffset, value) == Steinberg::kResultTrue)
				{
					switch (pid)
					{
					case kParamMasterVolume:
					{
						paramState.masterVolume = value;
						break;
					}
					case kParamMasterTuning:
					{
						paramState.masterTuning = 2 * (value - 0.5);
						break;
					}
					case kParamVelToLevel:
					{
						paramState.velToLevel = value;
						break;
					}
					case kParamFilterFreqModDepth:
					{
						paramState.freqModDepth = 2 * (value - 0.5);
						break;
					}

					case kParamNoiseVolume:
					{
						paramState.noiseVolume = value;
						break;
					}
					case kParamSinusVolume:
					{
						paramState.sinusVolume = value;
						break;
					}
					case kParamTriangleVolume:
					{
						paramState.triangleVolume = value;
						break;
					}
					case kParamSquareVolume:
					{
						paramState.squareVolume = value;
						break;
					}
					case kParamReleaseTime:
					{
						paramState.releaseTime = value;
						break;
					}
					case kParamSinusDetune:
					{
						paramState.sinusDetune = 2 * (value - 0.5);
						break;
					}

					case kParamTriangleSlop:
					{
						paramState.triangleSlop = value;
						break;
					}
					case kParamFilterType:
					{
						paramState.filterType = std::min<Steinberg::int8>((Steinberg::int8)(NUM_FILTER_TYPE * value), NUM_FILTER_TYPE - 1);
						break;
					}
					case kParamFilterFreq:
					{
						paramState.filterFreq = value;
						break;
					}
					case kParamFilterQ:
					{
						paramState.filterQ = value;
						break;
					}
					case kParamBypassSNA:
					{
						paramState.bypassSNA = (value >= 0.5) ? 1 : 0;
						break;
					}
					case kParamTuningRange:
					{
						paramState.tuningRange = std::min<Steinberg::int8>((Steinberg::int8)(NUM_TUNING_RANGE * value), NUM_TUNING_RANGE - 1);
						break;
					}

					}
				}
			}
		}
	}

	Steinberg::tresult result;

	// flush mode
	if (data.numOutputs < 1)
		result = Steinberg::kResultTrue;
	else
		result = voiceProcessor->process(data);
	if (result == Steinberg::kResultTrue)
	{
		if (data.outputParameterChanges)
		{
			Steinberg::int32 index;
			Steinberg::Vst::IParamValueQueue* queue = data.outputParameterChanges->addParameterData(kParamActiveVoices, index);
			if (queue)
			{
				queue->addPoint(0, (Steinberg::Vst::ParamValue)voiceProcessor->getActiveVoices() / (Steinberg::Vst::ParamValue)MAX_VOICES, index);
			}
		}
		if (voiceProcessor->getActiveVoices() == 0 && data.numOutputs > 0)
		{
			data.outputs[0].silenceFlags = 0x11; // left and right channel are silent
		}
	}
	return result;
}

//------------------------------------------------------------------------
} // namespace Huntley
} // namespace Carlsound
