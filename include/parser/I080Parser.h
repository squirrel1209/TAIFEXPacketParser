// I080Parser.h
#pragma once

#include "controller/TAIFEXParserInterface.h"
#include "data/OrderBookInfo.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <memory>
#include <cstring>

/// ===========================================================
/// 📘 I080Parser：解析 I080 委託簿揭示訊息
/// - 對應 OrderBookInfo 結構（含五檔買賣價 + 衍生價）
/// - 資料格式為固定長度，可直接使用 memcpy 對應欄位
/// - 若 derivedFlag ≠ "01"，會清空衍生欄位保護資料正確性
/// ===========================================================
class I080Parser : public TAIFEXParserInterface {
public:
    /// ✅ 主要解析邏輯
    std::shared_ptr<ParsedResultBase> parse(const std::vector<uint8_t>& body) override {
        constexpr size_t baseLength = sizeof(OrderBookInfo);

        // 👉 安全檢查：資料長度不足
        if (body.size() < baseLength) {
            throw std::runtime_error("I080Parser: payload size too small");
        }

        // ✅ 配置解析結果結構（使用模板封裝）
        auto result = std::make_shared<ParsedResultBaseImpl<OrderBookInfo>>();

        // ✅ 使用 memcpy 直接轉入 POD 結構
        std::memcpy(&result->data, body.data(), baseLength);

        // ⚠️ 衍生價格揭示欄位處理：
        // 若 derivedFlag ≠ "01"，則下列欄位不應使用，手動清空避免誤用
        if (result->data.derivedFlag.toInt() != 1) {
            std::memset(&result->data.derivedBuyPrice1, 0, sizeof(BCD9));
            std::memset(&result->data.derivedBuyQty1, 0, sizeof(BCD8));
            std::memset(&result->data.derivedSellPrice1, 0, sizeof(BCD9));
            std::memset(&result->data.derivedSellQty1, 0, sizeof(BCD8));
        }

        // 👉 回傳結果（指向 OrderBookInfo 結構資料）
        return result;
    }

    /// ✅ 顯示封包對應的格式名稱（給 UI 或 log 使用）
    std::string formatName() const override {
        return "I080 委託簿揭示訊息";
    }
};
