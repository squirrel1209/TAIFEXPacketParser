// PacketDispatcher.cpp
#include "dispatcher/PacketDispatcher.h"
#include "parser/I010Parser.h"
#include "parser/I012Parser.h"
#include "parser/I020Parser.h"
#include "parser/I080Parser.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <stdexcept>
#include <iostream>

// ✅ 封包分派邏輯實作
// 功能：根據 header 的 messageKind，決定使用哪個解析器處理封包
std::shared_ptr<ParsedResultBase> PacketDispatcher::dispatch(
    const CommonHeader& header,
    const uint8_t* bodyPtr,
    std::size_t bodyLen
) {
    // 從 header 取出 messageKind 欄位（格式代號）
    const std::string kindStr = header.messageKind.toString();

    // 🛡️ 防呆檢查 1️⃣：messageKind 空字串
    if (kindStr.empty()) {
        std::cerr << "⚠️ messageKind 空字串，跳過封包！\n";
        return nullptr;
    }

    // 取出單一字元作為代碼
    const char kind = kindStr[0];

    // 🔎 Debug 輸出
    //std::cerr << "🔎 Raw messageKind = [" << kind << "] (Hex = 0x" 
    //          << std::hex << static_cast<int>(kind) << ")\n";

    // 將原始 body bytes 包裝為 vector<uint8_t>，方便傳給 Parser 使用
    std::vector<uint8_t> body(bodyPtr, bodyPtr + bodyLen);

    try {
        // === I010Parser: 商品基本資料 ===
        if (kind == '1') {
            I010Parser parser;
            return parser.parse(body);
        }
        // === I012Parser: 漲跌幅限制資訊 ===
        else if (kind == 'A') {
            I012Parser parser;
            return parser.parse(body);
        }
        // === I080Parser: 委託簿快照 ===
        else if (kind == '2') {
            I080Parser parser;
            return parser.parse(body);
        }
        // === I020Parser: 撮合成交資訊 ===
        else if (kind == '0') {
            I020Parser parser;
            return parser.parse(body);
        }
        // === 無對應格式 ===
        else {
            std::cerr << "⚠️ 未知封包格式，無法解析，messageKind = [" << kind << "]\n";
            return nullptr;
        }
    }
    catch (const std::exception& e) {
        std::cerr << "❌ PacketDispatcher 解析失敗 (messageKind = " << kind << "): " << e.what() << "\n";
        return nullptr;
    }
}
