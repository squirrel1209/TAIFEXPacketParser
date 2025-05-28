#include "storage/TAIFEXMemoryDatabase.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <iostream>

/// =======================================================
/// 📦 新增解析後的封包資料到記憶體資料庫
/// - 根據 FormatName 決定加入哪一個 map
/// - 目前支援格式：I010、I012、I020、I080
/// =======================================================
void TAIFEXMemoryDatabase::add(const std::shared_ptr<ParsedResultBase>& result) {
    const std::string formatName = result->getFormatName(); // 取得格式名稱

    // ============ 根據格式分派 ====================
    if (formatName == "I020") {
        // 撮合成交資料 (I020)
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<MatchInfo>>(result);
        if (!ptr) return; // 轉型失敗保護
        // 根據商品代碼分類儲存
        matchInfoMap[ptr->getData().prodId.toString()].push_back(ptr->getData());
    } 
    else if (formatName == "I012") {
        // 漲跌幅資料 (I012)
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<PriceBandInfo>>(result);
        if (!ptr) return;
        priceBandMap[ptr->getData().productId.toString()].push_back(ptr->getData());
    } 
    else if (formatName == "I080") {
        // 委託簿資料 (I080)
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<OrderBookInfo>>(result);
        if (!ptr) return;
        orderBookMap[ptr->getData().productId.toString()].push_back(ptr->getData());
    } 
    else if (formatName == "I010") {
        // 商品基本資料 (I010)
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<ProductInfo>>(result);
        if (!ptr) return;
        productInfoMap[ptr->getData().productId.toString()] = ptr->getData();
    } 
    else {
        // 不支援的封包格式，可視情況新增處理邏輯
        // std::cerr << "⚠️ 無法識別的 FormatName: " << formatName << std::endl;
    }
}

/// =======================================================
/// 📊 輸出目前資料庫統計摘要（僅統計商品筆數，不是封包總數）
/// =======================================================
void TAIFEXMemoryDatabase::dump() const {
    std::cout << "📊 MatchInfo（I020）：共 " << matchInfoMap.size() << " 筆商品\n";
    std::cout << "📊 PriceBandInfo（I012）：共 " << priceBandMap.size() << " 筆商品\n";
    std::cout << "📊 OrderBookInfo（I080）：共 " << orderBookMap.size() << " 筆商品\n";
    std::cout << "📊 ProductInfo（I010）：共 " << productInfoMap.size() << " 筆商品\n";
}
