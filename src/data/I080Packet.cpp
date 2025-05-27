#include "I080Packet.h"

/// 🔸 取得封包格式名稱（固定回傳 "I080"）
std::string I080Packet::getFormatName() const {
    return "I080";
}

/// 🔸 將封包內容轉為字串（回傳商品代碼為主要輸出）
std::string I080Packet::toString() const {
    return body.productId.toString();
}
