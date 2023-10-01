#ifndef AUDIOSTREAMMYTONE_H
#define AUDIOSTREAMMYTONE_H

#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/godot.hpp>

namespace godot {

class AudioStreamMyTone : public AudioStream {
    GDCLASS(AudioStreamMyTone, AudioStream)

  private:
    friend class AudioStreamPlaybackMyTone;
    float pos;
    int mix_rate;
    bool stereo;
    int hz;

  public:
    void reset();
    void set_position(uint64_t pos);
    virtual String get_stream_name() const;
    float gen_tone();
    virtual float get_length() const {
        return 0;
    }  // if supported, otherwise return 0
    AudioStreamMyTone();
    Ref<AudioStreamPlayback> _instantiate_playback();

  protected:
    static void _bind_methods();
};
}  // namespace godot

#endif
