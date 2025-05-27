#include "parser/I012Parser.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <cstring>
#include <stdexcept>

std::shared_ptr<ParsedResultBase> I012Parser::parse(const std::vector<uint8_t>& body) {
    const uint8_t* ptr = body.data();             // 位元組起始指標
    const uint8_t* end = ptr + body.size();       // 安全界限指標

    if (body.size() < 11) {
        throw std::runtime_error("I012Parser: payload too small for header");
    }

    PriceBandInfo info;

    // ✅ 1. 固定欄位：商品代碼（10 bytes）
    std::memcpy(&info.productId, ptr, 10);
    ptr += 10;

    // ✅ 2. 漲停階數（1 byte）
    info.numRaiseLevels.assign(ptr);
    ptr += 1;

    // ✅ 3. 動態讀取漲停階段清單（每階 6 bytes）
    for (int i = 0; i < info.numRaiseLevels.toInt(); ++i) {
        if (ptr + 6 > end) throw std::runtime_error("I012Parser: body ends in raise list");

        PriceBandLevel level;
        level.level.assign(ptr);  // 1 byte
        ptr += 1;

        // 5 bytes BCD 價格
        for (int j = 0; j < 5; ++j)
            level.price.data()[j] = *ptr++;

        info.raiseLimitList.push_back(level);
    }

    // ✅ 4. 跌停階數（1 byte）
    if (ptr + 1 > end) throw std::runtime_error("I012Parser: missing fall level count");
    info.numFallLevels.assign(ptr);
    ptr += 1;

    // ✅ 5. 動態讀取跌停階段清單（每階 6 bytes）
    for (int i = 0; i < info.numFallLevels.toInt(); ++i) {
        if (ptr + 6 > end) throw std::runtime_error("I012Parser: body ends in fall list");

        PriceBandLevel level;
        level.level.assign(ptr);  // 1 byte
        ptr += 1;

        for (int j = 0; j < 5; ++j)
            level.price.data()[j] = *ptr++;

        info.fallLimitList.push_back(level);
    }

    // ✅ 回傳泛型解析結果
    return std::make_shared<ParsedResultBaseImpl<PriceBandInfo>>(info, "I012");
}
