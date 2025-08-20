/******************************************************************/
/*!
\file      TransformComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the video


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once

#include "Component.h"
#include "../Asset Manager/Video.h"

namespace ecs {

	class VideoComponent : public Component {

	public:

		bool play = false;

		std::string filename;
		
		bool pause = false;

		bool loop = true;

		int layer = 0;

	public:

		REFLECTABLE(VideoComponent, filename, pause, loop, layer);

	};


}
