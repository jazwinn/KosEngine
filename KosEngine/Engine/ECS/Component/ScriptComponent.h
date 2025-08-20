/******************************************************************/
/*!
\file      ScriptComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for rigid body


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef SCRIPTCOM_H
#define SCRIPTCOM_H

#include "Component.h"
#include <mono/metadata/object.h>

namespace ecs {

	/******************************************************************/
	/*!
	\class     ScriptComponent
	\brief     Contains rigid body data
	*/
	/******************************************************************/
	class ScriptComponent : public Component {

	public:

		std::vector<std::tuple<std::string, bool, std::unordered_map<std::string, std::string> >> m_scripts; // bool is true if script is enabled

		/*
		Tupple

		0 - std::String: Name of the script
		1 - bool: true if script is enabled
		2 - std::unordered_map<std::string, void*>: store script public data, key being varaible name, second being base64
		
		
		
		*/

	

		// need not serialize after this line

		std::map<std::string, std::pair<MonoObject*, bool>> m_scriptInstances; // bool here is to check to see if "start" function is called

		std::vector<int> m_scriptHandler;


		//REFLECTABLE(ScriptComponent, m_scripts)

	};

}




#endif SCRIPTCOM_H