#include "storage/TAIFEXMemoryDatabase.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <iostream>

/// 新增解析後的封包到資料庫
void TAIFEXMemoryDatabase::add(const std::shared_ptr<ParsedResultBase>& result) {
    const std::string formatName = result->getFormatName();
    std::cout << "✅ add() 收到封包 FormatName: " << formatName << std::endl;

    if (formatName == "I020") {
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<MatchInfo>>(result);
        if (!ptr) {
            std::cerr << "❌ dynamic_pointer_cast<I020> 失敗！" << std::endl;
            return;
        }
        std::cout << "✅ I020 productId = [" << ptr->getData().prodId.toString() << "]" << std::endl;
        matchInfoMap[ptr->getData().prodId.toString()].push_back(ptr->getData());
    } 
    else if (formatName == "I012") {
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<PriceBandInfo>>(result);
        if (!ptr) {
            std::cerr << "❌ dynamic_pointer_cast<I012> 失敗！" << std::endl;
            return;
        }
        std::cout << "✅ I012 productId = [" << ptr->getData().productId.toString() << "]" << std::endl;
        priceBandMap[ptr->getData().productId.toString()].push_back(ptr->getData());
    } 
    else if (formatName == "I080") {
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<OrderBookInfo>>(result);
        if (!ptr) {
            std::cerr << "❌ dynamic_pointer_cast<I080> 失敗！" << std::endl;
            return;
        }
        std::cout << "✅ I080 productId = [" << ptr->getData().productId.toString() << "]" << std::endl;
        orderBookMap[ptr->getData().productId.toString()].push_back(ptr->getData());
    } 
    else if (formatName == "I010") {
        auto ptr = std::dynamic_pointer_cast<ParsedResultBaseImpl<ProductInfo>>(result);
        if (!ptr) {
            std::cerr << "❌ dynamic_pointer_cast<I010> 失敗！" << std::endl;
            return;
        }
        std::cout << "✅ I010 productId = [" << ptr->getData().productId.toString() << "]" << std::endl;
        productInfoMap[ptr->getData().productId.toString()] = ptr->getData();
    } 
    else {
        std::cerr << "⚠️ 無法識別的 FormatName: " << formatName << std::endl;
    }
}

/// 輸出目前資料統計
void TAIFEXMemoryDatabase::dump() const {
    std::cout << "📊 MatchInfo（I020）：共 " << matchInfoMap.size() << " 筆商品\n";
    std::cout << "📊 PriceBandInfo（I012）：共 " << priceBandMap.size() << " 筆商品\n";
    std::cout << "📊 OrderBookInfo（I080）：共 " << orderBookMap.size() << " 筆商品\n";
    std::cout << "📊 ProductInfo（I010）：共 " << productInfoMap.size() << " 筆商品\n";
}
