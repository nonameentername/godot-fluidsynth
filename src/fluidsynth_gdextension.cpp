#include "fluidsynth_gdextension.h"

using namespace godot;

FluidSynth *FluidSynth::singleton = NULL;

FluidSynth::FluidSynth() {
    buffer = new float[44100 * 2];
    settings = new_fluid_settings();
    synth = new_fluid_synth(settings);
    fluid_sfloader_t *sfloader = new_fluid_defsfloader(settings);
    fluid_sfloader_set_callbacks(sfloader, SoundFontFileReader::sf_open, SoundFontFileReader::sf_read,
                                 SoundFontFileReader::sf_seek, SoundFontFileReader::sf_tell,
                                 SoundFontFileReader::sf_close);
    fluid_synth_add_sfloader(synth, sfloader);
    set_process_internal(true);

    singleton = this;

    call_deferred("initialize");
}

FluidSynth::~FluidSynth() {
    delete_fluid_synth(synth);
    delete_fluid_settings(settings);
    singleton = NULL;
}

FluidSynth *FluidSynth::get_singleton() {
    return singleton;
}

void FluidSynth::set_soundfont(String node_path) {
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
}

Ref<SoundFontFileReader> FluidSynth::get_soundfont() {
    return soundfont;
}

void FluidSynth::set_midi_file(Ref<MidiFileReader> p_midi_file) {
    midi_file = p_midi_file;
}

Ref<MidiFileReader> FluidSynth::get_midi_file() {
    return midi_file;
}

int FluidSynth::gen_tone(AudioFrame *p_buffer, float p_rate, int p_frames) {
    int64_t to_fill = p_frames;
    if (to_fill > 44100) {
        to_fill = 44100;
    }

    int result = fluid_synth_write_float(synth, to_fill, buffer, 0, 2, buffer, 1, 2);
    if (result == FLUID_FAILED) {
        godot::UtilityFunctions::print("failed to write to fluid_synth");
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

void FluidSynth::play_midi(Ref<MidiFileReader> p_midi_file) {
    Ref<MidiFileReader> local_midi_file;

    if (p_midi_file != NULL) {
        if (ResourceLoader::get_singleton()->exists(p_midi_file->get_path())) {
            Variant resource = ResourceLoader::get_singleton()->load(p_midi_file->get_path());
            local_midi_file = resource;
        }
    } else {
        local_midi_file = midi_file;
    }

    if (local_midi_file != NULL) {
        PackedByteArray byte_array = local_midi_file->get_data();
        if (byte_array.size() > 0) {
            fluid_player_t *fluid_player;
            fluid_player = new_fluid_player(synth);

            char midi_file[byte_array.size()];

            for (int i = 0; i < byte_array.size(); i++) {
                midi_file[i] = byte_array[i];
            }

            fluid_player_add_mem(fluid_player, midi_file, byte_array.size());
            fluid_player_play(fluid_player);
            fluid_players.append(fluid_player);
        }
    }
}

void FluidSynth::initialize() {
    Object::cast_to<SceneTree>(Engine::get_singleton()->get_main_loop())->get_root()->add_child(this);
}

void FluidSynth::_notification(int p_what) {
    switch (p_what) {
    case NOTIFICATION_INTERNAL_PROCESS: {
        process(get_process_delta_time());
    }
    }
}

void FluidSynth::process(double delta) {
    Vector<fluid_player_t *> fluid_players_to_remove;
    for (int i = 0; i < fluid_players.size(); ++i) {
        if (fluid_player_get_status(fluid_players[i]) == FLUID_PLAYER_DONE) {
            fluid_players_to_remove.append(fluid_players[i]);
        }
    }
    for (int i = 0; i < fluid_players_to_remove.size(); ++i) {
        delete_fluid_player(fluid_players_to_remove[i]);
        fluid_players.erase(fluid_players_to_remove[i]);
    }
}

void FluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("initialize"), &FluidSynth::initialize);
    ClassDB::bind_method(D_METHOD("process", "delta"), &FluidSynth::process);
    ClassDB::bind_method(D_METHOD("program_select"), &FluidSynth::program_select);
    ClassDB::bind_method(D_METHOD("note_on"), &FluidSynth::note_on);
    ClassDB::bind_method(D_METHOD("note_off"), &FluidSynth::note_off);
    ClassDB::bind_method(D_METHOD("pitch_bend"), &FluidSynth::pitch_bend);
    ClassDB::bind_method(D_METHOD("play_midi"), &FluidSynth::play_midi, DEFVAL(NULL));
    ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"), &FluidSynth::set_soundfont);
    ClassDB::bind_method(D_METHOD("get_soundfont"), &FluidSynth::get_soundfont);
    ClassDB::add_property(
        "FluidSynth", PropertyInfo(Variant::OBJECT, "soundfont", PROPERTY_HINT_RESOURCE_TYPE, "SoundFontFileReader"),
        "set_soundfont", "get_soundfont");
    ClassDB::bind_method(D_METHOD("set_midi_file", "midi_file"), &FluidSynth::set_midi_file);
    ClassDB::bind_method(D_METHOD("get_midi_file"), &FluidSynth::get_midi_file);
    ClassDB::add_property("FluidSynth",
                          PropertyInfo(Variant::OBJECT, "midi_file", PROPERTY_HINT_RESOURCE_TYPE, "MidiFileReader"),
                          "set_midi_file", "get_midi_file");
}
