// I080Parser.cpp
#include "parser/I080Parser.h"
#include <stdexcept>
#include <cstring>  // for memcpy, memset

// ✅ 解析 I080 封包
std::shared_ptr<ParsedResultBase> I080Parser::parse(const std::vector<uint8_t>& body) {
    constexpr size_t baseLength = sizeof(OrderBookInfo);

    // ⚠️ 資料長度檢查
    if (body.size() < baseLength) {
        throw std::runtime_error("I080Parser: payload size too small");
    }

    // ✅ 配置解析結果結構（使用模板封裝）
    auto result = std::make_shared<ParsedResultBaseImpl<OrderBookInfo>>();

    // ✅ 使用 memcpy 將原始 bytes 轉為結構體（POD 設計，可直接對應）
    std::memcpy(&result->data, body.data(), baseLength);

    // ⚠️ 衍生價格揭示欄位檢查：derivedFlag ≠ 1 則清空欄位
    if (result->data.derivedFlag.toInt() != 1) {
        std::memset(&result->data.derivedBuyPrice1, 0, sizeof(BCD9));
        std::memset(&result->data.derivedBuyQty1, 0, sizeof(BCD8));
        std::memset(&result->data.derivedSellPrice1, 0, sizeof(BCD9));
        std::memset(&result->data.derivedSellQty1, 0, sizeof(BCD8));
    }

    return result;
}
