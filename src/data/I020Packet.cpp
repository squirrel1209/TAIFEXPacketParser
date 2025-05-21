#include "data/I020Packet.h"

I020Packet::I020Packet(const CommonHeader& h, const MatchInfo& m)
    : header(h), body(m) {}

std::string I020Packet::getFormatName() const {
    return "I020";
}

std::string I020Packet::toString() const {
    return "I020Packet: " + body.prodId.toString();
}
