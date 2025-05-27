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

    db.dump();
    std::cout << "✅ 封包解析與資料儲存流程結束。\n";
    std::cout << "=========================================\n";

    // ================================================
    // 4️⃣ 輸出 4 個格式的 .csv 檔案
    // ================================================

    // ====== I020.csv: 撮合成交資料 ======
    std::ofstream i020File("I020.csv");
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
    std::cout << "✅ I020.csv 輸出完成！\n";

    // ====== I012.csv: 漲跌幅資料 ======
    std::ofstream i012File("I012.csv");
    i012File << "productId,limitLevel,limitPrice\n";
    for (const auto& [productId, bands] : db.getAllPriceBandInfo()) {
        for (const auto& band : bands) {
            for (const auto& r : band.raiseLimitList) {
                i012File << band.productId.toString() << ","
                         << r.level.toInt() << ","
                         << r.price.toDecimalString(2) << "\n";
            }
            // 若需要跌停價，可在這裡加 band.fallLimitList 處理
        }
    }
    i012File.close();
    std::cout << "✅ I012.csv 輸出完成！\n";

    // ====== I080.csv: 委託簿資料 ======
    std::ofstream i080File("I080.csv");
    i080File << "productId,bidPrice1,bidVolume1,askPrice1,askVolume1\n";
    for (const auto& [productId, orders] : db.getAllOrderBookInfo()) {
        for (const auto& ob : orders) {
            i080File << ob.productId.toString() << ","
                     << ob.buyPrice1.toDecimalString(2) << ","
                     << ob.buyQty1.toInt() << ","
                     << ob.sellPrice1.toDecimalString(2) << ","
                     << ob.sellQty1.toInt() << "\n";
        }
    }
    i080File.close();
    std::cout << "✅ I080.csv 輸出完成！\n";

    // ====== I010.csv: 商品基本資料 ======
    std::ofstream i010File("I010.csv");
    i010File << "productId,deliveryDate\n";
    for (const auto& [productId, info] : db.getAllProductInfo()) {
        i010File << info.productId.toString() << ","
                 << info.deliveryDate.toString() << "\n";
    }
    i010File.close();
    std::cout << "✅ I010.csv 輸出完成！\n";

    return 0;
}
