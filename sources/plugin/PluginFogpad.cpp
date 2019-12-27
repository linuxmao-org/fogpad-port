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

#include "PluginFogpad.hpp"
#include "SharedFogpad.hpp"
#include "paramids.h"
#include "calc.h"
#include <math.h>

namespace Igorski {

// -----------------------------------------------------------------------

PluginFogpad::PluginFogpad()
    : Plugin(kNumParameters, 0, 0)
    , fReverbSize( 0.5f )
    , fReverbWidth( 1.f )
    , fReverbDryMix( 0.5f )
    , fReverbWetMix( 0.5f )
    , fReverbFreeze( 0.f )
    , fReverbPlaybackRate( 0.5f )
    , fBitResolution( 0.f )
    , fBitResolutionChain( 0.f )
    , fLFOBitResolution( 0.f )
    , fLFOBitResolutionDepth( 0.5f )
    , fDecimator( 0.f )
    , fFilterCutoff( 0.5f )
    , fFilterResonance( 1.f )
    , fLFOFilter( 0.f )
    , fLFOFilterDepth( 0.5f )
    , outputGain( 0.f )
    , reverbProcess( nullptr )
{
    fParameterRanges = new ParameterRangesSimple[kNumParameters];

    for (unsigned i = 0; i < kNumParameters; ++i) {
        Parameter param;
        SharedFogpad::InitParameter(i, param);
        fParameterRanges[i] = ParameterRangesSimple{param.ranges.def, param.ranges.min, param.ranges.max};
    }

    sampleRateChanged(getSampleRate());

    for (unsigned i = 0; i < kNumParameters; ++i) {
        ParameterRangesSimple range = fParameterRanges[i];
        setParameterValue(i, range.def);
    }
}

PluginFogpad::~PluginFogpad()
{
    // free all allocated resources
    delete reverbProcess;
    delete[] fParameterRanges;
}

// -----------------------------------------------------------------------
// Init

void PluginFogpad::initParameter(uint32_t index, Parameter& parameter) {
    SharedFogpad::InitParameter(index, parameter);
}

// -----------------------------------------------------------------------
// Internal data

/**
  Optional callback to inform the plugin about a sample rate change.
*/
void PluginFogpad::sampleRateChanged(double newSampleRate) {
    reverbProcess = new ReverbProcess( 2, newSampleRate );

    syncModel();
}

/**
  Get the current value of a parameter.
*/
float PluginFogpad::getParameterValue(uint32_t index) const {
    float value;

    switch (index) {
    case kReverbSizeId:
        value = fReverbSize;
        break;

    case kReverbWidthId:
        value = fReverbWidth;
        break;

    case kReverbDryMixId:
        value = fReverbDryMix;
        break;

    case kReverbWetMixId:
        value = fReverbWetMix;
        break;

    case kReverbFreezeId:
        value = fReverbFreeze;
        break;

    case kReverbPlaybackRateId:
        value = fReverbPlaybackRate;
        break;

    case kBitResolutionId:
        value = fBitResolution;
        break;

    case kBitResolutionChainId:
        value = fBitResolutionChain;
        break;

    case kLFOBitResolutionId:
        value = fLFOBitResolution;
        break;

    case kLFOBitResolutionDepthId:
        value = fLFOBitResolutionDepth;
        break;

    case kDecimatorId:
        value = fDecimator;
        break;

    case kFilterCutoffId:
        value = fFilterCutoff;
        break;

    case kFilterResonanceId:
        value = fFilterResonance;
        break;

    case kLFOFilterId:
        value = fLFOFilter;
        break;

    case kLFOFilterDepthId:
        value = fLFOFilterDepth;
        break;

    case kVuPPMId:
        value = outputGain;
        break;

    default:
        DISTRHO_SAFE_ASSERT_RETURN(false, 0.0f);
    }

    ParameterRangesSimple range = fParameterRanges[index];
    return value * (range.max - range.min) + range.min;
}

/**
  Change a parameter value.
*/
void PluginFogpad::setParameterValue(uint32_t index, float value) {
    DISTRHO_SAFE_ASSERT_RETURN(index < kNumParameters, );

    ParameterRangesSimple range = fParameterRanges[index];
    value = (value - range.min) / (range.max - range.min);

    switch (index) {
    case kReverbSizeId:
        fReverbSize = value;
        break;

    case kReverbWidthId:
        fReverbWidth = value;
        break;

    case kReverbDryMixId:
        fReverbDryMix = value;
        break;

    case kReverbWetMixId:
        fReverbWetMix = value;
        break;

    case kReverbFreezeId:
        fReverbFreeze = value;
        break;

    case kReverbPlaybackRateId:
        fReverbPlaybackRate = value;
        break;

    case kBitResolutionId:
        fBitResolution = value;
        break;

    case kBitResolutionChainId:
        fBitResolutionChain = value;
        break;

    case kLFOBitResolutionId:
        fLFOBitResolution = value;
        break;

    case kLFOBitResolutionDepthId:
        fLFOBitResolutionDepth = value;
        break;

    case kDecimatorId:
        fDecimator = value;
        break;

    case kFilterCutoffId:
        fFilterCutoff = value;
        break;

    case kFilterResonanceId:
        fFilterResonance = value;
        break;

    case kLFOFilterId:
        fLFOFilter = value;
        break;

    case kLFOFilterDepthId:
        fLFOFilterDepth = value;
        break;

    case kVuPPMId:
        outputGain = value;
        break;

    default:
        DISTRHO_SAFE_ASSERT_RETURN(false, );
    }

    syncModel();
}

// -----------------------------------------------------------------------
// Process

void PluginFogpad::activate() {
    // plugin is activated
}



void PluginFogpad::run(const float** inputs, float** outputs, uint32_t frames) {
    //-------------------------------------
    //---Process Audio---------------------
    //-------------------------------------

    int32 numInChannels  = DISTRHO_PLUGIN_NUM_INPUTS;
    int32 numOutChannels = DISTRHO_PLUGIN_NUM_OUTPUTS;

    // process the incoming sound!
    reverbProcess->process<float>(
        const_cast<float **>(inputs), outputs, numInChannels, numOutChannels,
        frames, frames * sizeof(float)
    );

    // output flags
    outputGain = reverbProcess->limiter->getLinearGR();
}

// -----------------------------------------------------------------------

void PluginFogpad::syncModel()
{
    reverbProcess->setRoomSize( fReverbSize );
    reverbProcess->setWidth( fReverbWidth );
    reverbProcess->setDry( fReverbDryMix );
    reverbProcess->setWet( fReverbWetMix );
    reverbProcess->setMode( fReverbFreeze );
    reverbProcess->setPlaybackRate( fReverbPlaybackRate );

    reverbProcess->bitCrusherPostMix = Calc::toBool( fBitResolutionChain );

    reverbProcess->bitCrusher->setAmount( fBitResolution );
    reverbProcess->bitCrusher->setLFO( fLFOBitResolution, fLFOBitResolutionDepth );

    // invert the decimator range 0 == max bits (no distortion), 1 == min bits (severely distorted)
    float scaledDecimator = abs( fDecimator - 1.0f );
    int decimation = ( int )( scaledDecimator * 32.f );
    reverbProcess->decimator->setBits( decimation );
    reverbProcess->decimator->setRate( scaledDecimator );

    reverbProcess->filter->updateProperties( fFilterCutoff, fFilterResonance, fLFOFilter, fLFOFilterDepth );
}

// -----------------------------------------------------------------------

}

// -----------------------------------------------------------------------

Plugin* DISTRHO::createPlugin() {
    return new Igorski::PluginFogpad;
}
