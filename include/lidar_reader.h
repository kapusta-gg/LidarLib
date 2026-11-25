#ifndef LIDAR_READER
#define LIDAR_READER

#include <iostream>
#include <fstream>

class LidarReader{
    private:
        std::string filename = "lidar_data.cpp";
        char sep = ';';
        std::ifstream file;

        std::optional<std::vector<std::string>> pending_row;
    
    public:
        LidarReader();
        LidarReader(const std::string& filename);
        LidarReader(char sep);
        LidarReader(const std::string& filename, char sep);
        std::vector<std::string> LidarReader::get_line();
        std::vector<std::vector<std::string>> LidarReader::get_lines();
        std::optional<LidarData>  LidarReader::read_scan();
        std::vector<LidarData> LidarReader::read_scans()
};

#endif