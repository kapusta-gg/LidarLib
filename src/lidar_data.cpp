#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <math.h>
#include <chrono>
#include <thread>

#include "lidar_data.h"
#include "sl_lidar.h" 
#include "sl_lidar_driver.h"


#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif

using namespace sl;

// TODO: Rewrite LidarData, create writer & viewer


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
        printf("LIDAR Channel creation error\n");
        exit(-2);
    }

    if (SL_IS_FAIL(res_drv.err)){
        printf("LIDAR Driver creation error\n");
        exit(-2);
    }

    helper->drv = *res_drv;
    helper->channel = *res;

    auto con = (helper->drv)->connect(helper->channel);
    if (SL_IS_OK(con)) {
        printf("LIDAR connected\n");
    }
    else{
        printf("LIDAR connection error\n");
        exit(-2);
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

void LidarManager::get_data(){
    if(!helper->drv->isConnected()) {
        printf("No driver connection!\n");
        return;
    }

    double angle, dist;
    double x, y;

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
        return;
    }

    if (SL_IS_OK(res)) {
        helper->drv->ascendScanData(helper->nodes, helper->count);
        for (int i = 0; i < int(helper->count); ++i) {
            angle = ((helper->nodes[i].angle_z_q14 * 90.f) / 16384.f) * (M_PI / 180.f);
            dist = helper->nodes[i].dist_mm_q2 / 4.0f;
            x = dist * cos(angle); y = dist * sin(angle);
           // printf("x: %03.2f y: %03.2f angle: %03.2f dist: %03.2f \n", x, y, angle, dist);
        }
    }
    else{
        printf("grabScanDataHq error: 0x%08X\n", res);
    }
    printf("Data grab finished successfully\n");
}

