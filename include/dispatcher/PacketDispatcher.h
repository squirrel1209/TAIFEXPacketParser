#pragma once

#include <memory>
#include <cstddef>
#include <cstdint>
#include "protocol/CommonHeader.h"
#include "base/ParsedResultBase.h"

// ✅ PacketDispatcher：封包分派器（根據 messageKind 決定用哪個 parser）
// 由主控器（TAIFEXPacketParser）呼叫，根據封包 header，將封包交由對應格式的 parser 處理
class PacketDispatcher {
public:
    // 傳入 header + 封包 body buffer（起始位址與長度），回傳解析完成後的結果
    // 若無對應 parser，回傳 nullptr
    std::shared_ptr<ParsedResultBase> dispatch(
        const CommonHeader& header,
        const uint8_t* bodyPtr,
        std::size_t bodyLen
    );
};
