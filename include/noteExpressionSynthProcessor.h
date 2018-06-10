//------------------------------------------------------------------------
#pragma once

#define _USE_MATH_DEFINES 
#include <cmath>

#include "public.sdk/source/vst/vstaudioeffect.h"

#include "noteExpressionSynthController.h"
#include "noteExpressionSynthVoice.h"

#include "maximilian.h"
#include <public.sdk/source/vst/vstparameters.h>
#include <public.sdk/samples/vst/common/voiceprocessor.h>

namespace Carlsound {
namespace Huntley {

//-----------------------------------------------------------------------------
class Processor : public Steinberg::Vst::AudioEffect
{
public:
	//-----------------------------------------------------------------------------
	Processor ();

	Steinberg::tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API setBusArrangements (Steinberg::Vst::SpeakerArrangement* inputs,
													  Steinberg::int32 numIns,
			   										  Steinberg::Vst::SpeakerArrangement* outputs,
													  Steinberg::int32 numOuts) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API setState(Steinberg::IBStream* state) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API getState(Steinberg::IBStream* state) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API canProcessSampleSize(Steinberg::int32 symbolicSampleSize) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API setActive (Steinberg::TBool state) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API process (Steinberg::Vst::ProcessData& data) SMTG_OVERRIDE;

//------------------------------------------------------------------------

	static FUnknown* createInstance (void*)
	{
		return (Steinberg::Vst::IAudioProcessor*)new Processor ();
	}

	//template <class T> void bufferSampleGain(T inBuffer, T outBuffer, const int sampleLocation, const double gainValue);


protected:
	//-----------------------------------------------------------------------------
	Steinberg::Vst::VoiceProcessor * voiceProcessor;
	GlobalParameterState paramState;
};

//------------------------------------------------------------------------
} // namespace Huntley
} // namespace Carlsound
