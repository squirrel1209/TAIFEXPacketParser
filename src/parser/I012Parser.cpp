#include "parser/I012Parser.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <cstring>
#include <stdexcept>

std::shared_ptr<ParsedResultBase> I012Parser::parse(const std::vector<uint8_t>& body) {
    const uint8_t* ptr = body.data();             // 位元組起始指標
    const uint8_t* end = ptr + body.size();       // 安全界限指標

    if (body.size() < sizeof(PriceBandInfo::productId) + sizeof(PriceBandInfo::numRaiseLevels)) {
        throw std::runtime_error("I012Parser: payload too small for header");
    }

    PriceBandInfo info;

    // ✅ 1. 讀取固定欄位：商品代碼（productId）
    constexpr size_t PRODUCT_ID_SIZE = sizeof(info.productId);  // 10
    std::memcpy(info.productId.data(), ptr, PRODUCT_ID_SIZE);
    ptr += PRODUCT_ID_SIZE;

    // ✅ 2. 漲停階數
    constexpr size_t LEVEL_SIZE = sizeof(info.numRaiseLevels);  // 1
    info.numRaiseLevels.assign(ptr);
    ptr += LEVEL_SIZE;

    // ✅ 3. 動態讀取漲停階段清單（每階段由 PriceBandLevel 結構定義）
    constexpr size_t PRICE_BAND_LEVEL_SIZE = sizeof(PriceBandLevel::level) + sizeof(PriceBandLevel::price);  // 1 + 5 = 6
    for (int i = 0; i < info.numRaiseLevels.toInt(); ++i) {
        if (ptr + PRICE_BAND_LEVEL_SIZE > end) {
            throw std::runtime_error("I012Parser: body ends in raise list");
        }

        PriceBandLevel level;
        level.level.assign(ptr);
        ptr += sizeof(level.level);

        std::memcpy(level.price.data().data(), ptr, sizeof(level.price));
        ptr += sizeof(level.price);

        info.raiseLimitList.push_back(level);
    }

    // ✅ 4. 跌停階數
    if (ptr + LEVEL_SIZE > end) {
        throw std::runtime_error("I012Parser: missing fall level count");
    }
    info.numFallLevels.assign(ptr);
    ptr += LEVEL_SIZE;

    // ✅ 5. 動態讀取跌停階段清單
    for (int i = 0; i < info.numFallLevels.toInt(); ++i) {
        if (ptr + PRICE_BAND_LEVEL_SIZE > end) {
            throw std::runtime_error("I012Parser: body ends in fall list");
        }

        PriceBandLevel level;
        level.level.assign(ptr);
        ptr += sizeof(level.level);

        std::memcpy(level.price.data().data(), ptr, sizeof(level.price));
        ptr += sizeof(level.price);

        info.fallLimitList.push_back(level);
    }

    // ✅ 回傳泛型解析結果
    return std::make_shared<ParsedResultBaseImpl<PriceBandInfo>>(info, "I012");
}
