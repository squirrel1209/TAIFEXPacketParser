#pragma once
#include "common/FixedArray.h"
#include "common/TBCD.h"
#include <vector>
#include <string>
#include <sstream>

#pragma pack(push, 1)

/// ===========================================
/// 📊 PriceBandLevel：單一階段的漲/跌停價資訊
/// - 包含：階數 + 對應價格（BCD）
/// - 長度固定為 6 bytes（1 + 5）
/// ===========================================
struct PriceBandLevel {
    BCD1 level;     ///< 階數（BCD，最多 99）
    BCD5 price;     ///< 該階數價格（BCD5，支援小數）

    /// 顯示用字串
    std::string toString() const {
        std::ostringstream oss;
        oss << "Lv" << level.toInt() << ": " << price.toDecimalString(2);
        return oss.str();
    }
};

/// ===========================================
/// 📦 PriceBandInfo：I012 整筆商品的漲跌限制清單
/// - 前面固定部分可 memcpy（商品代碼）
/// - 後面階段資料為動態長度
/// ===========================================
struct PriceBandInfo {
    FixedArray<char, 10> productId;                     ///< 商品代碼（ASCII 10 bytes）
    BCD1 numRaiseLevels;                                ///< 漲停階數總數（BCD）
    std::vector<PriceBandLevel> raiseLimitList;         ///< 漲停階資訊清單
    BCD1 numFallLevels;                                 ///< 跌停階數總數（BCD）
    std::vector<PriceBandLevel> fallLimitList;          ///< 跌停階資訊清單

    /// 顯示用字串
    std::string toString() const {
        std::ostringstream oss;
        oss << "ProductID: " << productId.toString()
            << ", RaiseLv: " << int(numRaiseLevels.toInt()) << " [";
        for (const auto& r : raiseLimitList)
            oss << r.toString() << " ";
        oss << "], FallLv: " << int(numFallLevels.toInt()) << " [";
        for (const auto& f : fallLimitList)
            oss << f.toString() << " ";
        oss << "]";
        return oss.str();
    }
};

#pragma pack(pop)
