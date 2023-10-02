#include "audio_stream_fluidsynth.h"

#include <godot_cpp/variant/utility_functions.hpp>

#include "audio_stream_player_fluidsynth.h"

using namespace godot;

AudioStreamFluidSynth::AudioStreamFluidSynth()
    : mix_rate(44100), stereo(false), hz(639) {}

AudioStreamFluidSynth::~AudioStreamFluidSynth() {}

Ref<AudioStreamPlayback> AudioStreamFluidSynth::_instantiate_playback() {
    Ref<AudioStreamPlaybackFluidSynth> talking_tree;
    talking_tree.instantiate();
    talking_tree->base = Ref<AudioStreamFluidSynth>(this);
    return talking_tree;
}

String AudioStreamFluidSynth::get_stream_name() const { return "FluidSynth"; }

void AudioStreamFluidSynth::reset() { set_position(0); }

void AudioStreamFluidSynth::set_position(uint64_t p) { pos = p; }

int AudioStreamFluidSynth::gen_tone(AudioFrame *p_buffer, float p_rate,
                                    int p_frames) {
    if (FluidSynth::get_singleton() != NULL) {
        return FluidSynth::get_singleton()->gen_tone(p_buffer, p_rate,
                                                     p_frames);
    }
    return 0;
}

void AudioStreamFluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset"), &AudioStreamFluidSynth::reset);
    ClassDB::bind_method(D_METHOD("get_stream_name"),
                         &AudioStreamFluidSynth::get_stream_name);
}
