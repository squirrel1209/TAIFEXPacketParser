#include "I012Packet.h"

/// 🔸 取得封包格式名稱（固定回傳 "I012"）
std::string I012Packet::getFormatName() const {
    return "I012";
}

/// 🔸 將封包內容轉為字串（調用 PriceBandInfo.toString()）
std::string I012Packet::toString() const {
    return body.toString();
}
