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
    // 將原始 body bytes 包裝為 vector<uint8_t>，方便傳給 Parser 使用
    std::vector<uint8_t> body(bodyPtr, bodyPtr + bodyLen);

    // 從 header 取出 messageKind 欄位（格式代號）
    const std::string kind = header.messageKind.toString();

    try {
        // === I010Parser: 商品基本資料 ===
        // 根據 TAIFEX 規範，I010 對應 messageKind == "1"
        if (kind == "1") {
            I010Parser parser;
            return parser.parse(body);
        }
        
        // === I012Parser: 漲跌幅限制資訊 ===
        // I012 對應 messageKind == "A"
        else if (kind == "A") {
            I012Parser parser;
            return parser.parse(body);
        }

        // === I080Parser: 委託簿快照 ===
        // I080 對應 messageKind == "2"
        else if (kind == "2") {
            I080Parser parser;
            return parser.parse(body);
        }

        // === I020Parser: 撮合成交資訊 ===
        // I020 對應 messageKind == "0"
        else if (kind == "0") { // I020
            I020Parser parser;
            return parser.parse(body);
        }

        // === 無對應格式 ===
        else {
            std::cerr << "⚠️ 未知封包格式，無法解析，messageKind = " << kind << "\n";
            return nullptr;
        }
    }
    catch (const std::exception& e) {
        // 捕獲解析過程中的錯誤，並印出錯誤訊息
        std::cerr << "❌ PacketDispatcher 解析失敗 (" << kind << "): " << e.what() << "\n";
        return nullptr;
    }
}
