/******************************************************************/
/*!
\file      RaycastComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the animation data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef RAYCOM_H
#define RAYCOM_H

#include "Component.h"
#include "../ECS/Layers.h"

namespace ecs {



	class RaycastComponent : public Component
	{
	public:

		class Raycast {
		public:

			std::string m_rayID;

			bool m_isRaycasting{true};

			vector2::Vec2 m_targetPosition{};

			std::vector<layer::LAYERS> m_Layers;

			// need not reflect or serialize after this line

			float m_distance{}; // distance between target and entity pos

			bool m_targetReached{};// true if hits target, false if anything in between

			vector2::Vec2 m_hitposition{};

			REFLECTABLE(Raycast, m_rayID, m_isRaycasting, m_targetPosition, m_Layers, m_distance, m_targetReached, m_hitposition);
		};
		
		std::vector<Raycast> m_raycast{};


		REFLECTABLE(RaycastComponent, m_raycast);

	};

}

#endif