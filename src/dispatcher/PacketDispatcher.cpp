#include "dispatcher/PacketDispatcher.h"
#include "parser/I010Parser.h"
#include "parser/I012Parser.h"
#include "parser/I020Parser.h"
#include "parser/I080Parser.h"
#include <iostream>
#include <unordered_map>
#include <functional>

/// === Parser 註冊表（定義每種 Parser 的工廠函式） ===
using ParserFactory = std::function<std::shared_ptr<ParsedResultBase>(const std::vector<uint8_t>&)>;

static const std::unordered_map<char, ParserFactory> parserMap = {
    { '1', [](const std::vector<uint8_t>& body) { I010Parser p; return p.parse(body); } },
    { 'A', [](const std::vector<uint8_t>& body) { I012Parser p; return p.parse(body); } },
    { '0', [](const std::vector<uint8_t>& body) { I020Parser p; return p.parse(body); } },
    { '2', [](const std::vector<uint8_t>& body) { I080Parser p; return p.parse(body); } }
};

std::shared_ptr<ParsedResultBase> PacketDispatcher::dispatch(
    const CommonHeader& header,
    const uint8_t* bodyPtr,
    std::size_t bodyLen
) {
    // 取出 messageKind 字元
    const std::string kindStr = header.messageKind.toString();
    if (kindStr.empty()) {
        std::cerr << "⚠️ messageKind 空字串，跳過封包！\n";
        return nullptr;
    }
    const char kind = kindStr[0];

    // 將 body bytes 包裝為 vector<uint8_t>
    std::vector<uint8_t> body(bodyPtr, bodyPtr + bodyLen);

    // 查找對應 Parser
    auto it = parserMap.find(kind);
    if (it != parserMap.end()) {
        try {
            return it->second(body);
        } catch (const std::exception& e) {
            std::cerr << "❌ 解析失敗 (messageKind = " << kind << "): " << e.what() << "\n";
            return nullptr;
        }
    }

    // 沒找到對應 Parser
    std::cerr << "⚠️ 未知封包格式，無法解析，messageKind = [" << kind << "]\n";
    return nullptr;
}
