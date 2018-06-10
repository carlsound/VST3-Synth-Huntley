#pragma once

#include "pluginterfaces/base/futils.h"
#include "public.sdk/source/vst/vstnoteexpressiontypes.h"
#include "base/source/fstring.h"
#include "pluginterfaces/base/ustring.h"
#include "../include/noteExpressionSynthController.h"

namespace Carlsound
{
	namespace Huntley
	{
		class ReleaseTimeModNoteExpressionType : public Steinberg::Vst::NoteExpressionType
		{
		public:
			ReleaseTimeModNoteExpressionType()
				: NoteExpressionType(Controller::kReleaseTimeModTypeID, 
					                 Steinberg::String("Release Time"), 
					                 Steinberg::String("RelTime"), 
					                 Steinberg::String("%"),
					                 -1, 
					                 0.5,
					                 0., 
					                 1., 
					                 0, 
					                 Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar | Steinberg::Vst::NoteExpressionTypeInfo::kIsOneShot)
			{
			}

			Steinberg::tresult getStringByValue(Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/, 
				                                Steinberg::Vst::String128 string /*out*/) SMTG_OVERRIDE
			{
				Steinberg::UString128 wrapper;
				double timeFactor = pow(100., 2 * (valueNormalized - 0.5));
				wrapper.printFloat(timeFactor, timeFactor > 10 ? 1 : 2);
				wrapper.copyTo(string, 128);
				return Steinberg::kResultTrue;
			}

			Steinberg::tresult getValueByString(const Steinberg::Vst::TChar* string /*in*/, 
				                                Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/) SMTG_OVERRIDE
			{
				Steinberg::String wrapper((Steinberg::Vst::TChar*)string);
				Steinberg::Vst::ParamValue tmp;
				if (wrapper.scanFloat(tmp))
				{
					valueNormalized = Steinberg::Bound(0.0, 1.0, log10(tmp) / 4. + 0.5);
					return Steinberg::kResultTrue;
				}
				return Steinberg::kResultFalse;
			}
			OBJ_METHODS(ReleaseTimeModNoteExpressionType, NoteExpressionType)
		};
	}
}