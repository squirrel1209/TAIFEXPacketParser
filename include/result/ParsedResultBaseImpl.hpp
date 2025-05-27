#pragma once

#include "base/ParsedResultBase.h"
#include <string>

/// ===========================================================
/// 🌐 ParsedResultBaseImpl<T>
/// - 模板包裝類別，將資料結構 T 包裝為多型結果
/// - 繼承 ParsedResultBase，提供 getFormatName() / toString() 多型接口
/// ===========================================================
template <typename T>
class ParsedResultBaseImpl : public ParsedResultBase {
public:
    T data;  ///< 解析後的資料結構（如 ProductInfo, MatchInfo...）
    std::string formatName;  ///< 封包格式名稱（由 Parser 設定，如 "I010"）

    /// ⭐ 建構子
    /// - 接收解析後的資料（T）與格式名稱（由 Parser 提供）
    ParsedResultBaseImpl(const T& d, const std::string& fmt)
        : data(d), formatName(fmt) {}

    /// ⭐ 回傳封包格式名稱
    /// - 提供給統一處理邏輯使用
    std::string getFormatName() const override {
        return formatName;
    }

    /// ⭐ 回傳解析結果的簡易字串表示（for debug / log）
    /// - 呼叫 T 的 toString()，假設每個 T 都有實作 toString()
    std::string toString() const override {
        return data.toString();
    }

    /// ⭐ 回傳封包資料本體
    /// - 提供給 MemoryDatabase 存取封包資料
    T getData() const {
        return data;
    }
};
