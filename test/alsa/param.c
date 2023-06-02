//#include "utils.h"
#include <alsa/asoundlib.h>
#include <stdlib.h>

int main ( int argc, char *argv[] )
{
    int i;
    int ret;
    int dir;
    unsigned int val, val2;
    snd_pcm_t* handle;
    snd_pcm_hw_params_t* params;
    //1. open
    if( (ret = snd_pcm_open(&handle, "default", SND_PCM_STREAM_PLAYBACK, 0)) < 0)
    {
        printf("open pcm device error:%s\n", snd_strerror(ret));
        return -1;
    }
    //2. alloc and init param
    snd_pcm_hw_params_alloca(&params);
    snd_pcm_hw_params_any(handle, params);
    snd_pcm_hw_params_set_access(handle, params, SND_PCM_ACCESS_RW_INTERLEAVED);
    snd_pcm_hw_params_set_format(handle, params, SND_PCM_FORMAT_S16_LE);
    snd_pcm_hw_params_set_channels(handle, params, 2);
    val = 44100;
    snd_pcm_hw_params_set_rate_near(handle,params, &val, &dir);
    
    //3. set param to driver
    if((ret=snd_pcm_hw_params(handle, params)) < 0)
    {
        printf("set hw params error:%s\n", snd_strerror(ret));
        return -1;
    }
    
    printf("PCM handle name=%s\n", snd_pcm_name(handle));
    printf("PCM state=%s\n", snd_pcm_state_name(snd_pcm_state(handle)));
    
    snd_pcm_hw_params_get_access(params, (snd_pcm_access_t*)&val);
    printf("access type=%s\n",snd_pcm_access_name((snd_pcm_access_t)val));

    snd_pcm_hw_params_get_format(params, &val);
    printf("format=%s (%s)\n",snd_pcm_format_name((snd_pcm_format_t)val),
            snd_pcm_format_description((snd_pcm_format_t)val));
    
    snd_pcm_close(handle);
    return EXIT_SUCCESS;
}
