#pragma once

#include <JuceHeader.h>

namespace AudioEffectsLibrary
{
    class __declspec(dllexport) Delay
    {
    public:
        /// <summary>
        /// ������ ��������� ������� Delay.
        /// </summary>
        struct DelayParams
        {
            /// <summary>
            /// "�����" �������� � ����� �������� ����� ���-���������, ��.
            /// </summary>
            float delayLength;

            /// <summary>
            /// ������� ��������� ��������������� ��������� �������, %.
            /// </summary>
            float dryMix;

            /// <summary>
            /// ������� ��������� ������������� ��������� �������, %.
            /// </summary>
            float wetMix;

            /// <summary>
            /// ������� ��������� ���, %.
            /// </summary>
            float feedback;
        };

        /// <summary>
        /// �������������� ������ ������������ � ������ ��������� ��-���������.
        /// </summary>
        /// <param name="spec">��������� ������������.</param>
        /// <param name="params">��������� ������������.</param>
        void init(const DelayParams& params, double sampleRate);

        
        /// <summary>
        /// ����������� ����������� �� �������� ����������� � ������.
        /// </summary>
        /// <param name="numInputChannels">����� ������� �������.</param>
        /// <param name="numOutputChannels">����� �������� �������.</param>
        /// <param name="numSamples">����� �������������� �������.</param>
        /// <param name="buffer">����������.</param>
        void process(int numInputChannels, int numOutputChannels, int numSamples, juce::AudioBuffer<float>& buffer);

        /// <summary>
        /// ���������� ��������� ������������.
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