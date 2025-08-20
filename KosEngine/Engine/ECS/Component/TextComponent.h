/******************************************************************/
/*!
\file      TextComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains data for the text component


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef TEXTCOM_H
#define TEXTCOM_H

#include "Component.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     TextComponet
	\brief     Contains data required to render text
	*/
	/******************************************************************/
	class TextComponent : public Component {

	public:

		std::string m_text{"Insert Text"};
		std::string m_fileName{};
		int m_fontLayer{};
		float m_fontSize{ 100.f };
		bool m_isCentered{ true };

		vector3::Vec3 m_color;

		REFLECTABLE(TextComponent, m_fontLayer, m_isCentered);
	};

}




#endif TEXTCOM_H
