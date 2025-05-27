#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "base/ParsedResultBase.h"
#include "data/MatchInfo.h"
#include "data/ProductInfo.h"
#include "data/PriceBandInfo.h"
#include "data/OrderBookInfo.h"

/// ============================================================
/// 🏦 TAIFEXMemoryDatabase：記憶體內資料庫
/// - 儲存各格式封包資料（I010, I012, I020, I080）
/// - 提供快速查詢、後續統計與輸出
/// - 使用 unordered_map 依商品代碼分類存放
/// ============================================================
class TAIFEXMemoryDatabase {
public:
    /// 新增封包結果（自動辨識格式，加入對應資料表）
    void add(const std::shared_ptr<ParsedResultBase>& result);

    /// 查詢指定商品的撮合成交紀錄（I020）
    const std::vector<MatchInfo>& getDealsByCode(const std::string& code) const;

    /// 查詢指定商品的基本資料（I010）
    const ProductInfo* getProductByCode(const std::string& code) const;

    /// 查詢指定商品的漲跌幅階段資訊（I012）
    const std::vector<PriceBandInfo>& getPriceBandsByCode(const std::string& code) const;

    /// 查詢指定商品的委託簿快照（I080）
    const std::vector<OrderBookInfo>& getOrderBooksByCode(const std::string& code) const;

    /// 輸出所有資料內容（for debug）
    void dump() const;

private:
    /// 撮合成交資料表（I020）：key = 商品代碼，value = 多筆成交紀錄
    std::unordered_map<std::string, std::vector<MatchInfo>> dealMap;

    /// 商品基本資料表（I010）：key = 商品代碼，value = 單筆商品資料
    std::unordered_map<std::string, ProductInfo> productMap;

    /// 漲跌幅階段資料表（I012）：key = 商品代碼，value = 多筆時間序列
    std::unordered_map<std::string, std::vector<PriceBandInfo>> priceBandMap;

    /// 委託簿快照資料表（I080）：key = 商品代碼，value = 多筆時間序列
    std::unordered_map<std::string, std::vector<OrderBookInfo>> orderBookMap;
};
