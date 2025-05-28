#pragma once

#include <vector>
#include <memory>
#include <cstdint>
#include <iostream>
#include "base/ParsedResultBase.h"
#include "controller/HeaderParser.h"
#include "dispatcher/PacketDispatcher.h"

/// ===================================================
/// 📦 TAIFEXPacketParser：主封包解析控制器
/// 主要功能：
/// - 從原始資料 buffer 中切分封包
/// - 檢查封包起始碼、長度、結尾
/// - 使用 HeaderParser 解析 Header
/// - 使用 PacketDispatcher 分派格式解析
/// - 將解析結果存入 results 容器
/// ===================================================
class TAIFEXPacketParser {
public:
    /// 建構子：接收外部提供的原始資料 buffer
    explicit TAIFEXPacketParser(const std::vector<uint8_t>& raw);

    /// 🚀 啟動完整解析流程
    void parseAll();

    /// 取得所有解析結果
    const std::vector<std::shared_ptr<ParsedResultBase>>& getResults() const;

private:
    const std::vector<uint8_t>& rawData;  ///< 原始資料參考
    std::vector<std::shared_ptr<ParsedResultBase>> results; ///< 解析後結果
};
