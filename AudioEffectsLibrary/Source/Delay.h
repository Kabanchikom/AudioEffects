#pragma once

#include <JuceHeader.h>

namespace AudioEffectsLibrary
{
    class __declspec(dllexport) Delay
    {
    public:
        /// <summary>
        /// Хранит параметры эффекта Delay.
        /// </summary>
        struct DelayParams
        {
            /// <summary>
            /// "Длина" задержки – время задержки между эхо-сигналами, мс.
            /// </summary>
            float delayLength;

            /// <summary>
            /// Уровень громкости необработанного звукового сигнала, %.
            /// </summary>
            float dryMix;

            /// <summary>
            /// Уровень громкости обработанного звукового сигнала, %.
            /// </summary>
            float wetMix;

            /// <summary>
            /// Уровень громкости эха, %.
            /// </summary>
            float feedback;
        };

        /// <summary>
        /// Инициализирует каналы аудиоэффекта и задает параметры по-умолчанию.
        /// </summary>
        /// <param name="spec">Параметры аудиодорожки.</param>
        /// <param name="params">Параметры аудиоэффекта.</param>
        void init(const DelayParams& params, double sampleRate);

        
        /// <summary>
        /// Накладывает аудиоээфект на фрагмент аудиозаписи в буфере.
        /// </summary>
        /// <param name="numInputChannels">Число входных каналов.</param>
        /// <param name="numOutputChannels">Число выходных каналов.</param>
        /// <param name="numSamples">Число обрабатываемых сэмплов.</param>
        /// <param name="buffer">Аудиобуфер.</param>
        void process(int numInputChannels, int numOutputChannels, int numSamples, juce::AudioBuffer<float>& buffer);

        /// <summary>
        /// Установить параметры аудиоэффекта.
        /// </summary>
        void setParameters(DelayParams params);

        ~Delay();
        
    private:
        juce::AudioSampleBuffer delayBuffer;
        DelayParams parameters;
        int delayBufferLength;
        int delayReadPosition;
        int delayWritePosition;
    };
}