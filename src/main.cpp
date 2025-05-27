// main.cpp
#include <fstream>
#include <vector>
#include <cstdint>
#include <iostream>
#include <thread>
#include <iomanip>
#include <filesystem>  // C++17 for std::filesystem

#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"

// =============================================
// 🔸 儲存資料庫內容到多個 CSV 檔
// =============================================
void saveToCSV(const TAIFEXMemoryDatabase& db, const std::string& prefix) {
    // ===== 1️⃣ I020 撮合成交資訊 =====
    {
        std::ofstream ofs(prefix + "_I020.csv");
        ofs << "productId,time,price,volume\n";
        for (const auto& [productId, matchList] : db.matchInfoMap) {
            for (const auto& match : matchList) {
                ofs << match.prodId.toString() << ","
                    << match.matchTime.toInt() << ","
                    << match.firstPrice.toDecimalString(2) << ","
                    << match.firstQty.toInt() << "\n";
            }
        }
        ofs.close();
    }

    // ===== 2️⃣ I012 漲跌幅資訊 =====
    {
        std::ofstream ofs(prefix + "_I012.csv");
        ofs << "productId,level,type,price\n";
        for (const auto& [productId, bandList] : db.priceBandMap) {
            for (const auto& band : bandList) {
                for (const auto& r : band.raiseLimitList) {
                    ofs << band.productId.toString() << ","
                        << int(r.level.toInt()) << ","
                        << "raise" << ","
                        << r.price.toDecimalString(2) << "\n";
                }
                for (const auto& f : band.fallLimitList) {
                    ofs << band.productId.toString() << ","
                        << int(f.level.toInt()) << ","
                        << "fall" << ","
                        << f.price.toDecimalString(2) << "\n";
                }
            }
        }
        ofs.close();
    }

    // ===== 3️⃣ I080 五檔委託簿資訊 =====
    {
        std::ofstream ofs(prefix + "_I080.csv");
        ofs << "productId,bidPrice1,bidQty1,askPrice1,askQty1\n";
        for (const auto& [productId, orderList] : db.orderBookMap) {
            for (const auto& order : orderList) {
                ofs << order.productId.toString() << ","
                    << order.buyPrice1.toDecimalString(2) << ","
                    << order.buyQty1.toInt() << ","
                    << order.sellPrice1.toDecimalString(2) << ","
                    << order.sellQty1.toInt() << "\n";
            }
        }
        ofs.close();
    }

    // ===== 4️⃣ I010 商品基本資料 =====
    {
        std::ofstream ofs(prefix + "_I010.csv");
        ofs << "productId,referencePrice,deliveryDate\n";
        for (const auto& [productId, prod] : db.productInfoMap) {
            ofs << prod.productId.toString() << ","
                << prod.referencePrice.toDecimalString(prod.decimalLocator.toInt()) << ","
                << prod.deliveryDate.toString() << "\n";
        }
        ofs.close();
    }

    std::cout << "[" << prefix << "] ✅ 所有 CSV 檔案已完成輸出！\n";
}

// =============================================
// 🔸 處理單一檔案（讀取 → 解析 → 存入資料庫 → 輸出 CSV）
// =============================================
void processFile(const std::string& filePath, const std::string& prefix) {
    std::cout << "[" << prefix << "] 🔍 嘗試打開檔案：" << filePath << std::endl;
    std::cout << "[" << prefix << "] 📂 當前工作目錄：" << std::filesystem::current_path() << std::endl;

    std::ifstream file(filePath, std::ios::binary);
    if (!file) {
        std::cerr << "[" << prefix << "] ❌ 無法開啟檔案：" << filePath << "\n";
        return;
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    std::cout << "[" << prefix << "] ✅ 檔案讀取完成，大小：" << buffer.size() << " bytes\n";

    TAIFEXPacketParser parser(buffer);
    parser.parseAll();

    TAIFEXMemoryDatabase db;
    for (const auto& result : parser.getResults()) {
        db.add(result);
    }

    std::cout << "[" << prefix << "] 📊 資料庫內容如下：\n";
    db.dump();

    saveToCSV(db, prefix);
}

// =============================================
// 🔸 主程式入口
// =============================================
int main() {
    std::cout << "🎬 開始處理檔案...\n";

    std::thread futThread(processFile, std::string("../data/Fut.bin"), std::string("Fut"));
    std::thread optThread(processFile, std::string("../data/Opt.bin"), std::string("Opt"));

    futThread.join();
    optThread.join();

    std::cout << "🎉 所有檔案處理完成！\n";
    return 0;
}
