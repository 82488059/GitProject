#pragma once
// MP3解码
#include <libmad/mad.h>
#include <thread>
#include <memory>
#include <windows.h>
#include <mutex>
#include <ms/log/LogToFile.h>
#include <assert.h>
#pragma comment(lib, "libmad.lib")

#define  USE_OLD_INPUT 1

// mp3 头
struct mp3FrameHeader
{
    unsigned int sync1 : 8;                      //同步信息1

    unsigned int error_protection : 1;       //CRC校验
    unsigned int layer : 2;                        //层
    unsigned int version : 2;                    //版本
    unsigned int sync2 : 3;                      //同步信息2

    unsigned int extension : 1;                 //版权
    unsigned int padding : 1;                   //填充空白字
    unsigned int sample_rate_index : 2;    //采样率索引
    unsigned int bit_rate_index : 4;           //位率索引

    unsigned int emphasis : 2;                 //强调方式
    unsigned int original : 1;                    //原始媒体
    unsigned int copyright : 1;                 //版权标志
    unsigned int mode_extension : 2;        //扩展模式，仅用于联合立体声
    unsigned int channel_mode : 2;          //声道模式
};

class mp3_decoder
{
public:
    enum {DOUBLE_BUFFER = 2, MAX_OUTPUTBUFFER = (48000 * 4 * 2) };
    bool Start();
    void Stop();
    bool RunState() const 
    {
        return run_;
    }
    // Callback functions for decoder
    static enum mad_flow input_func(void *data, struct mad_stream *stream);
    static enum mad_flow header_func(void *data, struct mad_header const*);
    static enum mad_flow filter_func(void *data, struct mad_stream const*stream, struct mad_frame* frame);
    static enum mad_flow output_func(void *data, struct mad_header const *header, struct mad_pcm *pcm);
    static enum mad_flow error_func(void *data, struct mad_stream *stream, struct mad_frame *frame);
    
    unsigned char* FindNextFrame(unsigned char* begin, unsigned char*end)
    {
        while (begin+2 < end)
        {
            ++begin;
            if (0xff == *begin /*&& (begin + 1) != end*/
                && (0xfa == *(begin + 1) || 0xfb == *(begin + 1)))
            {
                return begin;
            }
        }
        return NULL;
    }
    int find_bitrate(int version, int index);
    unsigned char* FindFrame(unsigned char* begin, unsigned char*end)
    {
        while (begin+1 < end)
        {
            if (0xff == *begin
                && (0xfa == *(begin + 1) || 0xfb == *(begin + 1)))
            {
                return begin;
            }
            ++begin;
        }
        return NULL;
    }
    int push(const unsigned char * const buffer, int size);
    int TakePcmData(unsigned char ** pcm);

    typedef std::function<int(unsigned char *pcm, int size)> out_call;
private:
    void DecoderRun();
    bool run_{false};
    bool startdecode_{false};
    struct mad_decoder decoder_;
    std::shared_ptr<std::thread> thread_ptr_;

    struct BUFFER{
        volatile int length{0};
        volatile bool use{ false };
        unsigned char data[MAX_OUTPUTBUFFER];
    };

    // 解码器BUFFER
    std::mutex bufin_mutex_;
    int bufinIndex_{0};
    BUFFER bufin_[DOUBLE_BUFFER]; // 输入缓冲

    std::mutex bufout_mutex_;
    BUFFER bufout_; // 输出缓冲

    // 
    int samplerate_{16000};
    volatile bool frame_error_{ false };
    int frame_size{ 576 };
    int error_frame_size_{0};
    int out_put_size_{0};
    int mp3_kbps_{ -1 };

    mp3FrameHeader head_;
    DWORD frame_play_time_;
public:
    void SetSamplerate(int samplerate)
    {
        samplerate_ = samplerate;
    }
    // Write a block of signed 16-bit little-endian PCM samples
    unsigned int CombinePCM16(unsigned char *data,
        unsigned int nsamples, mad_fixed_t const *left, mad_fixed_t const *right)
    {
        unsigned int len = nsamples;
        register signed int sample0, sample1;

        // Case of stereo
        if (right)
        {
            while (len--)
            {
                sample0 = Scale(16, *left++);
                sample1 = Scale(16, *right++);
                data[0] = sample0 >> 0;
                data[1] = sample0 >> 8;
                data[2] = sample1 >> 0;
                data[3] = sample1 >> 8;
                data += 4;
            }
            return nsamples * 2 * 2;
        }
        else // Case of mono
        {
            while (len--)
            {
                sample0 = Scale(16, *left++);
                data[0] = sample0 >> 0;
                data[1] = sample0 >> 8;
                data += 2;
            }
            return nsamples * 2;
        }
    }
    // Write a block of signed 24-bit little-endian PCM samples
    unsigned int CombinePCM24(unsigned char *data,
        unsigned int nsamples, mad_fixed_t const *left, mad_fixed_t const *right)
    {
        unsigned int len = nsamples;
        register signed long sample0, sample1;

        // Case of stereo
        if (right)
        {
            while (len--)
            {
                sample0 = Scale(24, *left++);
                sample1 = Scale(24, *right++);
                data[0] = sample0 >> 0;
                data[1] = sample0 >> 8;
                data[2] = sample0 >> 16;
                data[3] = sample1 >> 0;
                data[4] = sample1 >> 8;
                data[5] = sample1 >> 16;
                data += 6;
            }
            return nsamples * 3 * 2;
        }
        else // Case of mono
        {
            while (len--)
            {
                sample0 = Scale(24, *left++);
                data[0] = sample0 >> 0;
                data[1] = sample0 >> 8;
                data[2] = sample0 >> 16;
                data += 3;
            }
            return nsamples * 3;
        }
    }

    //
    // The following utility routine performs simple rounding, clipping, and
    // scaling of MAD's high-resolution samples down to specified bits. It does not
    // perform any dithering or noise shaping, which would be recommended to
    // obtain any exceptional audio quality. It is therefore not recommended to
    // use this routine if high-quality output is desired.
    //
    signed int Scale(unsigned int bits, mad_fixed_t sample)
    {
        // Round
        sample += (1L << (MAD_F_FRACBITS - bits));

        // Clip
        if (sample >= MAD_F_ONE)
        {
            sample = MAD_F_ONE - 1;
        }
        else if (sample < -MAD_F_ONE)
        {
            sample = -MAD_F_ONE;
        }

        // Quantize
        return sample >> (MAD_F_FRACBITS + 1 - bits);
    }

    mp3_decoder()
        : run_(false)
    {
    }
    ~mp3_decoder()
    {
    }

};

