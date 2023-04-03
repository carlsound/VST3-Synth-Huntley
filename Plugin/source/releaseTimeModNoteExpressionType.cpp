#pragma once

#include "../include/releaseTimeModNoteExpressionType.h"

namespace Carlsound
{
	namespace Huntley
	{
		ReleaseTimeModNoteExpressionType::ReleaseTimeModNoteExpressionType(): Steinberg::Vst::NoteExpressionType(Controller::kReleaseTimeModTypeID,
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

		Steinberg::tresult ReleaseTimeModNoteExpressionType::getStringByValue(Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/,
				                                                              Steinberg::Vst::String128 string /*out*/)
		{
			Steinberg::UString128 wrapper;
			double timeFactor = pow(100., 2 * (valueNormalized - 0.5));
			wrapper.printFloat(timeFactor, timeFactor > 10 ? 1 : 2);
			wrapper.copyTo(string, 128);
			return Steinberg::kResultTrue;
		}

		Steinberg::tresult ReleaseTimeModNoteExpressionType::getValueByString(const Steinberg::Vst::TChar* string /*in*/,
				                                                              Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/)
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
			
		//OBJ_METHODS(ReleaseTimeModNoteExpressionType, Steinberg::Vst::NoteExpressionType)
	}
}
