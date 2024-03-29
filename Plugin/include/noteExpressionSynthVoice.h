#pragma once

#include "public.sdk/samples/vst/common/voicebase.h"
#include "public.sdk/samples/vst/common/logscale.h"
#include "brownnoise.h"
#include "filter.h"
#include "noteExpressionSynthController.h"
#include "pluginterfaces/vst/ivstevents.h"
#include "pluginterfaces/base/futils.h"
#include <cmath>
#include <algorithm>

#ifndef M_PI
#define M_PI			3.14159265358979323846
#endif
#ifndef M_PI_MUL_2
#define M_PI_MUL_2		6.28318530717958647692
#endif

namespace Carlsound {
namespace Huntley {

#define MAX_VOICES				64
#define MAX_RELEASE_TIME_SEC	5.0
#define NUM_FILTER_TYPE			3
#define NUM_TUNING_RANGE		2 

//-----------------------------------------------------------------------------
struct GlobalParameterState
{
	BrownNoise<float>* noiseBuffer;

	Steinberg::Vst::ParamValue masterVolume;	// [0, +1]
	Steinberg::Vst::ParamValue masterTuning;	// [-1, +1]
	Steinberg::Vst::ParamValue velToLevel;		// [0, +1]

	Steinberg::Vst::ParamValue noiseVolume;		// [0, +1]
	Steinberg::Vst::ParamValue sinusVolume;		// [0, +1]
	Steinberg::Vst::ParamValue triangleVolume;	// [0, +1]
	Steinberg::Vst::ParamValue squareVolume;	// [0, +1]

	Steinberg::Vst::ParamValue releaseTime;		// [0, +1]

	Steinberg::Vst::ParamValue sinusDetune;		// [-1, +1]
	Steinberg::Vst::ParamValue triangleSlop;	// [0, +1]

	Steinberg::Vst::ParamValue filterFreq;		// [-1, +1]
	Steinberg::Vst::ParamValue filterQ;			// [-1, +1]
	Steinberg::Vst::ParamValue freqModDepth;	// [-1, +1]

	Steinberg::int8 filterType;			// [0, 1, 2]
	Steinberg::int8 tuningRange;			// [0, 1]

	Steinberg::int8 bypassSNA;				// [0, 1]

	Steinberg::tresult setState (Steinberg::IBStream* stream);
	Steinberg::tresult getState (Steinberg::IBStream* stream);
};

//-----------------------------------------------------------------------------
enum VoiceParameters
{
	kVolumeMod,
	kTuningMod,
	kPanningLeft,
	kPanningRight,
	kNoiseVolume,
	kFilterFrequencyMod,
	kFilterQMod,
	kSinusVolume,
	kTriangleVolume,
	kFilterType,
	kTriangleSlope,
	kSinusDetune,
	kReleaseTimeMod,
	kSquareVolume,

	kNumParameters
};

//-----------------------------------------------------------------------------
class VoiceStatics
{
public:
	//------------------------------------------------------------------------
	static double normalizedLevel2Gain (float normalized)
	{
		double level;
		if (normalized >= 0.5)
			level = scaleHeadRoom * ::pow (10, (normalized - 0.5f) * 24 / 20.0f);
		else
			level = scaleNorm2GainC1 * ::pow (normalized, scaleNorm2GainC2);	

		return level;
	}	

	enum {
		kNumFrequencies = 128
	};

	static float freqTab[kNumFrequencies];
	static const float scaleHeadRoom;
	static const float scaleNorm2GainC1;
	static const float scaleNorm2GainC2;
	static Steinberg::Vst::LogScale<Steinberg::Vst::ParamValue> freqLogScale;
	static const double kNormTuningOneOctave;
	static const double kNormTuningOneTune;

};

//-----------------------------------------------------------------------------
/** Example Note Expression Synth Voice Class

\sa Steinberg::Vst::VoiceBase
*/
template<class SamplePrecision>
class Voice : public Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>
{
public:
	Voice ();
	~Voice ();

	void setSampleRate (Steinberg::Vst::ParamValue sampleRate) SMTG_OVERRIDE;
	void noteOn (Steinberg::int32 pitch, Steinberg::Vst::ParamValue velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 nId) SMTG_OVERRIDE;
	void noteOff (Steinberg::Vst::ParamValue velocity, Steinberg::int32 sampleOffset) SMTG_OVERRIDE;
	bool process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples);
	void reset () SMTG_OVERRIDE;

	void setNoteExpressionValue (Steinberg::int32 index, Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;

protected:
	Steinberg::uint32 n;
	Steinberg::int32 noisePos;
	Steinberg::int32 noiseStep;

	Filter* filter;

	SamplePrecision trianglePhase;
	SamplePrecision sinusPhase;
	Steinberg::Vst::ParamValue currentTriangleF;
	Steinberg::Vst::ParamValue currentSinusF;
	Steinberg::Vst::ParamValue currentVolume;
	Steinberg::Vst::ParamValue currentPanningLeft;
	Steinberg::Vst::ParamValue currentPanningRight;
	Steinberg::Vst::ParamValue currentNoiseVolume;
	Steinberg::Vst::ParamValue currentSinusVolume;
	Steinberg::Vst::ParamValue currentSinusDetune;
	Steinberg::Vst::ParamValue currentSquareVolume;
	Steinberg::Vst::ParamValue currentTriangleVolume;
	Steinberg::Vst::ParamValue currentTriangleSlope;
	Steinberg::Vst::ParamValue currentLPFreq;
	Steinberg::Vst::ParamValue currentLPQ;

	Steinberg::Vst::ParamValue levelFromVel;
	Steinberg::Vst::ParamValue noteOffVolumeRamp;
};

//-----------------------------------------------------------------------------
template<class SamplePrecision>
void Voice<SamplePrecision>::setNoteExpressionValue (Steinberg::int32 index, Steinberg::Vst::ParamValue value)
{
	if (this->globalParameters->bypassSNA)
		return;

	switch (index)
	{
		//------------------------------
		case Steinberg::Vst::kVolumeTypeID:
		{
			Steinberg::Vst::ParamValue vol = VoiceStatics::normalizedLevel2Gain ((float)value);
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kVolumeMod, vol);
			break;
		}
		//------------------------------
		case Steinberg::Vst::kTuningTypeID:
		{
			if (value == 0.5)
			{
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kTuningMod, 0);
			}
			else if (value > 0.5)
			{
				if (this->globalParameters->tuningRange > 0.5)
					value = std::min<Steinberg::Vst::ParamValue> (0.5 + 2 * VoiceStatics::kNormTuningOneTune, value);
				else
					value = std::min<Steinberg::Vst::ParamValue> (0.5 + VoiceStatics::kNormTuningOneOctave, value);
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kTuningMod, (value - 0.5) * 2.);
			}
			else
			{
				if (this->globalParameters->tuningRange > 0.5)
					value = std::max<Steinberg::Vst::ParamValue> (0.5 - 3 * VoiceStatics::kNormTuningOneTune, value);
				else
					value = std::max<Steinberg::Vst::ParamValue> (0.5 - VoiceStatics::kNormTuningOneOctave, value);
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kTuningMod, (value - 0.5) * 2.);
			}
			break;
		}
		//------------------------------
		case Steinberg::Vst::kPanTypeID:
		{
			if (value == 0.5)
			{
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kPanningLeft, 1.);
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kPanningRight, 1.);
			}
			else if (value > 0.5)
			{
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kPanningLeft, 1.);
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kPanningRight, 1. + ((0.5 - value) * 2.));
			}
			else
			{
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kPanningLeft, value * 2.);
				Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kPanningRight, 1.);
			}
			break;
		}
		//------------------------------
        case kNoiseVolume:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kNoiseVolume, value * 2.);
			break;
		}
		//------------------------------
		case kSinusVolume:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kSinusVolume, value * 2.);
			break;
		}
		//------------------------------
		case kTriangleVolume:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kTriangleVolume, value * 2.);
			break;
		}
		//------------------------------
        case kSquareVolume:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kSquareVolume, value * 2.);
			break;
		}
		//------------------------------
		case kFilterFrequencyMod:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kFilterFrequencyMod, (value - 0.5) * 2.);
			break;
		}
		//------------------------------
		case kFilterQMod:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kFilterQMod, (value - 0.5) * 2.);
			break;
		}
		//------------------------------
		case kFilterType:
		{
			filter->setType ((Filter::Type)std::min<Steinberg::int32> ((Steinberg::int32)(NUM_FILTER_TYPE * value), NUM_FILTER_TYPE - 1));
			break;
		}
		//------------------------------
		case kTriangleSlope:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kTriangleSlope, value);
			break;
		}
		//------------------------------
		case kSinusDetune:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kSinusDetune, value);
			break;
		}
		//------------------------------
		case kReleaseTimeMod:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (kReleaseTimeMod, 2 * (value - 0.5));
			break;
		}
		//------------------------------
		default:
		{
			Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setNoteExpressionValue (index, value);
			break;
		}
	}
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
bool Voice<SamplePrecision>::process (SamplePrecision* outputBuffers[2], Steinberg::int32 numSamples)
{
	//---compute tuning-------------------------

	// main tuning
	Steinberg::Vst::ParamValue tuningInHz = 0.;
	if (this->values[kTuningMod] != 0. || this->globalParameters->masterTuning != 0 || this->tuning != 0)
	{
		tuningInHz = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, (this->values[kTuningMod] * 10 + this->globalParameters->masterTuning * 2.0 / 12.0 + this->tuning)) - 1);
	}
	
	Steinberg::Vst::ParamValue triangleFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz) * M_PI_MUL_2 / this->getSampleRate () / 2.;
	if (currentTriangleF == -1)
		currentTriangleF = triangleFreq;
	// check for frequency changes and update the phase so that it is crackle free
	if (triangleFreq != currentTriangleF)
	{
		// update phase
		trianglePhase = (SamplePrecision)((currentTriangleF - triangleFreq) * n + trianglePhase);
		currentTriangleF = triangleFreq;
	}

	// Sinus Detune
	if (currentSinusDetune != this->values[kSinusDetune])
	{
		currentSinusDetune = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, this->values[kSinusDetune] * 2.0 / 12.0) - 1);
	}
	Steinberg::Vst::ParamValue sinusFreq = (VoiceStatics::freqTab[this->pitch] + tuningInHz + currentSinusDetune) * M_PI_MUL_2 / this->getSampleRate ();
	if (currentSinusF == -1)
		currentSinusF = sinusFreq;
	if (sinusFreq != currentSinusF)
	{
		// update phase
		sinusPhase = (SamplePrecision)((currentSinusF - sinusFreq) * n) + sinusPhase;
		currentSinusF = sinusFreq;
	}

	//---calculate parameter ramps
	Steinberg::Vst::ParamValue volumeRamp = 0.;
	Steinberg::Vst::ParamValue panningLeftRamp = 0.;
	Steinberg::Vst::ParamValue panningRightRamp = 0.;
	Steinberg::Vst::ParamValue noiseVolumeRamp = 0.;
	Steinberg::Vst::ParamValue sinusVolumeRamp = 0.;
	Steinberg::Vst::ParamValue triangleVolumeRamp = 0.;
	Steinberg::Vst::ParamValue squareVolumeRamp = 0.;
	Steinberg::Vst::ParamValue filterFreqRamp = 0.;
	Steinberg::Vst::ParamValue filterQRamp = 0.;
	Steinberg::Vst::ParamValue triangleSlopeRamp = 0.;
	Steinberg::Vst::ParamValue rampTime = std::max<Steinberg::Vst::ParamValue> ((Steinberg::Vst::ParamValue)numSamples, (this->sampleRate * 0.005));
	
	Steinberg::Vst::ParamValue wantedVolume = VoiceStatics::normalizedLevel2Gain ((float)Steinberg::Bound (0.0, 1.0, this->globalParameters->masterVolume * levelFromVel + this->values[kVolumeMod]));
	if (wantedVolume != currentVolume)
	{
		volumeRamp = (wantedVolume - currentVolume) / rampTime;
	}

	if (this->values[kPanningLeft] != currentPanningLeft)
	{
		panningLeftRamp = (this->values[kPanningLeft] - currentPanningLeft) / rampTime;
	}
	if (this->values[kPanningRight] != currentPanningRight)
	{
		panningRightRamp = (this->values[kPanningRight] - currentPanningRight) / rampTime;
	}
	if (this->values[kNoiseVolume] != currentNoiseVolume)
	{
		noiseVolumeRamp = (this->values[kNoiseVolume] - currentNoiseVolume) / rampTime;
	}
	if (this->values[kSinusVolume] != currentSinusVolume)
	{
		sinusVolumeRamp = (this->values[kSinusVolume] - currentSinusVolume) / rampTime;
	}
	if (this->values[kSquareVolume] != currentSquareVolume)
	{
		squareVolumeRamp = (this->values[kSquareVolume] - currentSquareVolume) / rampTime;
	}
	if (this->values[kTriangleVolume] != currentTriangleVolume)
	{
		triangleVolumeRamp = (this->values[kTriangleVolume] - currentTriangleVolume) / rampTime;
	}

	Steinberg::Vst::ParamValue wantedLPFreq = Steinberg::Bound (0., 1., this->globalParameters->filterFreq + this->globalParameters->freqModDepth * this->values[kFilterFrequencyMod]);
	if (wantedLPFreq != currentLPFreq)
	{
		filterFreqRamp = (wantedLPFreq - currentLPFreq) / rampTime;
	}
	Steinberg::Vst::ParamValue wantedLPQ = Steinberg::Bound (0., 1., this->globalParameters->filterQ + this->values[kFilterQMod]);
	if (wantedLPQ != currentLPQ)
	{
		filterQRamp = (wantedLPQ - currentLPQ) / rampTime;
	}

	if (this->values[kTriangleSlope] != currentTriangleSlope)
	{
		triangleSlopeRamp = (this->values[kTriangleSlope] - currentTriangleSlope) / rampTime;
	}

	for (Steinberg::int32 i = 0; i < numSamples; i++)
	{
		this->noteOnSampleOffset--;
		this->noteOffSampleOffset--;

		if (this->noteOnSampleOffset <= 0)
		{
			// we are in Release
			if (this->noteOffSampleOffset == 0)
			{
 				volumeRamp = 0;
				if (currentVolume > 0)
				{
					// ramp note off
					currentVolume -= noteOffVolumeRamp;
					if (currentVolume < 0.)
						currentVolume = 0.;
					this->noteOffSampleOffset++;
				}
				else
				{
					this->noteOffSampleOffset = this->noteOnSampleOffset = -1;
					return false;
				}
			}
			SamplePrecision sample;
			SamplePrecision osc = (SamplePrecision)sin (n * triangleFreq + trianglePhase);
			// square osc
			sample = (SamplePrecision)((::floor (osc) + 0.5) * currentSquareVolume);
			// triangle osc
			sample += (SamplePrecision)((osc - ::fabs(sin (n * triangleFreq + trianglePhase + 1 + currentTriangleSlope))) * currentTriangleVolume);
			// sinus osc
			sample += (SamplePrecision)(sin (n * sinusFreq + sinusPhase) * currentSinusVolume);

			n++;
			
			// add noise
			sample += (SamplePrecision)(this->globalParameters->noiseBuffer->at (noisePos) * currentNoiseVolume);

			// filter
			if (filterFreqRamp != 0. || filterQRamp != 0.)
			{
				filter->setFreqAndQ (VoiceStatics::freqLogScale.scale (currentLPFreq), 1. - currentLPQ);
				currentLPFreq += filterFreqRamp;
				currentLPQ += filterQRamp;
			}
			sample = (SamplePrecision)filter->process (sample);

			// store in output
			outputBuffers[0][i] += (SamplePrecision)(sample * currentPanningLeft * currentVolume);
			outputBuffers[1][i] += (SamplePrecision)(sample * currentPanningRight * currentVolume);

			// advance noise
			noisePos += noiseStep;
			if (noisePos > this->globalParameters->noiseBuffer->getSize () - 2)
			{
				noisePos = (Steinberg::int32)((float)::rand () / (float)RAND_MAX * (this->globalParameters->noiseBuffer->getSize () - 2) + 2);		
				noiseStep = -1;
			}
			else if (noisePos < 2)
			{
				noiseStep = 1;
			}

			// ramp parameters
			currentVolume += volumeRamp;
			currentPanningLeft += panningLeftRamp;
			currentPanningRight += panningRightRamp;
			currentNoiseVolume += noiseVolumeRamp;
			currentSinusVolume += sinusVolumeRamp;
			currentSquareVolume += squareVolumeRamp;
			currentTriangleVolume += triangleVolumeRamp;
			currentTriangleSlope += triangleSlopeRamp;
		}
	}
	return true;
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
void Voice<SamplePrecision>::noteOn (Steinberg::int32 _pitch, Steinberg::Vst::ParamValue velocity, float tuning, Steinberg::int32 sampleOffset, Steinberg::int32 nId)
{	
	currentVolume = 0;
	this->values[kVolumeMod] = 0;
	levelFromVel = 1.f + this->globalParameters->velToLevel * (velocity - 1.);

	currentSinusVolume = this->values[kSinusVolume] = this->globalParameters->sinusVolume;
	currentTriangleVolume = this->values[kTriangleVolume] = this->globalParameters->triangleVolume;
	currentNoiseVolume = this->values[kNoiseVolume] = this->globalParameters->noiseVolume;
	currentTriangleSlope = this->values[kTriangleSlope] = this->globalParameters->triangleSlop;
	currentSquareVolume = this->values[kSquareVolume] = this->globalParameters->squareVolume;
	
	// filter setting
	currentLPFreq = this->globalParameters->filterFreq;
	this->values[kFilterFrequencyMod] = 0;
	currentLPQ =  this->globalParameters->filterQ;
	this->values[kFilterQMod] = 0;

	filter->setType ((Filter::Type)this->globalParameters->filterType);
	filter->setFreqAndQ (VoiceStatics::freqLogScale.scale (currentLPFreq), 1. - currentLPQ);

	currentSinusDetune = 0.;
	if (this->globalParameters->sinusDetune != 0.)
	{
		currentSinusDetune = VoiceStatics::freqTab[this->pitch] * (::pow (2.0, this->globalParameters->sinusDetune * 2.0 / 12.0) - 1);
	}
	this->values[kSinusDetune] = currentSinusDetune;
	this->values[kTuningMod] = 0;

	Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOn (_pitch, velocity, tuning, sampleOffset, nId);
	this->noteOnSampleOffset++;
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
void Voice<SamplePrecision>::noteOff (Steinberg::Vst::ParamValue velocity, Steinberg::int32 sampleOffset)
{
	Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::noteOff (velocity, sampleOffset);
	this->noteOffSampleOffset++;

	Steinberg::Vst::ParamValue timeFactor;
	if (this->values[kReleaseTimeMod] == 0)
		timeFactor = 1;
	else
		timeFactor = ::pow (100., this->values[kReleaseTimeMod]);
	
	noteOffVolumeRamp = 1.0 / (timeFactor * this->sampleRate * ((this->globalParameters->releaseTime * MAX_RELEASE_TIME_SEC) + 0.005));
	if (currentVolume)
		noteOffVolumeRamp *= currentVolume;
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
void Voice<SamplePrecision>::reset ()
{
	noiseStep = 1;
	noisePos = 0;
	n = 0;
	sinusPhase = trianglePhase = 0.;
	currentSinusF = currentTriangleF = -1.;
	this->values[kVolumeMod] = 0.;
	this->values[kTuningMod] = 0.;
	this->values[kFilterFrequencyMod] = 0.;
	this->values[kFilterQMod] = 0.;
	this->values[kReleaseTimeMod] = 0.;
	currentPanningLeft = this->values[kPanningLeft] = 1.;
	currentPanningRight = this->values[kPanningRight] = 1.;
	currentNoiseVolume = this->values[kNoiseVolume] = 0.5;
	currentSinusVolume = this->values[kSinusVolume] = 0.5;
	currentSquareVolume = this->values[kSquareVolume] = 0.5;
	currentTriangleVolume = this->values[kTriangleVolume] = 0.5;
	currentLPFreq = 1.;
	currentLPQ = 0.;
	filter->reset ();
	noteOffVolumeRamp = 0.005;
	
	Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::reset ();
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
void Voice<SamplePrecision>::setSampleRate (Steinberg::Vst::ParamValue sampleRate)
{
	filter->setSampleRate (sampleRate);
	Steinberg::Vst::VoiceBase<kNumParameters, SamplePrecision, 2, GlobalParameterState>::setSampleRate (sampleRate);
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
Voice<SamplePrecision>::Voice ()
{
	filter = new Filter (Filter::kLowpass);
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
Voice<SamplePrecision>::~Voice ()
{
	delete filter;
}

}
} // namespaces
