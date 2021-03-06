//
// Part of Metta OS. Check http://atta-metta.net for latest version.
//
// Copyright 2007 - 2014, Stanislav Karchebnyy <berkus@atta-metta.net>
//
// Distributed under the Boost Software License, Version 1.0.
// (See file LICENSE_1_0.txt or a copy at http://www.boost.org/LICENSE_1_0.txt)
//
#pragma once

#include <deque>
#include <mutex>
#include <boost/signals2/signal.hpp>
#include "voicebox/audio_source.h"
#include "voicebox/audio_sink.h"

namespace voicebox {

/**
 * It doesn't have, neither use, acceptor or producer, since it sits at the intersection
 * of push and pull threads, all other components call into packetizer.
 * It passes the data on, using synchronization.
 * The actual packetization happens in the producer for this node, this class only
 * accepts fixed-size packets.
 *
 * Packetizer needs a side channel to record and transport timestamp information
 * about every packet. It is currently embedded directly into the produced buffer.
 * Encoder sinks would embed these timestamps into the final buffer.
 * Jitterbuffer would use this timestamp info on the receiving side.
 *
 * accept_input packet format:
 * [frame_bytes()] uncompressed audio data frame (framed by an audio_source usually)
 *
 * produce_output packet format:
 * [8 bytes] microseconds since epoch (Jan 1, 2000 0:00:00 UTC)
 * [frame_bytes()] uncompressed audio data frame
 *
 * Packetizer's on_ready_read() signal is often connected to the network sink send driver,
 * since presence of packets in the output queue is a good enough reason to send, most of the time.
 */
class packetizer : public audio_source, public audio_sink
{
protected:
    std::mutex mutex_;
    std::deque<byte_array> queue_;

public:
    inline packetizer(audio_source* from = nullptr)
    {
        if (from) {
            from->set_acceptor(this);
        }
    }

    ~packetizer();

    void produce_output(byte_array& buffer) override; // from sink
    void accept_input(byte_array data) override; // from source

    using state_signal = boost::signals2::signal<void (void)>;
    state_signal on_ready_read;
    state_signal on_queue_empty;
};

}
