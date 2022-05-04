#pragma once

#include <JuceHeader.h>

namespace AudioEffectsLibrary
{
    /// <summary>
    /// ������������.
    /// </summary>
    class __declspec(dllexport) Reverb
    {
    public:
        /// <summary>
        /// �������������� ������ ������������ � ������ ��������� ��-���������.
        /// </summary>
        /// <param name="spec">��������� ������������.</param>
        void init(juce::dsp::ProcessSpec spec);

        /// ����������� ����������� �� �������� ����������� � ������.
        /// </summary>
        /// <param name="buffer">����������.</param>
        /// <param name="params">��������� ������������.</param>
        void process(juce::dsp::Reverb::DelayParams params, juce::AudioBuffer<float>& buffer);

        ~Reverb();
    private:
        juce::dsp::Reverb::DelayParams params;
        juce::dsp::Reverb left, right;
    };
}
