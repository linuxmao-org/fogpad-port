/**
 * The MIT License (MIT)
 *
 * Copyright (c) 2019 Jean Pierre Cimalando
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy of
 * this software and associated documentation files (the "Software"), to deal in
 * the Software without restriction, including without limitation the rights to
 * use, copy, modify, merge, publish, distribute, sublicense, and/or sell copies of
 * the Software, and to permit persons to whom the Software is furnished to do so,
 * subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in all
 * copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY, FITNESS
 * FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE AUTHORS OR
 * COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER LIABILITY, WHETHER
 * IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM, OUT OF OR IN
 * CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE SOFTWARE.
 */

#include "SharedFogpad.hpp"
#include "global.h"
#include "paramids.h"

namespace Igorski {
namespace SharedFogpad {

void InitParameter(uint32_t index, Parameter& parameter)
{
    parameter.ranges = ParameterRanges(0.0, 0.0, 1.0);
    parameter.hints = kParameterIsAutomable;

    switch (index) {
    case kReverbSizeId:
        parameter.symbol = "ReverbSize";
        parameter.name = "Size";
        parameter.ranges.def = 0.5;
        break;

    case kReverbWidthId:
        parameter.symbol = "ReverbWidth";
        parameter.name = "Width";
        parameter.ranges.def = 1.0;
        break;

    case kReverbDryMixId:
        parameter.symbol = "ReverbDryMix";
        parameter.name = "Dry mix";
        parameter.ranges.def = 0.5;
        break;

    case kReverbWetMixId:
        parameter.symbol = "ReverbWetMix";
        parameter.name = "Wet mix";
        parameter.ranges.def = 0.5;
        break;

    case kReverbFreezeId:
        parameter.symbol = "ReverbFreeze";
        parameter.name = "Freeze";
        break;

    case kReverbPlaybackRateId:
        parameter.symbol = "ReverbPlaybackRate";
        parameter.name = "Wobble";
        parameter.ranges.def = 0.5;
        break;

    case kBitResolutionId:
        parameter.symbol = "BitResolution";
        parameter.name = "Bother bits";
        parameter.ranges = ParameterRanges(16.0, 1.0, 16.0);
        break;

    case kBitResolutionChainId:
        parameter.symbol = "BitResolutionChain";
        parameter.name = "Bother pre/post";
        parameter.hints |= kParameterIsBoolean|kParameterIsInteger;
        break;

    case kLFOBitResolutionId:
        parameter.symbol = "LFOBitResolution";
        parameter.name = "Bother LFO rate";
        parameter.ranges = ParameterRanges(Igorski::VST::MIN_LFO_RATE(), Igorski::VST::MIN_LFO_RATE(), Igorski::VST::MAX_LFO_RATE());
        parameter.unit = "Hz";
        break;

    case kLFOBitResolutionDepthId:
        parameter.symbol = "LFOBitResolutionDepth";
        parameter.name = "Bother LFO depth";
        parameter.ranges.def = 0.5;
        break;

    case kDecimatorId:
        parameter.symbol = "Decimator";
        parameter.name = "Prick";
        parameter.ranges = ParameterRanges(1.0, 1.0, 32.0);
        parameter.hints |= kParameterIsInteger;
        break;

    case kFilterCutoffId:
        parameter.symbol = "FilterCutoff";
        parameter.name = "Filter cutoff";
        parameter.ranges = ParameterRanges(0.5 * Igorski::VST::FILTER_MAX_FREQ, Igorski::VST::FILTER_MIN_FREQ, Igorski::VST::FILTER_MAX_FREQ);
        parameter.unit = "Hz";
        break;

    case kFilterResonanceId:
        parameter.symbol = "FilterResonance";
        parameter.name = "Filter resonance";
        parameter.ranges = ParameterRanges(Igorski::VST::FILTER_MAX_RESONANCE, Igorski::VST::FILTER_MIN_RESONANCE, Igorski::VST::FILTER_MAX_RESONANCE);
        break;

    case kLFOFilterId:
        parameter.symbol = "LFOFilter";
        parameter.name = "Filter LFO rate";
        parameter.ranges = ParameterRanges(Igorski::VST::MIN_LFO_RATE(), Igorski::VST::MIN_LFO_RATE(), Igorski::VST::MAX_LFO_RATE());
        parameter.unit = "Hz";
        break;

    case kLFOFilterDepthId:
        parameter.symbol = "LFOFilterDepth";
        parameter.name = "Filter LFO depth";
        parameter.ranges.def = 0.5;
        break;

    case kVuPPMId:                 // for the Vu value return to host
        parameter.symbol = "VuPPM";
        parameter.name = "Output gain";
        parameter.hints |= kParameterIsOutput;
        break;

    default:
        DISTRHO_SAFE_ASSERT_RETURN(false, );
    }
}

}
}
