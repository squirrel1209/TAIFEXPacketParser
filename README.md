# 🧠 TAIFEXPacketProject

TAIFEXPacketProject 是一個專為台灣期交所（TAIFEX）封包格式所打造的高效能封包解析系統。專案具有模組化結構，支援多格式（如 I020、I012、I080）解析、記憶體內資料庫管理、以及可擴展的查詢與輸出介面，適合用於市場資料監控、交易系統前處理等應用場景。
---
⚙️ 功能項目總覽
功能名稱	            說明
封包格式自動識別	    根據封包 Header 解析格式代碼（如 I020、I012）
各格式封包解析支援	    支援多種期交所封包格式（I020、I012、I080 等）
即時資料解析	        可應用於 socket 串流資料（設計預留）
記憶體資料儲存	        使用 TAIFEXMemoryDatabase 儲存解析結果
查詢功能	            提供商品代碼、時間區間等條件查詢
輸出功能	            支援 JSON / CSV 輸出封包內容
狀態追蹤	            即時撮合價格與數量的狀態更新
易於擴充與測試	        採用模組化、介面導向設計，可快速支援新格式
---

## 🧩 UML 模組關聯圖（實體圖）

![模組物件圖](system_diagram.png)

---

## 🔄 資料處理流程圖

![資料處理流程圖](data_flowchart.png)

---

## 📁 專案結構總覽

```
TAIFEXPacketProject/
├── CMakeLists.txt              # 🔧 CMake 建置設定
├── data/                       # 📂 測試用二進位封包檔
├── include/                    # 📁 所有標頭檔 (.h)
│   ├── base/                   # 🌱 抽象類別與介面
│   ├── common/                 # 🔧 固定長度結構工具
│   ├── protocol/               # 🧾 封包通用欄位（Header）
│   ├── controller/             # 🎮 主解析流程控制
│   ├── dispatcher/             # 📨 封包分發邏輯
│   ├── parser/                 # 🧩 各格式 payload 解析器
│   ├── data/                   # 📊 封包資料結構（封裝 header + body）
│   ├── storage/                # 💾 記憶體內資料庫
│   ├── output/                 # 🖨️ 結果輸出（JSON / CSV）
│   ├── query/                  # 🔍 查詢模組（供 API/GUI 使用）
│   └── state/                  # 🔁 即時狀態追蹤模組
├── src/                        # 📂 對應 .cpp 檔案
└── main.cpp                    # 🧪 測試整合主程式
```

---

## 🔧 功能模組說明

| 模組名稱              | 說明 |
|-----------------------|------|
| `TAIFEXPacketParser`  | 驅動整體封包流程，包含 HeaderParser 與 Dispatcher |
| `HeaderParser`        | 擷取封包格式代碼與時間等共通欄位 |
| `PacketDispatcher`    | 根據格式代號（如 I020）分發至對應 `Parser` 類別 |
| `I020Parser`          | 撮合資訊格式（I020）之 payload 解碼器 |
| `MatchInfo`           | 撮合資訊欄位結構（數據對應欄位定義） |
| `TAIFEXMemoryDatabase`| 儲存所有解析結果，提供查詢與輸出來源 |
| `OutputFormatter`     | 將資料庫結果輸出為 JSON / CSV 檔 |
| `DataQuery`           | 查詢模組，支援商品代碼、時間區間查詢等 |
| `StateUpdater`        | 即時撮合狀態追蹤，可支援圖形化顯示模組 |

---

## 📦 已支援封包格式

| 格式代碼 | 封包名稱           | 狀態     |
|----------|--------------------|----------|
| I020     | 撮合資訊           | ✅ 已完成 |
| I012     | 漲跌幅資訊         | 🚧 開發中 |
| I080     | 委託簿快照資訊     | 🚧 開發中 |
| I010     | 商品基本資訊       | 🕓 計劃中 |

---

## 🧪 執行方式與測試資料

請放置測試檔案於 `data/` 目錄，例如：

```
data/sample.bin
```

並使用以下步驟建置與執行：

```bash
mkdir build && cd build
cmake ..
make
./TAIFEXMain
```

主程式將讀取 `sample.bin`、解析封包，並輸出資訊至 console 或儲存至資料庫物件。

---

## 📌 擴充與客製化設計

此專案設計時即考慮：

- ✅ 支援多封包格式並可快速擴充 `parser/` 與 `data/`
- ✅ 封裝解析邏輯與資料結構，提升重用性與可測試性
- ✅ 可串接至 API、GUI 查詢模組
- ✅ 可擴充為即時 socket 串流解析架構（WIP）

---

## 📚 延伸模組建議

| 模組名稱        | 功能構想                              |
|-----------------|---------------------------------------|
| `SocketReceiver`| 建立 socket server，自動接收封包串流 |
| `TimeSeriesDB`  | 接上 MongoDB / TimescaleDB 儲存歷史數據 |
| `MetricsModule` | 統計指標模組（如最大/最小/均值等）     |

---

## 👨‍💻 開發者筆記

- 所有解析結構盡量符合 POD（Plain Old Data）設計，方便記憶體 mapping 與高速處理。
- 使用 `FixedArray`, `FixedString`, `TBCD` 等工具實現封包位元對齊、BCD 編碼處理。
- 實作時可優先測試單一格式（如 I020），再逐步擴充解析支援。

---

## 👨‍💻 輸出格式
🖨️ CSV 輸出格式範例
本專案支援將解析後的封包資料輸出為 .csv 格式，位於 output/ 目錄，方便後續以 Excel、Pandas 等工具分析。以下為各格式輸出範例：

✅ I020 撮合資訊
time	productId	price	volume
093001	TXF202405	20500	5
093002	TXF202405	20525	1

✅ I012 漲跌幅資訊（多階段漲跌停）
time	productId	limitLevel	limitPrice
093000	TXF202405	1	21000.00
093000	TXF202405	2	21500.00

✅ I080 委託簿快照（簡化）
time	productId	bidPrice1	bidVolume1	askPrice1	askVolume1
093001	TXF202405	20475	10	20500	12

✅ I010 商品基本資訊
productId	name	deliveryMonth
TXF202405	台指期05	202405

📂 檔案輸出位置
執行主程式後將產生下列檔案於 output/：

lua
複製
編輯
output/
├── I020.csv
├── I012.csv
├── I080.csv
└── I010.csv
如需匯出請呼叫 OutputFormatter::exportToCSV(database, "output/")。
