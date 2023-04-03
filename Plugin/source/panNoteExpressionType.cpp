#pragma once

#include "../include/panNoteExpressionType.h"

namespace Carlsound
{
	namespace Huntley
	{
		PanNoteExpressionType::PanNoteExpressionType(Steinberg::Vst::NoteExpressionTypeID typeId = Steinberg::Vst::kPanTypeID,
			                                         const Steinberg::Vst::TChar* title = Steinberg::String("Pan"),
			                                         const Steinberg::Vst::TChar* shortTitle = Steinberg::String("Pan"),
			                                         const Steinberg::Vst::TChar* units = 0,
			                                         Steinberg::int32 unitId = -1,
			                                         Steinberg::Vst::NoteExpressionValue defaultPlainValue = 0,
			                                         Steinberg::Vst::NoteExpressionValue plainMin = -100,
			                                         Steinberg::Vst::NoteExpressionValue plainMax = 100,
			                                         Steinberg::int32 flags = Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar | Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute,
			                                         Steinberg::int32 precision = 0)
		   : Steinberg::Vst::RangeNoteExpressionType(typeId,
			                                         title,
			                                         shortTitle,
			                                         units,
			                                         unitId,
			                                         defaultPlainValue,
			                                         plainMin,
			                                         plainMax,
			                                         flags,
			                                         precision)
		{
			
		}

		Steinberg::tresult PanNoteExpressionType::getStringByValue(Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/,
			                                                       Steinberg::Vst::String128 string /*out*/)
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

		Steinberg::tresult PanNoteExpressionType::getValueByString(const Steinberg::Vst::TChar* string /*in*/,
			Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/)
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
			return Steinberg::Vst::RangeNoteExpressionType::getValueByString(string, valueNormalized);
		}
	} // Huntley
} // Carlsound