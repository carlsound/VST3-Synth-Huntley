

#include "../include/noteExpressionSynthController.h"

#include "../include/panNoteExpressionType.h"
#include "../include/releaseTimeModNoteExpressionType.h"

#include "base/source/fstreamer.h"
#include "pluginterfaces/base/ibstream.h"
#include <string>

#include "public.sdk/source/vst/hosting/stringconvert.h"
#include "public.sdk/samples/vst/mda-vst3/source/mdaMultiBandController.h"

#include "pluginterfaces/base/ustring.h"
#include "pluginterfaces/base/futils.h"
#include "pluginterfaces/vst/ivstmidicontrollers.h"
#include "base/source/fstring.h"
#include <public.sdk/samples/vst/common/logscale.h>


namespace Carlsound {
namespace Huntley {
	//-----------------------------------------------------------------------------
	/*
	class PanNoteExpressionType : public Steinberg::Vst::RangeNoteExpressionType
	{
	public:
		PanNoteExpressionType()
			: RangeNoteExpressionType(Steinberg::Vst::kPanTypeID, Steinberg::String("Pan"), Steinberg::String("Pan"), 0, -1, 0, -100, 100, Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar | Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute, 0)
		{
		}

		Steinberg::tresult getStringByValue(Steinberg::Vst::NoteExpressionValue valueNormalized, //in
	                                        Steinberg::Vst::String128 string //out
	                                        ) SMTG_OVERRIDE
		{
			if (valueNormalized == 0.5)
				Steinberg::UString128("C").copyTo(string, 128);
			else if (valueNormalized == 0)
				Steinberg::UString128("L").copyTo(string, 128);
			else if (valueNormalized == 1)
				Steinberg::UString128("R").copyTo(string, 128);
			else
				RangeNoteExpressionType::getStringByValue(valueNormalized, string);
			return Steinberg::kResultTrue;
		}

		Steinberg::tresult getValueByString(const Steinberg::Vst::TChar* string, //in 
	                                        Steinberg::Vst::NoteExpressionValue& valueNormalized //out
	                                        ) SMTG_OVERRIDE
		{
			Steinberg::String str(string);
			if (str == "C")
			{
				valueNormalized = 0.5;
				return Steinberg::kResultTrue;
			}
			else if (str == "L")
			{
				valueNormalized = 0.;
				return Steinberg::kResultTrue;
			}
			else if (str == "R")
			{
				valueNormalized = 1.;
				return Steinberg::kResultTrue;
			}
			return RangeNoteExpressionType::getValueByString(string, valueNormalized);
		}
		OBJ_METHODS(PanNoteExpressionType, RangeNoteExpressionType)
	};
	*/

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::initialize (FUnknown* context)
{
    Steinberg::tresult result = EditController::initialize (context);
	if (result == Steinberg::kResultTrue)
	{
		//---Create Parameters------------
		/*
		parameters.addParameter (STR16 ("Bypass"), // title
			                     0, // units
			                     1, // stepCount
			                     0, // defaultValueNormalized
                                 Steinberg::Vst::ParameterInfo::kCanAutomate | Steinberg::Vst::ParameterInfo::kIsBypass, // flags
		                         HuntleyParams::kBypassId, // tag
			                     0, // unitUD
			                     STR16("Bypass")); // shortTitle

		parameters.addParameter (STR16 ("Speed"), // title
		                         STR16 ("sec"), // units
			                     99, // stepCount
			                     0.1, // defaultValueNormalized
                                 Steinberg::Vst::ParameterInfo::kCanAutomate, HuntleyParams::kParamSpeedId, // flags
			                     0, // unitID
		                         STR16 ("Speed")); // shortTitle
		

		
		m_speedRangeParameter = std::make_shared<Steinberg::Vst::RangeParameter> (STR16("Speed"), // title
			                                                                 HuntleyParams::kParamSpeedId, // ParamID
			                                                                 STR16("sec"), // units
			                                                                 0.1, // minPlain
			                                                                 10.0, // maxPlain
			                                                                 1.0, // defaultValuePlain
			                                                                 99, // stepCount
			                                                                 Steinberg::Vst::ParameterInfo::kCanAutomate, // flags
			                                                                 0, // unitID
			                                                                 STR16("Speed")); // shortTitle
		parameters.addParameter(m_speedRangeParameter->getInfo());
		*/
		/////////////////////////////////
		Steinberg::Vst::Parameter* param;
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Master Volume"),
			                                       kParamMasterVolume,
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       80);
		param->setPrecision(1);
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Master Tuning"), 
			                                       kParamMasterTuning, 
			                                       USTRING("cent"), 
			                                       -200, 
			                                       200, 
			                                       0);
		
		param->setPrecision(0);
		
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Velocity To Level"), 
			                                       kParamVelToLevel, 
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       100);
		
		param->setPrecision(1);
		
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Release Time"), 
			                                       kParamReleaseTime, 
			                                       USTRING("sec"), 
			                                       0.005, 
			                                       MAX_RELEASE_TIME_SEC, 
			                                       0.025);
		
		param->setPrecision(3);
		
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Noise Volume"), 
			                                       kParamNoiseVolume, 
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       0);
		
		param->setPrecision(1);
		parameters.addParameter(param);
		//
		param = new Steinberg::Vst::RangeParameter(USTRING("Sinus Volume"), 
			                                       kParamSinusVolume, 
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       80);
		param->setPrecision(1);
		parameters.addParameter(param);
		//
		param = new Steinberg::Vst::RangeParameter(USTRING("Triangle Volume"), 
			                                       kParamTriangleVolume, 
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       20);
		
		param->setPrecision(1);
		parameters.addParameter(param);
		//
		param = new Steinberg::Vst::RangeParameter(USTRING("Square Volume"), 
			                                       kParamSquareVolume, 
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       80);
		
		param->setPrecision(1);
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Sinus Detune"), 
			                                       kParamSinusDetune, 
			                                       USTRING("cent"), 
			                                       -200, 
			                                       200, 
			                                       0);
		
		param->setPrecision(0);
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Triangle Slop"), 
			                                       kParamTriangleSlop, 
			                                       USTRING("%"), 
			                                       0, 
			                                       100, 
			                                       50);
		
		param->setPrecision(0);
		parameters.addParameter(param);
		/////////////////////////////////
		Steinberg::Vst::StringListParameter* filterTypeParam = new Steinberg::Vst::StringListParameter(USTRING("Filter Type"), 
			                                                                                           kParamFilterType);
		
		filterTypeParam->appendString(USTRING("Lowpass"));
		filterTypeParam->appendString(USTRING("Highpass"));
		filterTypeParam->appendString(USTRING("Bandpass"));
		
		parameters.addParameter(filterTypeParam);
		/////////////////////////////////
		param = new Steinberg::Vst::LogScaleParameter<Steinberg::Vst::ParamValue>(USTRING("Filter Frequency"), 
			                                                                      kParamFilterFreq, 
			                                                                      VoiceStatics::freqLogScale);
		
		param->setPrecision(1);
		parameters.addParameter(param);
		/////////////////////////////////
		param = new Steinberg::Vst::RangeParameter(USTRING("Frequency Mod Depth"), 
			                                       kParamFilterFreqModDepth, 
			                                       USTRING("%"), 
			                                       -100, 
			                                       100, 
			                                       100);
		
		param->setPrecision(1);
		parameters.addParameter(param);
		/////////////////////////////////
		param = parameters.addParameter(USTRING("Filter Q"), 
			                            0, 
			                            0, 
			                            0, 
			                            Steinberg::Vst::ParameterInfo::kCanAutomate, 
			                            kParamFilterQ);
		
		param->setPrecision(2);
		/////////////////////////////////
		parameters.addParameter(USTRING("Bypass SNA"), 
			                    0, 
			                    1, 
			                    0, 
			                    Steinberg::Vst::ParameterInfo::kCanAutomate, 
			                    kParamBypassSNA);
		/////////////////////////////////
		parameters.addParameter(new Steinberg::Vst::RangeParameter(USTRING("Active Voices"), 
			                                                       kParamActiveVoices, 
			                                                       0, 
			                                                       0, 
			                                                       MAX_VOICES, 
			                                                       0, 
			                                                       MAX_VOICES, 
			                                                       Steinberg::Vst::ParameterInfo::kIsReadOnly));
		/////////////////////////////////
		Steinberg::Vst::StringListParameter* tuningRangeParam = new Steinberg::Vst::StringListParameter(USTRING("Tuning Range"), 
			                                                                                            kParamTuningRange, 
			                                                                                            0, 
			                                                                                            Steinberg::Vst::ParameterInfo::kIsList);
		
		tuningRangeParam->appendString(USTRING("[-1, +1] Octave"));
		tuningRangeParam->appendString(USTRING("[-3, +2] Tunes"));
		
		parameters.addParameter(tuningRangeParam);
		/////////////////////////////////
		/////////////////////////////////
		// Init Note Expression Types
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(Steinberg::Vst::kVolumeTypeID, 
			                                      Steinberg::String("Volume"), 
			                                      Steinberg::String("Vol"), 
			                                      0, 
			                                      -1, 
			                                      1., 
			                                      0., 
			                                      1., 
			                                      0, 
			                                      0));
		//noteExpressionTypes.addNoteExpressionType(new PanNoteExpressionType());
		Steinberg::Vst::NoteExpressionType* panNoteExpression = new Steinberg::Vst::RangeNoteExpressionType(Steinberg::Vst::kTuningTypeID, 
			                                                                                                Steinberg::String("Tuning"), 
			                                                                                                Steinberg::String("Tun"), 
			                                                                                                Steinberg::String("Half Tone"), -
			                                                                                                1, 
			                                                                                                0, 
			                                                                                                120, 
			                                                                                                -120, 
			                                                                                                Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar);
		
		panNoteExpression->getInfo().valueDesc.minimum = 0.5 - VoiceStatics::kNormTuningOneOctave;
		panNoteExpression->getInfo().valueDesc.maximum = 0.5 + VoiceStatics::kNormTuningOneOctave;
		
		noteExpressionTypes.addNoteExpressionType(panNoteExpression);
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(kSinusVolumeTypeID, 
			                                                                             Steinberg::String("Sinus Volume"), Steinberg::String("Sin Vol"), 
			                                                                             Steinberg::String("%"), 
			                                                                             -1, 
			                                                                             getParameterObject(kParamSinusVolume), 
			                                                                             Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(kSinusDetuneTypeID, 
			                                                                             Steinberg::String("Sinus Detune"), 
			                                                                             Steinberg::String("Sin Detune"), 
			                                                                             Steinberg::String("Cent"), -1, 
			                                                                             getParameterObject(kParamSinusDetune), 
			                                                                             Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute | Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(kTriangleVolumeTypeID, 
			                                                                             Steinberg::String("Triangle Volume"), 
			                                                                             Steinberg::String("Tri Vol"), 
			                                                                             Steinberg::String("%"), 
			                                                                             -1, 
			                                                                             getParameterObject(kParamTriangleVolume), 
			                                                                             Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(kSquareVolumeTypeID, 
			                                                                             Steinberg::String("Square Volume"), 
			                                                                             Steinberg::String("Square Vol"), 
			                                                                             Steinberg::String("%"), 
			                                                                             -1, 
			                                                                             getParameterObject(kParamSquareVolume), 
			                                                                             Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(kNoiseVolumeTypeID, 
			                                                                             Steinberg::String("Noise Volume"), 
			                                                                             Steinberg::String("Noise Vol"), 
			                                                                             Steinberg::String("%"), 
			                                                                             -1, 
			                                                                             getParameterObject(kParamNoiseVolume), 
			                                                                             Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::RangeNoteExpressionType(kFilterFreqModTypeID, 
			                                                                                  Steinberg::String("Filter Frequency Modulation"), 
			                                                                                  Steinberg::String("Freq Mod"), 
			                                                                                  0, 
			                                                                                  -1, 
			                                                                                  0, 
			                                                                                  -100, 
			                                                                                  100, 
			                                                                                  Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar, 
			                                                                                  0));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::RangeNoteExpressionType(kFilterQModTypeID, 
			                                      Steinberg::String("Filter Q Modulation"), 
			                                      Steinberg::String("Q Mod"), 
			                                      0, 
			                                      -1, 
			                                      0, 
			                                      -100, 
			                                      100, 
			                                      Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar,
			                                      0));
		
		noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(kFilterTypeTypeID, 
			                                      Steinberg::String("Filter Type"), 
			                                      Steinberg::String("Flt Type"), 
			                                      0, 
			                                     -1, 
			                                     getParameterObject(kParamFilterType), 
			                                     Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar));
		
		noteExpressionTypes.addNoteExpressionType(new ReleaseTimeModNoteExpressionType());
	}
    return Steinberg::kResultTrue;
}

//-----------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Carlsound::Huntley::Controller::terminate()
{
	noteExpressionTypes.removeAll();
	return EditController::terminate();
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::setComponentState (Steinberg::IBStream* state)
{
	// we receive the current state of the component (processor part)
	// we read our parameters and bypass value...
	GlobalParameterState gps;
	Steinberg::tresult result = gps.setState(state);

	if (result == Steinberg::kResultTrue)
	{
		setParamNormalized(kParamMasterVolume, 
			               gps.masterVolume);

		setParamNormalized(kParamMasterTuning,
			               (gps.masterTuning + 1) / 2.);

		setParamNormalized(kParamVelToLevel, 
			               gps.velToLevel);

		setParamNormalized(kParamFilterFreqModDepth,
			               (gps.freqModDepth + 1) / 2.);
		//
		//
		setParamNormalized(kParamReleaseTime,
			               gps.releaseTime);

		setParamNormalized(kParamNoiseVolume,
			               gps.noiseVolume);

		setParamNormalized(kParamSinusVolume,
			               gps.sinusVolume);

		setParamNormalized(kParamTriangleVolume,
			               gps.triangleVolume);

		setParamNormalized(kParamSquareVolume,
			               gps.squareVolume);
		//
		//
		setParamNormalized(kParamSinusDetune,
			               (gps.sinusDetune + 1) / 2.);

		setParamNormalized(kParamTriangleSlop,
			               gps.triangleSlop);
		//
		//
		setParamNormalized(kParamFilterType,
			               plainParamToNormalized(kParamFilterType,
						   gps.filterType));

		setParamNormalized(kParamFilterFreq,
			               gps.filterFreq);

		setParamNormalized(kParamFilterQ,
			               gps.filterQ);
		//
		//
		setParamNormalized(kParamBypassSNA,
			               gps.bypassSNA);
		//
		//
		setParamNormalized(kParamTuningRange,
			               plainParamToNormalized(kParamTuningRange,
						   gps.tuningRange));

	}
	return result;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::setParamNormalized(Steinberg::Vst::ParamID tag,
		                                         Steinberg::Vst::ParamValue value)
{
	bool newRange = false;
	if (tag == kParamTuningRange && getParamNormalized(tag) != value)
	{
		newRange = true;
		Steinberg::Vst::NoteExpressionType* net = noteExpressionTypes.getNoteExpressionType(Steinberg::Vst::kTuningTypeID);
		if (value > 0)
		{
			noteExpressionTypes.addNoteExpressionType(new Steinberg::Vst::NoteExpressionType(Controller::kTriangleSlopeTypeID, 
				                                      Steinberg::String("Triangle Slope"),
				                                      Steinberg::String("Tri Slope"),
				                                      Steinberg::String("%"),
				                                      -1,
				                                      getParameterObject(kParamTriangleSlop),
				                                      Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute));
			if (net)
			{
				net->getInfo().valueDesc.minimum = 0.5 - 3 * VoiceStatics::kNormTuningOneTune;
				net->getInfo().valueDesc.maximum = 0.5 + 2 * VoiceStatics::kNormTuningOneTune;
			}
		}
		else
		{
			noteExpressionTypes.removeNoteExpressionType(Controller::kTriangleSlopeTypeID);
			if (net)
			{
				net->getInfo().valueDesc.minimum = 0.5 - VoiceStatics::kNormTuningOneOctave;
				net->getInfo().valueDesc.maximum = 0.5 + VoiceStatics::kNormTuningOneOctave;
			}
		}
	}

	Steinberg::tresult res = Steinberg::Vst::EditController::setParamNormalized(tag, value);

	if (newRange && componentHandler)
		componentHandler->restartComponent(Steinberg::Vst::kNoteExpressionChanged);

	return res;
}

//------------------------------------------------------------------------
/*
Steinberg::Vst::ParamValue PLUGIN_API Controller::normalizedParamToPlain(Steinberg::Vst::ParamID tag,
		Steinberg::Vst::ParamValue valueNormalized)
{
	if(kParamSpeedId == tag)
	{
		return ((valueNormalized * ((10.0 - 0.1) / 1.0)) + 0.10);
	}
	else
	{
		return valueNormalized;
	}
}
*/

//------------------------------------------------------------------------
/*
Steinberg::Vst::ParamValue PLUGIN_API Controller::plainParamToNormalized(Steinberg::Vst::ParamID tag,
	                                                              Steinberg::Vst::ParamValue value)
{
	if (kParamSpeedId == tag)
	{
		return (value / 10.0);
	}
	else
	{
		return value;
	}
}
*/

//------------------------------------------------------------------------
/*
Steinberg::tresult Controller::getParamStringByValue(Steinberg::Vst::ParamID tag,
	                                                     Steinberg::Vst::ParamValue valueNormalized,
	                                                     Steinberg::Vst::String128 string)
{
	
	if(HuntleyParams::kBypassId == tag)
	{
		std::string str;
		if(valueNormalized)
		{
			str = "On";
		}
		else
		{
			str = "Off";
		}
		string = new TCHAR[str.size() + 1];
		string[str.size()] = 0;
		std::copy(str.begin(), str.end(), string);
	}
	if (HuntleyParams::kParamSpeedId == tag)
	{
		std::string str;
		str = (std::to_string((valueNormalized * ((10.0 - 0.1) / 1.0)) + 0.10));
		string = new TCHAR[str.size() + 1];
		string[str.size()] = 0;
		std::copy(str.begin(), str.end(), string);
	}
	return Steinberg::kResultOk;
}
*/

//------------------------------------------------------------------------
Steinberg::int32 PLUGIN_API Controller::getNoteExpressionCount(Steinberg::int32 busIndex,
	                                               Steinberg::int16 channel)
{
	if (busIndex == 0 && channel == 0)
	{
		return noteExpressionTypes.getNoteExpressionCount();
	}
	return 0;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::getNoteExpressionInfo(Steinberg::int32 busIndex,
	                                                Steinberg::int16 channel,
	                                                Steinberg::int32 noteExpressionIndex,
	                                                Steinberg::Vst::NoteExpressionTypeInfo& info /*out*/)
{
	if (busIndex == 0 && channel == 0)
	{
		return noteExpressionTypes.getNoteExpressionInfo(noteExpressionIndex, info);
	}
	return Steinberg::kResultFalse;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::getNoteExpressionStringByValue(Steinberg::int32 busIndex,
	                                                         Steinberg::int16 channel,
	                                                         Steinberg::Vst::NoteExpressionTypeID id,
	                                                         Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/,
	                                                         Steinberg::Vst::String128 string /*out*/)
{
	if (busIndex == 0 && channel == 0)
	{
		return noteExpressionTypes.getNoteExpressionStringByValue(id, valueNormalized, string);
	}
	return Steinberg::kResultFalse;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::getNoteExpressionValueByString(Steinberg::int32 busIndex,
	                                                         Steinberg::int16 channel,
	                                                         Steinberg::Vst::NoteExpressionTypeID id,
	                                                         const Steinberg::Vst::TChar* string /*in*/,
	                                                         Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/)
{
	if (busIndex == 0 && channel == 0)
	{
		return noteExpressionTypes.getNoteExpressionValueByString(id, string, valueNormalized);

	}
	return Steinberg::kResultFalse;
}

//------------------------------------------------------------------------
Steinberg::tresult PLUGIN_API Controller::getMidiControllerAssignment(Steinberg::int32 busIndex,
	                                                      Steinberg::int16 channel,
	                                                      Steinberg::Vst::CtrlNumber midiControllerNumber,
	                                                      Steinberg::Vst::ParamID& id/*out*/)
{
	if (busIndex == 0 && channel == 0)
	{
		id = 0;
		switch (midiControllerNumber)
		{
			case Steinberg::Vst::kPitchBend: 
			{
				id = kParamMasterTuning;
				break;
			}
			case Steinberg::Vst::kCtrlVolume: 
			{
				id = kParamMasterVolume;
				break;
			}
			case Steinberg::Vst::kCtrlFilterCutoff: 
			{
				id = kParamFilterFreq;
				break;
			}
			case Steinberg::Vst::kCtrlFilterResonance: 
			{
				id = kParamFilterQ;
				break;
			}
		}
		return id != 0 ? Steinberg::kResultTrue : Steinberg::kResultFalse;
	}
	return Steinberg::kResultFalse;
}

//------------------------------------------------------------------------
} // namespace Huntley
} // namespace Carlsound
