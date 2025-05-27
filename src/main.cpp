#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"
#include <fstream>
#include <vector>
#include <iostream>

int main() {
    // ================================================
    // 1️⃣ 載入封包檔案（請將測試檔放在 ./data 目錄下）
    // ================================================
    std::ifstream file("../data/Opt.bin", std::ios::binary); // 📝 可修改檔案名稱
    if (!file) {
        std::cerr << "❌ 無法開啟封包檔案！請確認路徑與檔名是否正確。\n";
        return 1;
    }

    // 讀取檔案至 buffer（以二進位模式讀取完整檔案內容）
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    std::cout << "✅ 封包檔案讀取完成，大小：" << buffer.size() << " bytes\n";

    // ================================================
    // 2️⃣ 初始化封包解析器並執行解析流程
    // ================================================
    TAIFEXPacketParser parser(buffer);
    parser.parseAll();

    // ================================================
    // 3️⃣ 建立記憶體資料庫，存入所有解析結果
    // ================================================
    TAIFEXMemoryDatabase db;
    for (const auto& result : parser.getResults()) {
        db.add(result);
    }

    // ================================================
    // 4️⃣ 輸出解析後的封包資訊（for Debug/驗證用）
    // ================================================
    db.dump();

    std::cout << "✅ 封包解析與資料儲存流程結束。\n";
    std::cout << "=========================================\n";

    return 0;
}
