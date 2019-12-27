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

#ifndef PLUGIN_FOGPAD_H
#define PLUGIN_FOGPAD_H

#include "DistrhoPlugin.hpp"
#include "reverbprocess.h"
#include "global.h"

namespace Igorski {

// -----------------------------------------------------------------------

class PluginFogpad : public Plugin {
public:
    PluginFogpad();
    ~PluginFogpad();

protected:
    // -------------------------------------------------------------------
    // Information

    const char* getLabel() const noexcept override {
        return "Fogpad";
    }

    const char* getDescription() const override {
        return "A reverb effect in which the reflections can be frozen, filtered, pitch shifted and ultimately disintegrated";
    }

    const char* getMaker() const noexcept override {
        return "Igorski";
    }

    const char* getHomePage() const override {
        return "https://www.igorski.nl/download/fogpad--reverb-plugin";
    }

    const char* getLicense() const noexcept override {
        return "https://spdx.org/licenses/MIT";
    }

    uint32_t getVersion() const noexcept override {
        return d_version(0, 1, 0);
    }

    // Go to:
    //
    // http://service.steinberg.de/databases/plugin.nsf/plugIn
    //
    // Get a proper plugin UID and fill it in here!
    //
    // jpc: below value is not a "proper UID", but who cares
    int64_t getUniqueId() const noexcept override {
        return d_cconst('F', 'o', 'g', 'p');
    }

    // -------------------------------------------------------------------
    // Init

    void initParameter(uint32_t index, Parameter& parameter) override;

    // -------------------------------------------------------------------
    // Internal data

    float getParameterValue(uint32_t index) const override;
    void setParameterValue(uint32_t index, float value) override;

    // -------------------------------------------------------------------
    // Optional

    // Optional callback to inform the plugin about a sample rate change.
    void sampleRateChanged(double newSampleRate) override;

    // -------------------------------------------------------------------
    // Process

    void activate() override;

    void run(const float**, float** outputs, uint32_t frames) override;


    // -------------------------------------------------------------------

private:
    float fReverbSize;
    float fReverbWidth;
    float fReverbDryMix;
    float fReverbWetMix;
    float fReverbFreeze;
    float fReverbPlaybackRate;

    float fBitResolution;
    float fBitResolutionChain;
    float fLFOBitResolution;
    float fLFOBitResolutionDepth;

    float fDecimator;

    float fFilterCutoff;
    float fFilterResonance;
    float fLFOFilter;
    float fLFOFilterDepth;

    float outputGain; // for visualizing output gain in DAW

    Igorski::ReverbProcess* reverbProcess;

    // synchronize the processors model with UI led changes

    void syncModel();

    // -------------------------------------------------------------------

    struct ParameterRangesSimple
    {
        float def, min, max;
    };

    ParameterRangesSimple *fParameterRanges;

    // -------------------------------------------------------------------

    DISTRHO_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR(PluginFogpad)
};

// -----------------------------------------------------------------------

}

#endif  // #ifndef PLUGIN_FOGPAD_H
