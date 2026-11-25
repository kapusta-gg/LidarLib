#include <optional>
#include <stdexcept>
#include <string>
#include <sstream>

#include "lidar_manager.h"
#include "lidar_reader.h"

LidarReader::LidarReader(): file(filename) {};
LidarReader::LidarReader(const std::string& filename): filename(filename), file(filename) {};
LidarReader::LidarReader(char sep): sep(sep), file(filename) {};
LidarReader::LidarReader(const std::string& filename, char sep): filename(filename), sep(sep), file(filename) {};


std::vector<std::string> LidarReader::get_line()
{
    std::string line;

    if (!std::getline(this->file, line)) {
        return {};
    }
    std::stringstream ss(line);
    std::vector<std::string> values;
    std::string value;

    while (std::getline(ss, value, this->sep)) {
        values.push_back(value);
    }

    return values;
}

std::vector<std::vector<std::string>> LidarReader::get_lines()
{
    std::vector<std::vector<std::string>> rows;

    while (file.good()) {
        auto row = get_line();

        if (!row.empty()) {
            rows.push_back(row);
        }
    }

    return rows;
}

std::optional<LidarData> LidarReader::read_scan()
{
    std::vector<std::string> row;

    // Берём сохранённую строку или читаем новую
    if (pending_row) {
        row = std::move(*pending_row);
        pending_row.reset();
    }
    else {
        row = get_line();
    }

    // Пропускаем заголовок
    if (!row.empty() && row[0] == "timestamp") {
        row = get_line();
    }

    if (row.empty()) {
        return std::nullopt;
    }

    if (row.size() != 5) {
        throw std::runtime_error("Invalid CSV row");
    }

    long long timestamp = std::stoll(row[0]);
    LidarData data(timestamp);

    while (!row.empty()) {
        if (row.size() != 5) {
            throw std::runtime_error("Invalid CSV row");
        }

        long long current_timestamp = std::stoll(row[0]);

        // Начался следующий скан
        if (current_timestamp != timestamp) {
            pending_row = std::move(row);
            break;
        }

        float angle = std::stof(row[3]);
        float dist = std::stof(row[4]);

        data.add_node(angle, dist);

        row = get_line();
    }

    return data;
}

std::vector<LidarData> LidarReader::read_scans()
{
    auto rows = get_lines();
    std::vector<LidarData> scans;

    if (rows.empty()) {
        return scans;
    }

    for (const auto& row : rows) {

        // Пропускаем заголовок
        if (!row.empty() && row[0] == "timestamp") {
            continue;
        }

        if (row.size() != 5) {
            throw std::runtime_error("Invalid CSV row");
        }

        long long timestamp = std::stoll(row[0]);

        // Создаём новый скан при изменении timestamp
        if (scans.empty() ||
            scans.back().get_timestamp() != timestamp) {

            scans.emplace_back(timestamp);
        }

        float angle = std::stof(row[3]);
        float dist = std::stof(row[4]);

        scans.back().add_node(angle, dist);
    }

    return scans;
}