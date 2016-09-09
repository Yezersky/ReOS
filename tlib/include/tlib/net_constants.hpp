//=======================================================================
// Copyright Baptiste Wicht 2013-2016.
// Distributed under the terms of the MIT License.
// (See accompanying file LICENSE or copy at
//  http://www.opensource.org/licenses/MIT) //=======================================================================

#ifndef TLIB_NET_CONSTANTS_H
#define TLIB_NET_CONSTANTS_H

#include <types.hpp>

#include "tlib/config.hpp"

THOR_NAMESPACE(tlib, network) {

namespace ip {

struct address {
    uint32_t raw_address = 0;

    address(){}
    address(uint32_t raw) : raw_address(raw) {}

    uint8_t operator()(size_t index) const {
        return (raw_address >> ((3 - index) * 8)) & 0xFF;
    }

    void set_sub(size_t index, uint8_t value){
        raw_address |= uint32_t(value) << ((3 - index) * 8);
    }

    bool operator==(const address& rhs) const {
        return this->raw_address == rhs.raw_address;
    }
};

inline address make_address(uint8_t a, uint8_t b, uint8_t c, uint8_t d){
    address addr;
    addr.set_sub(0, a);
    addr.set_sub(1, b);
    addr.set_sub(2, c);
    addr.set_sub(3, d);
    return addr;
}

} // end of namespace ip

namespace icmp {

enum class type : uint8_t {
    ECHO_REPLY = 0,
    UNREACHABLE = 3,
    SOURCE_QUENCH = 4,
    REDICT = 5,
    ECHO_REQUEST = 8,
    ROUTER_ADVERTISEMENT = 9,
    ROUTER_SOLICITATION = 10,
    TIME_EXCEEDED = 11,
    PARAMETER_PROBLEM = 12,
    TIMESTAMP = 13,
    TIMESTAMP_REPLY = 14,
    INFORMATION_REQUEST = 15,
    INFORMATION_REPLY = 16,
    ADDRESS_MASK_REQUEST = 17,
    ADDRESS_MASK_REPLY = 18,
    TRACEROUTE = 30
};

struct header {
    uint8_t type;
    uint8_t code;
    uint16_t checksum;
    uint32_t rest; ///< Depends on the type of packet type
} __attribute__((packed));

struct echo_request_header {
    uint16_t identifier;
    uint16_t sequence;
} __attribute__((packed));

struct packet_descriptor {
    size_t payload_size;
    ip::address target_ip;
    icmp::type type;
    size_t code;
};

} // end of namespace icmp

enum class socket_domain : size_t {
    AF_INET
};

enum class socket_type : size_t {
    RAW
};

enum class socket_protocol : size_t {
    ICMP
};

} // end of network namespace

#endif
