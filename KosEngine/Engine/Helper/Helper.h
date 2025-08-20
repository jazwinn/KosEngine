/******************************************************************/
/*!
\file      Helper.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains varaibles that is called and used anywhere
		   in the application.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef HELP_H
#define HELP_H

#include "../Math/Vector3.h"
#include <memory>
#include <string>

namespace Helper {
/******************************************************************/
/*!
\class     Helpers
\brief     Contains varaibles that can be called in anywhere
*/
/******************************************************************/
	class Helpers {
	public:
		//singleton
		static Helpers* GetInstance() {
			if (!InstancePtr) {
				InstancePtr.reset(new Helpers{});
			}
			return InstancePtr.get();
		}

		std::string m_startScene;

		int m_currWindowWidth;

		int m_currWindowHeight;

		int m_viewportOffsetX;

		int m_viewportOffsetY;

		float m_windowWidth;

		float m_windowHeight;

		float m_windowScalar;

		bool m_windowMinimise{ false };

		bool m_closeWindow{ false };

		float m_gameRunTime;

		float m_deltaTime;

		float m_fpsCap;

		float m_fps;

		int currentNumberOfSteps;

		float m_fixedDeltaTime;

		float m_accumulatedTime = 0.f;

		vector3::Vec3 m_colour{0.86f,0.86f,0.86f};

		float m_timeScale = 1.0f; //default to 1x

		std::string m_currMousePicture{ "default" };

		bool m_isMouseCentered{ false };
		
	private:
		static std::unique_ptr<Helpers> InstancePtr;

	};

}


#endif // !HELP_H



