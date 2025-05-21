#pragma once

#include <string>

// ✅ ParsedResultBase：所有解析結果的抽象基底類別
// 目的：允許不同格式資料以多型方式存入容器中（如 vector<shared_ptr<>>）
class ParsedResultBase {
public:
    virtual ~ParsedResultBase() = default;

    // ✅ 取得格式名稱（例如：I020）
    virtual std::string getFormatName() const = 0;

    // ✅ 輸出基本描述（for debug/log 用）
    virtual std::string toString() const = 0;
};
