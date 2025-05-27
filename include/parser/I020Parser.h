#pragma once

#include "base/TAIFEXParserInterface.h"
#include "data/MatchInfo.h"
#include <memory>

/// ========================================
/// 🧩 I020Parser：撮合成交資訊解析器
/// - 繼承 TAIFEXParserInterface
/// - 回傳多型指標 ParsedResultBase
/// ========================================
class I020Parser : public TAIFEXParserInterface {
public:
    std::shared_ptr<ParsedResultBase> parse(const std::vector<uint8_t>& body) override;
};
