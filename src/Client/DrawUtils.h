#pragma once

#ifdef QOLMOD_IMGUI
#include <imgui-cocos.hpp>
#endif

class DrawUtils
{
    public:
        #ifdef QOLMOD_IMGUI
        static inline ImFont* title = nullptr;
        static inline ImFont* mod = nullptr;

        static ImVec2 addImVec2(ImVec2 arg1, ImVec2 arg2)
        {
            float x = 0, y = 0;

            x = arg1.x;
            x += arg2.x;

            y = arg1.y;
            y += arg2.y;

            return ImVec2(x, y);
        }

        static void drawRect(ImVec2 pos, ImVec2 size, ImColor colour)
        {
            ImGui::GetWindowDrawList()->AddRectFilled(pos, addImVec2(pos, size), colour, 0.0f);
        }

        static ImColor LerpColor(const ImColor& c1, const ImColor& c2, float t) {
            ImVec4 col1 = c1;
            ImVec4 col2 = c2;
            ImVec4 lerped_color;
            lerped_color.x = col1.x + (col2.x - col1.x) * t;
            lerped_color.y = col1.y + (col2.y - col1.y) * t;
            lerped_color.z = col1.z + (col2.z - col1.z) * t;
            lerped_color.w = col1.w + (col2.w - col1.w) * t;
            return ImColor(lerped_color);
        }

        static void drawGradient(ImVec2 apos, ImVec2 size, ImColor colour, ImColor colour2)
        {
            auto list = ImGui::GetWindowDrawList();

            for (size_t i = 0; i < size.x; i++)
            {
                auto pos = ImVec2(apos.x + i, apos.y);

                list->AddLine(pos, addImVec2(pos, ImVec2(0, size.y)), LerpColor(colour, colour2, i / size.x));
            }

        }

        static void anchoredText(ImVec2 pos, ImVec2 size, std::string text, ImColor colour = ImColor(ImVec4(255, 255, 255, 255)), ImVec2 anchor = ImVec2(0.5f, 0.5f))
        {
            float s = size.x / ImGui::CalcTextSize(text.c_str()).x;
            if (s > 1)
                s = 1;
            
            ImGui::SetCursorPos(ImVec2(pos.x + ((size.x) * anchor.x) - ImGui::CalcTextSize(text.c_str()).x * anchor.x, pos.y + ((size.y) * anchor.y) - ImGui::CalcTextSize(text.c_str()).y * anchor.y));
            
            
            ImGui::TextColored(colour, text.c_str());
        }

        static ImColor imColorFromccColor3B(geode::prelude::ccColor3B col)
        {
            return ImColor(ImVec4(col.r / 255.0f, col.g / 255.0f, col.b / 255.0f, 255));
        }

        /// <summary>
        /// z = width, w = height
        /// </summary>
        static bool mouseWithinRect(ImVec4 rect)
        {
            ImVec2 point = ImGui::GetIO().MousePos;

            if (point.x > rect.x && point.y > rect.y)
            {
                if (point.x < rect.x + rect.z && point.y < rect.y + rect.w)
                {
                    return true;
                }
            }

            return false;
        }

        #endif

        /// <summary>
        /// z = width, w = height
        /// </summary>

        enum animType
        {
            SlideLeft,
            SlideRight,
            SlideUp,
            SlideDown,
            v6,
        };

        static inline animType animationIn = animType::v6;
        static inline animType animationOut = animType::SlideUp;
};