#pragma once

#include "pluginterfaces/base/ftypes.h"
#include <stdlib.h>
#include <cmath>

namespace Carlsound {
namespace Huntley {

//-----------------------------------------------------------------------------
template<class SamplePrecision>
class BrownNoise
{
public:
	BrownNoise (Steinberg::int32 bufferSize, SamplePrecision sampleRate);
	~BrownNoise ();

	SamplePrecision at (Steinberg::int32 pos) const { return buffer[pos]; }
	Steinberg::int32 getSize () const { return bufferSize; }
protected:
	SamplePrecision* buffer;
	Steinberg::int32 bufferSize;
};

//-----------------------------------------------------------------------------
template<class SamplePrecision>
BrownNoise<SamplePrecision>::BrownNoise (Steinberg::int32 bufferSize, SamplePrecision sampleRate)
: buffer (0)
, bufferSize (bufferSize)
{
	buffer = new SamplePrecision [bufferSize];

	const SamplePrecision f = (SamplePrecision)0.0045;
	SamplePrecision accu = (SamplePrecision)0.;
	SamplePrecision y;
	for (Steinberg::int32 frame = 0; frame < bufferSize; frame++)
	{
		y = ((SamplePrecision)rand () / (SamplePrecision)RAND_MAX - (SamplePrecision)0.5) * (SamplePrecision)2.;

		accu = (f * y) + (((SamplePrecision)1.0 - f) * accu);
		y = (SamplePrecision)1.55 * accu * (SamplePrecision)100. / (SamplePrecision)::sqrt (::sqrt (sampleRate));
		buffer[frame] = y;
	}
}

//-----------------------------------------------------------------------------
template<class SamplePrecision>
BrownNoise<SamplePrecision>::~BrownNoise ()
{
	delete [] buffer;
}

}
} // namespaces
