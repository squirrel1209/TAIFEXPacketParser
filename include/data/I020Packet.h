#pragma once

#include "protocol/CommonHeader.h"
#include "data/MatchInfo.h"
#include "base/ParsedResultBase.h"
#include <string>

// ✅ I020 封裝後的完整封包資料：Header + Payload
class I020Packet : public ParsedResultBase {
public:
    I020Packet(const CommonHeader& h, const MatchInfo& m);

    // 封包類型名稱
    std::string getFormatName() const override;

    // 輸出為字串（簡易）
    std::string toString() const override;

    const CommonHeader header;
    const MatchInfo body;
};
