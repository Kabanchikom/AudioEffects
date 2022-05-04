#include "Delay.h"
namespace AudioEffectsLibrary
{
    Delay::~Delay()
    {
    }

    void Delay::init(const DelayParams& params, double sampleRate)
    {
        delayBuffer = juce::AudioSampleBuffer(2, 1);

        parameters.delayLength = 0.5;
        parameters.dryMix = 1.0;
        parameters.wetMix = 0.0;
        parameters.feedback = 0.75;

        delayBufferLength = 1;
        delayReadPosition = 0;
        delayWritePosition = 0;

        parameters = params;

        delayBufferLength = (int)2.0 * sampleRate;
        if (delayBufferLength < 1)
            delayBufferLength = 1;

        delayBuffer.setSize(2, delayBufferLength);
        delayBuffer.clear();

        delayReadPosition = (int)(delayWritePosition - (params.delayLength * sampleRate) + delayBufferLength) % delayBufferLength;
    }

    void Delay::process(int numInputChannels, int numOutputChannels, int numSamples, juce::AudioBuffer<float>& buffer)
    {
        int dpr, dpw;

        for (int channel = 0; channel < numInputChannels; ++channel)
        {
            float* channelData = buffer.getWritePointer(channel);
            float* delayData = delayBuffer.getWritePointer(juce::jmin(channel, delayBuffer.getNumChannels() - 1));

            dpr = delayReadPosition;
            dpw = delayWritePosition;

            for (int i = 0; i < numSamples; ++i) {
                const float in = channelData[i];
                float out = 0.0;

                out = (parameters.dryMix * in + parameters.wetMix * delayData[dpr]);

                delayData[dpw] = in + (delayData[dpr] * parameters.feedback);

                if (++dpr >= delayBufferLength)
                    dpr = 0;
                if (++dpw >= delayBufferLength)
                    dpw = 0;

                channelData[i] = out;
            }
        }

        delayReadPosition = dpr;
        delayWritePosition = dpw;
    }

    void Delay::setParameters(DelayParams params)
    {
        parameters = params;
    }
}
