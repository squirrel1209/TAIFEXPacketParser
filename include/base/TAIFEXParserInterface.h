#pragma once

#include <vector>
#include <memory>
#include "base/ParsedResultBase.h"

/// ==========================================
/// 🌐 TAIFEXParserInterface
/// - 所有解析器（I010, I012, I020, I080）的共同介面
/// - 提供 parse() 方法回傳多型結果
/// ==========================================
class TAIFEXParserInterface {
public:
    virtual ~TAIFEXParserInterface() = default;

    /// 🧩 執行封包解析，回傳泛型結果
    virtual std::shared_ptr<ParsedResultBase> parse(const std::vector<uint8_t>& body) = 0;
};
