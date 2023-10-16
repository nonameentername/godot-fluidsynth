using Godot;
using System;

public partial class main : Node2D
{
    private Node fluidsynth;

    [Export(PropertyHint.ResourceType, "MidiFileReader")]
    private Resource midi_file;

    public override void _Ready()
    {
        fluidsynth = (Node)Engine.GetSingleton("FluidSynth");
        fluidsynth.Call("play_midi");
    }

    public void _on_fluidsynth_ready()
    {
        GD.Print("fluidsynth _ready");
    }

    public override bool _Set(StringName property, Variant value)
    {
        GD.Print(property, value);
        return base._Set(property, value);
    }


    public void _on_CheckButton_toggled(bool button_pressed)
    {
        if (button_pressed)
        {
            fluidsynth.Call("note_on", 1, 60, 90);
        }
        else
        {
            fluidsynth.Call("note_off", 1, 60);
        }
    }

    public void _on_CheckButton2_toggled(bool button_pressed)
    {
        if (button_pressed)
        {
            fluidsynth.Call("note_on", 1, 64, 90);
        }
        else
        {
            fluidsynth.Call("note_off", 1, 64);
        }
    }

    public void _on_CheckButton3_toggled(bool button_pressed)
    {
        if (button_pressed)
        {
            fluidsynth.Call("note_on", 13, 67, 90);
        }
        else
        {
            fluidsynth.Call("note_off", 13, 67);
        }
    }

    public void _on_v_slider_value_changed(double value)
    {
        fluidsynth.Call("pitch_bend", 1, value);
    }

    public void _on_button_pressed()
    {
        fluidsynth.Call("play_midi", midi_file);
    }

}
