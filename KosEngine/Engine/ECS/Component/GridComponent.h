/******************************************************************/
/*!
\file      GridComponent.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      Jan 25, 2025
\brief



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#ifndef GRIDCOM_H
#define GRIDCOM_H

#include "Component.h"
#include <vector>

namespace ecs {

	class GridComponent : public Component {

	public:

		vector2::Vec2 m_Anchor{ 0,0 };
		int m_GridRowLength{ 1 };
		int m_GridColumnLength{ 1 };
		bool m_SetCollidable{ true };
		int m_GridKey;                     
		std::vector<std::vector<int>> m_IsWall;

		REFLECTABLE(GridComponent,m_Anchor, m_GridRowLength, m_GridColumnLength, m_SetCollidable, m_GridKey)
	};
}

#endif GRIDCOM_H
