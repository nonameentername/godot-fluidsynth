#include "gdmidiplayer.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

void GDMidiAudioStreamPlayer::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_midi_file", "midi_file"), &GDMidiAudioStreamPlayer::set_midi_file);

    ClassDB::bind_method(D_METHOD("get_midi_file"), &GDMidiAudioStreamPlayer::get_midi_file);

    ClassDB::add_property("GDMidiAudioStreamPlayer",
                          PropertyInfo(Variant::OBJECT, "midi_file", PROPERTY_HINT_RESOURCE_TYPE, "MidiFileReader"),
                          "set_midi_file", "get_midi_file");
}

GDMidiAudioStreamPlayer::GDMidiAudioStreamPlayer() {
    in_editor = godot::Engine::get_singleton()->is_editor_hint();
}

GDMidiAudioStreamPlayer::~GDMidiAudioStreamPlayer() {
    if (!in_editor) {
        godot::UtilityFunctions::print("deleting GDMidiAudioStreamPlayer");
        delete_fluid_player(player);
    }
}

void GDMidiAudioStreamPlayer::_ready() {
    godot::UtilityFunctions::print("GDMidiAudioStreamPlayer::_ready");
}

void GDMidiAudioStreamPlayer::_process(float delta) {
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
