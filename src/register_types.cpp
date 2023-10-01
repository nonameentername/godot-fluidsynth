#include "register_types.h"

#include <gdextension_interface.h>

#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/core/defs.hpp>
#include <godot_cpp/godot.hpp>

#include "gdmidiplayer.h"
#include "audiostream_mytone.h"
#include "audiostreamplayer_mytone.h"
#include "gdfluidsynth.h"
#include "midi_file_reader.h"
#include "soundfont_file_reader.h"
#include "audio_stream_fluidsynth.h"
#include "audio_stream_player_fluidsynth.h"


using namespace godot;

void initialize_gdmidiplayer_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }

    ClassDB::register_class<MidiFileReader>();
    ClassDB::register_class<SoundFontFileReader>();
    ClassDB::register_class<GDMidiAudioStreamPlayer>();
    ClassDB::register_class<GDFluidSynth>();
    ClassDB::register_class<AudioStreamPlaybackMyTone>();
    ClassDB::register_class<AudioStreamMyTone>();
    ClassDB::register_class<AudioStreamPlaybackFluidSynth>();
    ClassDB::register_class<AudioStreamFluidSynth>();
}

void uninitialize_gdmidiplayer_module(ModuleInitializationLevel p_level) {
    if (p_level != MODULE_INITIALIZATION_LEVEL_SCENE) {
        return;
    }
}

extern "C" {
// Initialization.
GDExtensionBool GDE_EXPORT
gdmidiplayer_library_init(GDExtensionInterfaceGetProcAddress p_get_proc_address,
                          const GDExtensionClassLibraryPtr p_library,
                          GDExtensionInitialization *r_initialization) {
    godot::GDExtensionBinding::InitObject init_obj(p_get_proc_address,
                                                   p_library, r_initialization);

    init_obj.register_initializer(initialize_gdmidiplayer_module);
    init_obj.register_terminator(uninitialize_gdmidiplayer_module);
    init_obj.set_minimum_library_initialization_level(
        MODULE_INITIALIZATION_LEVEL_SCENE);

    return init_obj.init();
}
}
