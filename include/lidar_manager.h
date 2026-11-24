#ifndef LIDAR_MANAGER
#define LIDAR_MANAGER

#include <memory>
#include <string>
#include <optional>

#include "lidar_structs.h"
#include "sl_lidar.h"

struct LidarManagerHelper;

class LidarManager{
    public:
        LidarManager(std::string device, int baudrate);
        ~LidarManager();
        bool start_scan();
        std::optional<LidarData> scan_data();
    
    private:
        std::unique_ptr<LidarManagerHelper> helper;
        float get_angle(sl_lidar_response_measurement_node_hq_t data){ return (data.angle_z_q14 * 90.f) / 16384.f; }
        float get_dist(sl_lidar_response_measurement_node_hq_t data){ return data.dist_mm_q2 / 4.0f; }
};
#endif