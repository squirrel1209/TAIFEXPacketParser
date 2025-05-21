#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"
#include <fstream>
#include <vector>
#include <iostream>

int main() {
    // 1️⃣ 讀取封包檔案（請放在 ./data/sample.bin）
    std::ifstream file("../data/Opt.bin", std::ios::binary);
    if (!file) {
        std::cerr << "❌ 無法開啟檔案 \n";
        return 1;
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    std::cout << "✅ 檔案讀取完成，大小：" << buffer.size() << " bytes\n";

    // 2️⃣ 呼叫封包解析器
    TAIFEXPacketParser parser(buffer);
    parser.parseAll();

    // 3️⃣ 建立記憶體資料庫並加入結果
    TAIFEXMemoryDatabase db;
    for (const auto& result : parser.getResults()) {
        db.add(result);
    }

    // 4️⃣ 印出成交結果
    db.dump();

    return 0;
}
