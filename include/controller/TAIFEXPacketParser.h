#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <iostream>
#include "base/ParsedResultBase.h"
#include "controller/HeaderParser.h"
#include "dispatcher/PacketDispatcher.h"

// ✅ TAIFEXPacketParser：主封包解析控制器
// 功能：
// - 從原始資料 buffer 切分封包
// - 檢查封包起始碼與結尾
// - 根據封包長度切出封包
// - 呼叫 HeaderParser 解析通用 header
// - 交給 PacketDispatcher 根據格式分派對應解析器
// - 成功解析後儲存結果
class TAIFEXPacketParser {
public:
    // 建構子：傳入原始資料（通常是從檔案或網路讀取）
    explicit TAIFEXPacketParser(const std::vector<uint8_t>& raw) : rawData(raw) {}

    // 主解析函式：負責從 rawData 中找出一筆一筆封包並解析
    void parseAll() {
        size_t offset = 0;
        int index = 1;

        std::cout << "📦 [解析開始] 原始資料大小：" << rawData.size() << " bytes\n";

        while (offset + 3 <= rawData.size()) {
            // Step 1️⃣：封包開頭必須是 0x1B
            if (rawData[offset] != 0x1B) {
                ++offset;
                continue;
            }

            // Step 2️⃣：使用 BCD2 解出封包長度（raw[offset+1] 與 offset+2）
            if (offset + 3 > rawData.size()) break;
            uint8_t high = rawData[offset + 1];
            uint8_t low  = rawData[offset + 2];
            int len = ((high >> 4) & 0xF) * 1000 + (high & 0xF) * 100
                    + ((low >> 4) & 0xF) * 10 + (low & 0xF);

            // Step 3️⃣：檢查封包長度是否超過資料尾端
            if (offset + len > rawData.size()) {
                std::cout << "⚠️ 封包超出資料尾端，Offset: " << offset << ", Len: " << len << "\n";
                break;
            }

            // Step 4️⃣：檢查封包是否以 \r\n (0x0D 0x0A) 結尾
            if (rawData[offset + len - 2] != 0x0D || rawData[offset + len - 1] != 0x0A) {
                ++offset;
                continue;
            }

            // Step 5️⃣：解析 Header
            HeaderParser hParser;
            try {
                auto header = hParser.parse(rawData.data() + offset, len);

                // Step 6️⃣：交由 PacketDispatcher 解析對應格式
                PacketDispatcher dispatcher;
                auto result = dispatcher.dispatch(header, rawData.data() + offset + sizeof(CommonHeader), len - sizeof(CommonHeader));

                if (result) {
                    results.push_back(result);
                    ++index;
                    offset += len;
                    continue;
                }
            } catch (const std::exception& e) {
                std::cout << "❌ Header 解析失敗: " << e.what() << "\n";
            }

            ++offset; // fallback（若失敗則小步前進）
        }

        // 結尾：輸出解析完成狀態
        if (offset < rawData.size()) {
            std::cout << "\n[解析結束] 剩餘未處理資料：" << (rawData.size() - offset) << " bytes\n";
        } else {
            std::cout << "\n[解析結束] 所有封包解析完畢\n";
        }
    }

    // 回傳所有成功解析的結果
    const std::vector<std::shared_ptr<ParsedResultBase>>& getResults() const {
        return results;
    }

private:
    const std::vector<uint8_t>& rawData;  // 原始位元資料參考
    std::vector<std::shared_ptr<ParsedResultBase>> results; // 儲存解析後的結果
};
