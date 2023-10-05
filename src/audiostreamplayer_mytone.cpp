#include "audiostreamplayer_mytone.h"
#include "audiostream_mytone.h"

using namespace godot;

AudioStreamPlaybackMyTone::AudioStreamPlaybackMyTone() : active(false) {
}

AudioStreamPlaybackMyTone::~AudioStreamPlaybackMyTone() {
}

void AudioStreamPlaybackMyTone::_stop() {
    active = false;
    base->reset();
}

void AudioStreamPlaybackMyTone::_start(float p_from_pos) {
    active = true;
    mixed = 0.0;
}

void AudioStreamPlaybackMyTone::_seek(float p_time) {
    float max = _get_length();
    if (p_time < 0) {
        p_time = 0;
    }
    base->set_position(uint64_t(p_time * base->mix_rate) << MIX_FRAC_BITS);
}

int AudioStreamPlaybackMyTone::_mix(AudioFrame *p_buffer, float p_rate, int p_frames) {
    ERR_FAIL_COND_V(!active, 0);
    if (!active) {
        return 0;
    }
    for (int i = 0; i < p_frames; i++) {
        float sample = base->gen_tone();

        AudioFrame audio_frame;
        audio_frame.left = sample;
        audio_frame.right = sample;
        p_buffer[i] = audio_frame;
    }
    float mix_rate = base->mix_rate;
    mixed += p_frames / mix_rate;
    return p_frames;
}

int AudioStreamPlaybackMyTone::_get_loop_count() const {
    return 10;
}

double AudioStreamPlaybackMyTone::_get_playback_position() const {
    return mixed;
}

float AudioStreamPlaybackMyTone::_get_length() const {
    return 0.0;
}

bool AudioStreamPlaybackMyTone::_is_playing() const {
    return active;
}

void AudioStreamPlaybackMyTone::_bind_methods() {
}
