#include <math.h>

#include <lidar_structs.h>


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


const std::vector<LidarNode>& LidarData::get_nodes() const{
    return this->list;
}
LidarData::LidarData(){};