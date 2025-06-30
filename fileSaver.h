#ifdef FILE_SAVER_H
#define FILE_SAVER_H
#include <nlohmann/json.hpp>
using json=nlohmann::json;

class toJsonUtility{
        public:
                template<typename T>
                static json toJsonCollection(std::vector<std::shared_ptr<T>>& items);
                void writeToFile(const std::string& filename,const json& data);
                template<typename T>
                static std::vector<std::shared_ptr<T>> readCollectionFromFile(const std::string& filename);
}
#endif
