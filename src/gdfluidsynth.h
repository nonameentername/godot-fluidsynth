#ifndef GDFLUIDSYNTH_H
#define GDFLUIDSYNTH_H

#include <fluidsynth.h>

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_generator_playback.hpp>
#include <godot_cpp/classes/audio_stream_player.hpp>
#include <godot_cpp/classes/project_settings.hpp>
#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>

#include "soundfont_file_reader.h"

namespace godot {

class GDFluidSynth : public Node {
    GDCLASS(GDFluidSynth, Node)

  private:
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    int sfont_id;
    bool in_editor;
    float phase = 0;

    Ref<SoundFontFileReader> soundfont;
    AudioStreamPlayer *stream_player = NULL;
    Ref<AudioStreamGeneratorPlayback> stream_playback;

    void fill_buffer();

  protected:
    void _notification(int p_what);

  public:
    static void _bind_methods();

    GDFluidSynth();
    ~GDFluidSynth();

    void _ready();
    void _process(double delta);

    void set_player(AudioStreamPlayer *p_player);
    AudioStreamPlayer *get_player();

    void set_soundfont(Ref<SoundFontFileReader> p_soundfont);
    Ref<SoundFontFileReader> get_soundfont();

    void program_select(int chan, int bank_num, int preset_num);
    void note_on(int chan, int key, int vel);
    void note_off(int chan, int key);
    // val value (0-16383 with 8192 being center)
    void pitch_bend(int chan, int val);
};
}  // namespace godot

#endif
