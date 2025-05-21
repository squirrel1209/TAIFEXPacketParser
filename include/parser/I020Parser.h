#pragma once
#include "data/MatchInfo.h"

class I020Parser {
public:
    MatchInfo parseBody(const uint8_t* ptr, std::size_t length);
};
