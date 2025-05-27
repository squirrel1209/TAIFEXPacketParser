#pragma once

#include "protocol/CommonHeader.h"
#include "data/ProductInfo.h"
#include "base/ParsedResultBase.h"
#include <string>

/// ============================================
/// 📦 I010 封包結構：商品基本資料（Header + Payload）
/// - 封裝 CommonHeader 與 ProductInfo
/// - 繼承 ParsedResultBase，統一封包接口
/// ============================================
class I010Packet : public ParsedResultBase {
public:
    /// 🔸 建構子：初始化 Header 與 Body
    I010Packet(const CommonHeader& h, const ProductInfo& p)
        : header(h), body(p) {}

    /// 🔸 格式名稱（固定回傳 "I010"）
    std::string getFormatName() const override { return "I010"; }

    /// 🔸 簡易字串輸出（調用 ProductInfo.toString()）
    std::string toString() const override {
        return body.toString();
    }

    const CommonHeader header;     ///< 封包共用 Header
    const ProductInfo body;        ///< 封包主體（商品基本資料）
};
