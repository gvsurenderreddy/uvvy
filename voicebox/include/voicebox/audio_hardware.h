//
// Part of Metta OS. Check http://atta-metta.net for latest version.
//
// Copyright 2007 - 2014, Stanislav Karchebnyy <berkus@atta-metta.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <memory>
#include <functional>
#include "sss/server.h"
#include "sss/stream.h"

class RtAudio;

namespace voicebox {

class audio_sink;
class audio_source;

/**
 * Controls the audio layer and starts playback and capture.
 */
class audio_hardware
{
    RtAudio* audio_inst{0};

    audio_hardware();
    ~audio_hardware();

public:
    static audio_hardware* instance();

    static bool add_instream(voicebox::audio_source* in);
    static bool remove_instream(voicebox::audio_source* in);
    static bool add_outstream(voicebox::audio_sink* out);
    static bool remove_outstream(voicebox::audio_sink* out);

    static void reopen();
    static int get_sample_rate();
    static int get_frame_size();

    /**
     * Detect available devices.
     * @return Number of available audio devices.
     */
    int scan();

    void open_audio();
    void close_audio();

    void start_audio();
    void stop_audio();
    bool is_running() const;

    void set_input_level(int level);
    void set_output_level(int level);

    // Hardware I/O handlers called from rtcallback
    void capture(void* buffer, unsigned int nFrames);
    void playback(void* buffer, unsigned int nFrames);
};

} // voicebox namespace
