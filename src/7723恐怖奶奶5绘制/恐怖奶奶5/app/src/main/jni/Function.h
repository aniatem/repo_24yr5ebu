#include "Include.h"
void (*_Screen$$SetResolution)(...);
void Screen$$SetResolution(int w, int h, bool full) {}
MonoString *(*get_name)(void *inst);

bool m_IsAlive(void *a) {
    if (a == NULL) return false;
    return !(*(bool*)((uintptr_t)a + 0x2c));
}


MonoString *(*get_PlayerName)(void *player);
string EnemyName(void *a) {
    if (a == NULL) return "没有玩家";
    void *pawn = *(void**)((uintptr_t)a + 0x58);
    if (pawn == NULL) return "没有指针!";
    return get_PlayerName(pawn)->toString();
}



void (*_Player$$Update)(...);
void Player$$Update(void *a) {
    if (a != NULL) {
        if (!playerFind(a)) {
            players.push_back(a);
            LOGI("添加了新玩家，总玩家: %zu", players.size());
        }
        
        if (players.size() > 99) {
            players.clear();
        }
    }
    
    _Player$$Update(a);
}

void (*_Player$$OnDestroy)(...);
void Player$$OnDestroy(void *a) {
    if (a != NULL) {
        _Player$$OnDestroy(a);
        players.clear();
    }
}




void DrawEsp() {
    if (Var.Esp) {
        for (int i = 0; i < players.size(); ++i) {
            void *Enemy = players[i];
            void *Cam = get_main();

            if (Cam != NULL && m_IsAlive(Enemy)) {            
                void *Pointer = *(void**)((uintptr_t)Enemy + 0x140);
                monoArray<void*> *Bones = *(monoArray<void*>**)((uintptr_t)Pointer + 0x78);
                
                if (!Bones || Bones->getLength() <= 0) continue;
                
                void** BoneArray = (void**)Bones->getPointer();
                int maxBones = min(Bones->getLength(), 11);        
                            
                for (int ii = 0; ii < maxBones; ii++) {
                    void *BonesPos = BoneArray[ii];
                    if (!BonesPos) continue;
                    
                    Vector3 boneWorldPos = getPosition(BonesPos);
                    auto boneScreenPos = world(Cam, boneWorldPos);             
                          
                    if (boneScreenPos.z < 1.0f) continue;
                        
                    const char* BoneName[] = {"0", "1", "2", "3", "4", "5", "6", "7", "8", "9", "10"}; //骨骼索引
                    std::string BoneText = BoneName[ii];     
                    auto textSize = ImGui::CalcTextSize(BoneText.c_str());
                    绘制字体描边(25.f, boneScreenPos.x - (textSize.x / 2), screenHeight - boneScreenPos.y, ImVec4(Var.ESPBoxColor.x, Var.ESPBoxColor.y, Var.ESPBoxColor.z, Var.ESPBoxColor.w), BoneText.c_str());
                }
                
                Vector3 PlayerPos = getPosition(Enemy);
                Vector3 MyPos = getPosition(Cam);
                Vector3 Head = Vector3(PlayerPos.x, PlayerPos.y + 0.9f, PlayerPos.z);
                Vector3 Bottom = Vector3(PlayerPos.x, PlayerPos.y, PlayerPos.z);
                
                auto HeadPosition = world(Cam, Head);
                auto BottomPosition = world(Cam, Bottom);
                
                if (BottomPosition.z < 1.f) continue;
                
                if (Var.ESPLine) {
                    DrawAddLine::DrawLine(ImVec2(screenWidth * 0.5f, screenHeight * Var.ESPLineRotation), ImVec2(HeadPosition.x, screenHeight - HeadPosition.y), Var.ESPLineColor, Var.ThicknessLine);
                }
                float boxHeight = abs(HeadPosition.y - BottomPosition.y);
                float boxWidth = boxHeight * 0.65f;
                Rect playerRect(HeadPosition.x - (boxWidth / 2), (screenHeight - HeadPosition.y), boxWidth, boxHeight);
                
                if (Var.ESPBox) {
                    DrawAddLine::DrawBox2(playerRect, Var.ESPLineColor, Var.ThicknessLine);
                }
                
                if (Var.Name && get_name) {
                    MonoString* PlayerName = get_name(Enemy);
                    if (PlayerName != NULL) {
                        const char* nameStr = PlayerName->toChars();
                        if (nameStr) {
                            DrawAddLine::DrawText2(20.f, ImVec2(HeadPosition.x, screenHeight - HeadPosition.y - 30.f), Var.ESPBoxColor, nameStr);
                        }
                    }
                }
                
                if (Var.Number) {
                    std::string PlayerCount = "玩家数量: " + std::to_string(players.size());
                    auto textSize = ImGui::CalcTextSize(PlayerCount.c_str());
                    绘制字体描边(70.0f,  (screenWidth / 2) - (textSize.x / 2), 20, ImVec4(1.0f, 1.0f, 1.0f, 1.0f), PlayerCount.c_str());
                }                
            }
        }
    }
}
