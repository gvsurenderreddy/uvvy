//
// Part of Metta OS. Check http://atta-metta.net for latest version.
//
// Copyright 2007 - 2014, Stanislav Karchebnyy <berkus@atta-metta.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include "voicebox/audio_stream.h"
#include "arsenal/byte_array.h"

namespace voicebox {

/**
 * Sources push data to an acceptor.
 *
 * A source is an active object in the sense that the first source usually initiates send of data.
 * A source at the end of the chain is usually a mix of source and sink, providing a connection
 * and synchronization between sources and sinks.
 */
class audio_source : public audio_stream
{
    using super = audio_stream;

    audio_source* acceptor_{nullptr};

protected:
    inline audio_source* acceptor() const { return acceptor_; }

public:
    audio_source() = default;
    audio_source(int framesize, double samplerate, int channels = 1);

    inline void set_acceptor(audio_source* as) {
        acceptor_ = as;
    }

    /**
     * Accept input wrapped into a byte_array.
     * The parameters of data inside the array come from audio_stream's settings for
     * frame_size(), num_channels() and sample_rate().
     */
    virtual void accept_input(byte_array data);

    // Overrides to pass the request down the consumer chain.

    void set_enabled(bool enable) override;
    void set_frame_size(unsigned int frame_size) override;
    void set_sample_rate(double rate) override;
    void set_num_channels(unsigned int num_channels) override;
};

} // voicebox namespace
