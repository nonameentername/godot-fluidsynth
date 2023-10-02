#include "audio_stream_player_fluidsynth.h"

#include "audio_stream_fluidsynth.h"

using namespace godot;

AudioStreamPlaybackFluidSynth::AudioStreamPlaybackFluidSynth()
    : active(false) {}

AudioStreamPlaybackFluidSynth::~AudioStreamPlaybackFluidSynth() {}

void AudioStreamPlaybackFluidSynth::_stop() {
    active = false;
    base->reset();
}

void AudioStreamPlaybackFluidSynth::_start(float p_from_pos) {
    active = true;
    mixed = 0.0;
}

void AudioStreamPlaybackFluidSynth::_seek(float p_time) {
    float max = _get_length();
    if (p_time < 0) {
        p_time = 0;
    }
    base->set_position(uint64_t(p_time * base->mix_rate) << MIX_FRAC_BITS);
}

int AudioStreamPlaybackFluidSynth::_mix(AudioFrame *p_buffer, float p_rate,
                                        int p_frames) {
    ERR_FAIL_COND_V(!active, 0);
    if (!active) {
        return 0;
    }

    int frames = base->gen_tone(p_buffer, p_rate, p_frames);
    float mix_rate = base->mix_rate;
    mixed += frames / mix_rate;
    return frames;
}

int AudioStreamPlaybackFluidSynth::_get_loop_count() const { return 10; }

double AudioStreamPlaybackFluidSynth::_get_playback_position() const {
    return 0;
}

float AudioStreamPlaybackFluidSynth::_get_length() const { return 0.0; }

bool AudioStreamPlaybackFluidSynth::_is_playing() const { return active; }

void AudioStreamPlaybackFluidSynth::_bind_methods() {}
