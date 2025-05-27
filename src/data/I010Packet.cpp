#include "I010Packet.h"

/// 🔸 取得封包格式名稱（固定回傳 "I010"）
std::string I010Packet::getFormatName() const {
    return "I010";
}

/// 🔸 將封包內容轉為字串（調用 ProductInfo.toString()）
std::string I010Packet::toString() const {
    return body.toString();
}
