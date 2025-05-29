#include "parser/I020Parser.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <stdexcept>
#include <cstring>  // for memcpy

/// I020Parser::parse() - 撮合成交封包解析
std::shared_ptr<ParsedResultBase> I020Parser::parse(const std::vector<uint8_t>& body) {
    MatchInfo info;
    const uint8_t* ptr = body.data();
    const uint8_t* end = ptr + body.size();

    // 1️⃣ 商品代號（20 bytes ASCII）
    constexpr size_t PROD_ID_SIZE = sizeof(info.prodId);  // 20
    if (ptr + PROD_ID_SIZE > end) throw std::runtime_error("I020Parser: Missing prodId");
    std::memcpy(&info.prodId, ptr, PROD_ID_SIZE);
    ptr += PROD_ID_SIZE;

    // 2️⃣ 成交時間（6 bytes BCD）
    constexpr size_t TIME_SIZE = sizeof(info.matchTime);  // 6
    if (ptr + TIME_SIZE > end) throw std::runtime_error("I020Parser: Missing matchTime");
    std::memcpy(&info.matchTime, ptr, TIME_SIZE);
    ptr += TIME_SIZE;

    // 3️⃣ 第一筆成交（1 符號 + 5 價格 + 4 張數）
    constexpr size_t SIGN_SIZE = sizeof(info.firstSign);  // 1
    constexpr size_t PRICE_SIZE = sizeof(info.firstPrice);  // 5
    constexpr size_t QTY_SIZE = sizeof(info.firstQty);  // 4

    if (ptr + SIGN_SIZE + PRICE_SIZE + QTY_SIZE > end) throw std::runtime_error("I020Parser: Missing first deal");
    std::memcpy(&info.firstSign, ptr, SIGN_SIZE); ptr += SIGN_SIZE;
    std::memcpy(&info.firstPrice, ptr, PRICE_SIZE); ptr += PRICE_SIZE;
    std::memcpy(&info.firstQty, ptr, QTY_SIZE); ptr += QTY_SIZE;

    // 4️⃣ 顯示屬性（1 byte）
    constexpr size_t DISPLAY_SIZE = sizeof(info.matchDisplayItem);  // 1
    if (ptr + DISPLAY_SIZE > end) throw std::runtime_error("I020Parser: Missing matchDisplayItem");
    std::memcpy(&info.matchDisplayItem, ptr, DISPLAY_SIZE);
    ptr += DISPLAY_SIZE;

    // 5️⃣ 多筆成交明細（每筆 1 符號 + 5 價格 + 2 張數）
    constexpr size_t DETAIL_SIZE = sizeof(MatchDetail::sign) + sizeof(MatchDetail::price) + sizeof(MatchDetail::quantity);  // 1 + 5 + 2 = 8
    constexpr size_t REMAINING_TAIL_SIZE =
        sizeof(info.totalQty) + sizeof(info.buyCount) + sizeof(info.sellCount) + sizeof(info.statusCode);  // 4 + 4 + 4 + 1 = 13

    while (ptr + DETAIL_SIZE <= end - REMAINING_TAIL_SIZE) {
        MatchDetail detail;
        std::memcpy(&detail.sign, ptr, sizeof(detail.sign)); ptr += sizeof(detail.sign);
        std::memcpy(&detail.price, ptr, sizeof(detail.price)); ptr += sizeof(detail.price);
        std::memcpy(&detail.quantity, ptr, sizeof(detail.quantity)); ptr += sizeof(detail.quantity);
        info.matchList.push_back(detail);
    }

    // 6️⃣ 統計欄位（4 + 4 + 4 + 1 bytes）
    if (ptr + REMAINING_TAIL_SIZE > end) throw std::runtime_error("I020Parser: Missing summary fields");

    std::memcpy(&info.totalQty, ptr, sizeof(info.totalQty)); ptr += sizeof(info.totalQty);
    std::memcpy(&info.buyCount, ptr, sizeof(info.buyCount)); ptr += sizeof(info.buyCount);
    std::memcpy(&info.sellCount, ptr, sizeof(info.sellCount)); ptr += sizeof(info.sellCount);
    std::memcpy(&info.statusCode, ptr, sizeof(info.statusCode)); ptr += sizeof(info.statusCode);

    // ✅ 回傳泛型解析結果
    return std::make_shared<ParsedResultBaseImpl<MatchInfo>>(info, "I020");
}
