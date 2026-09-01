#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>

#include <lidar_data.h>


volatile std::sig_atomic_t ctrl_c_pressed = 0;

void ctrl_c(int)
{
    ctrl_c_pressed = 1;
}

// USE sudo to run programm
int main()
{
    std::signal(SIGINT, ctrl_c);

    LidarManager manager("/dev/ttyUSB0", 115200);
    bool status = manager.start_scan();
    
    if(!status) return 1;

    while(!ctrl_c_pressed){
        manager.get_data();
    } 

    printf("\nStopping programm...\n");
    return 0;
}
