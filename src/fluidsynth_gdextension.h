#ifndef FLUIDSYNTHGDEXTENTION_H
#define FLUIDSYNTHGDEXTENTION_H

#include <fluidsynth.h>

#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/node.hpp>
#include <godot_cpp/classes/resource_loader.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

#include "soundfont_file_reader.h"

namespace godot {

class FluidSynth : public Node {
    GDCLASS(FluidSynth, Node);

  private:
    fluid_settings_t *settings;
    fluid_synth_t *synth;
    int sfont_id;
    Ref<SoundFontFileReader> soundfont;
    float *buffer;

  protected:
    static void _bind_methods();
    static FluidSynth *singleton;

  public:
    FluidSynth();
    ~FluidSynth();
    static FluidSynth *get_singleton();
    void set_soundfont(String node_path);
    Ref<SoundFontFileReader> get_soundfont();
    void program_select(int chan, int bank_num, int preset_num);
    void note_on(int chan, int key, int vel);
    void note_off(int chan, int key);
    // val value (0-16383 with 8192 being center)
    void pitch_bend(int chan, int val);
    int gen_tone(AudioFrame *p_buffer, float p_rate, int p_frames);
};
}  // namespace godot

#endif
