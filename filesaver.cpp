#include "fileSaver.h"
#include "farmers.h"
#include "cooperatives.h"
#include <nlohmann/json.hpp>
#include <vector>
#include <iostream>
#include <fstream>
#include <string>
#include <stdexcept>
#include <memory>
using json=nlohmann::json;

template<typename T>
json toJsonUtility::toJsonCollection(std::vector<std::shared_ptr<T>>& items) {
    json jsonArray = json::array();
    for (const auto& item : items) {
        if (item) {
            jsonArray.push_back(item->toJson());
        }
    }
    return jsonArray;
}

// Explicit template instantiations
template json toJsonUtility::toJsonCollection<Farmers::farmerInfo::farmer>(std::vector<std::shared_ptr<Farmers::farmerInfo>>&);


void toJsonUtility::writeToFile(const std::string& filename, const json& data) {
    std::ofstream file(filename);
    if (file.is_open()) {
        file << data.dump(4);
        file.close();
    } else {
        std::cerr << "Error opening file: " << filename << "\n";
    }
}

template<typename T>
std::vector<std::shared_ptr<T>> toJsonUtility::readCollectionFromFile(const std::string& filename) {
    std::vector<std::shared_ptr<T>> items;
    std::ifstream file(filename);

    if (file.is_open()) {
        try {
            json jsonData;
            file >> jsonData;

            if (jsonData.is_array()) {
                for (const auto& item : jsonData) {
                    auto obj = std::make_shared<T>();
                    obj->fromJson(item);
                    items.push_back(obj);
                }
                std::cout << "Data parsed successfully from " << filename << "\n";
            } else {
                std::cerr << "Invalid JSON format: expected array in " << filename << "\n";
            }
        } catch (const json::exception& e) {
            std::cerr << "JSON parsing error in " << filename << ": " << e.what() << "\n";
        }
        file.close();
    } else {
        std::cerr << "Error opening " << filename << "! Creating new empty file.\n";
        std::ofstream newFile(filename);
        newFile << "[]";
        newFile.close();
    }
    return items;
}

// Explicit template instantiations
template std::vector<std::shared_ptr<Farmers::farmersInfo::farmer>> toJsonUtility::readCollectionFromFile<Farmers::farmersInfo::farmer>(const std::string& filename);

