#ifndef __GLOBAL_HEADER__
#define __GLOBAL_HEADER__

#include <stdint.h>
#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>

typedef int8_t int8;
typedef int16_t int16;
typedef int32_t int32;
typedef int64_t int64;
typedef uint8_t uint8;
typedef uint16_t uint16;
typedef uint32_t uint32;
typedef uint64_t uint64;

#if __cplusplus >= 201703L
#   define maybe_unused [[maybe_unused]]
#elif defined(__GNUC__)
#   define maybe_unused [[gnu::unused]]
#else
#   define maybe_unused
#endif

namespace Igorski {
namespace VST {
    maybe_unused static const float PI     = 3.141592653589793f;
    maybe_unused static const float TWO_PI = PI * 2.f;

    // maximum and minimum filter frequency ranges
    // also see fogpad.uidesc to update the controls to match

    maybe_unused static const float FILTER_MIN_FREQ      = 30.f;
    maybe_unused static const float FILTER_MAX_FREQ      = 20000.f;
    maybe_unused static const float FILTER_MIN_RESONANCE = 0.1f;
    maybe_unused static const float FILTER_MAX_RESONANCE = 0.7071067811865476f; //sqrt( 2.f ) / 2.f;

    // maximum and minimum rate of oscillation in Hz
    // also see fogpad.uidesc to update the controls to match

    maybe_unused static float MAX_LFO_RATE() { return 10.f; }
    maybe_unused static float MIN_LFO_RATE() { return .1f; }

    // sine waveform used for the oscillator
    maybe_unused static const float TABLE[ 128 ] = { 0, 0.0490677, 0.0980171, 0.14673, 0.19509, 0.24298, 0.290285, 0.33689, 0.382683, 0.427555, 0.471397, 0.514103, 0.55557, 0.595699, 0.634393, 0.671559, 0.707107, 0.740951, 0.77301, 0.803208, 0.83147, 0.857729, 0.881921, 0.903989, 0.92388, 0.941544, 0.95694, 0.970031, 0.980785, 0.989177, 0.995185, 0.998795, 1, 0.998795, 0.995185, 0.989177, 0.980785, 0.970031, 0.95694, 0.941544, 0.92388, 0.903989, 0.881921, 0.857729, 0.83147, 0.803208, 0.77301, 0.740951, 0.707107, 0.671559, 0.634393, 0.595699, 0.55557, 0.514103, 0.471397, 0.427555, 0.382683, 0.33689, 0.290285, 0.24298, 0.19509, 0.14673, 0.0980171, 0.0490677, 1.22465e-16, -0.0490677, -0.0980171, -0.14673, -0.19509, -0.24298, -0.290285, -0.33689, -0.382683, -0.427555, -0.471397, -0.514103, -0.55557, -0.595699, -0.634393, -0.671559, -0.707107, -0.740951, -0.77301, -0.803208, -0.83147, -0.857729, -0.881921, -0.903989, -0.92388, -0.941544, -0.95694, -0.970031, -0.980785, -0.989177, -0.995185, -0.998795, -1, -0.998795, -0.995185, -0.989177, -0.980785, -0.970031, -0.95694, -0.941544, -0.92388, -0.903989, -0.881921, -0.857729, -0.83147, -0.803208, -0.77301, -0.740951, -0.707107, -0.671559, -0.634393, -0.595699, -0.55557, -0.514103, -0.471397, -0.427555, -0.382683, -0.33689, -0.290285, -0.24298, -0.19509, -0.14673, -0.0980171, -0.0490677 };

    // These values are tuned to 44.1 kHz sample rate and will be
    // recalculated to match the host sample recalculated

    maybe_unused static const int NUM_COMBS     = 8;
    maybe_unused static const int NUM_ALLPASSES = 4;

    maybe_unused static const int COMB_TUNINGS[ NUM_COMBS ] = { 1116, 1188, 1277, 1356, 1422, 1491, 1557, 1617 };
    maybe_unused static const int ALLPASS_TUNINGS[ NUM_ALLPASSES ] = { 556, 441, 341, 225 };
}
}

#endif
