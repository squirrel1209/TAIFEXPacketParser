// I080Parser.cpp
#include "parser/I080Parser.h"
#include "result/ParsedResultBaseImpl.hpp"  // 確保正確路徑
#include <stdexcept>
#include <cstring>

std::shared_ptr<ParsedResultBase> I080Parser::parse(const std::vector<uint8_t>& body) {
    constexpr size_t baseLength = sizeof(OrderBookInfo);

    if (body.size() < baseLength) {
        throw std::runtime_error("I080Parser: payload size too small");
    }

    OrderBookInfo info;
    std::memcpy(&info, body.data(), baseLength);

    if (info.derivedFlag.toInt() != 1) {
        std::memset(&info.derivedBuyPrice1, 0, sizeof(BCD9));
        std::memset(&info.derivedBuyQty1, 0, sizeof(BCD8));
        std::memset(&info.derivedSellPrice1, 0, sizeof(BCD9));
        std::memset(&info.derivedSellQty1, 0, sizeof(BCD8));
    }

    return std::make_shared<ParsedResultBaseImpl<OrderBookInfo>>(info, "I080");
}
