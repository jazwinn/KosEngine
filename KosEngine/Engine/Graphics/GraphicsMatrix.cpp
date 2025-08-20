/******************************************************************/
/*!
\file      GraphicsMatrix.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      8th Nov, 2024
\brief     This file implements the `GraphicsPipe` class functions for handling model transformations
		   and managing debug box and circle data within a graphics pipeline.

\details   The `GraphicsPipe` class includes functionality for calculating the transformation matrices
		   for models, handling texture data, and performing collision checks for debug geometries.
		   This file contains the implementation of matrix calculations that transform models from
		   local space to world space, as well as processing debug data (boxes and circles) for visual
		   debugging. The class supports managing model data and generating transformation matrices
		   for rendering models and debugging.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#include "../Config/pch.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Graphics/GraphicsCamera.h"

namespace graphicpipe
{
	void GraphicsPipe::m_funcCalculateModelToWorldMatrix()
	{
		//assetmanager::AssetManager* assets = assetmanager::AssetManager::m_funcGetInstance();
		if (m_modelData.size() > 0)
		{	
			//Generic Model Data
			for (int n{}; n < m_modelData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / m_unitWidth;
				float imageAspectRatio = static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_modelData[n].m_textureID].m_height);
				if (m_modelData[n].m_stripCount != 1)
				{
					m_modelData[n].m_transformation[0][0] = m_modelData[n].m_transformation[0][0] * widthRatio / imageAspectRatio;
					m_modelData[n].m_transformation[0][1] = m_modelData[n].m_transformation[0][1] * widthRatio / imageAspectRatio;
				}
				else
				{
					m_modelData[n].m_transformation[0][0] = m_modelData[n].m_transformation[0][0] * widthRatio;
					m_modelData[n].m_transformation[0][1] = m_modelData[n].m_transformation[0][1] * widthRatio;
				}
				m_modelData[n].m_transformation[1][1] = m_modelData[n].m_transformation[1][1] * heightRatio;
				m_modelData[n].m_transformation[1][0] = m_modelData[n].m_transformation[1][0] * heightRatio;
				m_modelMatrix.push_back(m_modelData[n].m_transformation);
				m_iVec3Array.push_back({ m_modelData[n].m_stripCount, m_modelData[n].m_frameNumber, m_modelData[n].m_textureID});
				m_layers.push_back(m_modelData[n].m_layer);
				m_colors.push_back(m_modelData[n].m_color);
			}
			m_modelData.clear();
		}

		if (m_unlitModelData.size() > 0)
		{
			//Unlit Model Data
			for (int n{}; n < m_unlitModelData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_unlitModelData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_unlitModelData[n].m_textureID].m_width) / m_unitWidth;
				float imageAspectRatio = static_cast<float>(m_imageData[m_unlitModelData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_unlitModelData[n].m_textureID].m_height);
				if (m_unlitModelData[n].m_stripCount != 1)
				{
					m_unlitModelData[n].m_transformation[0][0] = m_unlitModelData[n].m_transformation[0][0] * widthRatio / imageAspectRatio;
					m_unlitModelData[n].m_transformation[0][1] = m_unlitModelData[n].m_transformation[0][1] * widthRatio / imageAspectRatio;
				}
				else
				{
					m_unlitModelData[n].m_transformation[0][0] = m_unlitModelData[n].m_transformation[0][0] * widthRatio;
					m_unlitModelData[n].m_transformation[0][1] = m_unlitModelData[n].m_transformation[0][1] * widthRatio;
				}
				m_unlitModelData[n].m_transformation[1][1] = m_unlitModelData[n].m_transformation[1][1] * heightRatio;
				m_unlitModelData[n].m_transformation[1][0] = m_unlitModelData[n].m_transformation[1][0] * heightRatio;
				m_unlitModelMatrix.push_back(m_unlitModelData[n].m_transformation);
				m_unlitModelParams.push_back({ m_unlitModelData[n].m_stripCount, m_unlitModelData[n].m_frameNumber, m_unlitModelData[n].m_textureID });
				m_unlitLayers.push_back(m_unlitModelData[n].m_layer);
				m_unlitColors.push_back(m_unlitModelData[n].m_color);
			}
			m_unlitModelData.clear();
		}

		//Debug Model Data
		if (m_debugBoxData.size() > 0)
		{
			for (int i{}; i < m_debugBoxData.size(); i++)
			{
				if (m_debugBoxData[i].m_shapeType == SQUARE_LINES)
				{
					m_debugBoxToNDCMatrix.push_back(m_debugBoxData[i].m_transformation);
					m_debugBoxCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));
				}
				else
				{
					m_debugCircleToNDCMatrix.push_back(m_debugBoxData[i].m_transformation);
					m_debugCircleCollisionChecks.push_back(static_cast<float>(m_debugBoxData[i].m_isCollided));
				}
			}
			m_debugBoxData.clear();
		}

		//Tilemap Data
		if (m_tilemapData.size() > 0)
		{
			for (int n{}; n < m_tilemapData.size(); n++)
			{
				float heightRatio = static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_height) / m_unitHeight;
				float widthRatio = static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_width) / m_unitWidth;
				float imageAspectRatio = static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_width) / static_cast<float>(m_imageData[m_tilemapData[n].m_textureID].m_height);
				m_tilemapData[n].m_transformation[0][0] = m_tilemapData[n].m_transformation[0][0] * widthRatio / imageAspectRatio;
				m_tilemapData[n].m_transformation[0][1] = m_tilemapData[n].m_transformation[0][1] * widthRatio / imageAspectRatio;
				m_tilemapData[n].m_transformation[1][1] = m_tilemapData[n].m_transformation[1][1] * heightRatio;
				m_tilemapData[n].m_transformation[1][0] = m_tilemapData[n].m_transformation[1][0] * heightRatio;
				m_tileIndexes.push_back({});
				for (int i = 0; i < m_tilemapIndexArrays[n].size(); ++i)
				{
					
					m_tileIndexes[n].insert(m_tileIndexes[n].end(),
										    std::make_move_iterator(m_tilemapIndexArrays[n][i].begin()),
										    std::make_move_iterator(m_tilemapIndexArrays[n][i].end())
										    );
					
				}
			}
			m_transformedTilemaps = std::move(m_tilemapData);
			m_tilemapData.clear();
		}

		//Collider Grid Data
		if (m_colliderGridData.size() > 0)
		{
			for (int n{}; n < m_colliderGridData.size(); n++)
			{
				m_gridColliderChecks.push_back({});
				
				for (int i = 0; i < m_gridColliderArrays[n].size(); ++i)
				{
					m_gridColliderChecks[n].insert(m_gridColliderChecks[n].end(),
						std::make_move_iterator(m_gridColliderArrays[n][i].begin()),
						std::make_move_iterator(m_gridColliderArrays[n][i].end())
					);
				}
			}
		}

		//Lighting Data
		if (m_multiLightingData.size() > 0)
		{
			for (int n{}; n < m_multiLightingData.size(); n++)
			{
				m_multiLightingTransforms.push_back(m_multiLightingData[n].m_transformation);
				m_multiLightingColors.push_back(m_multiLightingData[n].m_color);
				m_multiLightingParams.push_back({ m_multiLightingData[n].m_intensity,m_multiLightingData[n].m_innerOuterRadius.x,m_multiLightingData[n].m_innerOuterRadius.y });
			}
			m_multiLightingData.clear();
		}

		if (m_additiveLightingData.size() > 0)
		{
			for (int n{}; n < m_additiveLightingData.size(); n++)
			{
				m_additiveLightingTransforms.push_back(m_additiveLightingData[n].m_transformation);
				m_additiveLightingColors.push_back(m_additiveLightingData[n].m_color);
				m_additiveLightingParams.push_back({ m_additiveLightingData[n].m_intensity,m_additiveLightingData[n].m_innerOuterRadius.x,m_additiveLightingData[n].m_innerOuterRadius.y });
			}
			m_additiveLightingData.clear();
		}
		

	}
}