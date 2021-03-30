using Godot;
using System;

public class main : Node2D
{
	private MidiIO midi_io;

	public override void _Ready()
	{
		midi_io = (MidiIO) GetNode("/root/midi_io");
	}

	private void _on_Button_pressed()
	{

	}
	
	private void _on_Button_button_down()
	{
		midi_io.NoteOn(1, 60, 90);
	}
	
	private void _on_Button2_button_down()
	{
		midi_io.NoteOff(1, 60);
	}


	private void _on_Button3_button_down()
	{
		midi_io.ProgramSelect(1, 0, 20);
	}
}











