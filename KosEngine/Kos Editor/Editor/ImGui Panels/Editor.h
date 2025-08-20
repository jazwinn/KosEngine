/********************************************************************/
/*!
\file      Editor.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This header file declares functions that manage ImGui operations
           - Initialize: Sets up ImGui with GLFW and OpenGL.
           - NewFrame: Prepares ImGui for rendering.
           - Render: Draws ImGui elements.
           - Shutdown: Cleans up ImGui resources.
           - DrawMainMenuBar: Renders the main menu bar UI.
           - DrawPerformanceWindow: Displays performance metrics like FPS.
           - DrawComponentWindow: Manages the ECS component user interface.
           - DrawInputWindow: Displays user input settings.
           - DrawLogsWindow: Displays log output.
           - DrawRenderScreenWindow: Sets up the window for rendering output.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#pragma once
#include "imgui.h"
#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "imgui_stdlib.h"
#include "ImGuizmo.h"
#include "ECS/ECS.h"
#include "Asset Manager/Audio.h"
#include "Events/EventsEventHandler.h"



namespace gui {

    class ImGuiHandler
    {
    public:
        /******************************************************************/
        /*!
        \fn      gui::ImGuiHandler::ImGuiHandler()
        \brief   Default constructor for the ImGuiHandler class.
        \details Sets up initial values for ImGui operations and prepares the context for UI rendering.
        */
        /******************************************************************/
        ImGuiHandler();

        /******************************************************************/
        /*!
        \fn      gui::ImGuiHandler::~ImGuiHandler()
        \brief   Destructor for the ImGuiHandler class.
        \details Cleans up and releases any resources used by ImGui, ensuring a proper shutdown.
        */
        /******************************************************************/
        ~ImGuiHandler();


        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_Initialize(GLFWwindow* window, const char* glsl_version)
            \brief     Initializes ImGui with GLFW and OpenGL settings.
            \param[in] window       The window handle used by GLFW.
            \param[in] glsl_version The OpenGL GLSL version string.
            \details   Sets up the ImGui context, configures platform and renderer bindings,
                       enables various ImGui features (such as keyboard, gamepad support, and
                       multi-viewport windows), and applies the dark style theme.
        */
        /******************************************************************/
        void m_Initialize(GLFWwindow* window, const char* glsl_version);

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_NewFrame()
            \brief     Prepares and starts a new ImGui frame.
            \details   This function is responsible for starting a new frame in ImGui by
                        initializing both OpenGL and GLFW backends and preparing for ImGui rendering.
        */
        /******************************************************************/
        void m_NewFrame();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_Render()
            \brief     Renders ImGui elements and windows.
            \details   This function manages the main rendering loop for ImGui, handling the
                        menu bar, performance window, hierarchy, component, and log windows.
                        It also manages ImGui's viewport and platform-specific rendering.
        */
        /******************************************************************/
        void m_update();

        /******************************************************************/
        /*!
        \fn      void m_InputUpdate(GLFWwindow* window)
        \brief   Updates input states based on the current GLFW window context.
        \param   window - Pointer to the GLFW window used to poll input events.
        */
        /******************************************************************/
        void m_InputUpdate(GLFWwindow* window);

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_render()
        \brief   Renders the ImGui UI elements.
        \details Draws all registered ImGui widgets and panels to the screen, handling the rendering logic.
        */
        /******************************************************************/
        void m_render();


        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_Shutdown()
            \brief     Cleans up and shuts down ImGui.
            \details   This function handles shutting down ImGui by cleaning up the OpenGL
                       and GLFW resources, destroying the ImGui and ImPlot contexts.
        */
        /******************************************************************/
        void m_Shutdown();

    private:

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawMainMenuBar()
            \brief     Renders the main menu bar and handles user input for file operations.
            \details   This function creates a main menu bar with options like "File" and "Window."
                       In the "File" menu, users can save the current state by either selecting the "Save" option
                       or pressing `CTRL + S`. The function interacts with the ECS framework to save component data
                       to a JSON file using the `Serialize::m_SaveComponentsJson` function.
        */
        /******************************************************************/
        void m_DrawMainMenuBar();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawPerformanceWindow(float fps)
            \brief     Displays the performance metrics in an ImGui window.
            \param[in] fps The current frames-per-second value.
            \details   This function renders a window showing performance statistics. It tracks FPS over time and
                       displays an FPS graph using `ImGui::PlotLines`. It also displays system time information
                       for the ECS systems, updating every second to show the percentage of total system time each
                       system uses, along with the actual time in seconds.
        */
        /******************************************************************/

        void m_DrawPerformanceWindow(float fps);

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawComponentWindow()
            \brief     Draws the ImGui component window, allowing users to interact with and modify ECS components.
            \details   Displays available components for the selected entity and provides options to add, view,
                       and modify components such as Movement, Transform, Sprite, Collider, and more.
                       The window updates real-time changes to the entity's components within the ECS framework.
        */
        /******************************************************************/
        void m_DrawComponentWindow();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawTestWindow()
            \brief     Displays a test window for various ECS and ImGui functionalities.
            \details   This function creates a test window with several buttons and options to:
                       - Play, stop, and control audio through the asset manager.
                       - Trigger an application crash using the `abort()` function.
                       - Test log functionality by adding log entries.
                       - Spawn and delete 2500 entities in the ECS.
                       - Perform a collision test by spawning two entities with movement and collider components.
                       - Control entity movement and reset their positions for collision testing.
        */
        /******************************************************************/
        void m_DrawTestWindow();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawInputWindow()
            \brief     Displays the input status window in ImGui.
            \details   This function creates a window that displays the current mouse position,
                       key status, and mouse button status. It retrieves these values from
                       the `Input::InputSystem` and outputs them as text within the ImGui window.
        */
        /******************************************************************/
        void m_DrawInputWindow();

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::DrawHierachyWindow()
            \brief     Draws the ImGui Hierarchy Window for managing game objects in the ECS.
            \return    The ID of the clicked entity in the hierarchy.
            \details   Displays a list of game objects (entities) with options to add, delete,
                       or duplicate them. The function updates the hierarchy view in real time
                   and interacts with the ECS framework.
        */
        /******************************************************************/
        unsigned int m_DrawHierachyWindow();

        /******************************************************************/
        /*!
        \fn      bool gui::ImGuiHandler::m_DrawEntityNode(ecs::EntityID entity)
        \brief   Draws the ImGui UI node for a specific entity.
        \param   entity - The ID of the entity to be drawn in the UI.
        \return  True if the entity node was successfully drawn, false otherwise.
        \details Renders a collapsible node representing an entity, allowing interaction with its properties.
        */
        /******************************************************************/
        bool m_DrawEntityNode(ecs::EntityID entity);

        /******************************************************************/
        /*!
            \fn        gui::ImGuiHandler::m_DrawLogsWindow()
            \brief     Displays the log output in an ImGui window.
            \details   This function creates a window that displays log entries in reverse order, with the most recent log entries shown first.
                       It retrieves the log entries from the logger instance and outputs them within an ImGui window.
        */
        /******************************************************************/
        void m_DrawLogsWindow();

        /******************************************************************/
        /*!
        \fn        void gui::ImGuiHandler::m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
        \brief     Renders the scene texture inside an ImGui window while maintaining the correct aspect ratio.

        \param[in] windowWidth
                   The width of the OpenGL window (scene) being rendered.

        \param[in] windowHeight
                   The height of the OpenGL window (scene) being rendered.

        \details   This function creates an ImGui window labeled "Scene Window" and renders
                   the scene as a texture inside it. The function calculates the appropriate
                   size and position for the texture within the window, ensuring that the
                   aspect ratio is preserved when the window is resized.

                   It uses the available content region size within the ImGui window to
                   adjust the texture size. If the aspect ratio of the texture differs from
                   that of the window, the texture is centered horizontally or vertically.

        \sa        graphicpipe::GraphicsPipe::m_funcGetInstance() to get the screen texture.
        */
        /******************************************************************/
        void m_DrawPlayPauseBar();

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight)
        \brief   Draws the render screen window within the ImGui interface.
        \param   windowWidth - The width of the window to be rendered.
        \param   windowHeight - The height of the window to be rendered.
        \details Displays the main rendering screen with the specified dimensions, allowing visualization of the game scene.
        */
        /******************************************************************/
        void m_DrawRenderScreenWindow(unsigned int windowWidth, unsigned int windowHeight);

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_DrawGizmo(float renderPosX, float renderPosY, float renderWidth, float renderHeight)
        \brief   Draws the Gizmo controls for transforming objects.
        \param   renderPosX - The X position for rendering the gizmo.
        \param   renderPosY - The Y position for rendering the gizmo.
        \param   renderWidth - The width of the render area for the gizmo.
        \param   renderHeight - The height of the render area for the gizmo.
        \details Provides interactive controls to manipulate objects within the rendered scene.
        */
        /******************************************************************/
        void m_DrawGizmo(float renderPosX, float renderPosY, float renderWidth, float renderHeight);

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_DrawLayerWindow()
        \brief   Draws the layer management window within the ImGui interface.
        \details Displays a UI window that allows the user to add, remove, and manage layers within the scene.
        */
        /******************************************************************/
        void m_DrawLayerWindow();

        void m_DrawGlobalSettingsWindow();

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_DrawGameSceneWindow()
        \brief   Draws the game scene window within the ImGui interface.
        \details Displays the current game scene, including entities and their properties, allowing the user to interact with the scene.
        */
        /******************************************************************/
        void m_DrawGameSceneWindow(std::string windowName);

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_DrawContentBrowser()
        \brief   Draws the content browser window within the ImGui interface.
        \details Displays the content browser, which allows the user to navigate and manage project assets.
        */
        /******************************************************************/
        void m_DrawContentBrowser();

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_DrawTilePicker()
        \brief   Draws the tile picker UI for selecting tiles.
        \details Displays a UI panel for selecting tiles, used in tile-based level design.
        */
        /******************************************************************/
        void m_DrawTilePicker();

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_OnButtonPress(const events::BaseEvent<events::ButtonEvents>& givenEvent)
        \brief   Handles button press events within the ImGui interface.
        \param   givenEvent - The event data representing the button press.
        \details Processes button interactions, triggering appropriate responses in the UI or game logic.
        */
        /******************************************************************/
        void m_OnButtonPress(const events::BaseEvent<events::ButtonEvents>& givenEvent);

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_OnAction(const events::BaseEvent<events::Actions>& givenEvent)
        \brief   Handles action events within the ImGui interface.
        \param   givenEvent - The event data representing the action.
        \details Processes specific user actions, executing commands or modifying the game state accordingly.
        */
        /******************************************************************/
        void m_OnAction(const events::BaseEvent<events::Actions>& givenEvent);

        /******************************************************************/
        /*!
        \fn      void gui::ImGuiHandler::m_UpdateOnPrefabMode()
        \brief   Updates the ImGui interface for prefab editing mode.
        \details Adjusts the UI elements and properties to enable editing of prefabs within the scene.
        */
        /******************************************************************/
        void m_UpdateOnPrefabMode();

        

        int m_clickedEntityId{ -1 };

        std::string m_activeScene{};

        /***********PREFAB*************/
        bool m_prefabSceneMode{ false };
        bool m_tilePickerMode{ false };
        bool m_collisionSetterMode{ false };
        std::unordered_map<std::string, bool> m_savedSceneState;
        /******************************/

        bool m_objectNameBox = false;
        char m_charBuffer[50] = "";  // Char Buffer

        std::vector<std::string>m_tags;

        ImVec4 clearColor = ImVec4(0.45f, 0.55f, 0.60f, 1.00f);

        
        /***********Window Names*************/
        std::string gamePreviewName = "Game Window";
        /************************************/

        std::string m_imgui_layout = "Editor/Imgui Panels/imgui.ini";

        inline void SaveLayout() {

            if (!ImGui::GetCurrentContext()) {
                std::cout << "Warning: No ImGui context found. Skipping LoadLayout()." << std::endl;
                return;
            }
            std::ifstream file(m_imgui_layout);
            if (!file.good()) {
                std::cout << "Layout file not found: " << m_imgui_layout << std::endl;
                return;
            }

            ImGui::SaveIniSettingsToDisk(m_imgui_layout.c_str());
        }

        inline void LoadLayout()
        {
            if (!ImGui::GetCurrentContext()) {
                std::cout << "Warning: No ImGui context found. Skipping LoadLayout()." << std::endl;
                return;
            }
            std::ifstream file(m_imgui_layout);
            if (!file.good()) {
                std::cout << "Layout file not found: " << m_imgui_layout << std::endl;
                return;
            }

            ImGui::LoadIniSettingsFromDisk(m_imgui_layout.c_str());
        }

        
};


bool containsSubstring(const std::string& x, const std::string& y);

}