#include "Others/Logger.h"
#include "Others/obfuscate.h"
#include "Others/Utils.h"
#include "Hook/Dobby/dobby.h"
#include "Hook/KittyMemory/MemoryPatch.h"
#include "Hook/And64InlineHook/And64InlineHook.hpp"

#include "Variable.h"

int screenWidth = 0;
int screenHeight = 0;
bool g_Initialized = false;
ImGuiWindow* g_window = NULL;

#define LIB OBFUSCATE("libil2cpp.so")
#define LIBU OBFUSCATE("libunity.so")

void DoHOOK(void *offseet, void* ptr, void **orig) {
    DobbyHook(offseet, ptr, orig);
}
#define DHOOK(offseet, ptr, orig) DoHOOK((void *)getAbsoluteAddress("libil2cpp.so", offseet), (void *)ptr, (void **)&orig)
#define oHex MemoryPatch::createWithHex
#define gAA getAbsoluteAddress
#define MOD Modify
#define RES Restore


Vector3 (*world)(void *cam, Vector3 pos);
void* (*get_transform)(void* player);
Vector3 (*get_position)(void* trans);
void* (*get_main)();
Vector3 getPosition(void *player) {
    return get_position(get_transform(player));
} 
