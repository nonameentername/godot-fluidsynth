#ifndef SOUNDFONTFILEREADER_H
#define SOUNDFONTFILEREADER_H

#include <fluidsynth.h>

#include <godot_cpp/classes/resource.hpp>
#include <godot_cpp/core/class_db.hpp>
#include <godot_cpp/godot.hpp>
#include <godot_cpp/variant/variant.hpp>

namespace godot {

class SoundFontFileReader : public Resource {
    GDCLASS(SoundFontFileReader, Resource)

private:
    String file;

    int position;
    long array_size;
    char *array_data;
    void clear_data();

public:
    SoundFontFileReader();
    ~SoundFontFileReader();

    void _init();

    void set_data(PackedByteArray data);
    PackedByteArray get_data();
    char *get_array_data();
    long get_array_size();
    String get_extension();

    void open();
    int read(void *buf, long long count);
    int seek(long long offset, int origin);
    long long tell();

    static void *sf_open(const char *filename);
    static int sf_read(void *buf, long long count, void *handle);
    static int sf_seek(void *handle, long long offset, int origin);
    static int sf_close(void *handle);
    static long long sf_tell(void *handle);

    static void _bind_methods();
};

} // namespace godot

#endif
