#include "controller/HeaderParser.h"
#include <stdexcept>

CommonHeader HeaderParser::parse(const uint8_t* buffer, std::size_t totalLength) {
    if (totalLength < sizeof(CommonHeader)) {
        throw std::runtime_error("Header 長度不足，無法解析");
    }
    const CommonHeader* headerPtr = reinterpret_cast<const CommonHeader*>(buffer);
    return *headerPtr; // 回傳複製結果，避免原地操作
}
