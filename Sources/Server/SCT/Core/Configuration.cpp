#include "Configuration.hpp"
#include <cstdlib>

namespace SCT
{
    namespace Configuration
    {
        std::string get_sut_path()
        {
            constexpr auto default_path = "./Server";
            auto sut_path = std::getenv("RUSTED_SCT_SERVER_PATH");
            return sut_path ? std::string(sut_path) : default_path;
        }
        std::string get_database_path()
        {
            constexpr auto default_path = "SctDataBase.sqlite3";
            auto db_path = std::getenv("RUSTED_SCT_DATABASE_PATH");
            return db_path ? std::string(db_path) : default_path;
        }

        std::string get_big_database_path()
        {
            constexpr auto default_path ="./Sources/Server/SCT/BigDataBase.sqlite3";
            auto db_path = std::getenv("RUSTED_SCT_BIG_DATABASE_PATH");
            return db_path ? std::string(db_path) : default_path;
        }

    }
}
