#ifndef GDMIDIPLAYER_H
#define GDMIDIPLAYER_H

#include <fluidsynth.h>

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "audiostream_mytone.h"
#include "audiostreamplayer_mytone.h"
#include "gdfluidsynth.h"
#include "midi_file_reader.h"
#include "soundfont_file_reader.h"

namespace godot {

class GDMidiAudioStreamPlayer : public AudioStreamPlayer {
    GDCLASS(GDMidiAudioStreamPlayer, AudioStreamPlayer)

  private:
    Ref<MidiFileReader> midi_file;
    bool in_editor;

    fluid_player_t *player;

  public:
    static void _bind_methods();

    GDMidiAudioStreamPlayer();
    ~GDMidiAudioStreamPlayer();

    void _ready();  // our initializer called by Godot
    void _process(float delta);

    void set_midi_file(Ref<MidiFileReader> p_midi_file);
    Ref<MidiFileReader> get_midi_file();
};

}  // namespace godot

#endif
