#include "gdfluidsynth.h"

#include <godot_cpp/classes/engine.hpp>
#include <godot_cpp/variant/utility_functions.hpp>

using namespace godot;

GDFluidSynth::GDFluidSynth() {}

GDFluidSynth::~GDFluidSynth() {}

void GDFluidSynth::_ready() { godot::UtilityFunctions::print("_ready"); }

void GDFluidSynth::_process(double delta) {}

void GDFluidSynth::_notification(int p_what) { Node::_notification(p_what); }

void GDFluidSynth::set_soundfont(Ref<SoundFontFileReader> p_soundfont) {
    soundfont = p_soundfont;
    if (soundfont != NULL && FluidSynth::get_singleton() != NULL) {
        FluidSynth::get_singleton()->set_soundfont(soundfont->get_path());
    }
}

Ref<SoundFontFileReader> GDFluidSynth::get_soundfont() { return soundfont; }

void GDFluidSynth::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_soundfont", "soundfont"),
                         &GDFluidSynth::set_soundfont);
    ClassDB::bind_method(D_METHOD("get_soundfont"),
                         &GDFluidSynth::get_soundfont);
    ClassDB::add_property(
        "GDFluidSynth",
        PropertyInfo(Variant::OBJECT, "soundfont", PROPERTY_HINT_RESOURCE_TYPE,
                     "SoundFontFileReader"),
        "set_soundfont", "get_soundfont");
}
