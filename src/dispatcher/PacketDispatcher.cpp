#include "dispatcher/PacketDispatcher.h"
#include "parser/I020Parser.h"
#include "data/I020Packet.h"

std::shared_ptr<ParsedResultBase> PacketDispatcher::dispatch(
    const CommonHeader& header,
    const uint8_t* bodyPtr,
    std::size_t bodyLen
) {
    // ✅ 根據 messageKind 判斷是哪一種格式（如 '1' = I020）
    char kind = header.messageKind[0];

    switch (kind) {
        case '1': { // 📦 I020 封包（成交明細）
            I020Parser parser;
            MatchInfo body = parser.parseBody(bodyPtr, bodyLen);
            return std::make_shared<I020Packet>(header, body);
        }

        // 🟡 待擴充格式範例（之後支援 I012, I080 可加入）
        // case '2': {
        //     I012Parser parser;
        //     PriceBandInfo body = parser.parseBody(bodyPtr, bodyLen);
        //     return std::make_shared<I012Packet>(header, body);
        // }

        default:
            // ❌ 不支援的格式
            return nullptr;
    }
}
