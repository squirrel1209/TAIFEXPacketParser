// I080Parser.h
#pragma once

#include "protocol/CommonHeader.h"
#include "data/OrderBookInfo.h"
#include "base/ParsedResultBase.h"
#include <memory>
#include <vector>

/// ================================================
/// 📦 I080Parser：解析 I080 封包（委託簿快照）
///
/// - 輸入原始 Payload 資料（std::vector<uint8_t>）
/// - 輸出解析結果（ParsedResultBase 指標）
/// ================================================
class I080Parser {
public:
    /// 解析封包 Payload，回傳多型結果
    std::shared_ptr<ParsedResultBase> parse(const std::vector<uint8_t>& body);
};
