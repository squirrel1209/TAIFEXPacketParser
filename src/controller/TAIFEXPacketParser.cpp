#include "controller/TAIFEXPacketParser.h"

/// ✅ 建構子實作
TAIFEXPacketParser::TAIFEXPacketParser(const std::vector<uint8_t>& raw)
    : rawData(raw) {}

/// ✅ 主要解析流程實作
void TAIFEXPacketParser::parseAll() {
    size_t offset = 0;
    int index = 1;

    std::cout << "📦 [解析開始] 原始資料大小：" << rawData.size() << " bytes\n";

    while (offset + 3 <= rawData.size()) {
        // 1️⃣ 檢查封包起始碼（0x1B）
        if (rawData[offset] != 0x1B) {
            ++offset;
            continue;
        }

        // 2️⃣ 解析封包長度（BCD2）
        if (offset + 3 > rawData.size()) break;
        uint8_t high = rawData[offset + 1];
        uint8_t low  = rawData[offset + 2];
        int len = ((high >> 4) & 0xF) * 1000 + (high & 0xF) * 100
                + ((low >> 4) & 0xF) * 10 + (low & 0xF);

        // 3️⃣ 檢查封包長度是否超過資料邊界
        if (offset + len > rawData.size()) {
            std::cout << "⚠️ 封包超出資料尾端，Offset: " << offset << ", Len: " << len << "\n";
            break;
        }

        // 4️⃣ 檢查封包結尾是否為 \r\n (0x0D 0x0A)
        if (rawData[offset + len - 2] != 0x0D || rawData[offset + len - 1] != 0x0A) {
            ++offset;
            continue;
        }

        // 5️⃣ 使用 HeaderParser 解析 Header
        HeaderParser hParser;
        try {
            auto header = hParser.parse(rawData.data() + offset, len);

            // 6️⃣ 使用 PacketDispatcher 解析對應格式
            PacketDispatcher dispatcher;
            auto result = dispatcher.dispatch(header, rawData.data() + offset + sizeof(CommonHeader), len - sizeof(CommonHeader));

            if (result) {
                results.push_back(result);
                ++index;
                offset += len; // 成功解析後前進到下一個封包
                continue;
            }
        } catch (const std::exception& e) {
            std::cout << "❌ Header 解析失敗: " << e.what() << "\n";
        }

        // 解析失敗則 offset + 1 嘗試下一個位置
        ++offset;
    }

    // 7️⃣ 結束解析
    if (offset < rawData.size()) {
        std::cout << "\n[解析結束] 剩餘未處理資料：" << (rawData.size() - offset) << " bytes\n";
    } else {
        std::cout << "\n[解析結束] 所有封包解析完畢\n";
    }
}

/// ✅ 取得所有解析結果
const std::vector<std::shared_ptr<ParsedResultBase>>& TAIFEXPacketParser::getResults() const {
    return results;
}
