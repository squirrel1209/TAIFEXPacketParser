#pragma once

#include "protocol/CommonHeader.h"
#include "data/PriceBandInfo.h"
#include "base/ParsedResultBase.h"
#include <string>

/// ============================================
/// 📦 I012 封包結構：漲跌幅資訊封裝（Header + Payload）
/// - 封裝 CommonHeader 與 PriceBandInfo
/// - 繼承 ParsedResultBase，統一封包接口
/// ============================================
class I012Packet : public ParsedResultBase {
public:
    /// 🔸 建構子：初始化 Header 與 Body
    I012Packet(const CommonHeader& h, const PriceBandInfo& p)
        : header(h), body(p) {}

    /// 🔸 格式名稱（固定回傳 "I012"）
    std::string getFormatName() const override { return "I012"; }

    /// 🔸 簡易字串輸出（調用 PriceBandInfo.toString()）
    std::string toString() const override {
        return body.toString();
    }

    const CommonHeader header;     ///< 封包共用 Header
    const PriceBandInfo body;      ///< 封包主體（漲跌幅資訊）
};
