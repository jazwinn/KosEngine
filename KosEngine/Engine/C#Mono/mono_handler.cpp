/********************************************************************/
/*!
\file      mono_handler.cpp
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 11, 2024
\brief     This header file implements functions and structures for handling Mono runtime operations
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
#include "../Config/pch.h"
#include "mono_handler.h"
#include "mono/metadata/mono-gc.h"
#include "mono/metadata/threads.h"

#include "../De&Serialization/json_handler.h"
#include "../Asset Manager/AssetManager.h"

#include "InternalCall.h"

namespace script {

    ScriptHandler::ScriptHandler() {


        //set output dll filename
        m_outputdll = "LogicScript";

        // Set Mono path
        mono_set_dirs("../Engine/Dependencies\\mono\\lib", "../Engine/Dependencies/mono/etc");



        // need for internal call during run time
        m_rootDomain = mono_jit_init("MonoDomain");

        mono_domain_set_config(m_rootDomain, ".", "config");

        mono_thread_set_main(mono_thread_current());

        // initialize internal call
        InternalCall::m_RegisterInternalCalls();

        m_LoadSecondaryDomain();


        m_ReloadAllDLL();


        

       
    }

    void ScriptHandler::m_LoadSecondaryDomain()
    {
        m_AppDomain = mono_domain_create_appdomain((char*)"AppDomainRuntime", nullptr);
        mono_domain_set(m_AppDomain, true);


        //load gamescript assembly location
        m_AddScripts("../Engine/scriptLibrary/GameScript/ScriptCoreDLL/GameScript.dll");
    }

    void ScriptHandler::m_UnloadSecondaryDomain()
    {
        if (m_AppDomain == nullptr)return;
        //reset any references to the MonoAssembly
        mono_domain_set(m_rootDomain, false);
        mono_domain_unload(m_AppDomain);
        m_AppDomain = nullptr;

        //clear entire script map
        m_loadedDLLMap.clear();
    }

    ScriptHandler::~ScriptHandler() {

        if (m_rootDomain) {
            mono_jit_cleanup(m_rootDomain);
        }
    }

    void ScriptHandler::m_UnloadDomain(const std::filesystem::path& filePath) {
        std::string filename = filePath.filename().stem().string();
        auto scriptEntry = m_loadedDLLMap.find(filename);
        if (scriptEntry == m_loadedDLLMap.end()) {
            LOGGING_ERROR("Script does not exist");
            return;
        }
        mono_gc_collect(2);

        // Free methods and image
        auto& scriptData = scriptEntry->second;
        m_methodMap.clear();
        if (scriptData.m_image) {
            mono_image_close(scriptData.m_image);
            scriptData.m_image = nullptr;
        }

        // Remove from map
        m_loadedDLLMap.erase(scriptEntry);
    }

    void ScriptHandler::m_AddScripts(const std::filesystem::path& scriptpath) {   
        //check if dll is alread in 
        std::string filename = scriptpath.filename().stem().string();
        if (m_loadedDLLMap.find(filename) != m_loadedDLLMap.end()) {
            LOGGING_ERROR("Script Already loaded");
            return;
        }
        //create new script
        m_loadedDLLMap[filename].m_fileName = filename;
        m_loadedDLLMap[filename].m_scriptPath = scriptpath;

        // Load assembly
        std::string assemblyPath = scriptpath.string();
        m_loadedDLLMap[filename].m_assembly = mono_domain_assembly_open(m_AppDomain, assemblyPath.c_str());
        if (!m_loadedDLLMap[filename].m_assembly) {
            LOGGING_ERROR("Failed to load assembly: {}", assemblyPath.c_str());
            return;
        }

        // Get Mono image
        MonoImage* image = mono_assembly_get_image(m_loadedDLLMap[filename].m_assembly);
        if (!image) {
            LOGGING_ERROR("Failed to load Mono image from assembly: {}", assemblyPath.c_str());
            return;
        }

        m_loadedDLLMap[filename].m_image = image;
        image = nullptr;

        LOGGING_DEBUG("Successfully Added Script");
    }

    bool ScriptHandler::m_LoadMethod(const std::string& scriptName, const std::string& className, const std::string& nameSpace, const std::string& methodName, int paramCount) {
        // Find the assembly image for the script
        MonoImage* image = m_loadedDLLMap.find(scriptName)->second.m_image;
        if (!image) {
            LOGGING_ERROR("Failed to find image for script: {}", scriptName.c_str());
            return false;
        }

        // Find the class inside the assembly
        MonoClass* m_testClass = mono_class_from_name(image, nameSpace.c_str(), className.c_str());
        if (!m_testClass) {// give exception for gamescript
            //LOGGING_ERROR("Failed to find class: {} in script: {}", className.c_str(), scriptName.c_str());
            return false;
        }

        // Find the method inside the class
        MonoMethod* method = mono_class_get_method_from_name(m_testClass, methodName.c_str(), paramCount);
        if (!method) {
           // LOGGING_ERROR("Failed to find method: {} in class: {}", methodName.c_str(), className.c_str());
            return false;
        }
        
        //
        //if (m_methodMap.find(scriptName)->second.find(methodName) != m_methodMap.find(scriptName)->second.end()) {
        //    LOGGING_ERROR("METHOD ALREADY LOADED");
        //    //return false;
        //}
        //Add method into container
        m_methodMap[className][methodName] = method;
        method = nullptr;

        return true;
    }

    MonoObject* ScriptHandler::m_CreateObjectInstance(const std::string& scriptName, const std::string& className)
    {
        MonoObject* inst = mono_object_new(m_AppDomain, mono_class_from_name(m_loadedDLLMap.find(scriptName)->second.m_image, "", className.c_str()));

        if (inst == nullptr) {
            LOGGING_ERROR("Fail to create instance");
            return inst;
        }

        //run the construct for the object
        mono_runtime_object_init(inst);

        return inst;
    }
    //void InvokeMonoMethod(MonoMethod* method, MonoObject* objInstance, void** args, MonoObject** sexception) {
    //    __try {
    //        mono_runtime_invoke(method, objInstance, args, sexception);
    //    }
    //    __except (EXCEPTION_EXECUTE_HANDLER) {
    //        printf("System exception caught: Access violation or other critical error.\n");
    //    }
    //}

    void ScriptHandler::m_InvokeMethod(const std::string& className, const std::string& func, MonoObject* objInstance, void** args) {

        if (objInstance == nullptr) return;
        if (className.empty()) return;

        //// Check if the method exists
        auto it = m_methodMap.find(className);

        if (it == m_methodMap.end()) {
            //LOGGING_ERROR("No method loaded to invoke for script {}", className.c_str());
            return;
        }

        MonoMethod* method = nullptr;

        auto methodit = it->second.find(func);

        if (methodit == it->second.end()) {
           // LOGGING_ERROR("No method loaded to invoke for script {}", className.c_str());
            return;
        }

        method = methodit->second;
        
        if (!method) {
            //LOGGING_ERROR("No method loaded to invoke for script {}", className.c_str());
            return;
        }


        // Invoke the method with the instance

        MonoObject* exception = nullptr;

        try {
            mono_runtime_invoke(method, objInstance, args, &exception);

            if (exception) {

                LOGGING_WARN("Script Exception Occured");
                mono_print_unhandled_exception(exception);
            }
        }
        catch (...) {
            throw 1;
        }


        
    }

    void ScriptHandler::m_ReloadAllDLL()
    {
        //reload all .dll file
        //by right only logiscript.dll only
        std::string scriptDLLDirectory = "../Assets/Scripts/ScriptsDLL";
        //if (std::filesystem::exists(scriptDLLDirectory)) {
        //    LOGGING_ERROR("Script Directory does not exist");
        //    return;
        //}

        for (auto& directoryPath : std::filesystem::directory_iterator(scriptDLLDirectory)) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            if (directoryPath.path().filename().extension().string() == ".dll") {
                m_AddScripts(directoryPath);

                //load all newly reloaded method

                std::string filename = directoryPath.path().filename().stem().string();

                //for (auto& script : m_CSScripts) {
                //    m_LoadMethod(filename, script.first, "Start", 0);
                //    m_LoadMethod(filename, script.first, "Update", 0);
                //    m_LoadMethod(filename, script.first, "Awake", 1);
                //    
                //    m_LoadMethod(filename, script.first, "LateUpdate",0);
                //}

                MonoImage* image = m_loadedDLLMap.find(directoryPath.path().filename().stem().string())->second.m_image;

                const MonoTableInfo* typeTable = mono_image_get_table_info(image, MONO_TABLE_TYPEDEF);
                int numTypes = mono_table_info_get_rows(typeTable);

                for (int i = 1; i < numTypes; i++) { // Start from 1 (skip <Module>)
                    uint32_t cols[MONO_TYPEDEF_SIZE];
                    mono_metadata_decode_row(typeTable, i, cols, MONO_TYPEDEF_SIZE);

                    const char* className = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAME]);
                    const char* namespaceName = mono_metadata_string_heap(image, cols[MONO_TYPEDEF_NAMESPACE]);

                    m_LoadMethod(filename, className, namespaceName, "Start", 0);
                    m_LoadMethod(filename, className, namespaceName, "Update", 0);
                    m_LoadMethod(filename, className, namespaceName, "Awake", 1);
                    m_LoadMethod(filename, className, namespaceName, "LateUpdate", 0);
                }



            }
        }

       





    }



    void ScriptHandler::m_CompileCSharpFile(const std::filesystem::path& filePath)
    {
        //IF COMPILE ERROR, MAKE SURE TO UNLOAD ASSEMBLY AND APPDOMAIN
        std::filesystem::path projectBasePath = std::filesystem::current_path();

        std::string compilepath = "\"" + projectBasePath.string() + "\\C#Mono\\CompilerCSC\\bin\\csc\"";

        std::filesystem::path referenceDLL = projectBasePath / "ScriptLibrary" / "GameScript" / "ScriptCoreDLL" / "GameScript.dll";

        std::string test = filePath.filename().stem().string();

        std::filesystem::path outputDLL = projectBasePath / "Assets" / "Scripts" / "ScriptsDLL" / (filePath.filename().stem().string() + ".dll");

        std::string command = compilepath + " /target:library \"/out:" + outputDLL.string() + "\" \"/reference:" + referenceDLL.string() + "\" \"" + filePath.string() + "\"";
        
        // Write the command to a batch file
        std::ofstream batchFile("run_command.bat");
        batchFile << command << std::endl;
        batchFile.close();

        LOGGING_INFO("Generated command: {}", command.c_str());

        //// Execute the command
        int result = system("run_command.bat");
            
        //remove bat file
        std::remove("run_command.bat");

        // Check the result of the command
        if (result == 0)
        {
            LOGGING_INFO("Compilation successful: {}.dll", filePath.filename().stem().string().c_str());
        }
        else
        {
            LOGGING_ERROR("Compilation failed!");
        }
    }

    void ScriptHandler::m_CompileAllCSharptoSingleDLL()
    {
        //
        m_UnloadSecondaryDomain();
        if (m_AppDomain == nullptr) {
            m_LoadSecondaryDomain();
        }

        //IF COMPILE ERROR, MAKE SURE TO UNLOAD ASSEMBLY AND APPDOMAIN
        std::filesystem::path projectBasePath = std::filesystem::current_path().parent_path() / "Engine";

        std::string compilepath = "\"" + projectBasePath.string() + "\\C#Mono\\CompilerCSC\\bin\\csc\"" + " /unsafe";

        std::filesystem::path referenceDLL = projectBasePath / "ScriptLibrary" / "GameScript" / "ScriptCoreDLL" / "GameScript.dll";

        std::string Accfile;

        std::for_each(m_CSScripts.begin(), m_CSScripts.end(), [&Accfile](const std::pair<std::string, std::filesystem::path>& file_path) { Accfile += ("\"" + file_path.second.string() + "\" ");});

        std::filesystem::path outputDLL = projectBasePath.parent_path() / "Assets" / "Scripts" / "ScriptsDLL" / (m_outputdll + ".dll");

        std::string command = compilepath + " /target:library \"/out:" + outputDLL.string() + "\" \"/reference:" + referenceDLL.string() + "\" " + Accfile;

        // Write the command to a batch file
        std::ofstream batchFile("run_command.bat");
        batchFile << command << std::endl;
        batchFile.close();

        LOGGING_INFO("Generated command: {}", command.c_str());

        //// Execute the command
        int result = system("run_command.bat");

        //remove bat file
        std::remove("run_command.bat");

        // Check the result of the command
        if (result == 0)
        {
            LOGGING_INFO("Compilation successful: {}.dll", m_outputdll);
        }
        else
        {
            LOGGING_ERROR("Compilation failed!");
        }

    }


    void ScriptHandler::m_HotReloadCompileAllCsharpFile(std::string cspath)
    {
        if (!std::filesystem::exists(cspath)) {
            LOGGING_ERROR("Script File location does not exist");
            return;
        }
        
        // load all .cs file in /Assests/Script
        for (auto& directoryPath : std::filesystem::directory_iterator(cspath)) {
            std::string filepath = directoryPath.path().string();
            std::replace(filepath.begin(), filepath.end(), '\\', '/');

            m_UnloadSecondaryDomain();

            if (directoryPath.is_directory()) {
                m_HotReloadCompileAllCsharpFile(filepath);
            }
            if (directoryPath.path().filename().extension().string() == ".cs") {

                m_CompileCSharpFile(filepath);
            }

            // did u call unload secodnary domain before this?
            if (m_AppDomain == nullptr) {
                m_LoadSecondaryDomain();
            }
        }
    }


    void ScriptHandler::m_assignVaraiblestoScript(ecs::ScriptComponent* sc, const std::string& script)
    {
        assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
        MonoImage* image = assetmanager->m_scriptManager.m_loadedDLLMap.find(assetmanager->m_scriptManager.m_outputdll)->second.m_image;
        if (image == NULL) return;

        MonoClass* scriptclass = mono_class_from_name(image, "", script.c_str());

        void* iter = nullptr;
        MonoClassField* field;
        while ((field = mono_class_get_fields(scriptclass, &iter)) != nullptr) {
            // Check if the field is public
            if ((mono_field_get_flags(field) & 0x0006) == 0x0006) { //0x0006 representing public
                const char* fieldName = mono_field_get_name(field);
                MonoType* fieldType = mono_field_get_type(field);
                int fieldTypeCode = mono_type_get_type(fieldType);

                MonoClassField* fields = mono_class_get_field_from_name(scriptclass, fieldName);

                //std::cout << "Found public field: " << fieldName << " (Type Code: " << fieldTypeCode << ")\n";

                // Change the value based on type (example for int and float)
                if (fieldTypeCode == MONO_TYPE_I4) { // Type code for int

                    const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                    if (it != sc->m_scripts.end()) {
                        auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                        if (variableit != std::get<2>(*it).end()) {// if field exist
                            auto decodedData = Serialization::Serialize::DecodeBase64(variableit->second);
                            if (decodedData) {
                                int integer = *(static_cast<int*>(decodedData.get()));
                                mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, &integer);
                            }
                        }

                    }
                    
                
                }
                else if (fieldTypeCode == MONO_TYPE_R4) { // Type code for float
                    const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                    if (it != sc->m_scripts.end()) {
                        auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                        if (variableit != std::get<2>(*it).end()) {// if field exist
                            auto decodedData = Serialization::Serialize::DecodeBase64(variableit->second);
                            if (decodedData) {
                                float _float = *(static_cast<float*>(decodedData.get()));
                                mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, &_float);
                            }
                        }

                    }
                   

                }
                else if (fieldTypeCode == MONO_TYPE_BOOLEAN) { // Type code for float
                    const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                    if (it != sc->m_scripts.end()) {
                        auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                        if (variableit != std::get<2>(*it).end()) {// if field exist
                            auto decodedData = Serialization::Serialize::DecodeBase64(variableit->second);
                            if (decodedData) {
                                bool _bool = *(static_cast<bool*>(decodedData.get()));
                                mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, &_bool);
                            }
                        }

                    }
                   


                }
                else if (fieldTypeCode == MONO_TYPE_STRING) { // Type code for string


                    const auto& it = std::find_if(sc->m_scripts.begin(), sc->m_scripts.end(), [&](auto& x) {return std::get<0>(x) == script; });

                    if (it != sc->m_scripts.end()) {
                        auto variableit = std::get<2>(*it).find(fieldName); // use map to find fieldname
                        if (variableit != std::get<2>(*it).end()) {// if field exist
                            MonoString* newMonoStr = mono_string_new(mono_domain_get(), variableit->second.c_str());
                            mono_field_set_value(sc->m_scriptInstances.find(script)->second.first, fields, newMonoStr);
                        }

                    }

                }
            }
        }



    }


}
