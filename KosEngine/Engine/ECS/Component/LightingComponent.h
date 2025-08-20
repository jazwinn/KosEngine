/******************************************************************/
/*!
\file      LightingComponent.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Jan 25, 2025
\brief   



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef LIGHT_H
#define LIGHT_H

#include "Component.h"
#include "../Graphics/GraphicsLighting.h"



namespace ecs
{

	class LightingComponent : public Component {

	public:

		graphicpipe::LightType m_lightType{ graphicpipe::LightType::GLOW };
		float m_intensity{ 0.5f };
		vector2::Vec2 m_innerOuterRadius{ 0.f,1.f };
		vector3::Vec3 m_colour{ 1.f,1.f,1.f };
		vector2::Vec2 m_light_OffSet{};
		vector2::Vec2 m_light_scale{ 1.f,1.f };
		float m_light_rotation{};

		REFLECTABLE(LightingComponent,m_innerOuterRadius, m_intensity, m_light_OffSet, m_light_scale, m_light_rotation, m_colour, m_lightType);
	};

}


#endif LIGHT_H
