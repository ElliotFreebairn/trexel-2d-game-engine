#pragma once

#include "imgui.h"
#include "glm/glm.hpp"
#include "../utilities/common.h"

#include <cstdio>
#include <string>

void drawField(const char *name, float& v)
{
    ImGui::DragFloat(name, &v, 0.1f);
}

void drawField(const char *name, glm::vec2& v)
{
    ImGui::DragFloat2(name, &v.x, 0.1f);
}

void drawField(const char *name, double& v)
{
    ImGui::InputDouble(name, &v);
}

void drawField(const char* name, int& v)
{
    ImGui::DragInt(name, &v);
}

void drawField(const char* name, bool& v)
{
    ImGui::Checkbox(name, &v);
}

void drawField(const char* name, std::string& v)
{
    char buf[256];
    std::snprintf(buf, sizeof(buf), "%s", v.c_str());
    if (ImGui::InputText(name, buf, sizeof(buf)))
    {
       v = buf;
    }
}

void drawField(const char *name, Flip& v)
{
    const char* items[] = { "None", "Horizontal", "Verticlal", "Both" };
    int i = static_cast<int>(v);
    if (ImGui::Combo(name, &i, items, IM_ARRAYSIZE(items)))
    {
        v = static_cast<Flip>(i);
    }
}

template <typename T>
void drawComponent(const char* label, T& component)
{
    if (ImGui::CollapsingHeader(label))
    {
        component.fields([](const char* name, auto& value)
        {
          drawField(name, value);
        });
    }
}
