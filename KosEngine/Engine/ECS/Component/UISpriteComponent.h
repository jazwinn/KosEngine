/******************************************************************/
/*!
\file      SpriteComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the sprite component


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef UISPRITE_H
#define UISPRITE_H

#include "Component.h"



namespace ecs {

	/******************************************************************/
	/*!
	\class     SpriteComponent
	\brief     Contains data required to render
	*/
	/******************************************************************/
	class UISpriteComponent : public Component {

	public:

		std::string m_imageFile{};
		int m_layer{};
		vector3::Vec3 m_color{ 1.f,1.f,1.f };
		float m_alpha{ 1.f };
		///bool m_isIlluminated{true}; alwasy false
		REFLECTABLE(UISpriteComponent, m_imageFile, m_color, m_alpha);
	};

}




#endif UISPRITE_H
