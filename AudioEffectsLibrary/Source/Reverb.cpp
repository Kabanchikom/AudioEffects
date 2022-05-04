#include "Reverb.h"
namespace AudioEffectsLibrary
{
    void Reverb::init(juce::dsp::ProcessSpec spec)
    {
        left.prepare(spec);
        right.prepare(spec);
    }

    void Reverb::process(juce::dsp::Reverb::DelayParams params, juce::AudioBuffer<float>& buffer)
    {
        left.setParameters(params);
        right.setParameters(params);

        juce::dsp::AudioBlock<float> block(buffer);

        auto leftBlock = block.getSingleChannelBlock(0);
        auto rightBlock = block.getSingleChannelBlock(1);

        juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
        juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

        left.process(leftContext);
        right.process(rightContext);
    }

    Reverb::~Reverb()
    {
    }
}