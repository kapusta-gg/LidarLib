#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>
#include <cstdio>
#include <cstdlib>
#include <csignal>

#include <GLFW/glfw3.h>
#include <lidar_data.h>
#include <lidar_structs.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"



volatile std::sig_atomic_t ctrl_c_pressed = 0;

void ctrl_c(int)
{
    ctrl_c_pressed = 1;
}



#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif


#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800

void error_callback(int error, const char* description)
{
    fprintf(stderr, "Error: %s\n", description);
}

static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if(key == GLFW_KEY_ESCAPE && action == GLFW_PRESS){
        glfwSetWindowShouldClose(window, GLFW_TRUE);
    }
}

// USE sudo to run programm
int main()
{

    LidarManager manager("/dev/ttyUSB0", 115200);
    bool status = manager.start_scan();

    if(!status) return 1;

    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

    glfwSetErrorCallback(error_callback);
    if (!glfwInit()){
        printf("GLFW Initialization fail\n");
        return 1;
    }
    printf("GLFW Initialization succese\n");

    GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test", NULL, NULL);
    if(!window){
        printf("Window creation failed");
        return 1;
    }
    printf("Window creation succese\n");

    glfwSetWindowTitle(window, "lidar app");
    glfwSetKeyCallback(window, key_callback);
    glfwMakeContextCurrent(window);

    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init();

    int temp = 0;
    bool flag = true;

    while (!glfwWindowShouldClose(window)){
        glfwPollEvents();
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        //

        ImDrawList* draw_list = ImGui::GetBackgroundDrawList();

        auto data = manager.scan_data();
        for (LidarNode elem: data->get_nodes()){
            draw_list->AddLine(
                ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
                ImVec2(WINDOW_WIDTH / 2 + (int)elem.x, WINDOW_HEIGHT / 2 + (int)elem.y),
                IM_COL32(0, 0, 255, 255),  
                2.0f
            );
        }

        
        ImGui::Render();

        glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  
        glClear(GL_COLOR_BUFFER_BIT);  
        
        if (temp < 0) flag = true;
        else if (temp > 100) flag = false;
        if (flag) temp++;
        else temp--;

        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
        glfwSwapBuffers(window);

    }

    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();
    glfwDestroyWindow(window);
    glfwTerminate();
}
