#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>

#include "base/ParsedResultBase.h"
#include "data/MatchInfo.h"
#include "data/PriceBandInfo.h"
#include "data/OrderBookInfo.h"
#include "data/ProductInfo.h"
#include "data/I020Packet.h"
#include "data/I012Packet.h"
#include "data/I080Packet.h"
#include "data/I010Packet.h"

/// ===========================================
/// 📦 TAIFEXMemoryDatabase
/// 將解析後的封包資料，依格式儲存到對應的 map
/// ===========================================
class TAIFEXMemoryDatabase {
public:
    /// 將解析結果加入資料庫
    void add(const std::shared_ptr<ParsedResultBase>& result);

    /// 輸出簡單的資料統計（for Debug）
    void dump() const;

    /// 封包資料查詢接口（回傳 const 參考）
    const std::unordered_map<std::string, std::vector<MatchInfo>>& getAllMatchInfo() const { return matchInfoMap; }
    const std::unordered_map<std::string, std::vector<PriceBandInfo>>& getAllPriceBandInfo() const { return priceBandMap; }
    const std::unordered_map<std::string, std::vector<OrderBookInfo>>& getAllOrderBookInfo() const { return orderBookMap; }
    const std::unordered_map<std::string, ProductInfo>& getAllProductInfo() const { return productInfoMap; }

private:
    std::unordered_map<std::string, std::vector<MatchInfo>> matchInfoMap;     ///< I020 對應資料
    std::unordered_map<std::string, std::vector<PriceBandInfo>> priceBandMap; ///< I012 對應資料
    std::unordered_map<std::string, std::vector<OrderBookInfo>> orderBookMap; ///< I080 對應資料
    std::unordered_map<std::string, ProductInfo> productInfoMap;              ///< I010 對應資料
};
