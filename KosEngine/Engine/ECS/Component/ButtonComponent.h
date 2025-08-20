/******************************************************************/
/*!
\file      ColliderComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the button data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef BUTTONCOM_H
#define BUTTONCOM_H

#include "Component.h"

namespace ecs {


	class ButtonComponent : public Component {

	public:
		vector2::Vec2 m_Position{};
		vector2::Vec2 m_Scale{1.f,1.f};
		bool m_IsClick = false;

		bool m_EnableHover = true;
		bool m_IsHover = false;
		REFLECTABLE(ButtonComponent, m_Position, m_Scale, m_IsClick, m_EnableHover, m_IsHover);
	};

}




#endif CAMCOM_H