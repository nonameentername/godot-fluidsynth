#ifndef AUDIOSTREAMPLAYERFLUIDSYNTH_H
#define AUDIOSTREAMPLAYERFLUIDSYNTH_H

#include <godot_cpp/classes/audio_frame.hpp>
#include <godot_cpp/classes/audio_server.hpp>
#include <godot_cpp/classes/audio_stream.hpp>
#include <godot_cpp/classes/audio_stream_playback.hpp>
#include <godot_cpp/godot.hpp>

#include "audio_stream_fluidsynth.h"

namespace godot {

class AudioStreamPlaybackFluidSynth : public AudioStreamPlayback {
    GDCLASS(AudioStreamPlaybackFluidSynth, AudioStreamPlayback)
    friend class AudioStreamFluidSynth;

private:
    enum {
        PCM_BUFFER_SIZE = 4096
    };
    enum {
        MIX_FRAC_BITS = 13,
        MIX_FRAC_LEN = (1 << MIX_FRAC_BITS),
        MIX_FRAC_MASK = MIX_FRAC_LEN - 1,
    };
    Ref<AudioStreamFluidSynth> base;
    bool active;
    float mixed;

public:
    static void _bind_methods();

    virtual void _start(float p_from_pos = 0.0);
    virtual void _stop();
    virtual bool _is_playing() const;
    virtual int _get_loop_count() const;
    virtual double _get_playback_position() const;
    virtual void _seek(float p_time);
    virtual int _mix(AudioFrame *p_buffer, float p_rate_scale, int p_frames);
    virtual float _get_length() const;
    AudioStreamPlaybackFluidSynth();
    ~AudioStreamPlaybackFluidSynth();
};
} // namespace godot

#endif
