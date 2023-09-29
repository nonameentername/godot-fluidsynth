using Godot;
using System;

public partial class main : Node2D
{
    private MidiIO midi_io;
    [Export]
    private AudioStreamPlayer fluidsynth;

    public override void _Ready()
    {
        fluidsynth.Call("note_on", 1, 60, 90);
    }

    public void _on_fluidsynth_ready()
    {
    }

    public override void _Process(double delta)
    {
        //GD.Print(fluidsynth.Playing);
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
            //midi_io.NoteOn(1, 60, 90);
        }
        else
        {
            fluidsynth.Call("note_off", 1, 60);
            //midi_io.NoteOff(1, 60);
        }
    }

    public void _on_CheckButton2_toggled(bool button_pressed)
    {
        if (button_pressed)
        {
            //midi_io.NoteOn(1, 64, 90);
        }
        else
        {
            //midi_io.NoteOff(1, 64);
        }
    }

    public void _on_CheckButton3_toggled(bool button_pressed)
    {
        if (button_pressed)
        {
            //midi_io.NoteOn(3, 67, 90);
        }
        else
        {
            //midi_io.NoteOff(3, 67);
        }
    }

    public void _on_VSlider_value_changed(float value)
    {
        //midi_io.SendControlChannel("cutoff", value);
    }

}
