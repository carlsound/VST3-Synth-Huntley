//------------------------------------------------------------------------
#include "public.sdk/source/main/pluginfactory.h"

#include "../include/noteExpressionSynthController.h"	// for createInstance
#include "../include/noteExpressionSynthProcessor.h"	// for createInstance
#include "../include/noteExpressionSynthIDs.h"			// for uids
#include "../include/version.h"			// for version and naming

#define stringSubCategory	"Instrument|Synth"	// Subcategory for this Plug-in (to be changed if needed, see PlugType in ivstaudioprocessor.h)

BEGIN_FACTORY_DEF (stringCompanyName, stringCompanyWeb,	stringCompanyEmail)

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Carlsound::Huntley::MyProcessorUID),
				Steinberg::PClassInfo::kManyInstances,	// cardinality  
				kVstAudioEffectClass,	// the component category (do not changed this)
				stringPluginName,		// here the Plug-in name (to be changed)
				Vst::kDistributable,	// means that component and controller could be distributed on different computers
		        Steinberg::Vst::PlugType::kInstrumentSynth,		// Subcategory for this Plug-in (to be changed)
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				Carlsound::Huntley::Processor::createInstance)	// function pointer called when this component should be instantiated

	DEF_CLASS2 (INLINE_UID_FROM_FUID(Carlsound::Huntley::MyControllerUID),
				Steinberg::PClassInfo::kManyInstances,  // cardinality   
				kVstComponentControllerClass,// the Controller category (do not changed this)
				stringPluginName "Controller",	// controller name (could be the same than component name)
				0,						// not used here
				"",						// not used here
				FULL_VERSION_STR,		// Plug-in version (to be changed)
				kVstVersionString,		// the VST 3 SDK version (do not changed this, use always this define)
				Carlsound::Huntley::Controller::createInstance)// function pointer called when this component should be instantiated

END_FACTORY

//------------------------------------------------------------------------
//  Module init/exit
//------------------------------------------------------------------------

//------------------------------------------------------------------------
// called after library was loaded
bool InitModule ()
{
	return true;
}

//------------------------------------------------------------------------
// called after library is unloaded
bool DeinitModule ()
{
	return true;
}
