#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <thread>
#include <filesystem>
#include <chrono>
#include <mutex>

namespace fs = std::filesystem;
using namespace std::chrono;

// 🔒 全域 Mutex：用來保護 cout，避免多執行緒時輸出混亂
std::mutex coutMutex;

// ⭐ 封裝安全輸出函式，確保多執行緒下的同步輸出
void safePrint(const std::string& msg) {
    std::lock_guard<std::mutex> lock(coutMutex);
    std::cout << msg << std::endl;
}

// 📦 單檔處理邏輯：讀取檔案、解析封包、寫入 4 個 CSV 檔案
void processFile(const std::string& filepath, const std::string& prefix) {
    {
        std::lock_guard<std::mutex> lock(coutMutex);
        std::cout << "🔍 開始處理檔案：" << filepath << std::endl;
    }

    // 開啟二進位檔案
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        safePrint("❌ 無法開啟檔案：" + filepath);
        return;
    }

    // 讀取完整檔案到記憶體
    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    safePrint("✅ [" + prefix + "] 讀取完成，檔案大小：" + std::to_string(buffer.size()) + " bytes");

    // 解析封包
    TAIFEXPacketParser parser(buffer);
    parser.parseAll();

    // 將結果加入記憶體資料庫
    TAIFEXMemoryDatabase db;
    for (const auto& result : parser.getResults()) {
        db.add(result);
    }

    // 確保輸出資料夾存在
    fs::create_directories("output");

    // 輸出 I020.csv (成交資料)
    std::ofstream i020File("output/" + prefix + "_I020.csv");
    i020File << "time,productId,price,volume,buySellFlag\n";
    for (const auto& [productId, deals] : db.getAllMatchInfo()) {
        for (const auto& deal : deals) {
            i020File << deal.matchTime.toInt() << ","
                     << deal.prodId.toString() << ","
                     << deal.firstPrice.toDecimalString(2) << ","
                     << deal.firstQty.toInt() << ","
                     << deal.firstSign.toString() << "\n";
        }
    }
    i020File.close();

    // 輸出 I012.csv (漲跌停價格)
    std::ofstream i012File("output/" + prefix + "_I012.csv");
    i012File << "productId,limitLevel,limitPrice,fallPrice\n";
    for (const auto& [productId, bands] : db.getAllPriceBandInfo()) {
        for (const auto& band : bands) {
            for (const auto& r : band.raiseLimitList) {
                i012File << band.productId.toString() << ","
                         << r.level.toInt() << ","
                         << r.price.toDecimalString(2) << ",\n";
            }
            for (const auto& f : band.fallLimitList) {
                i012File << band.productId.toString() << ","
                         << f.level.toInt() << ",,"
                         << f.price.toDecimalString(2) << "\n";
            }
        }
    }
    i012File.close();

    // 輸出 I080.csv (五檔報價)
    std::ofstream i080File("output/" + prefix + "_I080.csv");
    i080File << "time,productId,bidPrice1,bidVolume1,askPrice1,askVolume1\n";
    for (const auto& [productId, orders] : db.getAllOrderBookInfo()) {
        for (const auto& ob : orders) {
            i080File << "0,"
                     << ob.productId.toString() << ","
                     << ob.buyPrice1.toDecimalString(2) << ","
                     << ob.buyQty1.toInt() << ","
                     << ob.sellPrice1.toDecimalString(2) << ","
                     << ob.sellQty1.toInt() << "\n";
        }
    }
    i080File.close();

    // 輸出 I010.csv (商品基本資料)
    std::ofstream i010File("output/" + prefix + "_I010.csv");
    i010File << "productId,name,deliveryMonth\n";
    for (const auto& [productId, info] : db.getAllProductInfo()) {
        i010File << info.productId.toString() << ","
                 << "(NA),"
                 << info.deliveryDate.toString() << "\n";
    }
    i010File.close();

    safePrint("✅ [" + prefix + "] 輸出完成！");
}

int main() {
    // 記錄總執行開始時間
    auto start = high_resolution_clock::now();

    // 啟動兩個執行緒，分別處理 Fut.bin 與 Opt.bin
    std::thread futThread(processFile, "../data/Fut.bin", "Fut");
    std::thread optThread(processFile, "../data/Opt.bin", "Opt");

    // 等待兩個執行緒完成
    futThread.join();
    optThread.join();

    // 計算總耗時（以秒為單位）
    auto end = high_resolution_clock::now();
    auto durationSec = duration_cast<duration<double>>(end - start).count();

    // 安全輸出總執行時間
    safePrint("🎉 所有檔案處理完畢！總耗時: " + std::to_string(durationSec) + " 秒");

    return 0;
}
