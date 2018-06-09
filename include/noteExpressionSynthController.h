///------------------------------------------------------------------------

#pragma once

#include "public.sdk/source/vst/vsteditcontroller.h"
#include "pluginterfaces/vst/ivstnoteexpression.h"
#include "public.sdk/source/vst/vstnoteexpressiontypes.h"
#include <memory>
#include <atlconv.h>
#include <codecvt>
#include "public.sdk/samples/vst/note_expression_synth/source/note_expression_synth_controller.h"

namespace Carlsound {
namespace Huntley {

//-----------------------------------------------------------------------------
class Controller : public Steinberg::Vst::EditController,
	               public Steinberg::Vst::INoteExpressionController,
	               public Steinberg::Vst::IMidiMapping
{
public:
//------------------------------------------------------------------------
	// create function required for Plug-in factory,
	// it will be called to create new instances of this controller
//------------------------------------------------------------------------
	static FUnknown* createInstance (void*)
	{
		return (Steinberg::Vst::IEditController*) new Controller();
	}

	///////////////////////////////////////////////////////////////////////////
	//---from IPluginBase--------
	Steinberg::tresult PLUGIN_API initialize (FUnknown* context) SMTG_OVERRIDE;

	////////////////////////////////////////////////////////////////////////////
	//---from EditController-----
	Steinberg::tresult PLUGIN_API terminate() SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API setComponentState (Steinberg::IBStream* state) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API setParamNormalized(Steinberg::Vst::ParamID tag,
		                                             Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;

	Steinberg::Vst::ParamValue PLUGIN_API normalizedParamToPlain(Steinberg::Vst::ParamID tag,
		                                                         Steinberg::Vst::ParamValue valueNormalized) SMTG_OVERRIDE;
	Steinberg::Vst::ParamValue PLUGIN_API plainParamToNormalized(Steinberg::Vst::ParamID tag, 
		                                                         Steinberg::Vst::ParamValue value) SMTG_OVERRIDE;
	/*
	Steinberg::tresult PLUGIN_API getParamStringByValue(Steinberg::Vst::ParamID tag,
	                                                    Steinberg::Vst::ParamValue valueNormalized,
	                                                    Steinberg::Vst::String128 string) SMTG_OVERRIDE;
	*/
	//////////////////////////////////////////////////////////////////////////////
	//---from INoteExpressionController----
	Steinberg::int32 PLUGIN_API getNoteExpressionCount(Steinberg::int32 busIndex, 
		                                               Steinberg::int16 channel) SMTG_OVERRIDE;

	Steinberg::tresult PLUGIN_API getNoteExpressionInfo(Steinberg::int32 busIndex, 
		                                                Steinberg::int16 channel,
		                                                Steinberg::int32 noteExpressionIndex,
		                                                Steinberg::Vst::NoteExpressionTypeInfo& info /*out*/) SMTG_OVERRIDE;
	
	Steinberg::tresult PLUGIN_API getNoteExpressionStringByValue(Steinberg::int32 busIndex,
		                                                         Steinberg::int16 channel, 
		                                                         Steinberg::Vst::NoteExpressionTypeID id,
		                                                         Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/,
		                                                         Steinberg::Vst::String128 string /*out*/) SMTG_OVERRIDE;
	
	Steinberg::tresult PLUGIN_API getNoteExpressionValueByString(Steinberg::int32 busIndex, 
		                                                         Steinberg::int16 channel,
		                                                         Steinberg::Vst::NoteExpressionTypeID id,
		                                                         const Steinberg::Vst::TChar* string /*in*/,
		                                                         Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/) SMTG_OVERRIDE;

	//////////////////////////////////////////////////////////////////////////////
	//--from IMidiMapping----
	Steinberg::tresult PLUGIN_API getMidiControllerAssignment(Steinberg::int32 busIndex, 
		                                                      Steinberg::int16 channel,
		                                                      Steinberg::Vst::CtrlNumber midiControllerNumber, 
		                                                      Steinberg::Vst::ParamID& id/*out*/) SMTG_OVERRIDE;

	////////////////////////////////////////////////////////////////////////////////
	//---member variables----
	enum NoteExpressionTypeIds {
		kNoiseVolumeTypeID = Steinberg::Vst::NoteExpressionTypeIDs::kCustomStart,
		kFilterFreqModTypeID,
		kFilterQModTypeID,
		kSinusVolumeTypeID,
		kTriangleVolumeTypeID,
		kFilterTypeTypeID,
		kTriangleSlopeTypeID,
		kSinusDetuneTypeID,
		kReleaseTimeModTypeID,
		kTextInputTypeID,
		kSquareVolumeTypeID,
	};

protected:
	Steinberg::Vst::NoteExpressionTypeContainer noteExpressionTypes;

	//std::shared_ptr<Steinberg::Vst::RangeParameter> m_speedRangeParameter;
};

//------------------------------------------------------------------------
} // namespace Huntley
} // namespace Carlsound
