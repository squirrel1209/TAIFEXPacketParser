#pragma once

#include "protocol/CommonHeader.h"
#include "data/MatchInfo.h"
#include "base/ParsedResultBase.h"
#include <string>

// ✅ I020 封裝後的完整封包資料：Header + Payload
class I020Packet : public ParsedResultBase {
public:
    I020Packet(const CommonHeader& h, const MatchInfo& m);

    std::string getFormatName() const override { return "I020"; }

    std::string toString() const override { return "I020Packet: " + body.prodId.toString(); }

    const CommonHeader header;
    const MatchInfo body;
};
