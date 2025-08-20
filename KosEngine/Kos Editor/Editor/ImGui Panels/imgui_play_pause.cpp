/*!
\file      imgui_play_pause.cpp
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the imgui panel that has buttons for starting/pausing/stopping the editor

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "Editor.h"
#include "Helper/Helper.h"
#include "Asset Manager/AssetManager.h"
#include "Asset Manager/SceneManager.h"

#include "Windows.h"
#include <thread>

namespace gui {

    HWND hwnds;

    LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam) {
        switch (uMsg) {
        case WM_DESTROY:
            PostQuitMessage(0);
            return 0;
        case WM_PAINT: {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            // Set text color and background mode
            SetTextColor(hdc, RGB(0, 0, 0));
            SetBkMode(hdc, TRANSPARENT);

            // Draw text
            LPCWSTR message = L"Loading, please wait...";
            TextOut(hdc, 50, 50, message, lstrlenW(message));

            EndPaint(hwnd, &ps);
            return 0;
        }
        default:
            return DefWindowProc(hwnd, uMsg, wParam, lParam);
        }
    }

    HWND DrawLoading() {

        // Register the window class
        const char CLASS_NAME[] = "LoadingWindowClass";
        LPCWSTR WINDOOW_TITLE = L"Starting Game";

        WNDCLASS wc = {};
        wc.lpfnWndProc = WindowProc;
        wc.hInstance = GetModuleHandle(NULL);
        wc.lpszClassName = (LPCWSTR)CLASS_NAME;
        wc.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1); // Set background color

        RegisterClass(&wc);

        // Create the window
        hwnds = CreateWindowEx(
            0,                              // Optional window styles
            (LPCWSTR)CLASS_NAME,                     // Window class
            WINDOOW_TITLE,                   // Window title
            WS_OVERLAPPED | WS_CAPTION | WS_SYSMENU, // Window style (no buttons)
            CW_USEDEFAULT, CW_USEDEFAULT,   // Position
            300, 150,                       // Size
            NULL,                           // Parent window
            NULL,                           // Menu
            GetModuleHandle(NULL),          // Instance handle
            NULL                            // Additional application data
        );

        if (!hwnds) {
            return hwnds;
        }

        // Show the window
        ShowWindow(hwnds, SW_SHOW);
        UpdateWindow(hwnds);


        // Message loop
        MSG msg = {};
        while (GetMessage(&msg, NULL, 0, 0)) {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

        return hwnds;
    }

    void CloseLoadingWindow(HWND g_hwnd) {
        if (g_hwnd) {
            PostMessage(g_hwnd, WM_CLOSE, 0, 0);
        }
    }
	
    void ImGuiHandler::m_DrawPlayPauseBar() {
        static bool pause = true;
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        if (ecs->m_getState() == ecs::WAIT) {
            pause = true;
        }

        
        if (ImGui::BeginMenuBar()) {
            if (pause && ImGui::Button("Play")) {
                pause = false;
                ecs->m_nextState = (ecs->m_getState() == ecs::STOP) ? ecs::START : ecs::RUNNING;

                if (ecs->m_nextState == ecs::START) {

                    //save all active scenes
                    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
                    scenemanager->m_SaveAllActiveScenes(true); //do not include prefab

                    std::thread load(DrawLoading);
                    assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
                    assetmanager->m_scriptManager.m_CompileAllCSharptoSingleDLL();

                    assetmanager->m_scriptManager.m_ReloadAllDLL();
                    CloseLoadingWindow(hwnds);
                    load.join();
                    //reset accumulated time
                    Helper::Helpers::GetInstance()->m_deltaTime = 0.f;


                }
            }
            else if (!pause && ImGui::Button("Pause")) {

                pause = true;
                ecs->m_nextState = ecs::WAIT;





            }

            if ((ecs->m_getState() == ecs::RUNNING || ecs->m_getState() == ecs::WAIT)&& ImGui::Button("stop")) {
                if (ecs->m_getState() != ecs::STOP) {
                    ecs->m_nextState = ecs::STOP;
                    pause = true;

                    //stop all scene states
                    std::unordered_map<std::string, bool> saveStateScene;

                    for (const auto& scene : ecs->m_ECS_SceneMap) {
                        saveStateScene[scene.first] = scene.second.m_isActive;
                    }

                    //TODO load back scene
                    scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
                    scenemanager->m_ReloadScene();
                    m_clickedEntityId = -1;

                    //set back all scene to its active/inactive state
                    for (auto& scene : ecs->m_ECS_SceneMap) {
                        const auto& saveState = saveStateScene.find(scene.first);
                        if (saveState == saveStateScene.end())continue;

                        scene.second.m_isActive = saveState->second;
                    }


                    assetmanager::AssetManager::m_funcGetInstance()->m_audioManager.m_StopAllSounds();
                }

            }

        }
        ImGui::EndMenuBar();
    }
}