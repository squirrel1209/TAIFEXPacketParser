#pragma once

#include <unordered_map>
#include <vector>
#include <memory>
#include <string>
#include "base/ParsedResultBase.h"
#include "data/I020Packet.h"

// ✅ 記憶體內資料庫：儲存各格式封包資料（目前支援 I020）
class TAIFEXMemoryDatabase {
public:
    // 新增封包結果（會自動判斷格式）
    void add(const std::shared_ptr<ParsedResultBase>& result);

    // 查詢某股票代碼的成交明細（MatchInfo 列表）
    const std::vector<MatchInfo>& getDealsByCode(const std::string& code) const;

    // 印出所有儲存資料（for debug）
    void dump() const;

private:
    // 以商品代碼為 key，儲存 MatchInfo 的 vector（只支援 I020）
    std::unordered_map<std::string, std::vector<MatchInfo>> dealMap;
};
