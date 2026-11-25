#ifndef LIDAR_READER
#define LIDAR_READER

#include <iostream>
#include <fstream>

class LidarReader{
    private:
        std::string filename = "lidar_data.csv";
        char sep = ';';
        std::ifstream file;

        std::optional<std::vector<std::string>> pending_row;
    
    public:
        LidarReader();
        LidarReader(const std::string& filename);
        LidarReader(char sep);
        LidarReader(const std::string& filename, char sep);
        std::vector<std::string> get_line();
        std::vector<std::vector<std::string>> get_lines();
        std::optional<LidarData>  read_scan();
        std::vector<LidarData> read_scans();
};

#endif