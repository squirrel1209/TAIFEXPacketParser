#pragma once
#include "common/FixedArray.h"
#include "common/TBCD.h"
#include <string>
#include <sstream>

#pragma pack(push, 1)

/// ==============================================
/// 📦 I010 商品基本資料結構（POD）
/// - 使用 FixedArray<char, N> 與 TBCD<N> 封裝欄位
/// - 所有成員為 POD，可直接 memcpy
/// - 無 constructor/destructor
/// ==============================================
struct ProductInfo {
    FixedArray<char, 10> productId;               ///< 商品代號（ASCII 10）
    BCD5 referencePrice;                          ///< 參考價（BCD5）
    FixedArray<char, 1> prodKind;                 ///< 契約種類（ASCII 1）
    BCD1 decimalLocator;                          ///< 價格小數位數（BCD1）
    BCD1 strikePriceDecimalLocator;               ///< 履約價小數位數（BCD1）
    BCD4 beginDate;                               ///< 上市日期（YYYYMMDD, BCD4）
    BCD4 endDate;                                 ///< 下市日期（YYYYMMDD, BCD4）
    BCD1 flowGroup;                               ///< 流程群組（BCD1）
    BCD4 deliveryDate;                            ///< 最後結算日（YYYYMMDD, BCD4）
    FixedArray<char, 1> dynamicBanding;           ///< 是否適用動態漲跌（Y/N）

    /// 將結構轉為可顯示字串
    std::string toString() const {
        std::ostringstream oss;
        oss << "ProductID: " << productId.toString()
            << ", RefPrice: " << referencePrice.toDecimalString(decimalLocator.toInt())
            << ", Kind: " << prodKind.toString()
            << ", Decimal: " << decimalLocator.toInt()
            << ", Begin: " << beginDate.toString()
            << ", End: " << endDate.toString()
            << ", Delivery: " << deliveryDate.toString()
            << ", Dynamic: " << dynamicBanding.toString();
        return oss.str();
    }
};

#pragma pack(pop)
