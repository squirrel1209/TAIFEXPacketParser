#include "storage/TAIFEXMemoryDatabase.h"
#include <iostream>
#include <iomanip>

// ✅ 新增解析結果到資料庫（目前僅支援 I020Packet）
void TAIFEXMemoryDatabase::add(const std::shared_ptr<ParsedResultBase>& result) {
    // 嘗試將解析結果轉型為 I020Packet
    auto i020 = std::dynamic_pointer_cast<I020Packet>(result);
    if (!i020) return; // 若非 I020 封包則忽略

    // 取得商品代碼（轉為字串作為 map key）
    std::string code = i020->body.prodId.toString();

    // 加入成交紀錄（vector 自動建立）
    dealMap[code].push_back(i020->body);
}

// ✅ 查詢某商品的成交明細（若查無則回傳空 vector）
const std::vector<MatchInfo>& TAIFEXMemoryDatabase::getDealsByCode(const std::string& code) const {
    static const std::vector<MatchInfo> empty;

    auto it = dealMap.find(code);
    if (it != dealMap.end()) {
        return it->second;
    } else {
        return empty; // 查不到則給空 vector 參考
    }
}

// ✅ 印出目前儲存的所有成交資訊（for debug）
void TAIFEXMemoryDatabase::dump() const {
    std::cout << "📊 成交明細儲存狀態：\n";

    for (const auto& [code, deals] : dealMap) {
        std::cout << "🔹 [" << code << "] 共 " << deals.size() << " 筆成交\n";

        for (const auto& deal : deals) {
            std::cout << "    ⏱️ 時間: " << deal.matchTime.toString()
                      << " | 價格: " << deal.firstPrice.toDecimalString(2)
                      << " | 張數: " << deal.firstQty.toInt() << "\n";
        }
    }
}
