#pragma once

#include <vector>
#include "common/TBCD.h"
#include "common/FixedString.h"
#include "common/FixedArray.h"

// ✅ 一筆成交價量資訊（用於 OCCURS 多筆 MATCH-DATA）
struct MatchDetail {
    FixedArray<char, 1> sign;        // 價格正負符號：'0' 表正, '-' 表負
    BCD5 price;       // 成交價（PACK BCD, 單位元）
    BCD2 quantity;    // 成交數量（張）
};

// ✅ I020 主體資料（不含 Header）
struct MatchInfo {
    FixedString<20> prodId;       // 商品代號（固定 20 Bytes）
    BCD6 matchTime;               // 成交時間（HHMMSS + 毫秒微秒）

    FixedArray<char, 1> firstSign;               // 第一筆成交價符號（'0', '-'）
    BCD5 firstPrice;              // 第一筆成交價
    BCD4 firstQty;                // 第一筆成交張數

    FixedArray<char, 1> matchDisplayItem;        // 顯示屬性（BIT MAP 格式）

    std::vector<MatchDetail> matchList; // 後續多筆成交資料（最多 127 筆）

    BCD4 totalQty;                // 累計成交數量
    BCD4 buyCount;                // 累計買進筆數
    BCD4 sellCount;               // 累計賣出筆數

    BCD1 statusCode;              // 狀態碼（00：正常）

    std::string toString() const {
        std::ostringstream oss;
        oss << "ProductID: " << prodId.toString()
            << ", Time: " << matchTime.toString()
            << ", FirstSign: " << firstSign.toString()
            << ", FirstPrice: " << firstPrice.toDecimalString(2)
            << ", FirstQty: " << firstQty.toInt()
            << ", TotalQty: " << totalQty.toInt()
            << ", BuyCount: " << buyCount.toInt()
            << ", SellCount: " << sellCount.toInt()
            << ", Status: " << statusCode.toString()
            << ", MatchDetails: [";

        for (const auto& detail : matchList) {
            oss << detail.sign.toString() << detail.price.toDecimalString(2)
                << "@" << detail.quantity.toInt() << " ";
        }

        oss << "]";
        return oss.str();
    }

};
