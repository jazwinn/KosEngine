/******************************************************************/
/*!
\file      Listeners.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds any functions that are to be used as a callback

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#include "../Config/pch.h"
#include "EventsListeners.h"


namespace messaging {

	void SoundPlayed(const Message& givenMsg){
		//assetmanager::AssetManager* assetManager = assetmanager::AssetManager::m_funcGetInstance();
		//assetManager->m_audioManager.getSoundMap().find("zwing.wav")->second->m_PlaySound();
		LOGGING_INFO("Sound has been played! Messaging system called from: %s", givenMsg.m_GetData().c_str());
	}
}