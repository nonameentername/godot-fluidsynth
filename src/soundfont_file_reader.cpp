#include "soundfont_file_reader.h"

using namespace godot;

SoundFontFileReader::SoundFontFileReader() {
    array_data = NULL;
}

SoundFontFileReader::~SoundFontFileReader() {
    /*
    if (array_data != NULL) {
            memfree(array_data);
    }
    */
}

void SoundFontFileReader::_init() {
}

void SoundFontFileReader::clear_data() {
}

void SoundFontFileReader::set_data(PackedByteArray data) {
    PackedByteArray in_array = data;

    /*
    if (array_data != NULL) {
            memfree(array_data);
    }
    */

    array_size = in_array.size();
    array_data = (char *)memalloc((array_size + 1) * sizeof(char));

    for (int i = 0; i < array_size; i++) {
        array_data[i] = in_array[i];
    }
    array_data[array_size] = 0;
}

PackedByteArray SoundFontFileReader::get_data() {
    PackedByteArray out_array;
    for (int i = 0; i < array_size; i++) {
        out_array.append(array_data[i]);
    }
    return out_array;
}

char *SoundFontFileReader::get_array_data() {
    return array_data;
}

long SoundFontFileReader::get_array_size() {
    return array_size;
}

String SoundFontFileReader::get_extension() {
    return "sf2str";
}

void SoundFontFileReader::open() {
    position = 0;
}

int SoundFontFileReader::read(void *buf, long long count) {
    memcpy(buf, (array_data + position), count);
    position = position + count;

    return FLUID_OK;
}

int SoundFontFileReader::seek(long long offset, int origin) {
    switch (origin) {
    case SEEK_SET:
        position = offset;
        break;
    case SEEK_CUR:
        position = position + offset;
        break;
    default:
        position = array_size + offset;
        break;
    }

    if (position < 0 || position > array_size) {
        return FLUID_FAILED;
    }

    return FLUID_OK;
}

long long SoundFontFileReader::tell() {
    return position;
}

void *SoundFontFileReader::sf_open(const char *filename) {
    void *p;
    if (filename[0] != '&') {
        return NULL;
    }
    sscanf(filename, "&%p", &p);
    Ref<SoundFontFileReader> *reader = reinterpret_cast<Ref<SoundFontFileReader> *>(const_cast<void *>(p));
    (*reader)->open();
    return p;
}

int SoundFontFileReader::sf_read(void *buf, long long count, void *handle) {
    Ref<SoundFontFileReader> *reader = reinterpret_cast<Ref<SoundFontFileReader> *>(const_cast<void *>(handle));
    return (*reader)->read(buf, count);
}

int SoundFontFileReader::sf_seek(void *handle, long long offset, int origin) {
    Ref<SoundFontFileReader> *reader = reinterpret_cast<Ref<SoundFontFileReader> *>(const_cast<void *>(handle));
    return (*reader)->seek(offset, origin);
}

int SoundFontFileReader::sf_close(void *handle) {
    return FLUID_OK;
}

long long SoundFontFileReader::sf_tell(void *handle) {
    Ref<SoundFontFileReader> *reader = reinterpret_cast<Ref<SoundFontFileReader> *>(const_cast<void *>(handle));
    return (*reader)->tell();
}

void SoundFontFileReader::_bind_methods() {
    ClassDB::bind_method(D_METHOD("set_data"), &SoundFontFileReader::set_data);
    ClassDB::bind_method(D_METHOD("get_data"), &SoundFontFileReader::get_data);
    ClassDB::bind_method(D_METHOD("get_extension"), &SoundFontFileReader::get_extension);
    ClassDB::add_property("SoundFontFileReader", PropertyInfo(Variant::PACKED_BYTE_ARRAY, "data"), "set_data",
                          "get_data");
}
