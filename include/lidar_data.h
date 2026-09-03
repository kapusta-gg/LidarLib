#ifndef LIDAR_DATA_H
#define LIDAR_DATA_H

#include <memory>
#include <string>


struct LidarManagerHelper;

class LidarManager{
    public:
        LidarManager(std::string device, int baudrate);
        ~LidarManager();
        bool start_scan();
        void get_data();
    
    private:
        std::unique_ptr<LidarManagerHelper> helper;
};
#endif