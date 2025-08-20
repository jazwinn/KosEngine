/******************************************************************/
/*!
\file      Imgui_werformance_window.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the performance window that prints out
           the time taken for all systems in each frame.



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/


#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "implot.h"
#include "Debugging/Logging.h"
#include "Debugging/Performance.h"
#include <ECS/System/SystemType.h>
#include "Helper/Helper.h"

static float interval = 1;
static std::vector<std::string> VectorSystemText;

struct Buffer {
    int MaxSize;
    int Offset;
    ImVector<ImVec2> Data;
    Buffer(int max_size = 2000) {
        MaxSize = max_size;
        Offset = 0;
        Data.reserve(MaxSize);
    }
    void AddPoint(float x, float y) {
        if (Data.size() < MaxSize)
            Data.push_back(ImVec2(x, y));
        else {
            Data[Offset] = ImVec2(x, y);
            Offset = (Offset + 1) % MaxSize;
        }
    }
    void Erase() {
        if (Data.size() > 0) {
            Data.shrink(0);
            Offset = 0;
        }
    }
};



void gui::ImGuiHandler::m_DrawPerformanceWindow(float fps) {

    ImGui::Begin("Performance");

    static float FpsValues[90] = {};
    static int FpsValues_offset = 0;
    static double refresh_time = 0.0;
    if (refresh_time == 0.0)
        refresh_time = ImGui::GetTime();
    while (refresh_time < ImGui::GetTime()) // Create data at fixed 60 Hz rate
    {
        static float phase = 0.0f;
        FpsValues[FpsValues_offset] = fps;
        FpsValues_offset = (FpsValues_offset + 1) % IM_ARRAYSIZE(FpsValues);
        phase += 0.10f * FpsValues_offset;
        refresh_time += 1.0f / 60.0f;
    }

    {
        float average = 0.0f;
        for (int n = 0; n < IM_ARRAYSIZE(FpsValues); n++)
            average += FpsValues[n];
        average /= (float)IM_ARRAYSIZE(FpsValues);
        char overlay[32];
        sprintf_s(overlay, "FPS %f", average);

        float minFps = FLT_MAX;
        float maxFps = -FLT_MAX;

        for (int n = 0; n < IM_ARRAYSIZE(FpsValues); n++) {
            if (FpsValues[n] < minFps) minFps = FpsValues[n];
            if (FpsValues[n] > maxFps) maxFps = FpsValues[n];
        }

        // Ensure there is at least some margin on the Y-axis
        float yMin = minFps - 1.0f;
        float yMax = maxFps + 1.0f;
        
        //TODO change to ImPlot
        ImGui::PlotLines("##", FpsValues, IM_ARRAYSIZE(FpsValues), FpsValues_offset, overlay, yMin, yMax, ImVec2(260.f, 120.0f));

       // ImGui::PlotShaded();
    }


    if (ImGui::CollapsingHeader("System Time", ImGuiTreeNodeFlags_DefaultOpen)) {

       

        interval += Helper::Helpers::GetInstance()->m_deltaTime;

        if (interval > 1) { //updates every 1 second
            VectorSystemText.clear();

            //add ECS total time
            std::string EcsTime = "ECS Time: 100%% (" + std::to_string(performancetracker::Performance::m_GetTotalSystemTime()) + ")";
            VectorSystemText.push_back(EcsTime);

            //add time for each sytem
            for (int n{}; n <= ecs::TOTALTYPESYSTEM; n++) {

                float Percentage = ((performancetracker::Performance::m_GetSystemTime((ecs::TypeSystem)n)) / performancetracker::Performance::m_GetTotalSystemTime()) * 100.f;

                //remove the last 4 trailing digits
                std::string PercentageStr = std::to_string(Percentage);
                PercentageStr.erase(PercentageStr.length() - 4);


                std::string SystemText = 
               performancetracker::Performance::m_GetSystemString((ecs::TypeSystem)n) + ": "
              + PercentageStr + "%% ("
              + std::to_string(performancetracker::Performance::m_GetSystemTime((ecs::TypeSystem)n)) + ")";

                VectorSystemText.push_back(SystemText);

            }

            interval = 0;
        }
        


        for (std::string str : VectorSystemText) {
            ImGui::Text(str.c_str());
        }
        
    }




    ImGui::End();

}