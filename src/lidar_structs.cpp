#include <math.h>

#include <lidar_structs.h>

#include "sl_lidar.h" 
#include "sl_lidar_driver.h"

#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

LidarNode::LidarNode(float angle, float dist){
        this->angle = angle;
        this->dist = dist;
        float temp = angle * (M_PI / 180.0);
        this->x = dist * cos(temp);
        this->y = dist * sin(temp);
};

void LidarData::add_node(float angle, float dist){
    this->list.push_back(LidarNode(angle, dist));
}


std::vector<LidarNode> LidarData::get_nodes(){
    return this->list;
}
LidarData::LidarData(){};