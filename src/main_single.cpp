#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <chrono>

void processFile(const std::string& filePath, const std::string& prefix) {
    try {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::cerr << "❌ 無法開啟封包檔案：" << filePath << "\n";
            return;
        }

        std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
        std::cout << "✅ 讀取完成：" << filePath << "，大小：" << buffer.size() << " bytes\n";

        TAIFEXPacketParser parser(buffer);
        parser.parseAll();

        TAIFEXMemoryDatabase db;
        for (const auto& result : parser.getResults()) {
            db.add(result);
        }

        db.dump();

        // 輸出 CSV（以 I020 為例）
        std::ofstream i020File(prefix + "_I020.csv");
        i020File << "time,productId,price,volume\n";
        for (const auto& [productId, deals] : db.getAllMatchInfo()) {
            for (const auto& deal : deals) {
                i020File << deal.matchTime.toInt() << ","
                         << deal.prodId.toString() << ","
                         << deal.firstPrice.toDecimalString(2) << ","
                         << deal.firstQty.toInt() << "\n";
            }
        }
        i020File.close();

        std::cout << "✅ " << filePath << " 處理完成！\n";
    }
    catch (const std::exception& e) {
        std::cerr << "❌ 錯誤：" << e.what() << "\n";
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    processFile("../data/Fut.bin", "Fut");
    processFile("../data/Opt.bin", "Opt");

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    std::cout << "⏱️ 總執行時間（單執行緒）：" << elapsed.count() << " 秒\n";

    return 0;
}
