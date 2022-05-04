#include "EQ.h"
namespace AudioEffectsLibrary
{
    void EQ::init(juce::dsp::ProcessSpec spec, const ChainSettings& chainSettings)
    {
        EQ::chainSettings = chainSettings;
        EQ::sampleRate = spec.sampleRate;

        leftChain.prepare(spec);
        rightChain.prepare(spec);

        updateFilters(chainSettings);
    }

    void EQ::process(juce::AudioBuffer<float>& buffer, const ChainSettings& chainSettings)
    {
        updateFilters(chainSettings);

        juce::dsp::AudioBlock<float> block(buffer);

        auto leftBlock = block.getSingleChannelBlock(0);
        auto rightBlock = block.getSingleChannelBlock(1);

        juce::dsp::ProcessContextReplacing<float> leftContext(leftBlock);
        juce::dsp::ProcessContextReplacing<float> rightContext(rightBlock);

        leftChain.process(leftContext);
        rightChain.process(rightContext);
    }

    juce::dsp::IIR::Filter<float>::CoefficientsPtr EQ::makePeakFilter(const ChainSettings& chainSettings)
    {
        return juce::dsp::IIR::Coefficients<float>::makePeakFilter(sampleRate,
            chainSettings.peakFreq,
            chainSettings.peakQuality,
            juce::Decibels::decibelsToGain(chainSettings.peakGainInDecibels));
    }

    EQ::~EQ()
    {
    }

    void EQ::updatePeakFilter(const ChainSettings& chainSettings)
    {
        auto peakCoefficients = makePeakFilter(chainSettings);

        //leftChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypassed);
        //rightChain.setBypassed<ChainPositions::Peak>(chainSettings.peakBypassed);

        updateCoefficients(leftChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
        updateCoefficients(rightChain.get<ChainPositions::Peak>().coefficients, peakCoefficients);
    }

    void EQ::updateLowCutFilters(const ChainSettings& chainSettings)
    {
        auto lowCutCoefficients = juce::dsp::FilterDesign<float>::designIIRHighpassHighOrderButterworthMethod(
            chainSettings.lowCutFreq,
            sampleRate,
            2 * (chainSettings.lowCutSlope + 1));

        auto& leftLowCut = leftChain.get<ChainPositions::LowCut>();
        auto& rightLowCut = rightChain.get<ChainPositions::LowCut>();

        updateCutFilter(leftLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
        updateCutFilter(rightLowCut, lowCutCoefficients, chainSettings.lowCutSlope);
    }

    void EQ::updateHighCutFilters(const ChainSettings& chainSettings)
    {
        auto highCutCoefficients = juce::dsp::FilterDesign<float>::designIIRLowpassHighOrderButterworthMethod(
            chainSettings.highCutFreq,
            sampleRate,
            2 * (chainSettings.lowCutSlope + 1));

        auto& leftHighCut = leftChain.get<ChainPositions::HighCut>();
        auto& rightHighCut = rightChain.get<ChainPositions::HighCut>();

        updateCutFilter(leftHighCut, highCutCoefficients, chainSettings.highCutSlope);
        updateCutFilter(rightHighCut, highCutCoefficients, chainSettings.highCutSlope);
    }

    void EQ::updateFilters(const ChainSettings& chainSettings)
    {
        updateLowCutFilters(chainSettings);
        updatePeakFilter(chainSettings);
        updateHighCutFilters(chainSettings);
    }

    void EQ::updateCoefficients(Coefficients& old, const Coefficients& replacements)
    {
        *old = *replacements;
    }
}