/******************************************************************/
/*!
\file      Image.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      2nd Oct, 2024
\brief     This file provides declarations for functions for loading, padding, and serializing images,
		   specifically handling sprite sheets, padding non-square textures, and working with
		   RapidJSON to save and load image metadata.


Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef IMAGE_H
#define IMAGE_H

#include "../Config/pch.h"


namespace image {


	/**
	 * @class Image
	 * @brief Represents an image loaded into the engine.
	 *
	 * The Image class is responsible for loading image files, extracting metadata such as
	 * sprite names and strip counts from filenames, handling padding for non-square textures,
	 * and creating OpenGL textures. It also provides functionality for serialization and
	 * deserialization of image data to and from JSON files. This class facilitates the
	 * management of image assets within the engine.
	 */

	class Image {
	public:
		std::string m_spriteName{}; ///< Name of the sprite associated with this image.
		int m_width{};               ///< Width of the image in pixels.
		int m_height{};              ///< Height of the image in pixels.
		int m_channels{};            ///< Number of color channels in the image.
		int m_stripCount{};          ///< Number of strips in the image (for sprite sheets).
		bool m_isPadded = false;     ///< Indicates whether the image has been padded to be square.
		bool m_isTilable = false;
		int m_imageID{};             ///< Unique identif
		unsigned int textureID{};
	};






    class ImageManager {
		
    public:
		~ImageManager();
		/**
		 * @brief Loads an image file, handles padding if necessary, and creates an OpenGL texture.
		 *
		 * @param file Path to the image file.
		 * @return unsigned int OpenGL texture ID.
		 *
		 * This function loads an image, checks if padding is required for non-square textures,
		 * and creates an OpenGL texture using the loaded image data. Images are stored in
		 * the AssetManager's containers for later use.
		 */
        unsigned int m_LoadImage(const char* file);
		/**
		 * @brief Extracts the strip count from the filename using regex.
		 *
		 * @param filename Name of the image file.
		 * @return int Strip count extracted from the filename, defaults to 1 if not found.
		 */
		int m_extractStripCountFromFilename(const std::string& filename);
		/**
		 * @brief Extracts the sprite name from the filename using regex.
		 *
		 * @param filename Name of the image file.
		 * @return std::string Extracted sprite name or error message.
		 */
		std::string m_extractSpriteNameFromFilename(const std::string& filename);
		
		
		bool m_checkIfImageTilable(const std::string& filename);
		/**
		 * @brief Pads non-square textures to make them square.
		 *
		 * @param originalPixels Original pixel data.
		 * @param originalWidth Original image width.
		 * @param originalHeight Original image height.
		 * @param originalChannels Number of original color channels.
		 * @param targetWidth Width of the padded texture.
		 * @param targetHeight Height of the padded texture.
		 * @param targetChannels Number of target color channels.
		 * @return unsigned char* Padded texture data.
		 */
		unsigned char* m_funcPadTexture(const unsigned char* originalPixels, int originalWidth, int originalHeight, int originalChannels, int targetWidth, int targetHeight, int targetChannels);
		/**
		 * @brief Serializes the image data to a JSON file.
		 *
		 * @param filename Output filename for the JSON file.
		 */



		//Image channels required for our engine
		int m_targetChannels{ 4 }; ///< Number of color channels for the images (e.g., 3 for RGB, 4 for RGBA).
		int m_imageCount{0};      ///< Total number of images loaded into the engine.

	public:
		
		std::unordered_map<std::string, Image> m_imageMap;
		std::vector<unsigned char*> m_imagedataArray;
    };
}


#endif IMAGE_H
