/******************************************************************/
/*!
\file      Application.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the main update loop of the engine.
           It initializes the various major systems and call upon
           them in the update loop. When the window is closed,
           the applciations cleanup function is called to 
           call the major systems to clean their program.
           


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/





#include "Application.h"
#include "Graphics/GraphicsPipe.h"
#include "Asset Manager/AssetManager.h"
#include "Asset Manager/SceneManager.h"
#include "Events/EventsEventHandler.h"
#include "Actions/ActionManager.h"
#include "Inputs/Input.h"


namespace Application {

    /*--------------------------------------------------------------
      GLOBAL VARAIBLE
    --------------------------------------------------------------*/
    graphicpipe::GraphicsPipe* pipe;
    assetmanager::AssetManager* AstManager;
    std::vector<std::string> filePath;
    int Application::Init() {
        
        filePath = Serialization::Serialize::m_LoadFilePath("../configs");
        /*--------------------------------------------------------------
        INITIALIZE LOGGING SYSTEM
        --------------------------------------------------------------*/
        LOGGING_INIT_LOGS(filePath[0]);
        LOGGING_INFO("Application Start");
        LOGGING_INFO("Load Log Successful");
        logs.m_Setup_Abort_Handler();
        std::signal(SIGABRT, logging::Logger::m_Abort_Handler);

        /*--------------------------------------------------------------
          INITIALIZE WINDOW WIDTH & HEIGHT
       --------------------------------------------------------------*/
        Serialization::Serialize::m_LoadConfig("../configs");
        LOGGING_INFO("Load Config Successful");

       /*--------------------------------------------------------------
          INITIALIZE OPENGL WINDOW
       --------------------------------------------------------------*/
        lvWindow.init();
        LOGGING_INFO("Load Window Successful");

        /*--------------------------------------------------------------
           INITIALIZE ECS
        --------------------------------------------------------------*/
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        ecs->m_Load();
        ecs->m_Init();
        LOGGING_INFO("Load ECS Successful");

        /*--------------------------------------------------------------
           INITIALIZE Asset Manager
        --------------------------------------------------------------*/
        AstManager = assetmanager::AssetManager::m_funcGetInstance();
        AstManager->m_funcLoadAssets(filePath[1]);
        scenes::SceneManager* scenemanager = scenes::SceneManager::m_GetInstance();
        scenemanager->m_LoadScene(Helper::Helpers::GetInstance()->m_startScene);
        LOGGING_INFO("Load Asset Successful");


        /*--------------------------------------------------------------
           INITIALIZE GRAPHICS PIPE
        --------------------------------------------------------------*/
        pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
        pipe->m_funcInit();
        LOGGING_INFO("Load Graphic Pipeline Successful");

        /*--------------------------------------------------------------
           INITIALIZE Input
        --------------------------------------------------------------*/
        //call back must happen before imgui
        Input.SetCallBack(lvWindow.m_window);
        LOGGING_INFO("Set Input Call Back Successful");

        
        /*--------------------------------------------------------------
            INITIALIZE EDITOR // LAST INIT
         --------------------------------------------------------------*/

        const char* glsl_version = "#version 130";
        Editor.m_Initialize(lvWindow.m_window, glsl_version);
        LOGGING_INFO("Load ImGui Successful");
        

        LOGGING_INFO("Application Init Successful");
        return 0;
	}



    int Application::Run() {
        Helper::Helpers *help = Helper::Helpers::GetInstance();
        ecs::ECS* ecs = ecs::ECS::m_GetInstance();
        actions::ActionManager::m_GetManagerInstance();
        //float FPSCapTime = 1.f / help->m_fpsCap;
        double lastFrameTime = glfwGetTime();
        const double fixedDeltaTime = 1.0 / 60.0;
        help->m_fixedDeltaTime = static_cast<float>(fixedDeltaTime);
        ecs->m_DeltaTime = static_cast<float>(fixedDeltaTime);
        help->m_accumulatedTime = 0.0;

        

        /*--------------------------------------------------------------
            GAME LOOP
        --------------------------------------------------------------*/
        while (!glfwWindowShouldClose(lvWindow.m_window))
        {
            try {
                /* Poll for and process events */
                glfwPollEvents();

                ///*--------------------------------------------------------------
                //    Calculate time
                // --------------------------------------------------------------*/
                double currentFrameTime = glfwGetTime();
                help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);
                lastFrameTime = currentFrameTime;
                help->m_accumulatedTime += (help->m_deltaTime * help->m_timeScale);
                Helper::Helpers::GetInstance()->currentNumberOfSteps = 0;
                while (help->m_accumulatedTime >= (fixedDeltaTime  )) {
                    help->m_accumulatedTime -= static_cast<float>(fixedDeltaTime);
                    ++help->currentNumberOfSteps;
                }
                
                /*--------------------------------------------------------------
                    UPDATE INPUT
                --------------------------------------------------------------*/
                Input.m_inputUpdate();

                Editor.m_InputUpdate(lvWindow.m_window);

                /*--------------------------------------------------------------
                    UPDATE ECS
                --------------------------------------------------------------*/
                //ecs->m_Update(help->m_fixedDeltaTime * help->m_timeScale); 
                ecs->m_Update(help->m_fixedDeltaTime * help->m_timeScale);
                //ecs->m_Update(Helper::Helpers::GetInstance()->m_deltaTime);


                /*--------------------------------------------------------------
                    UPDATE Render Pipeline
                --------------------------------------------------------------*/
                pipe->m_funcUpdate();


                /*--------------------------------------------------------------
                    Update IMGUI FRAME
                --------------------------------------------------------------*/
                Editor.m_update();


                /*--------------------------------------------------------------
                    DRAWING/RENDERING Window
                --------------------------------------------------------------*/
                lvWindow.Draw();


                /*--------------------------------------------------------------
                    Draw IMGUI FRAME
                --------------------------------------------------------------*/
                Editor.m_render();

                /*--------------------------------------------------------------
                   Render Game Scene
                --------------------------------------------------------------*/
                pipe->m_funcRenderGameScene();
           

              

                //double currentFrameTime = glfwGetTime();
                //help->m_deltaTime = static_cast<float>(currentFrameTime - lastFrameTime);

                //while (help->m_deltaTime < FPSCapTime) {
                //    lastFrameTime = currentFrameTime;
                //    currentFrameTime = glfwGetTime();
                //    help->m_deltaTime += static_cast<float>(currentFrameTime - lastFrameTime);
                //}
                //lastFrameTime = glfwGetTime();
                help->m_fps = 1.f / help->m_deltaTime;
                

                glfwSwapBuffers(lvWindow.m_window);

            }
            catch (const std::exception& e) {
                LOGGING_ERROR("Exception in game loop: {}", e.what());
            }
        }
        return 0;
    }



	int Application::m_Cleanup() {
        ecs::ECS::m_GetInstance()->m_Unload();
        Editor.m_Shutdown();

        lvWindow.CleanUp();
        glfwTerminate();
        LOGGING_INFO("Application Closed");

        return 0;
	}

}