#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>
#include <optional>
#include <stdexcept>

#include "lidar_manager.h"
#include "lidar_structs.h"
#include "sl_lidar.h" 
#include "sl_lidar_driver.h"


#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace sl;


struct LidarManagerHelper{
    IChannel* channel = nullptr;
    ILidarDriver* drv = nullptr;
    LidarMotorInfo info;
    sl_lidar_response_measurement_node_hq_t nodes[8192];
    size_t count = _countof(nodes);

    ~LidarManagerHelper(){
        if (drv){
            drv->stop();
            drv->setMotorSpeed(0);
            drv->disconnect();
            delete drv;
            drv = nullptr;
        }
        if (channel){
            delete channel;
            channel = nullptr;
        }
    }
};

//"/dev/ttyUSB0", 115200)
LidarManager::LidarManager(std::string device, int baudrate){
    helper = std::make_unique<LidarManagerHelper>();

    auto res = createSerialPortChannel(device.c_str(), baudrate);
    auto res_drv = createLidarDriver();

    if (SL_IS_FAIL(res.err)){
        throw std::runtime_error("LIDAR Channel creation error\n");
    }

    if (SL_IS_FAIL(res_drv.err)){
        throw std::runtime_error("LIDAR Driver creation error\n");
    }

    helper->drv = *res_drv;
    helper->channel = *res;

    auto con = (helper->drv)->connect(helper->channel);
    if (SL_IS_OK(con)) {
        printf("LIDAR connected\n");
    }
    else{
        throw std::runtime_error("LIDAR connection error\n");
    }
}

LidarManager::~LidarManager() = default;

bool LidarManager::start_scan(){
    if(!helper->drv->isConnected()) {
        printf("No driver connection!");
        return false;
    }
    helper->drv->setMotorSpeed();
    std::this_thread::sleep_for(std::chrono::seconds(1));
    auto res = helper->drv->startScan(0, 1);
    if (!SL_IS_OK(res)) {
        printf("startScan error: 0x%08X\n", res);
        return false;
    }
    printf("Start scanning...\n");
    std::this_thread::sleep_for(std::chrono::seconds(1));
    return true;
} 

std::optional<LidarData> LidarManager::scan_data(){
    if(!helper->drv->isConnected()) {
        printf("No driver connection!\n");
        return std::nullopt;
    }

    LidarData data = {};
    double angle, dist;
    double x, y;

    helper->count = _countof(helper->nodes);

    auto res = helper->drv->grabScanDataHq(helper->nodes, helper->count);

    if (res == SL_RESULT_OPERATION_TIMEOUT) {
        printf(
            "Partial scan received: %zu nodes\n",
            helper->count
        );
    }

    if (!SL_IS_OK(res) &&
        res != SL_RESULT_OPERATION_TIMEOUT) {
        printf("Timeout...");
        return std::nullopt;
    }

    if (SL_IS_OK(res)) {

        helper->drv->ascendScanData(helper->nodes, helper->count);
        for (int i = 0; i < int(helper->count); ++i) {
            angle = this->get_angle(helper->nodes[i]);
            dist = this->get_dist(helper->nodes[i]);
            data.add_node(angle, dist);
           // printf("x: %03.2f y: %03.2f angle: %03.2f dist: %03.2f \n", x, y, angle, dist);
        }
    }
    else{
        printf("grabScanDataHq error: 0x%08X\n", res);
    }
    printf("Data grab finished successfully\n");
    return data;
}
