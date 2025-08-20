/******************************************************************/
/*!
\file      Text.cpp
\author    Clarence Boey
\par       c.boey@digipen.edu
\date      Sept 29, 2024
\brief     This file contains the definition of the Font and
           CharacterData classes, which provide functionality for
           loading and handling font data using FreeType and OpenGL.

           The CharacterData class holds information about individual
           glyphs, including texture ID, size, bearing, and advance.
           The Font class contains functionality to load fonts and
           store character data in an efficient manner for rendering
           text in OpenGL.

           This file uses the FreeType library for loading font files
           and converting them into textures.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "Text.h"
#include "../Debugging/Logging.h"
#include "AssetManager.h"

namespace text {

	void FontManager::LoadFont(std::string file) {

        std::filesystem::path filepath = file;
        std::string filename = filepath.filename().string();
        FT_Library ft;
        // All functions return a value different than 0 whenever an error occurred
        if (FT_Init_FreeType(&ft))
        {
            LOGGING_ERROR("ERROR::FREETYPE: Could not init FreeType Library");
            return;
        }

        // find path to font

        if (file.empty())
        {
            LOGGING_ERROR("ERROR::FREETYPE: Failed to load font_name - file path is empty");
            return;
        }

        // load font as face
        FT_Face face;
        if (FT_New_Face(ft, file.c_str(), 0, &face)) {
            LOGGING_ERROR("ERROR::FREETYPE: Failed to load font: {}", file.c_str());
            return;
        }
        else {
            // set size to load glyphs as
            FT_Set_Pixel_Sizes(face, 0, 48);


            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            const int numChars = 128;
            const int glyphWidth = face->size->metrics.max_advance >> 6;  // max glyph width
            const int glyphHeight = face->size->metrics.height >> 6;      // max glyph height
            const int atlasWidth = glyphWidth * 16;  // Assuming 16x8 grid for 128 characters
            const int atlasHeight = glyphHeight * 8;

            unsigned int atlasTexture;
            glGenTextures(1, &atlasTexture);
            glBindTexture(GL_TEXTURE_2D, atlasTexture);
            glTexImage2D(GL_TEXTURE_2D, 0, GL_RED, atlasWidth, atlasHeight, 0, GL_RED, GL_UNSIGNED_BYTE, nullptr);

            // Set texture parameters
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
            glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

            // Load and place each character in the atlas
            int xOffset = 0, yOffset = 0;
            assetmanager::AssetManager* assetmanager = assetmanager::AssetManager::m_funcGetInstance();
            assetmanager->m_fontManager.m_fonts[filename] = {};
      
            for (unsigned char c = 0; c < numChars; c++) {
                if (FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                    LOGGING_WARN("ERROR::FREETYPE: Failed to load Glyph for character: %c", c);
                    continue;
                }

                // Upload the glyph bitmap to the correct position in the atlas
                glTexSubImage2D(GL_TEXTURE_2D, 0, xOffset, yOffset, face->glyph->bitmap.width,
                    face->glyph->bitmap.rows, GL_RED, GL_UNSIGNED_BYTE, face->glyph->bitmap.buffer);

                if (c == 'p')
                {
                    // Store character data with atlas coordinates
                    CharacterData character = {
                        atlasTexture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x),
                        glm::vec2((float)xOffset / atlasWidth, (float)yOffset / atlasHeight),
                        glm::vec2((float)(xOffset + face->glyph->bitmap.width) / atlasWidth,
                                  (float)(yOffset + face->glyph->bitmap.rows) / atlasHeight)
                    };
                    assetmanager->m_fontManager.m_fonts[filename].insert(std::pair<char, CharacterData>(c, character));
                }
                else
                {
                    // Store character data with atlas coordinates
                    CharacterData character = {
                        atlasTexture,
                        glm::ivec2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        glm::ivec2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        static_cast<unsigned int>(face->glyph->advance.x),
                        glm::vec2((float)xOffset / atlasWidth, (float)yOffset / atlasHeight),
                        glm::vec2((float)(xOffset + face->glyph->bitmap.width) / atlasWidth,
                                  (float)(yOffset + face->glyph->bitmap.rows) / atlasHeight)
                    };
                    assetmanager->m_fontManager.m_fonts[filename].insert(std::pair<char, CharacterData>(c, character));
                }

                 

                // Move to the next position in the atlas
                xOffset += glyphWidth;
                if (xOffset + glyphWidth > atlasWidth) {
                    xOffset = 0;
                    yOffset += glyphHeight;
                }
            }
            glBindTexture(GL_TEXTURE_2D, 0);
        }
        FT_Done_Face(face);
        FT_Done_FreeType(ft);
	}

}