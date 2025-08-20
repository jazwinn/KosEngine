/******************************************************************/
/*!
\file      ComponentList.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      Oct 02, 2024
\brief     This file contains the enum for every componetn that 
		   have ever been created


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef COMPLIST_H
#define COMPLIST_H

namespace ecs {
/******************************************************************/
/*!
\enum     ComponentType
\brief    Contains enum for all the existing components
*/
/******************************************************************/
	enum ComponentType {
		TYPENAMECOMPONENT,
		TYPETRANSFORMCOMPONENT,
		TYPESPRITECOMPONENT,
		TYPEUISPRITECOMPONENT,
		TYPECOLLIDERCOMPONENT,
		TYPERIGIDBODYCOMPONENT,
		TYPEENEMYCOMPONENT,
		TYPETEXTCOMPONENT,
		TYPEANIMATIONCOMPONENT,
		TYPECAMERACOMPONENT,
		TYPESCRIPTCOMPONENT,
		TYPEBUTTONCOMPONENT,
		TYPETILEMAPCOMPONENT,
		TYPEAUDIOCOMPONENT,
		TYPELIGHTINGCOMPONENT,
		TYPEGRIDCOMPONENT,
		TYPERAYCASTINGCOMPONENT,
		TYPEPATHFINDINGCOMPONENT,
		TYPEPARTICLECOMPONENT,
		TYPEVIDEOCOMPONENT,
		TOTALTYPECOMPONENT
		
	};

}



#endif COMPLIST_H

