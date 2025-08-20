/******************************************************************/
/*!
\file      ColliderComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the animation data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef ANICOM_H
#define ANICOM_H

#include "Component.h"

namespace ecs {


	class AnimationComponent : public Component {

	public:
		int m_frameNumber{};
		int m_framesPerSecond{};
		float m_frameTimer{};
		bool m_isAnimating{ true };
		int m_stripCount{ 1 };

		REFLECTABLE(AnimationComponent, m_frameNumber, m_framesPerSecond, m_frameTimer, m_isAnimating, m_stripCount)
	};

}




#endif ANICOM_H