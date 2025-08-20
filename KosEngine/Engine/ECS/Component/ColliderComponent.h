/******************************************************************/
/*!
\file      ColliderComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the collider data 


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef BOXCOM_H
#define BOXCOM_H

#include "Component.h"
#include "../Physics/Physics.h"
#include "../Physics/PhysicsCollisionResponse.h"


namespace ecs {
/******************************************************************/
/*!
\class     ColliderComponent
\brief     Contains all the shape data
*/
/******************************************************************/

	class ColliderComponent : public Component {

	public:
		
		vector2::Vec2 m_Size = {1,1};

		vector2::Vec2 m_OffSet{};

		bool m_drawDebug{true};

		physicspipe::EntityType m_type = physicspipe::EntityType::RECTANGLE;

		float m_radius{0.5f};

		bool m_collisionResponse{ true }; 

		bool m_collisionCheck{ true }; // to pass collision data into the physics pipeline


		// no need to serialize or reflect
		float m_isCollided{ false };

		std::vector<EntityID> m_collidedWith{};

		std::vector<EntityID> m_triColWith{};
		std::unordered_map<EntityID, physicspipe::CollisionState> m_collisionState{};
		std::unordered_set<EntityID> m_keys;
		std::vector<physicspipe::CollisionResponseData> m_contactPoints{}; 

		//1 is top, 2 is right, 4 is bottom, 8 is left
		//3 is top right, 6 is bottom right, 9 is bottom left, 12 is top left;
		unsigned long m_blockedFlag = 0;
		std::bitset<4>m_bits{};

		mat3x3::Mat3x3 m_collider_Transformation{};

		REFLECTABLE(ColliderComponent, m_collisionResponse, m_collisionCheck, m_drawDebug, m_type, m_OffSet, m_radius , m_Size )
	};

}




#endif BOXCOM_H