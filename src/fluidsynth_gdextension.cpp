#include "fluidsynth_gdextension.h"

using namespace godot;

FluidSynth *FluidSynth::singleton = NULL;

FluidSynth::FluidSynth() {
    godot::UtilityFunctions::print("creating FluidSynth");
    buffer = new float[44100 * 2];
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    fluid_sfloader_t *sfloader = new_fluid_defsfloader(settings);
    fluid_sfloader_set_callbacks(
        sfloader, SoundFontFileReader::sf_open, SoundFontFileReader::sf_read,
        SoundFontFileReader::sf_seek, SoundFontFileReader::sf_tell,
        SoundFontFileReader::sf_close);
    fluid_synth_add_sfloader(synth, sfloader);

    singleton = this;
}

FluidSynth::~FluidSynth() {
    godot::UtilityFunctions::print("deleting FluidSynth");
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    singleton = NULL;
}

FluidSynth *FluidSynth::get_singleton() { return singleton; }

void FluidSynth::set_soundfont(String node_path) {
    godot::UtilityFunctions::print("before sfont_id: ", sfont_id);
    if (ResourceLoader::get_singleton()->exists(node_path)) {
        Variant resource = ResourceLoader::get_singleton()->load(node_path);
        Ref<SoundFontFileReader> soundfont = resource;
        if (soundfont != NULL) {
            char abused_filename[64];
            const void *pointer_to_sf2_in_mem = static_cast<void *>(&soundfont);
            sprintf(abused_filename, "&%p", pointer_to_sf2_in_mem);
            sfont_id = fluid_synth_sfload(synth, abused_filename, 1);
        }
    }
    godot::UtilityFunctions::print("after sfont_id: ", sfont_id);
}

Ref<SoundFontFileReader> FluidSynth::get_soundfont() { return soundfont; }

int FluidSynth::gen_tone(AudioFrame *p_buffer, float p_rate, int p_frames) {
    int64_t to_fill = p_frames;
    if (to_fill > 44100) {
        to_fill = 44100;
    }

    int result =
        fluid_synth_write_float(synth, to_fill, buffer, 0, 2, buffer, 1, 2);
    if (result == FLUID_FAILED) {
        godot::UtilityFunctions::print("failed");
    }

    int index = 0;
    int p_index = 0;
    while (to_fill > 0) {
        p_buffer[p_index].left = buffer[index];
        p_buffer[p_index].right = buffer[index + 1];

        index = index + 2;
        p_index = p_index + 1;
        to_fill = to_fill - 1;
    }

    return p_frames;
}

void FluidSynth::program_select(int chan, int bank_num, int preset_num) {
    fluid_synth_program_select(synth, chan, sfont_id, bank_num, preset_num);
}

void FluidSynth::note_on(int chan, int key, int vel) {
    fluid_synth_noteon(synth, chan, key, vel);
}

void FluidSynth::note_off(int chan, int key) {
    fluid_synth_noteoff(synth, chan, key);
}

void FluidSynth::pitch_bend(int chan, int val) {
    fluid_synth_pitch_bend(synth, chan, val);
}

void FluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("program_select"),
                         &FluidSynth::program_select);
    ClassDB::bind_method(D_METHOD("note_on"), &FluidSynth::note_on);
    ClassDB::bind_method(D_METHOD("note_off"), &FluidSynth::note_off);
    ClassDB::bind_method(D_METHOD("pitch_bend"), &FluidSynth::pitch_bend);
    ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"),
                         &FluidSynth::set_soundfont);
    ClassDB::bind_method(D_METHOD("get_soundfont"), &FluidSynth::get_soundfont);
    ClassDB::add_property(
        "FluidSynth",
        PropertyInfo(Variant::OBJECT, "soundfont", PROPERTY_HINT_RESOURCE_TYPE,
                     "SoundFontFileReader"),
        "set_soundfont", "get_soundfont");
}
