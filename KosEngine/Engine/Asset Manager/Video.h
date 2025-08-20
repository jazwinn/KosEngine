/******************************************************************/
/*!
\file      Video.h
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      March 01, 2025
\brief     This file contains the declaration for the video object.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#ifndef VIDEO_H
#define VIDEO_H

#include <glew.h>
#include <glfw3.h>
#include <glm.hpp>
#include "../ECS/ECSList.h"
#include "../Config/pch.h"

#include "pl_mpeg.h"

namespace video {

	enum VIDEO_FLAGS {
		AUDIO,
		LOOP,
		TOTAL
	};

	class Video
	{
	public:
		Video(std::string filepath, GLuint shaderProgram , std::bitset<VIDEO_FLAGS::TOTAL> FLAGS = 0);
		~Video();

       

	public:
		void DecodeAndUpdateVideo(bool pause);

		bool HasStopped();

		GLuint yTexture, uTexture, vTexture;

		GLint locTransformation, locView, locProjection;

		GLuint unilayer;

		int videoframes;

	private:
		void UpdateTextures(plm_frame_t* frame);

		plm_t* mpeg;

		int videoWidth, videoHieght;
		
		std::map<std::string, int> uniformLocations;

		GLuint uni_yTexture, uni_uTexture, uni_vTexture;

		GLuint m_shaderProgram;

		float elapsedTime;
		int videoFrameIndex = 0;
		std::chrono::steady_clock::time_point lastTime;
	};


	class VideoManager {






	public:

		std::unordered_map<std::string, std::filesystem::path> m_videopath; //stores path of video

		std::unordered_map<ecs::EntityID, std::unique_ptr<Video>> m_videoMap; // store all playing videos

	};




}



#endif VIDEO_H



