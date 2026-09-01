#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <string.h>

#include <GLFW/glfw3.h>

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"


#ifndef _countof
#define _countof(_Array) (int)(sizeof(_Array) / sizeof(_Array[0]))
#endif


#define WINDOW_WIDTH 500
#define WINDOW_HEIGHT 500

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
// int main()
// {
//     IMGUI_CHECKVERSION();
//     ImGui::CreateContext();
//     ImGuiIO& io = ImGui::GetIO();
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     
//     io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;

//     glfwSetErrorCallback(error_callback);
//     if (!glfwInit()){
//         printf("GLFW Initialization fail\n");
//         return 1;
//     }
//     printf("GLFW Initialization succese\n");

//     GLFWwindow* window = glfwCreateWindow(WINDOW_WIDTH, WINDOW_HEIGHT, "Test", NULL, NULL);
//     if(!window){
//         printf("Window creation failed");
//         return 1;
//     }
//     printf("Window creation succese\n");

//     glfwSetWindowTitle(window, "lidar app");
//     glfwSetKeyCallback(window, key_callback);
//     glfwMakeContextCurrent(window);

//     ImGui_ImplGlfw_InitForOpenGL(window, true);
//     ImGui_ImplOpenGL3_Init();

//     int temp = 0;
//     bool flag = true;

//     while (!glfwWindowShouldClose(window)){
//         glfwPollEvents();
//         ImGui_ImplOpenGL3_NewFrame();
//         ImGui_ImplGlfw_NewFrame();
//         ImGui::NewFrame();

//         //

//         ImDrawList* draw_list = ImGui::GetBackgroundDrawList();


//         draw_list->AddLine(
//             ImVec2(WINDOW_WIDTH / 2, WINDOW_HEIGHT / 2),
//             ImVec2(WINDOW_WIDTH / 2 + 50, WINDOW_HEIGHT / 2 + temp),
//             IM_COL32(0, 0, 255, 255),  
//             2.0f
//         );

//         //
//         ImGui::Render();

//         glClearColor(0.1f, 0.1f, 0.1f, 1.0f);  
//         glClear(GL_COLOR_BUFFER_BIT);  
        
//         if (temp < 0) flag = true;
//         else if (temp > 100) flag = false;
//         if (flag) temp++;
//         else temp--;

//         ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
//         glfwSwapBuffers(window);

//     }

//     ImGui_ImplOpenGL3_Shutdown();
//     ImGui_ImplGlfw_Shutdown();
//     ImGui::DestroyContext();
//     glfwDestroyWindow(window);
//     glfwTerminate();
// }
