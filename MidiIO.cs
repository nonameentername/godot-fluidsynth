using Godot;
using System.Collections.Generic;
using System;


public partial class MidiIO : Node
{
    private Node fluidsynth;

    public override void _Ready()
    {
        //fluidsynth = GetNode("/root/fluidsynth");
    }

    public void ProgramSelect(int channel, int bankNum, int presetNum)
    {
        //fluidsynth.Call("program_select", channel, bankNum, presetNum);
    }

    public void NoteOn(int channel, int key, int velocity)
    {
        //fluidsynth.Call("note_on", channel, key, velocity);
    }

    public void NoteOff(int channel, int key)
    {
        //fluidsynth.Call("note_off", channel, key);
    }

    public void SendControlChannel(string channel, float value)
    {
        //fluidsynth.Call("send_control_channel", channel, value);
    }
}
