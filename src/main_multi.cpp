#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <thread>
#include <mutex>
#include <chrono>

std::mutex logMutex;

void processFile(const std::string& filePath, const std::string& prefix) {
    try {
        std::ifstream file(filePath, std::ios::binary);
        if (!file) {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cerr << "❌ 無法開啟封包檔案：" << filePath << "\n";
            return;
        }

        std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
        {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cout << "✅ 讀取完成：" << filePath << "，大小：" << buffer.size() << " bytes\n";
        }

        TAIFEXPacketParser parser(buffer);
        parser.parseAll();

        TAIFEXMemoryDatabase db;
        for (const auto& result : parser.getResults()) {
            db.add(result);
        }

        {
            std::lock_guard<std::mutex> lock(logMutex);
            db.dump();
        }

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

        {
            std::lock_guard<std::mutex> lock(logMutex);
            std::cout << "✅ " << filePath << " 處理完成！\n";
        }
    }
    catch (const std::exception& e) {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cerr << "❌ 解析過程發生錯誤 [" << filePath << "]：" << e.what() << "\n";
    }
}

int main() {
    auto start = std::chrono::high_resolution_clock::now();

    std::thread futThread(processFile, "../data/Fut.bin", "Fut");
    std::thread optThread(processFile, "../data/Opt.bin", "Opt");

    futThread.join();
    optThread.join();

    auto end = std::chrono::high_resolution_clock::now();
    std::chrono::duration<double> elapsed = end - start;
    {
        std::lock_guard<std::mutex> lock(logMutex);
        std::cout << "✅ 多執行緒處理完成！\n";
        std::cout << "⏱️ 總執行時間（多執行緒）：" << elapsed.count() << " 秒\n";
    }

    return 0;
}
