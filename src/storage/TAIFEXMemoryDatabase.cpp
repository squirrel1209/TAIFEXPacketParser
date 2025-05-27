#include "storage/TAIFEXMemoryDatabase.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <iostream>

/// ============================================================
/// 新增封包結果
/// - 根據格式名稱自動轉型並存入對應資料表
/// - 使用 dynamic_pointer_cast 確保型別安全
/// ============================================================
void TAIFEXMemoryDatabase::add(const std::shared_ptr<ParsedResultBase>& result) {
    if (!result) return;

    const std::string format = result->getFormatName();

    if (format == "I020") {
        // 撮合成交資料（I020）
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<MatchInfo>>(result);
        if (ptr) {
            const auto& info = ptr->data;
            dealMap[info.prodId.toString()].push_back(info);
        }
    } else if (format == "I010") {
        // 商品基本資料（I010）
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<ProductInfo>>(result);
        if (ptr) {
            const auto& info = ptr->data;
            productMap[info.productId.toString()] = info;
        }
    } else if (format == "I012") {
        // 漲跌幅階段資訊（I012）
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<PriceBandInfo>>(result);
        if (ptr) {
            const auto& info = ptr->data;
            priceBandMap[info.productId.toString()].push_back(info);
        }
    } else if (format == "I080") {
        // 委託簿快照（I080）
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<OrderBookInfo>>(result);
        if (ptr) {
            const auto& info = ptr->data;
            orderBookMap[info.productId.toString()].push_back(info);
        }
    }
}

/// 查詢指定商品的撮合成交紀錄（I020）
const std::vector<MatchInfo>& TAIFEXMemoryDatabase::getDealsByCode(const std::string& code) const {
    static const std::vector<MatchInfo> empty;
    auto it = dealMap.find(code);
    return (it != dealMap.end()) ? it->second : empty;
}

/// 查詢指定商品的基本資料（I010）
const ProductInfo* TAIFEXMemoryDatabase::getProductByCode(const std::string& code) const {
    auto it = productMap.find(code);
    return (it != productMap.end()) ? &it->second : nullptr;
}

/// 查詢指定商品的漲跌幅階段資訊（I012）
const std::vector<PriceBandInfo>& TAIFEXMemoryDatabase::getPriceBandsByCode(const std::string& code) const {
    static const std::vector<PriceBandInfo> empty;
    auto it = priceBandMap.find(code);
    return (it != priceBandMap.end()) ? it->second : empty;
}

/// 查詢指定商品的委託簿快照（I080）
const std::vector<OrderBookInfo>& TAIFEXMemoryDatabase::getOrderBooksByCode(const std::string& code) const {
    static const std::vector<OrderBookInfo> empty;
    auto it = orderBookMap.find(code);
    return (it != orderBookMap.end()) ? it->second : empty;
}

/// 輸出所有儲存資料（for debug）
void TAIFEXMemoryDatabase::dump() const {
    std::cout << "\n====== Memory Database Dump ======\n";

    // 撮合成交 (I020)
    for (const auto& [code, deals] : dealMap) {
        std::cout << "[I020] " << code << " : " << deals.size() << " 筆成交紀錄\n";
    }

    // 商品基本資料 (I010)
    for (const auto& [code, info] : productMap) {
        std::cout << "[I010] " << code << " : 商品基本資料已儲存\n";
    }

    // 漲跌幅 (I012)
    for (const auto& [code, bands] : priceBandMap) {
        std::cout << "[I012] " << code << " : " << bands.size() << " 筆漲跌幅資料\n";
    }

    // 委託簿 (I080)
    for (const auto& [code, books] : orderBookMap) {
        std::cout << "[I080] " << code << " : " << books.size() << " 筆委託簿資料\n";
    }

    std::cout << "===================================\n";
}
