/********************************************************************/
/*!
\file      imgui_handler.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Oct 02, 2024
\brief     This file defines functions that handle ImGui operations
		   - Initialize: Sets up ImGui with GLFW and OpenGL.
		   - NewFrame: Prepares and starts a new ImGui frame.
		   - Render: Renders ImGui elements and windows.
		   - Shutdown: Cleans up ImGui resources.
		   - DrawMainMenuBar: Renders the main menu bar UI.
		   - DrawPerformanceWindow: Displays FPS and performance metrics.
		   - DrawComponentWindow: Manages the ECS component interface.
		   - DrawLogsWindow: Displays logs.
		   - DrawInputWindow: Handles input settings.
		   - DrawRenderScreenWindow: Sets up the window for render output.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/



#include "imgui_impl_glfw.h"
#include "imgui_impl_opengl3.h"
#include "implot.h"
#include "implot_internal.h"
#include "Dependencies/rapidjson/document.h"
#include "Dependencies/rapidjson/writer.h"
#include "Dependencies/rapidjson/stringbuffer.h"
#include "Editor.h"
#include "Editor/EditorCamera.h"
#include "Graphics/GraphicsPipe.h"
#include "Inputs/Input.h"
#include "Events/EventsEventHandler.h"
#include "Helper/Helper.h"
#include "Debugging/Performance.h"
#include "Asset Manager/Prefab.h"
#include "Actions/ActionManager.h"
#include "Actions/ModifyAction.h"
#include "Inputs/Input.h"
#include "Editor/WindowFile.h"
#include "Graphics/GraphicsCamera.h"

namespace gui {

	ImGuiHandler::ImGuiHandler() {
		REGISTER_BUTTON_LISTENER(events::ButtonEvents::EVENTBUTTONPRESS, ImGuiHandler::m_OnButtonPress, this)
		REGISTER_BUTTON_LISTENER(events::ButtonEvents::EVENTAUDIOFROMIMGUI, ImGuiHandler::m_OnButtonPress, this)
		REGISTER_ACTION_LISTENER(events::Actions::BASEACTION, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::TRANSFORMCOMP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::UNDO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::REDO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::ADDCOMP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::REMOVECOMP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::ADDENT, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::DELENT, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MOVECTC, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MOVECTP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MOVEPTC, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYANIM, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYAUDIO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYCAMERA, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYCOLL, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYENEMY, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYGRID, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYLIGHT, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYPATHFINDING, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYRIGID, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYSPRITE, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYTEXT, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYBUTTON, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYVIDEO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYNAME, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYPARTICLE, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::MODIFYTILEMAP, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::ADDAUDIO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::REMOVEAUDIO, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::ADDRAY, ImGuiHandler::m_OnAction, this)
		REGISTER_ACTION_LISTENER(events::Actions::REMOVERAY, ImGuiHandler::m_OnAction, this)
	} //CTORdoing 

	ImGuiHandler::~ImGuiHandler() {} //Destructor

	void ImGuiHandler::m_Initialize(GLFWwindow* window, const char* glsl_version)
	{
		
		
		// Setup Dear ImGui context
		IMGUI_CHECKVERSION();
		ImGui::CreateContext();
		ImGui::SetCurrentContext(ImGui::GetCurrentContext()); // Ensure context is set

		ImGui::GetIO().IniFilename = nullptr;


		ImGuiIO& io = ImGui::GetIO(); (void)io;
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard; // Enable Keyboard Controls
		io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;  // Enable Gamepad Controls
		io.ConfigFlags |= ImGuiConfigFlags_DockingEnable;// Enable Docking
		io.ConfigFlags |= ImGuiConfigFlags_ViewportsEnable;// Enable Multi-Viewport / Platform Windows

		// Setup Dear ImGui style
		ImGui::StyleColorsDark();

		
		ImGuiStyle& style = ImGui::GetStyle();
		ImVec4* colors = style.Colors;

		// General window settings
		style.WindowRounding = 5.0f;
		style.FrameRounding = 5.0f;
		style.ScrollbarRounding = 5.0f;
		style.GrabRounding = 5.0f;
		style.TabRounding = 5.0f;
		style.WindowBorderSize = 1.0f;
		style.FrameBorderSize = 1.0f;
		style.PopupBorderSize = 1.0f;
		style.PopupRounding = 5.0f;

		// Setting the colors (Light version)
		colors[ImGuiCol_Text] = ImVec4(0.10f, 0.10f, 0.10f, 1.00f);
		colors[ImGuiCol_TextDisabled] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_WindowBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f); // Light background
		colors[ImGuiCol_ChildBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_PopupBg] = ImVec4(0.98f, 0.98f, 0.98f, 1.00f);
		colors[ImGuiCol_Border] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_BorderShadow] = ImVec4(0.00f, 0.00f, 0.00f, 0.00f);
		colors[ImGuiCol_FrameBg] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light frame background
		colors[ImGuiCol_FrameBgHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_FrameBgActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_TitleBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TitleBgActive] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f);
		colors[ImGuiCol_TitleBgCollapsed] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_MenuBarBg] = ImVec4(0.95f, 0.95f, 0.95f, 1.00f);
		colors[ImGuiCol_ScrollbarBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_ScrollbarGrab] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabHovered] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_ScrollbarGrabActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);

		// Accent colors with a soft pastel gray-green
		colors[ImGuiCol_CheckMark] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Soft gray-green for check marks
		colors[ImGuiCol_SliderGrab] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f);
		colors[ImGuiCol_SliderGrabActive] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
		colors[ImGuiCol_Button] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Light button background
		colors[ImGuiCol_ButtonHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_ButtonActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_Header] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_HeaderHovered] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_HeaderActive] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_Separator] = ImVec4(0.60f, 0.60f, 0.60f, 1.00f);
		colors[ImGuiCol_SeparatorHovered] = ImVec4(0.65f, 0.65f, 0.65f, 1.00f);
		colors[ImGuiCol_SeparatorActive] = ImVec4(0.70f, 0.70f, 0.70f, 1.00f);
		colors[ImGuiCol_ResizeGrip] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Accent color for resize grips
		colors[ImGuiCol_ResizeGripHovered] = ImVec4(0.60f, 0.70f, 0.60f, 1.00f);
		colors[ImGuiCol_ResizeGripActive] = ImVec4(0.65f, 0.75f, 0.65f, 1.00f);
		colors[ImGuiCol_Tab] = ImVec4(0.85f, 0.85f, 0.85f, 1.00f); // Tabs background
		colors[ImGuiCol_TabHovered] = ImVec4(0.80f, 0.80f, 0.80f, 1.00f);
		colors[ImGuiCol_TabActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_TabUnfocused] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);
		colors[ImGuiCol_TabUnfocusedActive] = ImVec4(0.75f, 0.75f, 0.75f, 1.00f);
		colors[ImGuiCol_DockingPreview] = ImVec4(0.55f, 0.65f, 0.55f, 1.00f); // Docking preview in gray-green
		colors[ImGuiCol_DockingEmptyBg] = ImVec4(0.90f, 0.90f, 0.90f, 1.00f);

		// Additional styles
		style.FramePadding = ImVec2(8.0f, 4.0f);
		style.ItemSpacing = ImVec2(8.0f, 4.0f);
		style.IndentSpacing = 20.0f;
		style.ScrollbarSize = 16.0f;

		// Setup Platform/Renderer bindings
		ImGui_ImplGlfw_InitForOpenGL(window, true);
		ImGui_ImplOpenGL3_Init(glsl_version);


		// load current layout
		LoadLayout();

		//set first active scene
		ecs::ECS* ec = ecs::ECS::m_GetInstance();
		for (auto& scene : ec->m_ECS_SceneMap) {
			if (!scene.second.m_isPrefab) {
				m_activeScene = scene.first;
				break;
			}
		}

		m_tags = file::FileWindow::readEditorConfig("./Editor/editorConfig.txt");
		
	}

	void ImGuiHandler::m_NewFrame()
	{
		// Start a new ImGui frame
		ImGui_ImplOpenGL3_NewFrame(); //needs to be called every frame
		ImGui_ImplGlfw_NewFrame();
		ImGui::NewFrame();
		ImGuizmo::BeginFrame();
	}

	void ImGuiHandler::m_update()
	{
		
		m_NewFrame();
		
		if (Input::InputSystem::m_isKeyTriggered(keys::F11))
		{
			//std::cout << Input::InputSystem::m_isKeyTriggered(keys::F11) << std::endl;
			//Input::InputSystem::m_keyStateF11 = GLFW_RELEASE;
			graphicpipe::GraphicsPipe* pipe = graphicpipe::GraphicsPipe::m_funcGetInstance();
			EditorCamera::m_editorMode = (EditorCamera::m_editorMode) ? false : true;
			pipe->m_gameMode = (pipe->m_gameMode) ? false : true;

			if(pipe->m_gameMode) ImGui::GetIO().ConfigFlags |= ImGuiConfigFlags_NoMouse;
			else {
			
				ImGuiIO& io = ImGui::GetIO();
				ImGui::GetIO().ConfigFlags &= ~ImGuiConfigFlags_NoMouse;
				// Clear keyboard state
				memset(io.KeysDown, 0, sizeof(io.KeysDown));  // Clear all key states
				io.KeyCtrl = io.KeyShift = io.KeyAlt = io.KeySuper = false; // Clear modifier keys
				io.InputQueueCharacters.resize(0); // Clear text input queue

				// Clear mouse state
				io.MouseDown[0] = io.MouseDown[1] = io.MouseDown[2] = io.MouseDown[3] = io.MouseDown[4] = false;
				io.MouseWheel = io.MouseWheelH = 0.0f;
				io.MousePos = ImVec2(-FLT_MAX, -FLT_MAX); // Reset mouse position

			} 
		}
		
		if (EditorCamera::m_editorMode)
		{
			// Render ImGui

			//for gizmo - todo once camera is done
			//ImGuizmo::SetOrthographic(true);
			
			//viewport docking
			ImGui::DockSpaceOverViewport(0, ImGui::GetMainViewport());

			//update while prefabmode is on
			m_UpdateOnPrefabMode();
			
			Helper::Helpers* help = Helper::Helpers::GetInstance();
			ecs::ECS* ecs = ecs::ECS::m_GetInstance();

			//check if "m_activeScene", if not find first active scene
			const auto& scene = ecs->m_ECS_SceneMap.find(m_activeScene);
			if (scene == ecs->m_ECS_SceneMap.end()) {
				for (auto& _scene : ecs->m_ECS_SceneMap) {
					if (!_scene.second.m_isPrefab) {
						m_activeScene = _scene.first;
						_scene.second.m_isActive = true;
					}
				}
			}

			//check if selected entityid is loaded
			if (ecs->m_ECS_EntityMap.find(m_clickedEntityId) == ecs->m_ECS_EntityMap.end()) {
				m_clickedEntityId = -1;
			}


			ImVec2 windowSize = ImGui::GetIO().DisplaySize;
			// only render when window is not minimize
			if ((windowSize.x > 0 && windowSize.y > 0) || !EditorCamera::m_editorMode) {
				std::chrono::duration<float> duration{};
				auto start = std::chrono::steady_clock::now();
				m_DrawMainMenuBar();
				//m_DrawPlayPauseWindow();
				m_DrawHierachyWindow();
				m_DrawComponentWindow();
				m_DrawLogsWindow();
				//m_DrawTestWindow();
				m_DrawLayerWindow();
				m_DrawGlobalSettingsWindow();
				m_DrawInputWindow();
				m_DrawContentBrowser();
				m_DrawRenderScreenWindow(static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowWidth), static_cast<unsigned int>(Helper::Helpers::GetInstance()->m_windowHeight));
				m_DrawGameSceneWindow(gamePreviewName);
				auto end = std::chrono::steady_clock::now();
				duration = end - start;
				performancetracker::Performance::m_UpdateTotalSystemTime(duration.count());
				performancetracker::Performance::m_UpdateSystemTime(ecs::TOTALTYPESYSTEM, duration.count());
				m_DrawPerformanceWindow(help->m_fps);
			}

			
			if (Input::InputSystem::m_isKeyPressed(keys::LeftControl) && Input::InputSystem::m_isKeyTriggered(keys::Z)) {
				events::UndoLatest temp;
				//DISPATCH_ACTION_EVENT(temp);
			}

			if (Input::InputSystem::m_isKeyPressed(keys::LeftControl) && Input::InputSystem::m_isKeyTriggered(keys::Y)) {
				events::RedoPrevious temp;
				//DISPATCH_ACTION_EVENT(temp);
			}

			
		}
		ImGui::EndFrame();
	}

	void ImGuiHandler::m_render()
	{
		ImGuiIO& io = ImGui::GetIO();

		//if (EditorCamera::m_editorMode)
		//{

		//}

		ImGui::Render();

		if (io.ConfigFlags & ImGuiConfigFlags_ViewportsEnable)
		{
			GLFWwindow* backup_current_context = glfwGetCurrentContext();
			ImGui::UpdatePlatformWindows();
			ImGui::RenderPlatformWindowsDefault();
			glfwMakeContextCurrent(backup_current_context);
		}
		ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());
	}




	void ImGuiHandler::m_Shutdown()
	{
		// Shutdown ImGui
		ImGui_ImplOpenGL3_Shutdown();
		ImGui_ImplGlfw_Shutdown();
		ImGui::DestroyContext();
	}


	void ImGuiHandler::m_OnButtonPress([[maybe_unused]]const events::BaseEvent<events::ButtonEvents>& givenEvent) {
		assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();

		if (!assetManager) {
			std::cerr << "AssetManager instance is null.\n";
			return;
		}

		
	}

	/******************************************************************/
	/*!
	\fn        ImGuiHandler::m_OnAction(const events::BaseEvent<events::Actions>& givenEvent)
	\brief     This is the function that handles what to do when events happen in the editor
	*/
	/******************************************************************/
	void ImGuiHandler::m_OnAction(const events::BaseEvent<events::Actions>& givenEvent) {
		if (givenEvent.m_GetEventType() == events::Actions::TRANSFORMCOMP) {
			auto* newAct = new actions::ModifyTransformAction(givenEvent.m_ToType<events::TransformComponentChanged>().m_GetID(), givenEvent.m_ToType<events::TransformComponentChanged>().m_GetComp(),
															  givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldPos(), givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldRot(),
															  givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldScale(), givenEvent.m_ToType<events::TransformComponentChanged>().m_GetOldTrans());
			actions::ActionManager::m_GetManagerInstance()->m_DoAction(newAct);
		}else if (givenEvent.m_GetEventType() == events::Actions::ADDCOMP) {
			auto* newAct = new actions::AddComponentAction(givenEvent.m_ToType<events::AddComponent>().m_GetID(), givenEvent.m_ToType<events::AddComponent>().m_GetComponentType());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::REMOVECOMP) {
			auto* newAct = new actions::RemoveComponentAction(givenEvent.m_ToType<events::RemoveComponent>().m_GetID(), givenEvent.m_ToType<events::RemoveComponent>().m_GetComponentType());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		//else if (givenEvent.m_GetEventType() == events::Actions::ADDENT) {
		//	auto* newAct = new actions::AddEntityAction(givenEvent.m_ToType<events::AddEntity>().m_GetID());
		//	actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		//}
		//else if (givenEvent.m_GetEventType() == events::Actions::DELENT) {
		//	auto* newAct = new actions::RemoveEntityAction(givenEvent.m_ToType<events::RemoveEntity>().m_GetID());
		//	actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		//}
		else if (givenEvent.m_GetEventType() == events::Actions::MOVECTC) {
			auto* newAct = new actions::MoveEntityChildToChildAction(givenEvent.m_ToType<events::MoveEntityChildToChild>().m_GetID(), givenEvent.m_ToType<events::MoveEntityChildToChild>().m_GetOldParentID(), givenEvent.m_ToType<events::MoveEntityChildToChild>().m_GetNewParentID());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MOVECTP) {
			auto* newAct = new actions::MoveEntityChildToParentAction(givenEvent.m_ToType<events::MoveEntityChildToParent>().m_GetID(), givenEvent.m_ToType<events::MoveEntityChildToChild>().m_GetOldParentID());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MOVEPTC) {
			auto* newAct = new actions::MoveEntityParentToChildAction(givenEvent.m_ToType<events::MoveEntityParentToChild>().m_GetID(), givenEvent.m_ToType<events::MoveEntityParentToChild>().m_GetNewParentID());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::UNDO) {
			actions::ActionManager::m_GetManagerInstance()->m_Undo();
		}
		else if (givenEvent.m_GetEventType() == events::Actions::REDO) {
			actions::ActionManager::m_GetManagerInstance()->m_Redo();
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYANIM) {
			auto* newAct = new actions::ModifyAnimComponent(givenEvent.m_ToType<events::ModifyAnim>().m_GetID(),givenEvent.m_ToType<events::ModifyAnim>().m_GetComp(), givenEvent.m_ToType<events::ModifyAnim>().m_GetOldFrameNum(),
															givenEvent.m_ToType<events::ModifyAnim>().m_GetOldFPS(), givenEvent.m_ToType<events::ModifyAnim>().m_GetOldFT(), givenEvent.m_ToType<events::ModifyAnim>().m_GetOldIsAnim(),
															givenEvent.m_ToType<events::ModifyAnim>().m_GetOldStrip());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}else if (givenEvent.m_GetEventType() == events::Actions::MODIFYCAMERA) {
		auto* newAct = new actions::ModifyCamera(
													givenEvent.m_ToType<events::ModifyCamera>().m_GetID(),
													givenEvent.m_ToType<events::ModifyCamera>().m_GetComp(),
													givenEvent.m_ToType<events::ModifyCamera>().m_GetOldLeft(),
													givenEvent.m_ToType<events::ModifyCamera>().m_GetOldRight(),
													givenEvent.m_ToType<events::ModifyCamera>().m_GetOldTop(),
													givenEvent.m_ToType<events::ModifyCamera>().m_GetOldBottom(),
													givenEvent.m_ToType<events::ModifyCamera>().m_GetOldAspectRatio());
		actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYCOLL) {
			auto* newAct = new actions::ModifyCollider(
														givenEvent.m_ToType<events::ModifyCollider>().m_GetID(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetComp(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldSize(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldOffset(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldDrawDebug(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldRadius(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldCollisionResponse(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldCollisionCheck(),
														givenEvent.m_ToType<events::ModifyCollider>().m_GetOldShape());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYENEMY) {
			auto* newAct = new actions::ModifyEnemy(
													givenEvent.m_ToType<events::ModifyEnemy>().m_GetID(),
													givenEvent.m_ToType<events::ModifyEnemy>().m_GetComp(),
													givenEvent.m_ToType<events::ModifyEnemy>().m_GetOldTag(),
													givenEvent.m_ToType<events::ModifyEnemy>().m_GetOldType(),
													givenEvent.m_ToType<events::ModifyEnemy>().m_GetOldBehavior());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYGRID) {
			auto* newAct = new actions::ModifyGrid(
													givenEvent.m_ToType<events::ModifyGrid>().m_GetID(),
													givenEvent.m_ToType<events::ModifyGrid>().m_GetComp(),
													givenEvent.m_ToType<events::ModifyGrid>().m_GetOldAnchor(),
													givenEvent.m_ToType<events::ModifyGrid>().m_GetOldRowLength(),
													givenEvent.m_ToType<events::ModifyGrid>().m_GetOldColumnLength(),
													givenEvent.m_ToType<events::ModifyGrid>().m_GetOldCollidable(),
													givenEvent.m_ToType<events::ModifyGrid>().m_GetOldGridKey());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYLIGHT) {
			auto* newAct = new actions::ModifyLight(
													givenEvent.m_ToType<events::ModifyLight>().m_GetID(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetComp(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldType(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldIntensity(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldInnerOuterRadius(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldColor(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldOffset(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldScale(),
													givenEvent.m_ToType<events::ModifyLight>().m_GetOldRot());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYPATHFINDING) {
			auto* newAct = new actions::ModifyPathfinding(
														givenEvent.m_ToType<events::ModifyPathfinding>().m_GetID(),
														givenEvent.m_ToType<events::ModifyPathfinding>().m_GetComp(),
														givenEvent.m_ToType<events::ModifyPathfinding>().m_GetOldStartPos(),
														givenEvent.m_ToType<events::ModifyPathfinding>().m_GetOldEndPos(),
														givenEvent.m_ToType<events::ModifyPathfinding>().m_GetOldGridKey());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYRIGID) {
			auto* newAct = new actions::ModifyRigid(givenEvent.m_ToType<events::ModifyRigid>().m_GetID(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetComp(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldVelocity(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldAcceleration(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldRotation(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldAngularVelocity(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldAngularAcceleration(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldMass(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldInverseMass(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldLinearDamping(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldAngularDamping(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldForce(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldTorque(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldKinematic(),
													givenEvent.m_ToType<events::ModifyRigid>().m_GetOldStatic());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}

		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYTEXT) {
			auto* newAct = new actions::ModifyText(givenEvent.m_ToType<events::ModifyText>().m_GetID(),
													givenEvent.m_ToType<events::ModifyText>().m_GetComp(),
													givenEvent.m_ToType<events::ModifyText>().m_GetOldFile(),
													givenEvent.m_ToType<events::ModifyText>().m_GetOldText(),
													givenEvent.m_ToType<events::ModifyText>().m_GetOldLayer(),
													givenEvent.m_ToType<events::ModifyText>().m_GetOldSize(),
													givenEvent.m_ToType<events::ModifyText>().m_GetOldColor());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYSPRITE) {
			auto* newAct = new actions::ModifySprite(
				givenEvent.m_ToType<events::ModifySprite>().m_GetID(),
				givenEvent.m_ToType<events::ModifySprite>().m_GetComp(),
				givenEvent.m_ToType<events::ModifySprite>().m_GetOldFile(),
				givenEvent.m_ToType<events::ModifySprite>().m_GetOldLayer(),
				givenEvent.m_ToType<events::ModifySprite>().m_GetOldColor(),
				givenEvent.m_ToType<events::ModifySprite>().m_GetOldAlpha(),
				givenEvent.m_ToType<events::ModifySprite>().m_GetOldIlluminated());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYBUTTON) {
			auto* newAct = new actions::ModifyButton(
				givenEvent.m_ToType<events::ModifyButton>().m_GetID(),
				givenEvent.m_ToType<events::ModifyButton>().m_GetComp(),
				givenEvent.m_ToType<events::ModifyButton>().m_GetOldPos(),
				givenEvent.m_ToType<events::ModifyButton>().m_GetOldScale(),
				givenEvent.m_ToType<events::ModifyButton>().m_GetOldClick());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYVIDEO) {
			auto* newAct = new actions::ModifyVideoAction(
				givenEvent.m_ToType<events::ModifyVideo>().m_GetID(),
				givenEvent.m_ToType<events::ModifyVideo>().m_GetComp(),
				givenEvent.m_ToType<events::ModifyVideo>().m_GetOldFilename(),
				givenEvent.m_ToType<events::ModifyVideo>().m_GetOldPause(),
				givenEvent.m_ToType<events::ModifyVideo>().m_GetOldLoop(),
				givenEvent.m_ToType<events::ModifyVideo>().m_GetOldLayer());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYTILEMAP) {
			auto* newAct = new actions::ModifyTilemapAction(
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetID(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetComp(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldTilemapFile(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldTileLayer(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldColor(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldAlpha(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldTileIndex(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldRowLength(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldColumnLength(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldPictureRowLength(),
				givenEvent.m_ToType<events::ModifyTilemap>().m_GetOldPictureColumnLength());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYPARTICLE) {
			auto* newAct = new actions::ModifyParticleAction(
				givenEvent.m_ToType<events::ModifyParticle>().m_GetID(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetComp(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldWillSpawn(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldNoOfParticles(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldLifeSpan(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldVelocity(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldAcceleration(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldColor(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldConeRotation(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldConeAngle(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldRandomFactor(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldImageFile(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldStripCount(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldFrameNumber(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldLayer(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldFriction(),
				givenEvent.m_ToType<events::ModifyParticle>().m_GetOldFps());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYNAME) {
			auto* newAct = new actions::ModifyNameAction(
				givenEvent.m_ToType<events::ModifyName>().m_GetID(),
				givenEvent.m_ToType<events::ModifyName>().m_GetComp(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldName(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldTag(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldIsPrefab(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldPrefabName(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldLayer());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::MODIFYNAME) {
			auto* newAct = new actions::ModifyNameAction(
				givenEvent.m_ToType<events::ModifyName>().m_GetID(),
				givenEvent.m_ToType<events::ModifyName>().m_GetComp(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldName(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldTag(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldIsPrefab(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldPrefabName(),
				givenEvent.m_ToType<events::ModifyName>().m_GetOldLayer());
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::ADDAUDIO) {
			auto* newAct = new actions::AddAudio(givenEvent.m_ToType<events::AddAudio>().m_GetID(),
												givenEvent.m_ToType<events::AddAudio>().m_GetComp(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldFilename(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldFilePath(), 
												givenEvent.m_ToType<events::AddAudio>().m_GetOldVol(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldLoop(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldPlayOnStart(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldHasPlayed(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldPan(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldSFX(),
												givenEvent.m_ToType<events::AddAudio>().m_GetOldBGM()
												);
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::REMOVEAUDIO) {
			auto* newAct = new actions::RemoveAudio(givenEvent.m_ToType<events::RemoveAudio>().m_GetID(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetComp(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldFilename(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldFilePath(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldVol(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldLoop(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldPlayOnStart(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldHasPlayed(),
													givenEvent.m_ToType<events::RemoveAudio>().m_GetOldPan(),
													givenEvent.m_ToType<events::AddAudio>().m_GetOldSFX(),
													givenEvent.m_ToType<events::AddAudio>().m_GetOldBGM()
													);
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::ADDRAY) {
			auto* newAct = new actions::AddRay(givenEvent.m_ToType<events::AddRay>().m_GetID(),
												givenEvent.m_ToType<events::AddRay>().m_GetComp(),
												givenEvent.m_ToType<events::AddRay>().m_GetOldRayID(),
												givenEvent.m_ToType<events::AddRay>().m_GetOldIsRayCasting(),
												givenEvent.m_ToType<events::AddRay>().m_GetOldTargetPos(),
												givenEvent.m_ToType<events::AddRay>().m_GetOldLayers()
												);
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
		}
		else if (givenEvent.m_GetEventType() == events::Actions::REMOVERAY) {
			auto* newAct = new actions::RemoveRay(givenEvent.m_ToType<events::RemoveRay>().m_GetID(),
				givenEvent.m_ToType<events::RemoveRay>().m_GetComp(),
				givenEvent.m_ToType<events::RemoveRay>().m_GetOldRayID(),
				givenEvent.m_ToType<events::RemoveRay>().m_GetOldIsRayCasting(),
				givenEvent.m_ToType<events::RemoveRay>().m_GetOldTargetPos(),
				givenEvent.m_ToType<events::RemoveRay>().m_GetOldLayers()
			);
			actions::ActionManager::m_GetManagerInstance()->m_Push(newAct);
			}


		
	}

	void ImGuiHandler::m_UpdateOnPrefabMode()
	{
		Helper::Helpers* help = Helper::Helpers::GetInstance();
		static bool colorChanged = false;
		static vector3::Vec3 originalColor = {}; // Store original color

		if (m_prefabSceneMode)
		{
			if (!colorChanged)
			{
				originalColor = help->m_colour;
				colorChanged = true;
			}

			prefab::Prefab::m_UpdateAllPrefabEntity(m_activeScene);
			help->m_colour = { 0.133f, 0.23f, 0.32f };
		}
		else
		{
			if (colorChanged)
			{
				help->m_colour = originalColor;
				colorChanged = false;
			}
		}


	}

	void ImGuiHandler::m_InputUpdate(GLFWwindow* window)
	{
		if (!EditorCamera::m_editorMode)return;
		


		ImGuiWindow* imguiWindow = ImGui::FindWindowByName(gamePreviewName.c_str());
		
		


		if (imguiWindow && ecs::ECS::m_GetInstance()->m_getState() == ecs::RUNNING) { // && ()

			vector2::Vec2 mp = Input::InputSystem::m_getMousePosition();
			//std::cout << "Elijah input: " << mp.m_x << " , " << mp.m_y << std::endl;


			//std::cout << "GLFW: " << mousex << " , " << mousey << std::endl;
			static vector2::Vec2 prevCameraScale;

			ImVec2 pos, imageSize;
			pos.x = EditorCamera::m_gamePreviewWindowPosition.m_x;
			pos.y = EditorCamera::m_gamePreviewWindowPosition.m_y;
			imageSize.x = EditorCamera::m_gamePreviewWindowDimensions.m_x;
			imageSize.y = EditorCamera::m_gamePreviewWindowDimensions.m_y;

			float cordX = ImGui::GetMousePos().x - pos.x;
			float cordY = ImGui::GetMousePos().y - pos.y;


			//float windowHeight = Helper::Helpers::GetInstance()->m_windowHeight;
			//float windowWidth = Helper::Helpers::GetInstance()->m_windowWidth;
			
			
			int windowWidth, windowHeight;

			glfwGetWindowSize(window, &windowWidth, &windowHeight);
			//std::cout << imageSize.y << std::endl;
			//ImVec2 imguiWindowSize = imguiWindow->Size;
			float scaleX = static_cast<float>(windowWidth) / imageSize.x;
			float scaleY = static_cast<float>(windowHeight) / (imageSize.y);
			
			cordX *= scaleX;
			cordY *= scaleY;
			//TODO calculate mouse pos correctly
			//cordX = (cordX - imageSize.x / 2.f) / (imageSize.x / 2.f);
			//cordY = (std::abs(cordY) - imageSize.y / 2.f) / (imageSize.y / 2.f);

			//cordX *= windowWidth;
			//cordY *= windowHeight;

			//ImVec2 scaling = ImGui::GetIO().DisplayFramebufferScale;

			//float windowHeight = Helper::Helpers::GetInstance()->m_windowHeight;
			//float windowWidth = Helper::Helpers::GetInstance()->m_windowHeight;

			//// Get the ImGui window size
			//ImVec2 imguiWindowSize = imguiWindow->Size;
			//// Calculate scaling factors
			//float scaleX = static_cast<float>(windowWidth) / imguiWindowSize.x;
			//float scaleY = static_cast<float>(windowHeight) / imguiWindowSize.y;


			//// Get the ImGui window position
			//ImVec2 imguiWindowPos = imguiWindow->Pos;

			//// Transform the mouse position to ImGui coordinates
			//float imguiMouseX = (mousex - imguiWindowPos.x) * scaleX;
			//float imguiMouseY = (mousey - imguiWindowPos.y) * scaleY;
			cordY = windowHeight - cordY;


			///std::cout << "Sean cord: " << cordX << " , " << cordY << std::endl;

			//inject to mouse input
			Input::InputSystem::MousePosition = { cordX , cordY };


			//std::cout << "imgui: " << imguiMouseX << " , " << imguiMouseY << std::endl;

			//Helper::Helpers* helper = Helper::Helpers::GetInstance();
			//vector2::Vec2 mouse_Pos = Input::InputSystem::m_getMousePosition();
			////window height width
			//float width = Helper::Helpers::GetInstance()->m_windowWidth;
			//float height = Helper::Helpers::GetInstance()->m_windowHeight;
			////world coordinate
			//float world_Mouse_Pos_X = ((mouse_Pos.m_x - helper->m_viewportOffsetX) - (width / 2.f)) / (width / 2.f);
			//float world_Mouse_Pos_Y = (std::abs((mouse_Pos.m_y - helper->m_viewportOffsetY)) - (height / 2.f)) / (height / 2.f);
			//vector2::Vec2 world_Mouse_Pos{ world_Mouse_Pos_X, world_Mouse_Pos_Y };
			////include the camera
			////scale according to camera scale
			//world_Mouse_Pos.m_x *= graphicpipe::GraphicsCamera::m_currCameraScaleX;
			//world_Mouse_Pos.m_y *= graphicpipe::GraphicsCamera::m_currCameraScaleY;

			////aspect ratio
			//world_Mouse_Pos.m_x *= 1.f / graphicpipe::GraphicsCamera::m_aspectRatio;

			////translate 
			//world_Mouse_Pos.m_x += graphicpipe::GraphicsCamera::m_currCameraMatrix[2][0];
			//world_Mouse_Pos.m_y += graphicpipe::GraphicsCamera::m_currCameraMatrix[2][1];

			//ImGuiIO& io = ImGui::GetIO();

			//// Define your custom mouse position
			//ImVec2 customMousePos = ImVec2(world_Mouse_Pos.m_x, world_Mouse_Pos.m_y); // Replace with your calculated coordinates

			//// Override the ImGui mouse position
			//io.MousePos = customMousePos;
		}
		




	}

	bool containsSubstring(const std::string& x, const std::string& y) {
		// Convert both strings to lowercase
		std::string lower_x = x;
		std::string lower_y = y;

		std::transform(lower_x.begin(), lower_x.end(), lower_x.begin(), ::tolower);
		std::transform(lower_y.begin(), lower_y.end(), lower_y.begin(), ::tolower);

		// Check if the lowercase version of y is found in x
		return lower_x.find(lower_y) != std::string::npos;
	}
}

