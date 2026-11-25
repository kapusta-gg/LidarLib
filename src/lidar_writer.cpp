#include <stdio.h>
#include <stdlib.h>
#include <iostream>
#include <fstream>

#include "lidar_manager.h"
#include "lidar_writer.h"


LidarWriter::LidarWriter(LidarManager& manager) : manager(manager) {}
LidarWriter::LidarWriter(LidarManager& manager, char sep) : manager(manager), sep(sep) {}
LidarWriter::LidarWriter(LidarManager& manager, const std::string filename) : manager(manager), filename(filename) {}
LidarWriter::LidarWriter(LidarManager& manager, const std::string filename, char sep) : manager(manager), filename(filename), sep(sep) {}


bool LidarWriter::write_csv()
{
    auto data = manager.scan_data();

    if (!data) {
        return false;
    }

    std::ofstream file(this->filename, std::ios::app);

    if (!file.is_open()) {
        return false;
    }

    auto timestamp = data->get_timestamp();

    for (const auto& node : data->get_nodes()) {
        file 
            << timestamp << sep
            << node.x << sep
            << node.y << sep
            << node.angle << sep
            << node.dist << std::endl; 
    }

    return true;
}

bool LidarWriter::write_csv_header(){
    std::ofstream file(this->filename);
    file 
        << "timestamp" << sep
        << "x" << sep
        << "y" << sep
        << "angle" << sep
        << "dist" << std::endl;
    return true;
}