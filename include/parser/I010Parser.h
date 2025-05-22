#pragma once
#include "base/TAIFEXParserInterface.h"
#include "data/ProductInfo.h"

/// ==============================================
/// 🧩 I010Parser：商品基本資料格式解析器
/// - 採用 memcpy 快速轉換封包資料至 POD 結構
/// ==============================================
class I010Parser : public TAIFEXParserInterface {
public:
    std::shared_ptr<ParsedResultBase> parse(const std::vector<uint8_t>& body) override;
};
