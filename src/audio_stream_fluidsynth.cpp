#include "audio_stream_fluidsynth.h"
#include "audio_stream_player_fluidsynth.h"
#include "fluidsynth_gdextension.h"

#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

AudioStreamFluidSynth::AudioStreamFluidSynth() : mix_rate(44100), stereo(false), hz(639) {
}

AudioStreamFluidSynth::~AudioStreamFluidSynth() {
}

Ref<AudioStreamPlayback> AudioStreamFluidSynth::_instantiate_playback() const {
    Ref<AudioStreamPlaybackFluidSynth> talking_tree;
    talking_tree.instantiate();
    talking_tree->base = Ref<AudioStreamFluidSynth>(this);
    return talking_tree;
}

String AudioStreamFluidSynth::get_stream_name() const {
    return "FluidSynth";
}

void AudioStreamFluidSynth::reset() {
    set_position(0);
}

void AudioStreamFluidSynth::set_position(uint64_t p) {
    pos = p;
}

int AudioStreamFluidSynth::gen_tone(AudioFrame *p_buffer, float p_rate, int p_frames) {
    if (FluidSynth::get_singleton() != NULL) {
        return FluidSynth::get_singleton()->gen_tone(p_buffer, p_rate, p_frames);
    }
    return 0;
}

void AudioStreamFluidSynth::set_soundfont(Ref<SoundFontFileReader> p_soundfont) {
    soundfont = p_soundfont;
    if (soundfont != NULL && FluidSynth::get_singleton() != NULL) {
        FluidSynth::get_singleton()->set_soundfont(soundfont->get_path());
    }
}

Ref<SoundFontFileReader> AudioStreamFluidSynth::get_soundfont() {
    return soundfont;
}

void AudioStreamFluidSynth::set_midi_file(Ref<MidiFileReader> p_midi_file) {
    midi_file = p_midi_file;
    if (midi_file != NULL && FluidSynth::get_singleton() != NULL) {
        FluidSynth::get_singleton()->set_midi_file(midi_file);
    }
}

Ref<MidiFileReader> AudioStreamFluidSynth::get_midi_file() {
    return midi_file;
}

void AudioStreamFluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("reset"), &AudioStreamFluidSynth::reset);
    ClassDB::bind_method(D_METHOD("get_stream_name"), &AudioStreamFluidSynth::get_stream_name);
    ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"), &AudioStreamFluidSynth::set_soundfont);
    ClassDB::bind_method(D_METHOD("get_soundfont"), &AudioStreamFluidSynth::get_soundfont);
    ClassDB::add_property(
        "AudioStreamFluidSynth",
        PropertyInfo(Variant::OBJECT, "soundfont", PROPERTY_HINT_RESOURCE_TYPE, "SoundFontFileReader"), "set_soundfont",
        "get_soundfont");
    ClassDB::bind_method(D_METHOD("set_midi_file", "midi_file"), &AudioStreamFluidSynth::set_midi_file);
    ClassDB::bind_method(D_METHOD("get_midi_file"), &AudioStreamFluidSynth::get_midi_file);
    ClassDB::add_property("AudioStreamFluidSynth",
                          PropertyInfo(Variant::OBJECT, "midi_file", PROPERTY_HINT_RESOURCE_TYPE, "MidiFileReader"),
                          "set_midi_file", "get_midi_file");
}
