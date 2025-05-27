// I080Parser.cpp
#include "parser/I080Parser.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <stdexcept>
#include <cstring>  // for memcpy, memset

std::shared_ptr<ParsedResultBase> I080Parser::parse(const std::vector<uint8_t>& body) {
    constexpr size_t baseLength = sizeof(OrderBookInfo);

    // ⚠️ 資料長度檢查
    if (body.size() < baseLength) {
        throw std::runtime_error("I080Parser: payload size too small");
    }

    // ✅ 解析原始資料
    OrderBookInfo info;
    std::memcpy(&info, body.data(), baseLength);

    // ⚠️ 衍生價格欄位處理（只在 derivedFlag == 1 時有效）
    if (info.derivedFlag.toInt() != 1) {
        std::memset(&info.derivedBuyPrice1, 0, sizeof(BCD9));
        std::memset(&info.derivedBuyQty1, 0, sizeof(BCD8));
        std::memset(&info.derivedSellPrice1, 0, sizeof(BCD9));
        std::memset(&info.derivedSellQty1, 0, sizeof(BCD8));
    }

    // ✅ 使用模板包裝器，產生多型結果
    return std::make_shared<ParsedResultBaseImpl<OrderBookInfo>>(info, "I080");
}
