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

#include "fluidsynth_gdextension.h"
#include "soundfont_file_reader.h"

namespace godot {

class GDFluidSynth : public Node {
    GDCLASS(GDFluidSynth, Node)

private:
    Ref<SoundFontFileReader> soundfont;

protected:
    void _notification(int p_what);

public:
    static void _bind_methods();

    GDFluidSynth();
    ~GDFluidSynth();

    void _ready();
    void _process(double delta);

    void set_soundfont(Ref<SoundFontFileReader> p_soundfont);
    Ref<SoundFontFileReader> get_soundfont();
};
} // namespace godot

#endif
