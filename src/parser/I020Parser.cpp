#include "parser/I020Parser.h"
#include <stdexcept>
#include <cstring>  // for std::memcpy

MatchInfo I020Parser::parseBody(const uint8_t* ptr, std::size_t length) {
    MatchInfo info;
    const uint8_t* start = ptr;

    // 1️⃣ 商品代號（20 bytes ASCII）
    std::memcpy(&info.prodId.data[0], ptr, 20); ptr += 20;

    // 2️⃣ 成交時間（6 bytes BCD）
    std::memcpy(&info.matchTime.data()[0], ptr, 6); ptr += 6;

    // 3️⃣ 第一筆成交（1 符號 + 5 價格 + 4 張數）
    info.firstSign[0] = static_cast<char>(*ptr++);
    std::memcpy(&info.firstPrice[0], ptr, 5); ptr += 5;
    std::memcpy(&info.firstQty[0],   ptr, 4); ptr += 4;

    // 4️⃣ 顯示屬性（1 byte）
    info.matchDisplayItem[0] = static_cast<char>(*ptr++);

    // 5️⃣ 多筆成交明細（剩下每 8 bytes 一筆，直到剩 13 bytes）
    std::size_t variableEnd = length - 13;

    while (static_cast<std::size_t>(ptr - start) + 8 <= variableEnd) {
        MatchDetail detail;
        detail.sign[0] = static_cast<char>(*ptr++);
        std::memcpy(&detail.price[0],    ptr, 5); ptr += 5;
        std::memcpy(&detail.quantity[0], ptr, 2); ptr += 2;
        info.matchList.push_back(detail);
    }

    // 6️⃣ 統計欄位
    std::memcpy(&info.totalQty[0],  ptr, 4); ptr += 4;
    std::memcpy(&info.buyCount[0],  ptr, 4); ptr += 4;
    std::memcpy(&info.sellCount[0], ptr, 4); ptr += 4;

    // 7️⃣ 狀態碼
    info.statusCode.data()[0] = *ptr++;

    return info;
}
