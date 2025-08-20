/******************************************************************/
/*!
\file      EnemyComponent.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains player data


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef ENEMYCOM_H
#define ENEMYCOM_H

#include "Component.h"


namespace ecs {

	/******************************************************************/
	/*!
	\class     PlayerComponent
	\brief     Contains data specific to a player
	*/
	/******************************************************************/


	class EnemyComponent : public Component {

	public:

		int m_enemyTag{-1};
		int m_enemyTypeInt{-1};
		int m_enemyRoamBehaviourInt{-1};


		REFLECTABLE(EnemyComponent, m_enemyTag, m_enemyTypeInt, m_enemyRoamBehaviourInt);

	};

}




#endif ENEMYCOM_H
