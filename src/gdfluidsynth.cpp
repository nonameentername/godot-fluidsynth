#include "gdfluidsynth.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GDFluidSynth::GDFluidSynth() {
    in_editor = godot::Engine::get_singleton()->is_editor_hint();

    if (in_editor) {
        return;
    }

    /*
        godot::UtilityFunctions::print("creating GDFluidSynth");
        settings = new_fluid_settings();
        synth = new_fluid_synth(settings);
        fluid_sfloader_t *sfloader = new_fluid_defsfloader(settings);
        fluid_sfloader_set_callbacks(
            sfloader, SoundFontFileReader::sf_open,
       SoundFontFileReader::sf_read, SoundFontFileReader::sf_seek,
       SoundFontFileReader::sf_tell, SoundFontFileReader::sf_close);
        fluid_synth_add_sfloader(synth, sfloader);
        godot::UtilityFunctions::print("done creating GDFluidSynth");
        */
}

GDFluidSynth::~GDFluidSynth() {
    if (in_editor) {
        return;
    }
    godot::UtilityFunctions::print("deleting GDFluidSynth");
    /*
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    */
}

void GDFluidSynth::_ready() {
    if (in_editor) {
        return;
    }

    godot::UtilityFunctions::print("_ready");

    if (stream_player != NULL && !stream_player->is_playing()) {
        stream_player->play();
    }

    stream_playback = stream_player->get_stream_playback();
}

void GDFluidSynth::_process(double delta) {
    if (in_editor) {
        return;
    }

    // godot::UtilityFunctions::print("GDFluidSynth::_process");
    if (stream_player != NULL) {
        // godot::UtilityFunctions::print(stream_player->get_name());
    }

    /*
    if (stream_player != NULL && !stream_player->is_playing()) {
        stream_player->play();
        stream_playback = stream_player->get_stream_playback();
        godot::UtilityFunctions::print("setting stream_playback");
    }
    */

    fill_buffer();
}

void GDFluidSynth::fill_buffer() {
    stream_playback = stream_player->get_stream_playback();
    if (stream_playback == NULL) {
        return;
    }

    // godot::UtilityFunctions::print("steam_playback ", stream_playback);

    // temp
    int64_t to_fill2 = stream_playback->get_frames_available();
    // godot::UtilityFunctions::print("to_fill2 ", to_fill2);

    while (to_fill2 > 0) {
        Vector2 vector = Vector2(1, 1);
        stream_playback->push_frame(vector * sin(phase * Math_TAU));
        float sample_hz = 22050;
        float pulse_hz = 440;
        float increment = pulse_hz / sample_hz;
        phase = fmod(phase + increment, 1);
        to_fill2 -= 1;
    }

    return;
    // endtemp

    int64_t to_fill = stream_playback->get_frames_available();
    if (to_fill > 44100) {
        to_fill = 44100;
    }
    // godot::UtilityFunctions::print("to_fill ", to_fill);

    PackedVector2Array buffer;
    buffer.resize(to_fill);
    Vector2 *data = buffer.ptrw();
    fluid_synth_write_float(synth, to_fill, data, 0, 2, data, 1, 2);

    stream_playback->push_buffer(buffer);
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

    if (in_editor) {
        return;
    }

    /*
    if (soundfont != NULL) {
        char abused_filename[64];
        const void *pointer_to_sf2_in_mem = static_cast<void *>(&soundfont);
        sprintf(abused_filename, "&%p", pointer_to_sf2_in_mem);
        sfont_id = fluid_synth_sfload(synth, abused_filename, 1);
        godot::UtilityFunctions::print("after sfont_id: ", sfont_id);
    }
    */
}

Ref<SoundFontFileReader> GDFluidSynth::get_soundfont() { return soundfont; }

void GDFluidSynth::program_select(int chan, int bank_num, int preset_num) {
    // fluid_synth_program_select(synth, chan, sfont_id, bank_num, preset_num);
}

void GDFluidSynth::note_on(int chan, int key, int vel) {
    // fluid_synth_noteon(synth, chan, key, vel);
}

void GDFluidSynth::note_off(int chan, int key) {
    // fluid_synth_noteoff(synth, chan, key);
}

void GDFluidSynth::pitch_bend(int chan, int val) {
    // fluid_synth_pitch_bend(synth, chan, val);
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
