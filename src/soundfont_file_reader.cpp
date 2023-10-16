#include "soundfont_file_reader.h"

using namespace godot;

SoundFontFileReader::SoundFontFileReader() {
}

SoundFontFileReader::~SoundFontFileReader() {
}

void SoundFontFileReader::_init() {
}

void SoundFontFileReader::clear_data() {
}

void SoundFontFileReader::set_data(PackedByteArray data) {
    PackedByteArray in_array = data;
    PackedByteArray out_array;
    for (int i = 0; i < in_array.size(); i++) {
        out_array.append(in_array[i]);
    }
    array_data = out_array;
}

PackedByteArray SoundFontFileReader::get_data() {
    PackedByteArray out_array;
    for (int i = 0; i < array_data.size(); i++) {
        out_array.append(array_data[i]);
    }
    return out_array;
}

char *SoundFontFileReader::get_array_data() {
    return (char *)array_data.ptr();
}

long SoundFontFileReader::get_array_size() {
    return array_data.size();
}

String SoundFontFileReader::get_extension() {
    return "sf2str";
}

void SoundFontFileReader::open() {
    position = 0;
}

int SoundFontFileReader::read(void *buf, long long count) {
    char *ptr = (char *)array_data.ptr() + position;
    memcpy(buf, ptr, count);
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
        position = array_data.size() + offset;
        break;
    }

    if (position < 0 || position > array_data.size()) {
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
