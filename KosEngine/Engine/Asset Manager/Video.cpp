/******************************************************************/
/*!
\file      Video.cpp
\author    Ng Jaz winn, jazwinn.ng , 2301502
\par       jazwinn.ng@digipen.edu
\date      March 01, 2025
\brief     This file contains the defination for the video object.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "Video.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"

#define PL_MPEG_IMPLEMENTATION
#include "pl_mpeg.h"






namespace video {

    Video::Video(std::string filepath, GLuint shaderProgram, std::bitset<VIDEO_FLAGS::TOTAL> flag) : m_shaderProgram{ shaderProgram }
	{
		//graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();
		mpeg = plm_create_with_filename(filepath.c_str());

        videoframes = static_cast<int>(plm_get_framerate(mpeg));

        //check flags 
        if (flag.test(VIDEO_FLAGS::AUDIO)) {
            plm_set_audio_enabled(mpeg, true);
        }
        if (flag.test(VIDEO_FLAGS::LOOP)) {
            plm_set_loop(mpeg, true);
        }

        elapsedTime = 0.f;
        lastTime = std::chrono::high_resolution_clock::now();

        videoWidth = plm_get_width(mpeg);
        videoHieght = plm_get_height(mpeg);

        glGenTextures(1, &yTexture);
        glGenTextures(1, &uTexture);
        glGenTextures(1, &vTexture);

        glBindTexture(GL_TEXTURE_2D, yTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoWidth, videoHieght, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, uTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoWidth / 2, videoHieght / 2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        glBindTexture(GL_TEXTURE_2D, vTexture);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, videoWidth / 2, videoHieght / 2, 0, GL_RED, GL_UNSIGNED_BYTE, NULL);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL init texture Error: " << std::hex << err << std::dec << " - ";
            switch (err) {
            case GL_INVALID_ENUM: std::cerr << "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: std::cerr << "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: std::cerr << "GL_OUT_OF_MEMORY"; break;
            default: std::cerr << "Unknown Error"; break;
            }
            std::cerr << std::endl;
        }

        glUseProgram(m_shaderProgram);
        glUniform1i(glGetUniformLocation(m_shaderProgram, "yTexture"), 0); // Bind to texture unit 0
        glUniform1i(glGetUniformLocation(m_shaderProgram, "uTexture"), 1); // Bind to texture unit 1
        glUniform1i(glGetUniformLocation(m_shaderProgram, "vTexture"), 2); // Bind to texture unit 2

        

        locTransformation = glGetUniformLocation(m_shaderProgram, "transformation");
        locView = glGetUniformLocation(m_shaderProgram, "view");
        locProjection = glGetUniformLocation(m_shaderProgram, "projection");
        unilayer = glGetUniformLocation(m_shaderProgram, "layer");

        glUseProgram(0);
	}

    

    Video::~Video()
    {
        glDeleteTextures(1, &yTexture);
        glDeleteTextures(1, &uTexture);
        glDeleteTextures(1, &vTexture);
        plm_destroy(mpeg);
    }

	void Video::DecodeAndUpdateVideo(bool pause)
	{
        auto currentTime = std::chrono::high_resolution_clock::now();
        std::chrono::duration<float> deltaTime = currentTime - lastTime;
        lastTime = currentTime;

        if (pause) return;

        elapsedTime += deltaTime.count(); // Accumulate delta time

        int expectedFrame = static_cast<int>(elapsedTime / (1.f/videoframes));

        while (videoFrameIndex <= expectedFrame) {
            videoFrameIndex++;
            plm_frame_t* frame = plm_decode_video(mpeg);
            if (frame) {
                UpdateTextures(frame);
            }
        }



        
	}

    bool Video::HasStopped()
    {

        return mpeg && plm_has_ended(mpeg);

    }



    void Video::UpdateTextures(plm_frame_t* frame)
    {
        if (!frame || !frame->y.data || !frame->cb.data || !frame->cr.data) {
            std::cerr << "Invalid frame data!" << std::endl;
            return;
        }

        glBindTexture(GL_TEXTURE_2D, yTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->width, frame->height, GL_RED, GL_UNSIGNED_BYTE, frame->y.data);

        glBindTexture(GL_TEXTURE_2D, uTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->width / 2, frame->height / 2, GL_RED, GL_UNSIGNED_BYTE, frame->cb.data);

        glBindTexture(GL_TEXTURE_2D, vTexture);
        glTexSubImage2D(GL_TEXTURE_2D, 0, 0, 0, frame->width / 2, frame->height / 2, GL_RED, GL_UNSIGNED_BYTE, frame->cr.data);


        GLenum err = glGetError();
        if (err != GL_NO_ERROR) {
            std::cerr << "OpenGL Error: " << std::hex << err << std::dec << " - ";
            switch (err) {
            case GL_INVALID_ENUM: std::cerr << "GL_INVALID_ENUM"; break;
            case GL_INVALID_VALUE: std::cerr << "GL_INVALID_VALUE"; break;
            case GL_INVALID_OPERATION: std::cerr << "GL_INVALID_OPERATION"; break;
            case GL_INVALID_FRAMEBUFFER_OPERATION: std::cerr << "GL_INVALID_FRAMEBUFFER_OPERATION"; break;
            case GL_OUT_OF_MEMORY: std::cerr << "GL_OUT_OF_MEMORY"; break;
            default: std::cerr << "Unknown Error"; break;
            }
            std::cerr << std::endl;
        }

    }




}