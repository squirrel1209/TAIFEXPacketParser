#pragma once

#include <cstdint>
#include <cstddef>
#include "protocol/CommonHeader.h"

class HeaderParser {
public:
    // 傳入封包開頭位址與總長度，回傳解析後的 CommonHeader
    CommonHeader parse(const uint8_t* buffer, std::size_t totalLength);
};
