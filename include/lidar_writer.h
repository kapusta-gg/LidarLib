#ifndef LIDAR_WRITER
#define LIDAR_WRITER

#include "lidar_manager.h"

class LidarWriter{
    private:
        LidarManager& manager;
        std::string filename = "lidar_data.csv";
        char sep = ';';

    public:
        LidarWriter(LidarManager& manager);
        LidarWriter(LidarManager& manager, char sep);
        LidarWriter(LidarManager& manager, const std::string filename);
        LidarWriter(LidarManager& manager, const std::string filename, char sep);
        bool write_csv();
        bool write_csv_header();
};

#endif