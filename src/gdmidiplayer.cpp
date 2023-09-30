#include "gdmidiplayer.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

int position2 = 0;
long fsize2 = 0;

void *my_open2(const char *filename) {
    void *p;
    if (filename[0] != '&') {
        return NULL;
    }
    sscanf(filename, "&%p", &p);
    return p;
}

int my_read2(void *buf, long long count, void *handle) {
    memcpy(buf, (handle + position2), count);
    position2 = position2 + count;

    return FLUID_OK;
}

int my_seek2(void *handle, long long offset, int origin) {
    switch (origin) {
        case SEEK_SET:
            position2 = offset;
            break;
        case SEEK_CUR:
            position2 = position2 + offset;
            break;
        default:
            position2 = fsize2 + offset;
            break;
    }

    if (position2 < 0 || position2 > fsize2) {
        return FLUID_FAILED;
    }

    return FLUID_OK;
}

int my_close2(void *handle) { return FLUID_OK; }

long long my_tell2(void *handle) { return position2; }

void GDMidiAudioStreamPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("program_select"),
                         &GDMidiAudioStreamPlayer::program_select);
    ClassDB::bind_method(D_METHOD("note_on"),
                         &GDMidiAudioStreamPlayer::note_on);
    ClassDB::bind_method(D_METHOD("note_off"),
                         &GDMidiAudioStreamPlayer::note_off);
    ClassDB::bind_method(D_METHOD("pitch_bend"),
                         &GDMidiAudioStreamPlayer::pitch_bend);
    ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"),
                         &GDMidiAudioStreamPlayer::set_soundfont);
    ClassDB::bind_method(D_METHOD("get_soundfont"),
                         &GDMidiAudioStreamPlayer::get_soundfont);

    ClassDB::bind_method(D_METHOD("set_midi_file", "midi_file"),
                         &GDMidiAudioStreamPlayer::set_midi_file);

    ClassDB::bind_method(D_METHOD("get_midi_file"),
                         &GDMidiAudioStreamPlayer::get_midi_file);

    ClassDB::add_property(
        "GDMidiAudioStreamPlayer",
        PropertyInfo(Variant::OBJECT, "soundfont", PROPERTY_HINT_RESOURCE_TYPE,
                     "SoundFontFileReader"),
        "set_soundfont", "get_soundfont");

    ClassDB::add_property(
        "GDMidiAudioStreamPlayer",
        PropertyInfo(Variant::OBJECT, "midi_file", PROPERTY_HINT_RESOURCE_TYPE,
                     "MidiFileReader"),
        "set_midi_file", "get_midi_file");
}

GDMidiAudioStreamPlayer::GDMidiAudioStreamPlayer() {
    in_editor = godot::Engine::get_singleton()->is_editor_hint();

    if (in_editor) {
        set_process_mode(Node::ProcessMode::PROCESS_MODE_DISABLED);
    } else {
        godot::UtilityFunctions::print("creating GDMidiAudioStreamPlayer");
        buffer = new float[44100 * 2];
        settings = new_fluid_settings();

        // fluid_settings_setstr(settings, "audio.driver", "pulseaudio");
        synth = new_fluid_synth(settings);
        player = new_fluid_player(synth);
        fluid_player_set_loop(player, -1);
        fluid_sfloader_t *my_sfloader = new_fluid_defsfloader(settings);
        fluid_sfloader_set_callbacks(my_sfloader, my_open2, my_read2, my_seek2,
                                     my_tell2, my_close2);
        fluid_synth_add_sfloader(synth, my_sfloader);
        fluid_synth_system_reset(synth);
    }
}

GDMidiAudioStreamPlayer::~GDMidiAudioStreamPlayer() {
    if (!in_editor) {
        godot::UtilityFunctions::print("deleting GDMidiAudioStreamPlayer");
        delete buffer;
        delete_fluid_player(player);
        delete_fluid_synth(synth);
        delete_fluid_settings(settings);
    }
}

void GDMidiAudioStreamPlayer::_ready() {
    godot::UtilityFunctions::print("_ready");

    if (!in_editor) {
        play();
        stream_playback = get_stream_playback();
        fill_buffer();
    }
}

void GDMidiAudioStreamPlayer::_process(float delta) {
    // godot::UtilityFunctions::print("_process");
    fill_buffer();
}

// void GDMidiAudioStreamPlayer::_notification(int p_what) {
//
//     AudioStreamPlayer::_notification(p_what);
//
//	switch (p_what) {
//		case NOTIFICATION_READY: {
//			set_process_internal(true);
//		}
//		case NOTIFICATION_INTERNAL_PROCESS: {
//             /*
//             fluid_player_play(player);
//             if (fluid_player_get_status(player) == FLUID_PLAYER_READY) {
//             }
//
//             if (fluid_player_get_status(player) == FLUID_PLAYER_DONE) {
//                 fluid_player_stop(player);
//                 fluid_player_join(player);
//                 fluid_player_seek(player, 0);
//                 fluid_player_play(player);
//             }
//             */
//			break;
//		}
//         //default:
//         //    godot::UtilityFunctions::print("_notification", p_what);
//	}
// }

bool GDMidiAudioStreamPlayer::_set(const String property, const Variant value) {
    godot::UtilityFunctions::print("property: ", property, " value: ", value);
    return AudioStreamPlayer::_set(property, value);
}

void GDMidiAudioStreamPlayer::fill_buffer() {
    if (!is_playing()) {
        return;
    }

    if (stream_playback == NULL) {
        return;
    }

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

void GDMidiAudioStreamPlayer::set_soundfont(
    Ref<SoundFontFileReader> p_soundfont) {
    soundfont = p_soundfont;

    godot::UtilityFunctions::print("before sfont_id: ", sfont_id);

    if (soundfont != NULL) {
        char abused_filename[64];
        const void *pointer_to_sf2_in_mem = soundfont->get_array_data();
        sprintf(abused_filename, "&%p", pointer_to_sf2_in_mem);
        fsize2 = soundfont->get_array_size();
        if (!in_editor) {
            sfont_id = fluid_synth_sfload(synth, abused_filename, 1);
            godot::UtilityFunctions::print("after sfont_id: ", sfont_id);
        }
    }
}

Ref<SoundFontFileReader> GDMidiAudioStreamPlayer::get_soundfont() {
    return soundfont;
}

void GDMidiAudioStreamPlayer::set_midi_file(Ref<MidiFileReader> p_midi_file) {
    midi_file = p_midi_file;

    if (midi_file != NULL) {
        PackedByteArray byte_array = midi_file->get_data();

        if (byte_array.size() > 0) {
            char midi_file[byte_array.size()];

            for (int i = 0; i < byte_array.size(); i++) {
                midi_file[i] = byte_array[i];
            }

            if (!in_editor) {
                fluid_player_add_mem(player, midi_file, byte_array.size());
            }
        }
    }
}

Ref<MidiFileReader> GDMidiAudioStreamPlayer::get_midi_file() {
    return midi_file;
}

void GDMidiAudioStreamPlayer::fluidsynth_play() {}

void GDMidiAudioStreamPlayer::program_select(int chan, int bank_num,
                                             int preset_num) {
    fluid_synth_program_select(synth, chan, sfont_id, bank_num, preset_num);
}

void GDMidiAudioStreamPlayer::note_on(int chan, int key, int vel) {
    fluid_synth_noteon(synth, chan, key, vel);
}

void GDMidiAudioStreamPlayer::note_off(int chan, int key) {
    fluid_synth_noteoff(synth, chan, key);
}

void GDMidiAudioStreamPlayer::pitch_bend(int chan, int val) {
    fluid_synth_pitch_bend(synth, chan, val);
}
