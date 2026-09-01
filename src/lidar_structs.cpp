#include <math.h>

#include <lidar_structs.h>

#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

struct LidarNode
{
    float x;
    float y; 

    LidarNode(sl_u16 angle, sl_u32 dist){
        float temp = angle * (M_PI / 180.0);
        x = dist * cos(temp);
        y = dist * sin(temp);
    }
};



struct LidarData
{
    private:
        std::vector<LidarNode> list;
        sl_u16 get_angle(sl_lidar_response_measurement_node_hq_t data){ return (data.angle_z_q14 * 90.f) / 16384.f; }
        sl_u32 get_dist(sl_lidar_response_measurement_node_hq_t data){ return data.dist_mm_q2 / 4.0f; }

    public:
        LidarData (sl_lidar_response_measurement_node_hq_t nodes[]){
            for (int i = 0; i < int(_countof(nodes)); ++i){
                list.push_back(LidarNode(get_angle(nodes[i]), get_dist(nodes[i])));
            }
        }
};