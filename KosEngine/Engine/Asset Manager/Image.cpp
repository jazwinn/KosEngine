/******************************************************************/
/*!
\file      Image.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     This file provides functionalities for loading, padding, and serializing images,
           specifically handling sprite sheets, padding non-square textures, and working with
           RapidJSON to save and load image metadata.

\dependencies
         - STB image library for loading images.
         - RapidJSON for serialization/deserialization.
         - OpenGL for texture handling.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "Image.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"


#include "../Dependencies/rapidjson/document.h"
#include "../Dependencies/rapidjson/writer.h"
#include "../Dependencies/rapidjson/stringbuffer.h"
#include "../Dependencies/rapidjson/fwd.h"

#include "../Graphics/GraphicsPipe.h"
#include "AssetManager.h"
#include <regex>

namespace image {
    ImageManager::~ImageManager()
    {
        //may worng
        for (auto& image : m_imageMap) {
            if (m_imagedataArray[image.second.m_imageID])
            {
                if (image.second.m_isPadded)
                {
                    delete m_imagedataArray[image.second.m_imageID];
                }
                else
                {
                    stbi_image_free(m_imagedataArray[image.second.m_imageID]);
                }
            }
        }

    }
    unsigned int ImageManager::m_LoadImage(const char* file) {

       // assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
        graphicpipe::GraphicsPipe* graphics = graphicpipe::GraphicsPipe::m_funcGetInstance();

        stbi_set_flip_vertically_on_load(true);
        Image image{};
        image.m_stripCount = m_extractStripCountFromFilename(file);
        //image.m_spriteName = m_extractSpriteNameFromFilename(file);
        image.m_spriteName = std::filesystem::path{ file }.filename().string();
        image.m_isTilable = m_checkIfImageTilable(image.m_spriteName);



        unsigned char* data = stbi_load(file, &image.m_width, &image.m_height, &image.m_channels, 0);
        if (!data)
        {
            LOGGING_ASSERT_WITH_MSG("Error: Could not load image {0}", file);
        }

        if (image.m_channels != m_targetChannels)
        {

            LOGGING_WARN("Warning: Color channels for {0} are not following RGBA specifications ", file);
           
            int targetHeight{};
            int targetWidth{};
            if (image.m_width > image.m_height)
            {
                targetHeight = image.m_width;
                targetWidth = image.m_width;
            }
            else
            {
                targetHeight = image.m_height;
                targetWidth = image.m_height;
            }
            unsigned char* newData = m_funcPadTexture(data, image.m_width, image.m_height, image.m_channels, targetWidth, targetHeight, m_targetChannels);
            stbi_image_free(data);
            image.m_isPadded = true;
            image.m_imageID = m_imageCount;
            image.m_width = targetWidth;
            image.m_height = targetHeight;
            image.m_channels = m_targetChannels;
            m_imageCount++;
            m_imageMap[image.m_spriteName] = image;
            m_imagedataArray.push_back(newData);
            LOGGING_INFO("Texture Padded for {0}", image.m_spriteName);
           
        }

        else if (image.m_isTilable)
        {
            LOGGING_INFO("{0} labled as tilemap", image.m_spriteName);
            image.m_imageID = m_imageCount;
            m_imageCount++;
            m_imageMap[image.m_spriteName] = image;
            m_imagedataArray.push_back(data);
        }
        else if (image.m_stripCount == 1)
        {
            /*if (image.m_width != image.m_height)
            {
                int targetHeight{};
                int targetWidth{};
                if (image.m_width > image.m_height)
                {
                    targetHeight = image.m_width;
                    targetWidth = image.m_width;
                }
                else
                {
                    targetHeight = image.m_height;
                    targetWidth = image.m_height;
                }
                unsigned char* newData = m_funcPadTexture(data, image.m_width, image.m_height, image.m_channels, targetWidth, targetHeight, m_targetChannels);
                stbi_image_free(data);
                image.m_isPadded = true;
                image.m_imageID = m_imageCount;
                image.m_width = targetWidth;
                image.m_height = targetHeight;
                image.m_channels = m_targetChannels;
                m_imageCount++;
                m_imageMap[image.m_spriteName] = image;
                m_imagedataArray.push_back(newData);
                LOGGING_INFO("Texture Padded for {0}", image.m_spriteName);
            }
            else
            {*/
                image.m_imageID = m_imageCount;
                m_imageCount++;
                m_imageMap[image.m_spriteName] = image;
                m_imagedataArray.push_back(data);
           // }
        }
        else
        {
            image.m_imageID = m_imageCount;
            m_imageCount++;
            m_imageMap[image.m_spriteName] = image;
            m_imagedataArray.push_back(data);
        }

        unsigned int textureID;
        glGenTextures(1, &textureID);
        glBindTexture(GL_TEXTURE_2D, textureID);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
        glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
        glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, m_imageMap[image.m_spriteName].m_width, m_imageMap[image.m_spriteName].m_height, 0, GL_RGBA, GL_UNSIGNED_BYTE, m_imagedataArray.back());
        glGenerateMipmap(GL_TEXTURE_2D);

        m_imageMap.find(image.m_spriteName)->second.textureID = textureID;

        graphics->m_textureIDs.push_back(textureID);
        LOGGING_INFO("Texture Binded, Texture ID : {0} ", textureID);
        LOGGING_INFO("Image ID : {0} ", image.m_imageID);
        graphics->m_imageData.push_back(image);

        return textureID;

	}

    int ImageManager::m_extractStripCountFromFilename(const std::string& filename)
    {
        // Use regex to find the strip count in the format "something_strip(number).png"
        std::regex pattern("([[:alnum:]])_strip([[:digit:]]+)\\.png");
        std::smatch match;

        if (std::regex_search(filename, match, pattern))
        {
            LOGGING_INFO("Strip Success for {0}", filename);
            return std::stoi(match[2].str());
        }

        pattern = ("([[:alnum:]])_strip([[:digit:]]+)\\.jpg");
        if (std::regex_search(filename, match, pattern))
        {
            LOGGING_INFO("Strip Success for {0}", filename);
            return std::stoi(match[2].str());
        }

        // Default to 1 if no number is found
        return 1;
    }

    std::string ImageManager::m_extractSpriteNameFromFilename(const std::string& filename)
    {
        std::regex pattern("([_[:alnum:]]+)([_]+)([[:alnum:]]*)\\.png"); // For animation
        std::smatch match;

        if (std::regex_search(filename, match, pattern))
        {
            return match[0].str();
        }

        pattern = ("([_[:alnum:]]+)([_]+)([[:alnum:]]*)\\.jpg"); // For single sprites
        if (std::regex_search(filename, match, pattern))
        {
            return match[0].str();
        }

        pattern = ("([[:alnum:]]+)([_]*)([[:alnum:]]*)\\.png"); // For single sprites
        if (std::regex_search(filename, match, pattern))
        {
            return match[0].str();
        }
        
        pattern = ("([[:alnum:]]+)([_]*)([[:alnum:]]*)\\.jpg"); // For single sprites
        if (std::regex_search(filename, match, pattern))
        {
            return match[0].str();
        }

        return "Error_Cannot_Read_Sprite_Name";
    }

    bool ImageManager::m_checkIfImageTilable(const std::string& filename)
    {
        std::regex pattern("^(Tile|tile)_[[:alnum:]]+\\.png$");  // Pattern for "Tile_image.png"

        if (!std::regex_match(filename, pattern))
        {
            pattern = ("^(Tile|tile)_[[:alnum:]]+\\.jpg$");
        }

        return std::regex_match(filename, pattern);
    }

    unsigned char* ImageManager::m_funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels)
    {
        unsigned char* paddedPixels = new unsigned char[targetWidth * targetHeight * targetChannels];

        // Fill with padding (e.g., transparent for RGBA)
        memset(paddedPixels, 0, targetWidth * targetHeight * targetChannels);

        //If the image doesn't have alpha channel
        if (targetChannels > originalChannels)
        {
            for (int y = 0; y < originalHeight; ++y)
            {
                for (int x = 0; x < originalWidth; ++x)
                {
                    int c = 3;
                    int dstIndex = ((y + (targetHeight - originalHeight) / 2) * targetWidth + (x + (targetWidth - originalWidth) / 2)) * targetChannels + c;

                    // Make Opaque
                    paddedPixels[dstIndex] = 255;
                }
            }
        }

        // Calculate the maximum valid indices for source and destination arrays
        int maxSrcIndex = originalHeight * originalWidth * originalChannels;
        int maxDstIndex = targetHeight * targetWidth * targetChannels;

        // Copy the original texture into the center of the padded texture
        for (int y = 0; y < originalHeight; ++y)
        {
            for (int x = 0; x < originalWidth; ++x)
            {
                for (int c = 0; c < originalChannels; ++c)
                {
                    // Calculate source and destination indices
                    int srcIndex = (y * originalWidth + x) * originalChannels + c;
                    int dstIndex = ((y + (targetHeight - originalHeight) / 2) * targetWidth + (x + (targetWidth - originalWidth) / 2)) * targetChannels + c;

                    if (srcIndex < maxSrcIndex && dstIndex < maxDstIndex)
                    {
                        // Copy pixel data
                        paddedPixels[dstIndex] = originalPixels[srcIndex];
                    }
                    else
                    {
                        LOGGING_ERROR("Buffer Overflow as SrcIndx > MaxSrcIndex || dstIndex > maxDstIndex");
                    }

                }
            }
        }

        return paddedPixels;
    }


}