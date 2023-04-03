#pragma once

#include "base/source/fobject.h"
#include "public.sdk/source/vst/vstnoteexpressiontypes.h"
#include "base/source/fstring.h"
#include "pluginterfaces/base/ustring.h"

namespace Carlsound
{
	namespace Huntley
	{
		class PanNoteExpressionType : public Steinberg::Vst::RangeNoteExpressionType
		{
		public:
			/*PanNoteExpressionType(Steinberg::Vst::NoteExpressionTypeID typeId,
				                  const Steinberg::Vst::TChar* title,
				                  const Steinberg::Vst::TChar* shortTitle,
				                  const Steinberg::Vst::TChar* units,
				                  Steinberg::int32 unitId,
			                      Steinberg::Vst::NoteExpressionValue defaultPlainValue, 
				                  Steinberg::Vst::NoteExpressionValue plainMin,
			                      Steinberg::Vst::NoteExpressionValue plainMax, 
				                  Steinberg::int32 flags, 
				                  Steinberg::int32 precision);*/

			PanNoteExpressionType()
				: RangeNoteExpressionType(Steinberg::Vst::kPanTypeID, Steinberg::String("Pan"), Steinberg::String("Pan"), 0, -1, 0, -100, 100, Steinberg::Vst::NoteExpressionTypeInfo::kIsBipolar | Steinberg::Vst::NoteExpressionTypeInfo::kIsAbsolute, 0)
			{
			}

			Steinberg::tresult getStringByValue(Steinberg::Vst::NoteExpressionValue valueNormalized /*in*/, Steinberg::Vst::String128 string /*out*/) SMTG_OVERRIDE
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

			Steinberg::tresult getValueByString(const Steinberg::Vst::TChar* string /*in*/, Steinberg::Vst::NoteExpressionValue& valueNormalized /*out*/) SMTG_OVERRIDE
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

		}; // panNoteExpressionType
	} // Huntley
} // Carlsound