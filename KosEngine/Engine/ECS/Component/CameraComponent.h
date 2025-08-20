/******************************************************************/
/*!
\file      ColliderComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Nov 11, 2024
\brief     This file contains the camera data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef CAMCOM_H
#define CAMCOM_H

#include "Component.h"

namespace ecs {


	class CameraComponent : public Component {

	public:
		float m_left;
		float m_right;
		float m_top;
		float m_bottom;
		float m_aspectRatio;

		REFLECTABLE(CameraComponent, m_left, m_right, m_top, m_bottom, m_aspectRatio)
	};

}




#endif CAMCOM_H