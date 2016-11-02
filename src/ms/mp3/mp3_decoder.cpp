#pragma once
#include "stdafx.h"
// MP3½âÂë
#include <ms/mp3/mp3_decoder.h>
#include <AudioService/services/AudioAnalyse.h>
int mp3_decoder::find_bitrate(int version, int index)
{
    static long br[2][15] =
    { { 0, 8, 16, 24, 32, 40, 48, 56, 64, 80, 96, 112, 128, 144, 160 },   /* mpeg 2/2.5 */
    { 0, 32, 40, 48, 56, 64, 80, 96, 112, 128, 160, 192, 224, 256, 320 } };  /* mpeg 1 */
//    int i;
    return br[version & 1][index];
}

int mp3_decoder::push(const unsigned char * const buffer, int size)
{
    if (*buffer != 255)
    {
        DebugPrint(_T("mp3 recv error "));
    }
    if (-1 == mp3_kbps_)
    {
        memcpy(&head_, buffer, sizeof(head_));
        mp3_kbps_ = find_bitrate(head_.version, head_.bit_rate_index);
    }
    while (bufin_[bufinIndex_].use)
    {
        Sleep(1);
    }
    std::lock_guard<std::mutex> lck(bufin_mutex_);
    memcpy(bufin_[bufinIndex_].data + bufin_[bufinIndex_].length, buffer, size);
    bufin_[bufinIndex_].length += size;
    return size;
}

int mp3_decoder::TakePcmData(unsigned char ** pcm)
{
    if (bufout_.length == 0)
    {
        return 0;
    }
    std::lock_guard<std::mutex> lck(bufout_mutex_);
    int size = bufout_.length;
    *pcm = new unsigned char[size];
    memcpy(*pcm, bufout_.data, size);
    bufout_.length = 0;
    return size;
}

void mp3_decoder::DecoderRun()
{
    mad_decoder_options(&decoder_, MAD_OPTION_IGNORECRC);
    // Configure input, output, and error functions
    mad_decoder_init(&decoder_,
        this,
        input_func,	 // Input callback
        0,           // Header callback
        filter_func, // Filter callback
        output_func, // Output callback
        error_func,  // Error callback
        0			 // Message callback
        );

    // Start decoding 
    /*nResult = */mad_decoder_run(&decoder_, MAD_DECODER_MODE_SYNC);

    //return nResult;
}

void mp3_decoder::Stop()
{
    if (run_)
    {
        mad_decoder_finish(&decoder_);
        if (thread_ptr_)
        {
            thread_ptr_->join();
        }
        run_ = false;
    }
}

bool mp3_decoder::Start()
{
    if (!thread_ptr_)
    {
        run_ = true;
        thread_ptr_ = std::make_shared<std::thread>(std::bind(&mp3_decoder::DecoderRun, this));
    }
    return run_;
}

enum mad_flow mp3_decoder::error_func(void *data, struct mad_stream *stream, struct mad_frame *frame)
{
    mp3_decoder* pThis = (mp3_decoder*)data;
//     DebugPrint(_T("mp3 frame error %d, error code %d "), pThis->error_frame_size_++, stream->error);
//     DebugPrint(_T(" ÎóÂëÂÊ %f, %d"), pThis->error_frame_size_*1.0 / (pThis->error_frame_size_ + pThis->out_put_size_), stream->bufend-stream->buffer);
//     DebugPrint(_T("no decoder %d"), stream->bufend - stream->next_frame);
    // return MAD_FLOW_IGNORE;
    return MAD_FLOW_CONTINUE;
}

enum mad_flow mp3_decoder::input_func(void *data, struct mad_stream *stream)
{
    mp3_decoder* pThis = (mp3_decoder*)data;

#if USE_OLD_INPUT
    {
        std::lock_guard<std::mutex> lck(pThis->bufin_mutex_);

        int oldLength = stream->bufend - stream->next_frame;
        int nextIndex = (pThis->bufinIndex_ + 1) % DOUBLE_BUFFER;
        BUFFER *next = &pThis->bufin_[nextIndex];
        for (int i = next->length - 1; i >= 0; --i)
        {
            next->data[i + oldLength] = next->data[i];
        }
        memcpy(next->data, stream->next_frame, oldLength);
        next->length += oldLength;
    }
#endif

    pThis->bufin_[pThis->bufinIndex_].use = false;
    pThis->bufin_[pThis->bufinIndex_].length = 0;
    
    pThis->bufinIndex_ = (pThis->bufinIndex_+1)%DOUBLE_BUFFER;
    while (true)
    {
        if (!pThis->run_)
        {
            return MAD_FLOW_STOP;
        }
        if (pThis->startdecode_)
        {
            if (pThis->bufin_[pThis->bufinIndex_].length > 1152)
            {
                break;
            }
        }
        else
        {
            if (pThis->bufin_[pThis->bufinIndex_].length > 4000)
            {
                pThis->startdecode_ = true;
                break;
            }
        }
        Sleep(5);
    }
    pThis->bufin_[pThis->bufinIndex_].use = true;//state
    mad_stream_buffer(stream, pThis->bufin_[pThis->bufinIndex_].data, pThis->bufin_[pThis->bufinIndex_].length);
    return MAD_FLOW_CONTINUE;    
}

enum mad_flow mp3_decoder::output_func(void *data, struct mad_header const *header, struct mad_pcm *pcm)
{
    mp3_decoder* pThis = (mp3_decoder*)data;
    //CLibmadDecoder* pThis = (CLibmadDecoder*)data;

    mad_fixed_t const *left_ch = pcm->samples[0];
    mad_fixed_t const *right_ch = pcm->samples[1];
    //         static unsigned int sample_count = 0;
//    signed int sample0, sample1;
    int nsamples = pcm->length;
    //         float current_time = 0;

    // Calculate the current time
    //         sample_count += nsamples;
    //         current_time = (float)sample_count / pcm->samplerate;

    // Case of mono
    if (pcm->channels == 1)
    {
        right_ch = NULL;
    }
    unsigned char buf[MAX_OUTPUTBUFFER];
    int length = pThis->CombinePCM16(&buf[0], nsamples, left_ch, right_ch);
    pThis->out_put_size_++;

#if USE_CALLBACK
    if (pThis->out_func_)
    {
        pThis->out_func_(buf, length);
    }
#else
    while (pThis->bufout_.length + length > MAX_OUTPUTBUFFER)
    {
        return MAD_FLOW_CONTINUE;
    }
    std::lock_guard<std::mutex> lck(pThis->bufout_mutex_);
    memcpy(pThis->bufout_.data + pThis->bufout_.length, buf, length);
    pThis->bufout_.length += length;

#endif
    return MAD_FLOW_CONTINUE;
}


enum mad_flow mp3_decoder::header_func(void *data, struct mad_header const*)
{
    mp3_decoder* pThis = (mp3_decoder*)data;
    return MAD_FLOW_CONTINUE;
}

enum mad_flow mp3_decoder::filter_func(void *data, struct mad_stream const*stream, struct mad_frame* frame)
{
    mp3_decoder* pThis = (mp3_decoder*)data;
    return MAD_FLOW_CONTINUE;
}
