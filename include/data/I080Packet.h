#pragma once

#include "protocol/CommonHeader.h"
#include "data/OrderBookInfo.h"
#include "base/ParsedResultBase.h"
#include <string>

/// ============================================
/// 📦 I080 封包結構：委託簿快照（Header + Payload）
/// - 封裝 CommonHeader 與 OrderBookInfo
/// - 繼承 ParsedResultBase，統一封包接口
/// ============================================
class I080Packet : public ParsedResultBase {
public:
    /// 🔸 建構子：初始化 Header 與 Body
    I080Packet(const CommonHeader& h, const OrderBookInfo& o)
        : header(h), body(o) {}

    /// 🔸 格式名稱（固定回傳 "I080"）
    std::string getFormatName() const override { return "I080"; }

    /// 🔸 簡易字串輸出（僅回傳商品代碼）
    std::string toString() const override {
        return body.productId.toString();
    }

    const CommonHeader header;     ///< 封包共用 Header
    const OrderBookInfo body;      ///< 封包主體（五檔買賣盤）
};
