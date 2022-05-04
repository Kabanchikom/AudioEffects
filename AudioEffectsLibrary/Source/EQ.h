#pragma once

#include <JuceHeader.h>

namespace AudioEffectsLibrary
{
    /// <summary>
    /// ����������.
    /// </summary>
    class __declspec(dllexport) EQ
    {
    public:
        /// <summary>
        /// ������� ����� ������, ��/���.
        /// </summary>
        enum Slope
        {
            Slope_12,
            Slope_24,
            Slope_36,
            Slope_48
        };

        /// <summary>
        /// ������� � ���� �����������.
        /// </summary>
        enum ChainPositions
        {
            LowCut,
            Peak,
            HighCut
        };

        /// <summary>
        /// ��������� ���� �����������.
        /// </summary>
        struct ChainSettings
        {
            /// <summary>
            /// ������ ���������� �������, ��,
            /// </summary>
            float peakFreq{ 0 };

            /// <summary>
            /// ������� �������� ���������� ������� ��� �� �� ���������, ��.
            /// </summary>
            float peakGainInDecibels{ 0 };

            /// <summary>
            /// ��������� �������� ������� �������� Peak Gain � Peak Freq.
            /// </summary>
            float peakQuality{ 1.f };

            /// <summary>
            /// ���� ������ ������, ��.
            /// </summary>
            float lowCutFreq{ 0 };

            /// <summary>
            /// ���� ������� ������, ��.
            /// </summary>
            float highCutFreq{ 0 };

            /// <summary>
            /// ������� ����� ������ ������, ��/���.
            /// </summary>
            Slope lowCutSlope{ Slope::Slope_12 };

            /// <summary>
            /// ������� ����� ������� ������, ��/���.
            /// </summary>
            Slope highCutSlope{ Slope::Slope_12 };

            //bool lowCutBypassed{ false }, peakBypassed{ false }, highCutBypassed{ false };
        };

        /// <summary>
        /// �������������� ������ ������������ � ������ ��������� ��-���������.
        /// </summary>
        /// <param name="spec">��������� ������������.</param>
        /// <param name="chainSettings">��������� ������������.</param>
        void init(juce::dsp::ProcessSpec spec, const ChainSettings& chainSettings);

        /// <summary>
        /// ����������� ����������� �� �������� ����������� � ������.
        /// </summary>
        /// <param name="buffer">����������.</param>
        /// <param name="chainSettings">��������� ������������.</param>
        void process(juce::AudioBuffer<float>& buffer, const ChainSettings& chainSettings);

        ~EQ();
    private:
        using Filter = juce::dsp::IIR::Filter<float>;
        using CutFilter = juce::dsp::ProcessorChain<Filter, Filter, Filter, Filter>;
        using MonoChain = juce::dsp::ProcessorChain<CutFilter, Filter, CutFilter>;
        using Coefficients = Filter::CoefficientsPtr;

        double sampleRate;
        ChainSettings chainSettings;
        MonoChain leftChain, rightChain;

        void updatePeakFilter(const ChainSettings& chainSettings);
        void updateLowCutFilters(const ChainSettings& chainSettings);
        void updateHighCutFilters(const ChainSettings& chainSettings);
        void updateFilters(const ChainSettings& chainSettings);

        static void updateCoefficients(Coefficients& old, const Coefficients& replacements);

        Coefficients makePeakFilter(const ChainSettings& chainSettings);

        template<int Index, typename ChainType, typename CoefficientType>
        void update(ChainType& chain, const CoefficientType& coefficients)
        {
            updateCoefficients(chain.template get<Index>().coefficients, coefficients[Index]);
            chain.template setBypassed<Index>(false);
        }

        template<typename ChainType, typename CoefficientType>
        void updateCutFilter(ChainType& chain,
            const CoefficientType& coefficients,
            const Slope& slope)
        {
            chain.template setBypassed<0>(true);
            chain.template setBypassed<1>(true);
            chain.template setBypassed<2>(true);
            chain.template setBypassed<3>(true);

            switch (slope)
            {
                case Slope_48:
                {
                    update<3>(chain, coefficients);
                }
                case Slope_36:
                {
                    update<2>(chain, coefficients);
                }
                case Slope_24:
                {
                    update<1>(chain, coefficients);
                }
                case Slope_12:
                {
                    update<0>(chain, coefficients);
                }
            }
        }
    };
}
