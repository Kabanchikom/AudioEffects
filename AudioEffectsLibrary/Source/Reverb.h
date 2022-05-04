#pragma once

#include <JuceHeader.h>

namespace AudioEffectsLibrary
{
    /// <summary>
    /// Реверберация.
    /// </summary>
    class __declspec(dllexport) Reverb
    {
    public:
        /// <summary>
        /// Инициализирует каналы аудиоэффекта и задает параметры по-умолчанию.
        /// </summary>
        /// <param name="spec">Параметры аудиодорожки.</param>
        void init(juce::dsp::ProcessSpec spec);

        /// Накладывает аудиоээфект на фрагмент аудиозаписи в буфере.
        /// </summary>
        /// <param name="buffer">Аудиобуфер.</param>
        /// <param name="params">Параметры аудиоэффекта.</param>
        void process(juce::dsp::Reverb::DelayParams params, juce::AudioBuffer<float>& buffer);

        ~Reverb();
    private:
        juce::dsp::Reverb::DelayParams params;
        juce::dsp::Reverb left, right;
    };
}
