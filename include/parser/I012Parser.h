#pragma once
#include "base/TAIFEXParserInterface.h"
#include "data/PriceBandInfo.h"

/// ===========================================
/// 🧩 I012Parser：解析商品漲跌幅限制資訊
/// - 使用 pointer forwarding 解析變長階數清單
/// - 高效不動態建構結構體
/// ===========================================
class I012Parser : public TAIFEXParserInterface {
public:
    std::shared_ptr<ParsedResultBase> parse(const std::vector<uint8_t>& body) override;
};
