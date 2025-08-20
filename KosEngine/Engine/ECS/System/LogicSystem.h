
/******************************************************************/
/*!
\file      LogicSystem.h
\author    Ng Jaz Winn
\par       jazwinn.ng@digipen.edu
\date      Nov 30, 2024
\brief     

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef LOGICSYS_H
#define LOGICSYS_H

#include "System.h"
#include "../ECS/Component/ScriptComponent.h"
#include "../ECS/ECSList.h"

namespace ecs {


    class LogicSystem : public ISystem {

    public:

        /******************************************************************/
        /*!
        \fn      void LogicSystem::m_RegisterSystem(EntityID)
        \brief   Registers an entity with the logic system.
        \param   EntityID - The ID of the entity to be registered.
        \details Adds the entity to the logic system, enabling it to run custom script logic.
        */
        /******************************************************************/
        void m_RegisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void LogicSystem::m_DeregisterSystem(EntityID)
        \brief   Deregisters an entity from the logic system.
        \param   EntityID - The ID of the entity to be deregistered.
        \details Removes the entity from the logic system, stopping its script logic updates.
        */
        /******************************************************************/
        void m_DeregisterSystem(EntityID) override;

        /******************************************************************/
        /*!
        \fn      void LogicSystem::m_Init()
        \brief   Initializes the logic system.
        \details Prepares necessary resources and sets up the logic system for use.
        */
        /******************************************************************/
        void m_Init() override;

        /******************************************************************/
        /*!
        \fn      void LogicSystem::m_StartLogic()
        \brief   Starts the logic for all entities registered in the system.
        \details Executes any startup logic scripts for the registered entities.
        */
        /******************************************************************/
        void m_StartLogic();

        /******************************************************************/
        /*!
        \fn      void LogicSystem::m_Update(const std::string& updateContext)
        \brief   Updates the logic system.
        \param   updateContext - The context or description of the update.
        \details Executes script updates for all entities registered in the system.
        */
        /******************************************************************/
        void m_Update(const std::string&) override;


    private:

        /******************************************************************/
        /*!
        \var     std::vector<ScriptComponent*> m_vecScriptComponentPtr
        \brief   Stores pointers to ScriptComponent instances for registered entities.
        \details Used to manage and execute scripts for the entities in the system.
        */
        /******************************************************************/
        std::vector<ScriptComponent*> m_vecScriptComponentPtr;

        std::vector<NameComponent*> m_vecNameComponentPtr;
        /******************************************************************/
        /*!
        \fn      void LogicSystem::CreateandStartScriptInstance(ecs::ScriptComponent* scriptComp)
        \brief   Creates and starts an instance of the provided script component.
        \param   scriptComp - Pointer to the script component to be started.
        \details This function initializes and runs the script associated with the provided component.
        */
        /******************************************************************/
        void CreateandStartScriptInstance(ecs::ScriptComponent* scriptComp);



    };

}

#endif LOGICSYS_H
