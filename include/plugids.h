//------------------------------------------------------------------------
#pragma once

namespace Carlsound {
namespace Huntley {

// HERE are defined the parameter Ids which are exported to the host
enum HuntleyParams : Steinberg::Vst::ParamID
{
	kBypassId = 100,
	kParamSpeedId = 102
};


// HERE you have to define new unique class ids: for processor and for controller
// you can use GUID creator tools like https://www.guidgenerator.com/

static const Steinberg::FUID MyProcessorUID (0x637A8E4C, 0x1F3048FC, 0xBC5E02C3, 0x4D61432F);
static const Steinberg::FUID MyControllerUID (0x25E25A21, 0x736B4128, 0x9AE0111E, 0x89EA6C0B);

//------------------------------------------------------------------------
} // namespace Huntley
} // namespace Carlsound
