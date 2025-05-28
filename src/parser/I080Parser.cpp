// I080Parser.cpp
#include "parser/I080Parser.h"                    // 解析器的標頭檔
#include "result/ParsedResultBaseImpl.hpp"        // 封裝解析結果的多型包裝
#include <stdexcept>                              // 處理例外情況
#include <cstring>                                // 使用 std::memcpy / std::memset

/// =============================================
/// 📦 I080Parser::parse() 實作
/// - 功能：解析 I080 委託簿封包的主體資料
/// - 傳入：封包主體的 byte 陣列
/// - 回傳：解析後的多型封裝結果
/// =============================================
std::shared_ptr<ParsedResultBase> I080Parser::parse(const std::vector<uint8_t>& body) {
    constexpr size_t baseLength = sizeof(OrderBookInfo); // I080 封包結構的固定長度

    // 1️⃣ 檢查資料長度是否足夠
    if (body.size() < baseLength) {
        throw std::runtime_error("I080Parser: payload size too small"); // 資料不足，拋出例外
    }

    // 2️⃣ 將 byte 陣列複製到結構體
    OrderBookInfo info;
    std::memcpy(&info, body.data(), baseLength); // 使用 memcpy 將原始資料複製進結構

    // 3️⃣ 檢查是否有衍生價格資料（Derived Flag）
    if (info.derivedFlag.toInt() != 1) {
        // 如果 derivedFlag 不為 1，則清除相關欄位內容（設為全 0）
        std::memset(&info.derivedBuyPrice1, 0, sizeof(BCD9));
        std::memset(&info.derivedBuyQty1, 0, sizeof(BCD8));
        std::memset(&info.derivedSellPrice1, 0, sizeof(BCD9));
        std::memset(&info.derivedSellQty1, 0, sizeof(BCD8));
    }

    // 4️⃣ 將解析後結果封裝成多型物件（格式名稱為 "I080"）
    return std::make_shared<ParsedResultBaseImpl<OrderBookInfo>>(info, "I080");
}
