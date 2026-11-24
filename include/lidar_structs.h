#ifndef LIDAR_DATA_STURCTS_H
#define LIDAR_DATA_STURCTS_H

#include <vector>

struct LidarNode
{
    float x;
    float y;
    float dist;
    float angle; 
    LidarNode(float angle, float dist);
};

struct LidarData
{
    private:
        std::vector<LidarNode> list;

    public:
        void add_node(float angle, float dist);
        const std::vector<LidarNode>& get_nodes() const;
        LidarData();
};

#endif