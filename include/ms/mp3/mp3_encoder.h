#pragma once
#include <vector>
#include <memory>
#include <lame/lame.h>
#include <lame/libmp3lame/lame_global_flags.h>
#pragma comment(lib, "libmp3lame.lib")

namespace ms{
namespace mp3{
    //////////////////////////////////////////////////////////////////////////
    // MP3±àÂëÆ÷
    class mp3_encode
    {
    public:
        mp3_encode(){}
        ~mp3_encode(){ close(); }
        int operator()(short* pcm, int pcmsize
            , unsigned char * mp3, int mp3size)
        {
            return encode(pcm, pcmsize, mp3, mp3size);
        }
        // l = 0, r = 1 ,pcm l&r pcmsize 
        int operator()(short* pcm, int pcmsize
            , unsigned char * mp3, int mp3size, int lorr)
        {
            switch (lorr)
            {
            case 0:
                return encode_l(pcm, pcmsize, mp3, mp3size);
                break;
            case 1:
                return encode_r(pcm, pcmsize, mp3, mp3size);
                break;
            default:
                return encode(pcm, pcmsize, mp3, mp3size);
                break;
            }
        }
        bool Init(int samplerate, int channel)
        {
            if (init_)
            {
                close();
            }
            lame_ = lame_init();
            lame_set_num_channels(lame_, channel);
            lame_set_in_samplerate(lame_, samplerate);
            lame_set_VBR(lame_, vbr_off);
            init_ =  -1 != lame_init_params(lame_);
            return init_;
        }
        int encode(short* pcm, int pcmsize
            , unsigned char * mp3, int mp3size)
        {
            if (!init_)
            {
                return -1;
            }
            if (pcmsize == 0)
                return lame_encode_flush(lame_, mp3, mp3size);
            else
                return lame_encode_buffer_interleaved(lame_, pcm, pcmsize / 2
                , mp3, mp3size);
        }
        int close()
        {
            if (!init_)
            {
                return 0;
            }
            int ret = lame_close(lame_);
            init_ = false;
            lame_ = NULL;
            return ret;
        }
    private:
        int encode_l(short* pcm, int pcmsize
            , unsigned char * mp3, int mp3size)
        {
            if (!init_)
            {
                return -1;
            }

            if (pcmsize == 0)
                return lame_encode_flush(lame_, mp3, mp3size);
            else
                return lame_encode_buffer(lame_, pcm, pcm, pcmsize / 2, mp3, mp3size);
                // return lame_encode_buffer_interleaved(lame_, pcm, pcmsize / lame_->num_channels, mp3, mp3size);
        }
        int encode_r(short* pcm, int pcmsize
            , unsigned char * mp3, int mp3size)
        {
            if (!init_)
            {
                return -1;
            }

            if (pcmsize == 0)
                return lame_encode_flush(lame_, mp3, mp3size);
            else
                return lame_encode_buffer(lame_, pcm+1, pcm+1, pcmsize / 2, mp3, mp3size);
                // return lame_encode_buffer_interleaved(lame_, pcm, pcmsize / lame_->num_channels, mp3, mp3size);
        }

        lame_t lame_{NULL};
        bool init_{false};
    };

    
}
}