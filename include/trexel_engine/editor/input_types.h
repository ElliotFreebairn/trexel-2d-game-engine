#pragma once

#include "imgui.h"
#include "glm/glm.hpp"
#include "../utilities/common.h"

#include <cstdio>
#include <optional>
#include <string>

void drawField(const char *name, float& v)
{
    ImGui::DragFloat(name, &v, 0.1f);
}

void drawField(const char *name, glm::vec2& v)
{
    ImGui::DragFloat2(name, &v.x, 0.1f);
}

void drawField(const char *name, glm::vec3& v)
{
    ImGui::DragFloat3(name, &v.x, 0.1f);
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

// STRING
void drawField(const char* name, std::string& v)
{
    char buf[256];
    std::snprintf(buf, sizeof(buf), "%s", v.c_str());
    if (ImGui::InputText(name, buf, sizeof(buf)))
    {
       v = buf;
    }
}

// FLIP
void drawField(const char *name, Flip& v)
{
    const char* items[] = { "None", "Horizontal", "Verticlal", "Both" };
    int i = static_cast<int>(v);
    if (ImGui::Combo(name, &i, items, IM_ARRAYSIZE(items)))
    {
        v = static_cast<Flip>(i);
    }
}

// SPRITE
// how do we deal with this struct in a struct concept?


template <typename T>
void drawField(const char* name, std::optional<T>& v)
{
    // we really should allow for the case where the current value is null but allow for creation!
    if (!v.has_value()) return;

    drawField(name, v.value());
}

// how do we deal with the std::optional?
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

template <typename T>
void drawComponent(const char *label, std::optional<T>& component)
{
    // is this how we want to deal with the failure case?
    if (!component.has_value()) return;

    drawComponent(label, component.value());
}
