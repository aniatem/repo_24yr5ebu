#ifndef ImGuiAndroid_Draww
#define ImGuiAndroid_Draww

#include "ImGui/imgui_internal.h"
#include "Struc.h"
#include "Vector3.h"
#include "Vector2.h"
#include "Quaternion.h"
namespace DrawAddLine
{
    void DrawLine(ImVec2 start, ImVec2 end, ImVec4 color, float thickness = 3.0f) {
        auto background = ImGui::GetBackgroundDrawList();
        if(background) {
            background->AddLine(start, end, ImColor(color.x, color.y, color.z, color.w), thickness);
        }
    }
    
    
    
    void DrawCrosshair(ImVec4 color, Vector2 center, float size = 20) {
        float x = center.X - (size / 2.0f);
        float y = center.Y - (size / 2.0f);
        DrawLine(ImVec2(x, center.Y), ImVec2(x + size, center.Y), color);
        DrawLine(ImVec2(center.X, y), ImVec2(center.X, y + size), color);
    }
    
    
    /*
    void DrawCrosshair(ImVec4 color, Vector2 center, float size = 20) {
        float x = center.X - (size / 2.0f);
        float y = center.Y - (size / 2.0f);
        DrawLine(ImVec2(x, center.Y), ImVec2(x + size, center.Y), ImVec4(120, 120, 120, 120));
        DrawLine(ImVec2(center.X, y), ImVec2(center.X, y + size), ImVec4(120, 120, 120, 120));
    }
    */
    void DrawBulletTracer(int x, int y, int x2, int y2, int getH) {
        DrawLine(ImVec2(x, getH - y), ImVec2(x2, getH - y2), ImVec4(255, 255, 255, 255), 5.0f);
        DrawCrosshair(ImVec4(77, 111, 192, 255), Vector2(x2, getH - y2), 100);
    }
    
    
    void DrawCircle(ImVec4 color, float X, float Y, float radius, bool filled) {
        auto background = ImGui::GetBackgroundDrawList();
        if(background) {
            if(filled) {
                background->AddCircleFilled(ImVec2(X, Y), radius, ImColor(color.x, color.y, color.z, color.w));
            }
            else {
                background->AddCircle(ImVec2(X, Y), radius, ImColor(color.x, color.y, color.z, color.w));
            }
        }
    }
    
    void DrawBox(Rect rect, ImVec4 color, float thickness = 3.0f) {
        ImVec2 v1(rect.x, rect.y);
        ImVec2 v2(rect.x + rect.width, rect.y);
        ImVec2 v3(rect.x + rect.width, rect.y + rect.height);
        ImVec2 v4(rect.x, rect.y + rect.height);

        DrawLine(v1, v2, color, thickness);
        DrawLine(v2, v3, color, thickness);
        DrawLine(v3, v4, color, thickness);
        DrawLine(v4, v1, color, thickness);
    }
    
    void DrawBox2(Rect rect, ImVec4 color, float thickness = 3.0f) {
        ImVec2 v1(rect.x, rect.y); //низ левый
        ImVec2 v11(rect.x + (rect.width / 3), rect.y);
        ImVec2 v12(rect.x, rect.y + (rect.height / 4));
        
        ImVec2 v2(rect.x + rect.width, rect.y); //низ правый
        ImVec2 v21((rect.x + rect.width) - (rect.width / 3), rect.y);
        ImVec2 v22(rect.x + rect.width, rect.y + (rect.height / 4));
        
        ImVec2 v3(rect.x + rect.width, rect.y + rect.height); //верх правый
        ImVec2 v31((rect.x + rect.width) - (rect.width / 3), rect.y + rect.height);
        ImVec2 v32(rect.x + rect.width, (rect.y + rect.height) - (rect.height / 4));
        
        ImVec2 v4(rect.x, rect.y + rect.height); //верх левый
        ImVec2 v41(rect.x + (rect.width / 3), rect.y + rect.height);
        ImVec2 v42(rect.x, (rect.y + rect.height) - (rect.height / 4));

        DrawLine(v1, v11, color, thickness);
        DrawLine(v1, v12, color, thickness);
        DrawLine(v2, v21, color, thickness);
        DrawLine(v2, v22, color, thickness);
        DrawLine(v3, v31, color, thickness);
        DrawLine(v3, v32, color, thickness);
        DrawLine(v4, v41, color, thickness);
        DrawLine(v4, v42, color, thickness);
    }

    
    void DrawText2(float fontSize, ImVec2 position, ImVec4 color, const char *text, bool center = true) {
        auto background = ImGui::GetBackgroundDrawList();
        if(background) {
            if (center) {
                ImVec2 textSize = ImGui::CalcTextSize(text);
                ImVec2 centeredPosition = ImVec2(position.x - (textSize.x * 0.5f), position.y - (textSize.y * 0.5f));
                background->AddText(NULL, fontSize, centeredPosition, ImColor(color.x, color.y, color.z, color.w), text);
            } else {
                background->AddText(NULL, fontSize, position, ImColor(color.x, color.y, color.z, color.w), text);
            }
        }
    }
    
    
    void DrawHorizontalHealthBar(Vector2 screenPos, float width, float maxHealth, float currentHealth, ImVec4 clr) {
        screenPos -= Vector2(0.0f, 8.0f);
        DrawBox(Rect(screenPos.X, screenPos.Y, width + 2, 5.0f), ImVec4(0, 0, 0, 255));
        screenPos += Vector2(1.0f, 1.0f);
        float hpWidth = (currentHealth * width) / maxHealth;
        if (currentHealth <= (maxHealth * 0.6)) {
            clr = ImVec4(255, 255, 0, 255);
        }
        if (currentHealth < (maxHealth * 0.3)) {
            clr = ImVec4(255, 0, 0, 255);
        }
        if (currentHealth <= (maxHealth * 0.0)) {
            clr = ImVec4(0, 0, 0, 0);
        }
        DrawBox(Rect(screenPos.X, screenPos.Y, hpWidth, 3.0f), clr);
    }
    
    void DrawVerticalHealth(Vector2 end, float h, float health) {
        float x = end.X;
        float y = end.Y;
        h = -h;
        ImVec4 clr = ImVec4(0, 255, 0, 255);

        float hpwidth = h - h * health / 100;

        if (health <= (100 * 0.6)) {
            clr = ImVec4(255, 255, 0, 255);
        }
        if (health < (100 * 0.3)) {
            clr = ImVec4(255, 0, 0, 255);
        }

        Rect hpbar((x + h / 4) - 8, y, 4.0f ,-h);
        Rect hp((x + h / 4) - 8, y - hpwidth, 2.0f, -h + hpwidth);

        DrawBox(hpbar, ImVec4(0, 0, 0, 255), 3);

        DrawBox(hp, clr, 3);
    }
    
    float get_3D_Distance(float Self_x, float Self_y, float Self_z, float Object_x, float Object_y, float Object_z)
    {
        float x, y, z;
        x = Self_x - Object_x;
        y = Self_y - Object_y;
        z = Self_z - Object_z;
        return (float)(sqrt(x * x + y * y + z * z));
    }
}


#endif
