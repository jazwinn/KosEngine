/******************************************************************/
/*!
\file      TransformComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the trnasformation of the object


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef TRANSCOM_H
#define TRANSCOM_H

#include "Component.h"


namespace ecs {
	/******************************************************************/
	/*!
	\class     TransformComponent
	\brief     Contains data required for transformation
	*/
	/******************************************************************/
	class TransformComponent : public Component {

	public:

		vector2::Vec2 m_position{0,0};
		float m_rotation{};
		vector2::Vec2 m_scale{1,1};

		// TRS for entity
		mat3x3::Mat3x3 m_transformation{};

	public:
		//for Entities with child
		std::vector<EntityID> m_childID{};
		
		mat3x3::Mat3x3 m_localChildTransformation{};

		//for entities with parents
		EntityID m_parentID{};
		
		bool m_haveParent{false};

		REFLECTABLE(TransformComponent, m_position, m_rotation, m_scale)
		
	};

}



#endif TRANSCOM_H