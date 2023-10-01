#include "audio_stream_fluidsynth.h"
#include "audio_stream_player_fluidsynth.h"

using namespace godot;

AudioStreamFluidSynth::AudioStreamFluidSynth()
    : mix_rate(44100), stereo(false), hz(639) {}

Ref<AudioStreamPlayback> AudioStreamFluidSynth::_instantiate_playback() {
    Ref<AudioStreamPlaybackFluidSynth> talking_tree;
    talking_tree.instantiate();
    talking_tree->base = Ref<AudioStreamFluidSynth>(this);
    return talking_tree;
}

String AudioStreamFluidSynth::get_stream_name() const { return "FluidSynth"; }

void AudioStreamFluidSynth::reset() { set_position(0); }

void AudioStreamFluidSynth::set_position(uint64_t p) { pos = p; }

float AudioStreamFluidSynth::gen_tone() {
    float inc = 1.0 / (float(mix_rate) / float(hz));
    pos += inc;
    if (pos > 1.0) {
        pos -= 1.0;
    }
    return sin(2.0 * Math_PI * pos);
}

void AudioStreamFluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset"), &AudioStreamFluidSynth::reset);
    ClassDB::bind_method(D_METHOD("get_stream_name"),
                         &AudioStreamFluidSynth::get_stream_name);
}
