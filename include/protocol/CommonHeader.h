#pragma once

#include "common/FixedArray.h"
#include "common/TBCD.h"

// ✅ 通用 TAIFEX 封包 Header 結構（所有格式共用）
// 對應手冊中 ESC-CODE、TRANSMISSION-CODE、MESSAGE-KIND 等欄位

// 🔸 傳輸層字元欄位（固定 ASCII）
using ESCCode = FixedArray<char, 1>;              // 固定 ASCII 27 = 0x1B
using TransmissionCode = FixedArray<char, 1>;     // '1' ~ '6'，來源類型（期貨、選擇權）
using MessageKind = FixedArray<char, 1>;          // '1', 'A', '2' 等訊息類型（代表資料格式）

// 🔸 共用封包 Header 結構（用於所有格式 I010~I080）
struct CommonHeader {
    ESCCode escCode;                     // 封包起始標記
    TransmissionCode transmissionCode;   // 傳輸來源（期貨、選擇權）
    MessageKind messageKind;             // 封包種類（代表格式）

    BCD6 informationTime;                // 發送時間（HHMMSS + 毫秒）
    BCD4 informationSeq;                 // 封包流水號（遞增）
    BCD1 versionNo;                      // 封包格式版本
    BCD2 bodyLength;                     // 封包長度（不含 header）
};
