#include "parser/I010Parser.h"
#include "result/ParsedResultBaseImpl.hpp"
#include <cstring>
#include <stdexcept>

/// 將 payload raw bytes 複製成 ProductInfo 結構（採用 POD + memcpy）
std::shared_ptr<ParsedResultBase> I010Parser::parse(const std::vector<uint8_t>& body) {
    if (body.size() < sizeof(ProductInfo)) {
        throw std::runtime_error("I010Parser: payload size too small");
    }

    ProductInfo info;

    // ✅ 高效率一次性複製，安全對應 POD 結構
    std::memcpy(&info, body.data(), sizeof(ProductInfo));

    // ✅ 包裝為泛型解析結果
    return std::make_shared<ParsedResultBaseImpl<ProductInfo>>(info);
}
