#include <list>
#include <vector>
#include <string.h>
#include <pthread.h>
#include <thread>
#include <cstring>
#include <jni.h>
#include <unistd.h>
#include <fstream>
#include <iostream>
#include <dlfcn.h>
#include <EGL/egl.h>
#include <Others/GLES2/gl2.h>
#include <android/sensor.h>
#include <android/asset_manager.h>
#include <android/log.h>
#include "Hook/Dobby/dobby.h"
#include "ImGui/imgui.h"
#include "ImGui/imgui_internal.h"
#include "ImGui/backends/imgui_impl_android.h"
#include "ImGui/backends/imgui_impl_opengl3.h"
#include "ImGui/theme/Themes.h"
#include "ImGui/FONTS/中文字体.h"
#include "Function.h"


extern "C" {
    JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_init(JNIEnv* env, jclass cls);
    JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height);
    JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_step(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj);
    JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj, jboolean down, jfloat PosX, jfloat PosY);
    JNIEXPORT jstring JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_getWindowRect(JNIEnv *env, jobject thiz);
    JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_real(JNIEnv* env, jobject obj, jint width, jint height);
};

JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_init(JNIEnv* env, jclass cls) {
    if (g_Initialized) return;
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO& io = ImGui::GetIO();
    Theme::SetBlackGoldTheme();

    
    ImGui_ImplOpenGL3_Init("#version 300 es");	
    ImGui::GetStyle().ScaleAllSizes(3.0f);
    g_Initialized = true;
    static const ImWchar icons_ranges[] = { 0xf000, 0xf3ff, 0x0900, 0x097F, 0, };
    ImFontConfig font_config;
    ImFontConfig icons_config;
    ImFontConfig CustomFont;
    CustomFont.FontDataOwnedByAtlas = false;
    icons_config.MergeMode = true;
    icons_config.PixelSnapH = true;
    icons_config.OversampleH = 2.5;
    icons_config.OversampleV = 2.5;
    io.Fonts->AddFontFromMemoryTTF((void*)OPPOSans_H,OPPOSans_H_size, 22.0f,NULL,io.Fonts->GetGlyphRangesChineseFull());    
}

JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_resize(JNIEnv* env, jobject obj, jint width, jint height) {
    screenWidth = (int)width;
    screenHeight = (int)height;
    glViewport(0, 0, width, height);
    ImGuiIO& io = ImGui::GetIO();
    io.ConfigWindowsMoveFromTitleBarOnly = true;
    io.IniFilename = NULL;
    ImGui::GetIO().DisplaySize = ImVec2((float)width, (float)height);
}



void BeginDraw() {
    ImGuiIO& io = ImGui::GetIO();
    ImVec2 center = ImGui::GetMainViewport()->GetCenter();
    ImGui::SetNextWindowPos(center, ImGuiCond_Appearing, ImVec2(0.5f, 0.5f));
    ImGui::SetNextWindowSize(ImVec2(500, 300));
    绘制字体描边(45.f, screenWidth / 2, screenHeight / 2 + 450, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), "弟弟");
   
    if (ImGui::Begin("IMGUI菜单", 0, ImGuiWindowFlags_NoSavedSettings)) {
        g_window = ImGui::GetCurrentWindow();
        if (ImGui::BeginTabBar("Tab", ImGuiTabBarFlags_FittingPolicyScroll)) {
            if (ImGui::BeginTabItem("ESP菜单")) {
                ImGui::Spacing();
                ImGui::Checkbox("启用Esp", &Var.Esp);
                ImGui::SameLine();
                ImGui::Checkbox("射线", &Var.ESPLine);
                ImGui::SameLine();
                ImGui::Checkbox("方框", &Var.ESPBox);                
                ImGui::Checkbox("玩家数量", &Var.Number);
                ImGui::SameLine();
                ImGui::Checkbox("名称", &Var.Name);
                ImGui::SameLine();
                ImGui::Checkbox("骨骼", &Var.Name);                
                ImGui::Spacing();
                ImGui::PushItemWidth(35);
                ImGui::ColorEdit4("调整框颜色", (float*)&Var.ESPLineColor);
                ImGui::PopItemWidth();
                ImGui::Spacing();
                ImGui::EndTabItem();
            }
            ImGui::Separator();
            ImGui::Text("绘制耗时 %.3f ms (%.1f FPS)", 1000.0f / io.Framerate, io.Framerate);

            ImGui::EndTabBar();
        }
    }
}


void *hack_thread(void *) {
    do {
        sleep(1);
            libBaseAddress = findLibrary(LIB);
    } while (libBaseAddress == 0 && !isLibraryLoaded(LIB));
    LOGI("libil2cpp.so地址: %p", libBaseAddress);
        
    get_main = (void *(*)()) gAA(LIB, 0x42E6030);
    world = (Vector3 (*)(void *, Vector3)) gAA(LIB, 0x42E5CC8);
    get_position = (Vector3 (*)(void *)) gAA(LIB, 0x4337884);
    get_transform = (void *(*)(void *)) gAA(LIB, 0x4325F20);
    get_name = (MonoString *(*)(void *)) gAA(LIB, 0x432EC70);

    DHOOK(0x1F4B928, Player$$Update, _Player$$Update);
    DHOOK(0x1F9E128, Player$$OnDestroy, _Player$$OnDestroy);
    DHOOK(0x42F0DB0, Screen$$SetResolution, _Screen$$SetResolution);
    pthread_exit(nullptr);
    return nullptr;
}


__attribute__((constructor))
void lib_main() {
    pthread_t ptid;
    pthread_create(&ptid, NULL, hack_thread, NULL);
    
}

JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_step(JNIEnv* env, jobject obj) {
    ImGuiIO& io = ImGui::GetIO();
    ImGui_ImplOpenGL3_NewFrame();
    ImGui_ImplAndroid_NewFrame(screenWidth, screenHeight);
    ImGui::NewFrame();           
    DrawEsp();
    BeginDraw();
    
    ImGui::Render();
    glClear(GL_COLOR_BUFFER_BIT);
    ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
}

JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_imgui_Shutdown(JNIEnv* env, jobject obj) {
    if (!g_Initialized) return;
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplAndroid_Shutdown();
    ImGui::DestroyContext();
    g_Initialized = false;
}

JNIEXPORT void JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_MotionEventClick(JNIEnv* env, jobject obj, jboolean down, jfloat PosX, jfloat PosY) {
    ImGuiIO& io = ImGui::GetIO();
    io.MouseDown[0] = down;
    io.MousePos = ImVec2(PosX, PosY);
}

JNIEXPORT jstring JNICALL Java_com_OpenUI_ImGui_GLES3JNIView_getWindowRect(JNIEnv* env, jobject thiz) {
    char result[256] = "0|0|0|0";
    if (g_window) {
        sprintf(result, "%d|%d|%d|%d", (int)g_window->Pos.x, (int)g_window->Pos.y, (int)g_window->Size.x, (int)g_window->Size.y);
    }
    return env->NewStringUTF(result);
}
