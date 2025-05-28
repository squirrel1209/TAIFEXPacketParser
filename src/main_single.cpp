#include "controller/TAIFEXPacketParser.h"
#include "storage/TAIFEXMemoryDatabase.h"
#include <fstream>
#include <vector>
#include <iostream>
#include <filesystem>
#include <chrono>

namespace fs = std::filesystem;
using namespace std::chrono;

void processFile(const std::string& filepath, const std::string& prefix) {
    std::ifstream file(filepath, std::ios::binary);
    if (!file) {
        std::cerr << "❌ 無法開啟檔案：" << filepath << std::endl;
        return;
    }

    std::vector<uint8_t> buffer(std::istreambuf_iterator<char>(file), {});
    std::cout << "✅ [" << prefix << "] 讀取完成，檔案大小：" << buffer.size() << " bytes\n";

    TAIFEXPacketParser parser(buffer);
    parser.parseAll();

    TAIFEXMemoryDatabase db;
    for (const auto& result : parser.getResults()) {
        db.add(result);
    }

    fs::create_directories("output");

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

    std::ofstream i010File("output/" + prefix + "_I010.csv");
    i010File << "productId,name,deliveryMonth\n";
    for (const auto& [productId, info] : db.getAllProductInfo()) {
        i010File << info.productId.toString() << ","
                 << "(NA),"
                 << info.deliveryDate.toString() << "\n";
    }
    i010File.close();

    std::cout << "✅ [" << prefix << "] 輸出完成！\n";
}

int main() {
    auto start = high_resolution_clock::now();

    processFile("../data/Fut.bin", "Fut");
    processFile("../data/Opt.bin", "Opt");

    auto end = high_resolution_clock::now();
    auto duration = duration_cast<milliseconds>(end - start).count();
    std::cout << "🎉 所有檔案處理完畢！總耗時: " << duration << " ms\n";

    return 0;
}
