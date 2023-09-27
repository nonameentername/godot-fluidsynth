Build
-----

Initialize git submodules:

    git submodule update --init --recursive

Compile godot-cpp library:

    make godot-cpp

Install dependencies:

    sudo apt install libfluidsynth-dev abcmidi

Compile gdnative library:

    make
