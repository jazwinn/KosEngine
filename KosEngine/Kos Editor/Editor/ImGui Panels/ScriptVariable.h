/********************************************************************/
/*!
\file      ScriptVariable.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This header file declares the `ScriptEditor` class, which provides
		   functions for displaying and modifying script components in an ECS entity.
		   - DisplayScriptComponents: Displays and allows editing of public fields
			 in a script component attached to an entity.

This file enables the dynamic editing of script component fields within the ECS,
allowing for real-time updates of properties such as integers, floats, and booleans.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#pragma once


#include "ECS/ECS.h"

namespace scripteditor {

	class ScriptEditor {

	public:

		/******************************************************************/
		/*!
			\fn        void ScriptEditor::DisplayScriptComponents(std::string script, ecs::EntityID entityID)
			\brief     Displays public fields of a specified script component for editing.
			\param[in] script   The name of the script attached to the entity.
			\param[in] entityID The ID of the entity to which the script component is attached.
			\details   Uses ImGui to render editable UI elements for each public field in the script,
					   supporting int, float, and boolean data types. Only fields marked as public
					   are displayed, allowing their values to be modified at runtime.
		*/
		/******************************************************************/

		static void DisplayScriptComponents(std::string script, ecs::EntityID);

	};




}