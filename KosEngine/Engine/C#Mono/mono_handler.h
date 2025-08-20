/********************************************************************/
/*!
\file      mono_handler.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 11, 2024
\brief     This header file declares functions and structures for handling Mono runtime operations
		   for scripting integration in the ECS framework.
		   - ScriptMonoInfo: Struct to store script metadata, including the script path, filename, image, and assembly.
		   - m_LoadSecondaryDomain: Loads an additional domain for isolated script compilation.
		   - m_UnloadSecondaryDomain: Unloads the secondary domain.
		   - m_UnloadDomain: Unloads a specified domain by file path.
		   - m_CompileCSharpFile: Compiles a C# script into a DLL file.
		   - m_AddScripts: Adds and loads assemblies for multiple scripts.
		   - m_LoadMethod: Loads a specified method from a C# script, preparing it for invocation.
		   - m_CreateObjectInstance: Creates a Mono object instance from a specified script and class.
		   - m_InvokeMethod: Invokes a previously loaded method within a specified C# script.
		   - m_Cleanup: Cleans up and unloads the Mono runtime, releasing resources.
		   - m_HotReloadCompileAllCsharpFile: Recompiles all C# files for hot reloading.
		   - m_ReloadAllDLL: Reloads all compiled DLLs for updated scripting.

This file enables C++ and C# interoperation by using Mono�s runtime for handling and executing C# scripts
within the game�s ECS framework. It supports functions for compiling scripts, creating object instances,
hot reloading scripts, and invoking script methods, which allows dynamic behavior customization.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef MONOHAND_H
#define MONOHAND_H

#include <mono/jit/jit.h>
#include <mono/metadata/assembly.h>
#include <mono/metadata/debug-helpers.h>

#include "../ECS/ECS.h"


namespace script {

	// 1 struct, 1 dll
	struct ScriptMonoInfo {
		std::filesystem::path m_scriptPath;
		std::string m_fileName;
		MonoImage* m_image;
		MonoAssembly* m_assembly;
	};

	class ScriptHandler {
	public:
		// Initialize Mono and load C#

		/******************************************************************/
		/*!
		\fn      ScriptHandler::ScriptHandler()
		\brief   Constructor that initializes the Mono runtime and loads C# scripts.
		\details Sets up the environment for managing C# scripting within the ECS framework.
		*/
		/******************************************************************/
		ScriptHandler();

		/******************************************************************/
		/*!
		\fn      ScriptHandler::~ScriptHandler()
		\brief   Destructor that cleans up and unloads the Mono runtime.
		\details Releases any resources held by the Mono runtime, ensuring proper cleanup.
		*/
		/******************************************************************/
		~ScriptHandler();

		/******************************************************************/
		/*!
			\fn        bool m_LoadSecondaryDomain()
			\brief     Loads a secondary Mono domain for isolated script compilation.
			\return    True if the domain was successfully loaded; otherwise, false.
			\details   This function creates and loads a secondary domain for compiling
					   and running scripts separately from the primary Mono domain.
		*/
		/******************************************************************/
		void m_LoadSecondaryDomain();

		/******************************************************************/
		/*!
			\fn        void m_UnloadSecondaryDomain()
			\brief     Unloads the secondary Mono domain.
			\details   This function releases the secondary Mono domain and associated
					   resources, ensuring no memory leaks from isolated script execution.
		*/
		/******************************************************************/
		void m_UnloadSecondaryDomain();

		/******************************************************************/
		/*!
			\fn        void m_UnloadDomain(const std::string& path)
			\brief     Unloads a Mono domain by a specified file path.
			\param[in] path The file path of the domain to unload.
			\details   This function removes a Mono domain associated with a specific
					   path, freeing resources and memory.
		*/
		/******************************************************************/
		void m_UnloadDomain(const std::filesystem::path& filePath);

		/******************************************************************/
		/*!
			\fn        bool m_CompileCSharpFile(const std::string& path)
			\brief     Compiles a C# script file into a DLL for Mono execution.
			\param[in] path The file path of the C# script to compile.
			\return    True if compilation is successful; otherwise, false.
			\details   This function compiles a C# script file into a .dll file, which
					   can then be loaded and executed within the Mono runtime.
		*/
		/******************************************************************/
		void m_CompileCSharpFile(const std::filesystem::path& filePath);

		/******************************************************************/
		/*!
		\fn      void m_CompileAllCSharptoSingleDLL()
		\brief   Compiles all C# scripts into a single DLL file for Mono execution.
		\details This function aggregates and compiles multiple C# script files into a single DLL,
				 simplifying the process of loading and managing C# assemblies within the application.
		*/
		/******************************************************************/
		void m_CompileAllCSharptoSingleDLL();

		/******************************************************************/
		/*!
			\fn        void m_AddScripts(const std::vector<std::string>& scripts)
			\brief     Adds and loads assemblies for a list of C# script files.
			\param[in] scripts A vector of script file paths to add.
			\details   This function loads the assemblies for each script provided in
					   the list, making them available for execution in Mono.
		*/
		/******************************************************************/		
		void m_AddScripts(const std::filesystem::path& scriptpath);

		/******************************************************************/
		/*!
			\fn        bool m_LoadMethod(const std::string& scriptName, const std::string& className, const std::string& methodName, int paramCount)
			\brief     Loads a specified method from a C# script, preparing it for later invocation.
			\param[in] scriptName The name of the C# script file containing the method.
			\param[in] className  The name of the class within the script containing the method.
			\param[in] methodName The name of the method to be loaded.
			\param[in] paramCount The number of parameters the method takes.
			\return    True if the method is successfully loaded; otherwise, false.
			\details   This function finds and loads a method from a specified class within a C# script.
					   The method can later be invoked, enabling scripted functionality within the ECS.
		*/
		/******************************************************************/
		bool m_LoadMethod(const std::string& scriptName, const std::string& className, const std::string&  nameSpace, const std::string& methodName, int paramCount);

		/******************************************************************/
		/*!
			\fn        MonoObject* m_CreateObjectInstance(const std::string& scriptName, const std::string& className)
			\brief     Creates a Mono object instance for a specified C# class.
			\param[in] scriptName The name of the script containing the class.
			\param[in] className  The name of the class to instantiate.
			\return    A pointer to the created MonoObject instance.
			\details   This function instantiates a class within a script, returning a
					   MonoObject pointer for further manipulation within the ECS.
		*/
		/******************************************************************/
		MonoObject* m_CreateObjectInstance(const std::string& scriptName, const std::string& className);

		/******************************************************************/
		/*!
			\fn        void m_InvokeMethod(const std::string& scriptName, const std::string& className, const std::string& methodName, void** args, int paramCount)
			\brief     Invokes a previously loaded method within a specified C# script.
			\param[in] scriptName The name of the C# script file containing the method.
			\param[in] className  The name of the class within the script containing the method.
			\param[in] methodName The name of the method to be invoked.
			\param[in] args       An array of arguments to pass to the method.
			\param[in] paramCount The number of parameters being passed to the method.
			\details   This function runs a method in the Mono runtime, allowing C# functions to interact with and modify game entities.
					   It handles exceptions that may arise during invocation.
		*/
		/******************************************************************/
		void m_InvokeMethod(const std::string& scriptName, const std::string& methodName, MonoObject* objInstance, void** args);

		/******************************************************************/
		/*!
			\fn        void m_HotReloadCompileAllCsharpFile()
			\brief     Recompiles all C# files for hot reloading, allowing scripts to be updated dynamically.
			\details   This function recompiles all C# script files, producing updated DLLs for each file,
					   and enables hot reloading of scripts without restarting the engine.
		*/
		/******************************************************************/
		void m_HotReloadCompileAllCsharpFile(std::string path);

		/******************************************************************/
		/*!
			\fn        void m_ReloadAllDLL()
			\brief     Reloads all compiled DLLs to reflect the latest version of the C# scripts.
			\details   This function unloads and reloads all script assemblies, ensuring that the
					   latest compiled versions of the scripts are active in the Mono runtime.
		*/
		/******************************************************************/
		void m_ReloadAllDLL();

		void m_assignVaraiblestoScript(ecs::ScriptComponent* sc, const std::string& script);

		/******************************************************************/
		/*!
		\var     std::vector<std::pair<std::string, std::filesystem::path>> m_CSScripts
		\brief   Stores all C# scripts in the asset folder.
		\details This vector contains pairs of script names and their corresponding file paths.
		*/
		/******************************************************************/
		std::vector<std::pair<std::string, std::filesystem::path>> m_CSScripts;

		/******************************************************************/
		/*!
		\var     std::unordered_map<std::string, ScriptMonoInfo> m_loadedDLLMap
		\brief   Maps script filenames to their corresponding Mono script metadata.
		\details This map stores the metadata (e.g., script path, Mono image, assembly) for each loaded DLL.
		*/
		/******************************************************************/
		std::unordered_map<std::string, ScriptMonoInfo> m_loadedDLLMap;

		/******************************************************************/
		/*!
		\var     std::unordered_map<std::string, std::unordered_map<std::string, MonoMethod*>> m_methodMap
		\brief   Maps script names to their methods.
		\details This nested map stores the methods for each script, allowing quick access to loaded Mono methods.
		*/
		/******************************************************************/
		std::unordered_map<std::string, std::unordered_map<std::string, MonoMethod*>> m_methodMap;

		/******************************************************************/
		/*!
		\fn      MonoDomain* m_GetDomain()
		\brief   Retrieves the current application Mono domain.
		\return  Pointer to the current application Mono domain.
		*/
		/******************************************************************/
		MonoDomain* m_GetDomain() { return m_AppDomain; };

		/******************************************************************/
		/*!
		\var     std::string m_outputdll
		\brief   Stores the output DLL file name for compiled C# scripts.
		\details Used to reference the compiled DLL generated from the C# scripts.
		*/
		/******************************************************************/
		std::string m_outputdll;


	private:
		//root domain
		MonoDomain* m_rootDomain = nullptr;

		MonoDomain* m_AppDomain = nullptr;
	};

	/*--------------------------------------------------------------
	TUTORIAL
	----------------------------------------------------------------

	1. **Creating C# Script (e.g., TemplateScript.cs)**
	   - There are a few requirements:
		 - The file name must match the class name exactly.
		 - The namespace should be named "Namespace" with a capital "N".
		 - Ensure that the script contains three functions: `Start`, `Update`, and `GetEntityID`.

	2. **Adding C# Script to the Project**
	   - Save your C# script and place it in the `Scripts/ScriptsCS` folder.
	   - In the game editor, go to the entity you want to apply the script to, add a Script Component, and then select your script (e.g., `TemplateScript`).
	   - Note: The script won’t run until you press the Play button.

	3. **Running and Updating the Script**
	   - After pressing Play, the system will compile your script. Once compiled successfully, the script will start running.
	   - To update the code in the update loop, edit the `Update` function in your C# script (e.g., add a print statement).
	   - Save your changes, return to the game, and hit Play again. The updates should now be reflected, with the `Start` and `Update` loops running as expected.

	4. **Setting up Internal Core Functions**
	   - To add a new internal core function, such as getting or setting a component, create the function as needed.
	   - Once the function is ready, right-click and build it.
	   - Ensure that the compiled `.dll` is stored in the `GameScript/ScriptCoreDLL` folder. This should happen automatically, but double-check if you encounter debugging issues.

	----------------------------------------------------------------*/

}

#endif