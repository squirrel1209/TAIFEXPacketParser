// OrderBookInfo.h
#pragma once

#include "common/FixedArray.h"
#include "common/TBCD.h"

#pragma pack(push, 1)

/// ================================================
/// 📦 I080 委託簿揭示資料結構（POD）
/// - 最多五檔買賣報價資訊
/// - 包含正負號、價格、數量
/// - 若有衍生價格，再加上 derived 欄位
/// - 可直接 memcpy，無 virtual / constructor
/// ================================================
struct OrderBookInfo {
    /// 商品代號（固定 20 bytes, ASCII 編碼）
    FixedArray<char, 20> productId;

    // === ✅ 買一到買五 ===
    FixedArray<char, 1> buySign1;  ///< '0' 正、'-' 負
    BCD9 buyPrice1;                ///< 價格（BCD 編碼）
    BCD8 buyQty1;                  ///< 數量（張數）

    FixedArray<char, 1> buySign2;
    BCD9 buyPrice2;
    BCD8 buyQty2;

    FixedArray<char, 1> buySign3;
    BCD9 buyPrice3;
    BCD8 buyQty3;

    FixedArray<char, 1> buySign4;
    BCD9 buyPrice4;
    BCD8 buyQty4;

    FixedArray<char, 1> buySign5;
    BCD9 buyPrice5;
    BCD8 buyQty5;

    // === ✅ 賣一到賣五 ===
    FixedArray<char, 1> sellSign1;
    BCD9 sellPrice1;
    BCD8 sellQty1;

    FixedArray<char, 1> sellSign2;
    BCD9 sellPrice2;
    BCD8 sellQty2;

    FixedArray<char, 1> sellSign3;
    BCD9 sellPrice3;
    BCD8 sellQty3;

    FixedArray<char, 1> sellSign4;
    BCD9 sellPrice4;
    BCD8 sellQty4;

    FixedArray<char, 1> sellSign5;
    BCD9 sellPrice5;
    BCD8 sellQty5;

    // === ✅ 衍生價格揭示標記與欄位 ===

    BCD2 derivedFlag;         ///< 衍生欄位是否存在（"01" 表示存在，其餘忽略下列欄位）

    BCD9 derivedBuyPrice1;    ///< 衍生買一價格
    BCD8 derivedBuyQty1;      ///< 衍生買一數量

    BCD9 derivedSellPrice1;   ///< 衍生賣一價格
    BCD8 derivedSellQty1;     ///< 衍生賣一數量

    std::string toString() const {
        std::ostringstream oss;
        oss << "ProductID: " << productId.toString() << ", ";

        oss << "BuySide: [";
        for (int i = 1; i <= 5; ++i) {
            oss << "B" << i << ": "
                << (i==1 ? buySign1.toString() + buyPrice1.toDecimalString(2) + "@" + std::to_string(buyQty1.toInt()) :
                    i==2 ? buySign2.toString() + buyPrice2.toDecimalString(2) + "@" + std::to_string(buyQty2.toInt()) :
                    i==3 ? buySign3.toString() + buyPrice3.toDecimalString(2) + "@" + std::to_string(buyQty3.toInt()) :
                    i==4 ? buySign4.toString() + buyPrice4.toDecimalString(2) + "@" + std::to_string(buyQty4.toInt()) :
                        buySign5.toString() + buyPrice5.toDecimalString(2) + "@" + std::to_string(buyQty5.toInt()))
                << " ";
        }
        oss << "], SellSide: [";
        for (int i = 1; i <= 5; ++i) {
            oss << "S" << i << ": "
                << (i==1 ? sellSign1.toString() + sellPrice1.toDecimalString(2) + "@" + std::to_string(sellQty1.toInt()) :
                    i==2 ? sellSign2.toString() + sellPrice2.toDecimalString(2) + "@" + std::to_string(sellQty2.toInt()) :
                    i==3 ? sellSign3.toString() + sellPrice3.toDecimalString(2) + "@" + std::to_string(sellQty3.toInt()) :
                    i==4 ? sellSign4.toString() + sellPrice4.toDecimalString(2) + "@" + std::to_string(sellQty4.toInt()) :
                        sellSign5.toString() + sellPrice5.toDecimalString(2) + "@" + std::to_string(sellQty5.toInt()))
                << " ";
        }
        oss << "], DerivedFlag: " << derivedFlag.toInt()
            << ", DerivedBuy: " << derivedBuyPrice1.toDecimalString(2) << "@" << derivedBuyQty1.toInt()
            << ", DerivedSell: " << derivedSellPrice1.toDecimalString(2) << "@" << derivedSellQty1.toInt();
        return oss.str();
    }

};

#pragma pack(pop)
