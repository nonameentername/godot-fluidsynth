#include "gdfluidsynth.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

int position = 0;
long fsize = 0;

void *my_open(const char *filename) {
    void *p;
    if (filename[0] != '&') {
        return NULL;
    }
    sscanf(filename, "&%p", &p);
    return p;
}

int my_read(void *buf, long long count, void *handle) {
    memcpy(buf, (handle + position), count);
    position = position + count;

    return FLUID_OK;
}

int my_seek(void *handle, long long offset, int origin) {
    switch (origin) {
        case SEEK_SET:
            position = offset;
            break;
        case SEEK_CUR:
            position = position + offset;
            break;
        default:
            position = fsize + offset;
            break;
    }

    if (position < 0 || position > fsize) {
        return FLUID_FAILED;
    }

    return FLUID_OK;
}

int my_close(void *handle) { return FLUID_OK; }

long long my_tell(void *handle) { return position; }

GDFluidSynth::GDFluidSynth() {
    godot::UtilityFunctions::print("creating GDFluidSynth");
    buffer = new float[44100 * 2];
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    fluid_sfloader_t *my_sfloader = new_fluid_defsfloader(settings);
    fluid_sfloader_set_callbacks(my_sfloader, my_open, my_read, my_seek,
                                 my_tell, my_close);
    fluid_synth_add_sfloader(synth, my_sfloader);
    godot::UtilityFunctions::print("done creating GDFluidSynth");
}

GDFluidSynth::~GDFluidSynth() {
    godot::UtilityFunctions::print("deleting GDFluidSynth");
    delete buffer;
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
}

void GDFluidSynth::_ready() {
    godot::UtilityFunctions::print("_ready");
    stream_player->play();
    stream_playback = stream_player->get_stream_playback();
    fill_buffer();
}

void GDFluidSynth::_process(double delta) {
    // godot::UtilityFunctions::print("GDFluidSynth::_process");
    if (stream_player != NULL) {
        // godot::UtilityFunctions::print(stream_player->get_name());
    }
    fill_buffer();
}

void GDFluidSynth::fill_buffer() {
    if (stream_playback == NULL) {
        return;
    }
    godot::UtilityFunctions::print("GDFluidSynth::fill_buffer");

    int64_t to_fill = stream_playback->get_frames_available();
    if (to_fill > 44100) {
        to_fill = 44100;
    }

    fluid_synth_write_float(synth, to_fill, buffer, 0, 2, buffer, 1, 2);

    int index = 0;
    while (to_fill > 0) {
        stream_playback->push_frame(Vector2(buffer[index], buffer[index + 1]));
        index = index + 2;
        to_fill = to_fill - 1;
    }
}

void GDFluidSynth::_notification(int p_what) {
    Node::_notification(p_what);

    // godot::UtilityFunctions::print("GDFluidSynth::_notification");
    if (stream_player != NULL) {
        // godot::UtilityFunctions::print(stream_player->get_name());
    }
}

void GDFluidSynth::set_player(AudioStreamPlayer *p_player) {
    stream_player = p_player;
}

AudioStreamPlayer *GDFluidSynth::get_player() { return stream_player; }

void GDFluidSynth::set_soundfont(Ref<SoundFontFileReader> p_soundfont) {
    soundfont = p_soundfont;

    godot::UtilityFunctions::print("before sfont_id: ", sfont_id);

    if (soundfont != NULL) {
        char abused_filename[64];
        const void *pointer_to_sf2_in_mem = soundfont->get_array_data();
        sprintf(abused_filename, "&%p", pointer_to_sf2_in_mem);
        position = 0;
        fsize = soundfont->get_array_size();
        sfont_id = fluid_synth_sfload(synth, abused_filename, 1);
        godot::UtilityFunctions::print("after sfont_id: ", sfont_id);
    }
}

Ref<SoundFontFileReader> GDFluidSynth::get_soundfont() { return soundfont; }

void GDFluidSynth::program_select(int chan, int bank_num, int preset_num) {
    fluid_synth_program_select(synth, chan, sfont_id, bank_num, preset_num);
}

void GDFluidSynth::note_on(int chan, int key, int vel) {
    fluid_synth_noteon(synth, chan, key, vel);
}

void GDFluidSynth::note_off(int chan, int key) {
    fluid_synth_noteoff(synth, chan, key);
}

void GDFluidSynth::pitch_bend(int chan, int val) {
    fluid_synth_pitch_bend(synth, chan, val);
}

void GDFluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_player", "player"),
                         &GDFluidSynth::set_player);
    ClassDB::bind_method(D_METHOD("get_player"), &GDFluidSynth::get_player);
    ClassDB::bind_method(D_METHOD("program_select"),
                         &GDFluidSynth::program_select);
    ClassDB::bind_method(D_METHOD("note_on"), &GDFluidSynth::note_on);
    ClassDB::bind_method(D_METHOD("note_off"), &GDFluidSynth::note_off);
    ClassDB::bind_method(D_METHOD("pitch_bend"), &GDFluidSynth::pitch_bend);
    ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"),
                         &GDFluidSynth::set_soundfont);
    ClassDB::bind_method(D_METHOD("get_soundfont"),
                         &GDFluidSynth::get_soundfont);

    ClassDB::add_property(
        "GDFluidSynth",
        PropertyInfo(Variant::OBJECT, "player", PROPERTY_HINT_NODE_TYPE,
                     "AudioStreamPlayer"),
        "set_player", "get_player");

    ClassDB::add_property(
        "GDFluidSynth",
        PropertyInfo(Variant::OBJECT, "soundfont", PROPERTY_HINT_RESOURCE_TYPE,
                     "SoundFontFileReader"),
        "set_soundfont", "get_soundfont");
}
