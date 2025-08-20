/******************************************************************/
/*!
\file      TilemapCalculations.cpp
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      29th Nov, 2024
\brief     Implementation of tilemap calculations and utilities,
		   providing functions for resizing tiles, setting individual
		   tiles, and debugging tile indices.

\details   This file includes:
		   - Logic to dynamically resize tilemaps based on input dimensions.
		   - Functions to update specific tiles and adjust tilemap bounds.
		   - Debugging utilities to output tilemap indices for troubleshooting.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/


#include "TilemapCalculations.h"
#include <algorithm>

namespace Tilemap
{
	void resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength)
	{
		if (rowLength <= 0)
		{
			tilemap->m_rowLength = 1;
			return;
		}
		if (colLength <= 0)
		{
			tilemap->m_columnLength = 1;
			return;
		}
		tilemap->m_rowLength = rowLength;
		tilemap->m_columnLength = colLength;
		if (tilemap->m_tilePictureIndex.size() != tilemap->m_columnLength)
		{
			tilemap->m_tilePictureIndex.resize(colLength);
		}
		for (std::vector<int>& row : tilemap->m_tilePictureIndex)
		{
			row.resize(rowLength);
		}
		
	}

	void resizeCollidableGrid(ecs::GridComponent* grid, int rowLength, int colLength) /// For Grid
	{
		if (rowLength <= 0)
		{
			grid->m_GridRowLength = 1;
			return;
		}
		if (colLength <= 0)
		{
			grid->m_GridColumnLength = 1;
			return;
		}
		grid->m_GridRowLength = rowLength;
		grid->m_GridColumnLength = colLength;
		
		if (grid->m_IsWall.size() != grid->m_GridColumnLength)
		{
			grid->m_IsWall.resize(colLength);
		}
		

		for (std::vector<int>& row : grid->m_IsWall) 
		{
			row.resize(rowLength);
		}
	}
	void setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap)
	{
		if ((tileCoordinates.m_x) > originCoordinates.m_x + tilemap->m_rowLength) // If selected tile is longer than current tileset
		{
			LOGGING_INFO("Extending row length by %d", static_cast<int>(ceil(tileCoordinates.m_x - originCoordinates.m_x)));
			resizeTiles(tilemap, static_cast<int>(ceil(tileCoordinates.m_x - originCoordinates.m_x)), tilemap->m_columnLength);
		}
		if (tileCoordinates.m_x < floor(originCoordinates.m_x))
		{
			resizeTiles(tilemap, tilemap->m_rowLength + static_cast<int>(ceil(originCoordinates.m_x - tileCoordinates.m_x)), tilemap->m_columnLength); // Change Row Length

			for (std::vector<int>& row : tilemap->m_tilePictureIndex)
			{
				std::rotate(row.rbegin(),
					row.rbegin() + static_cast<int>(ceil(originCoordinates.m_x - tileCoordinates.m_x)),
					row.rend()); // Rotate Indexes
			}
			
			originCoordinates.m_x = floor(tileCoordinates.m_x); //Set to center of tile
		}
		if (tileCoordinates.m_y < (originCoordinates.m_y + 1 - tilemap->m_columnLength)) // If selected tile is longer than current tileset
		{
			resizeTiles(tilemap, tilemap->m_rowLength , static_cast<int>(ceil(originCoordinates.m_y + 1 - tileCoordinates.m_y)));
		}
		if (tileCoordinates.m_y > floor(originCoordinates.m_y + 1))
		{
			resizeTiles(tilemap, tilemap->m_rowLength, tilemap->m_columnLength + static_cast<int>(ceil(tileCoordinates.m_y - (originCoordinates.m_y + 1)))); //Change Column Length
			std::rotate(tilemap->m_tilePictureIndex.rbegin(),
						tilemap->m_tilePictureIndex.rbegin() + static_cast<int>(ceil(tileCoordinates.m_y - (originCoordinates.m_y + 1))),
						tilemap->m_tilePictureIndex.rend()); // Rotate Indexes
			originCoordinates.m_y = floor(tileCoordinates.m_y); //Set to center of tile
		}

		int indexX = static_cast<int>(floor(tileCoordinates.m_x - originCoordinates.m_x));
		int indexY = static_cast<int>(floor(originCoordinates.m_y - tileCoordinates.m_y) + 1.f);

		if (indexX >= tilemap->m_tilePictureIndex[0].size() || indexY >= tilemap->m_tilePictureIndex.size())
		{
			LOGGING_WARN("Clicked out of range: indexX=%d, indexY=%d", indexX, indexY);
			return;
		}

		tilemap->m_tilePictureIndex[indexY][indexX] = tilemap->m_tileIndex;

	}

	void setCollidableTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::GridComponent* grid)
	{
		if ((tileCoordinates.m_x) > originCoordinates.m_x + grid->m_GridRowLength) // If selected tile is longer than current tileset
		{
			LOGGING_INFO("Extending row length by %d", static_cast<int>(ceil(tileCoordinates.m_x - originCoordinates.m_x)));
			resizeCollidableGrid(grid, static_cast<int>(ceil(tileCoordinates.m_x - originCoordinates.m_x)), grid->m_GridColumnLength);
		}
		if (tileCoordinates.m_x < floor(originCoordinates.m_x))
		{
			resizeCollidableGrid(grid, grid->m_GridRowLength + static_cast<int>(ceil(originCoordinates.m_x - tileCoordinates.m_x)), grid->m_GridColumnLength); // Change Row Length

			for (std::vector<int>& row : grid->m_IsWall) ///Grid
			{
				std::rotate(row.rbegin(),
					row.rbegin() + static_cast<int>(ceil(originCoordinates.m_x - tileCoordinates.m_x)),
					row.rend()); // Rotate Indexes
			}

			originCoordinates.m_x = floor(tileCoordinates.m_x); //Set to center of tile
		}
		if (tileCoordinates.m_y < (originCoordinates.m_y + 1 - grid->m_GridColumnLength)) // If selected tile is longer than current tileset
		{
			resizeCollidableGrid(grid, grid->m_GridRowLength, static_cast<int>(ceil(originCoordinates.m_y + 1 - tileCoordinates.m_y)));
		}
		if (tileCoordinates.m_y > floor(originCoordinates.m_y + 1))
		{
			resizeCollidableGrid(grid, grid->m_GridRowLength, grid->m_GridColumnLength + static_cast<int>(ceil(tileCoordinates.m_y - (originCoordinates.m_y + 1)))); //Change Column Length
			std::rotate(grid->m_IsWall.rbegin(),
				grid->m_IsWall.rbegin() + static_cast<int>(ceil(tileCoordinates.m_y - (originCoordinates.m_y + 1))), ///Grid
				grid->m_IsWall.rend()); // Rotate Indexes
			originCoordinates.m_y = floor(tileCoordinates.m_y); //Set to center of tile
		}

		int indexX = static_cast<int>(floor(tileCoordinates.m_x - originCoordinates.m_x));
		int indexY = static_cast<int>(floor(originCoordinates.m_y - tileCoordinates.m_y) + 1.f);

		if (indexX >= grid->m_IsWall[0].size() || indexY >= grid->m_IsWall.size())
		{
			LOGGING_WARN("Clicked out of range: indexX=%d, indexY=%d", indexX, indexY);
			return;
		}

		grid->m_IsWall[indexY][indexX] = (int)grid->m_SetCollidable;

	}
	void debugTileIndex(ecs::TilemapComponent* tilemap)
	{
		for (int i = 0; i < tilemap->m_tilePictureIndex.size(); ++i)
		{
			std::ostringstream rowStream;

			rowStream << "Row " << i << ": ";
			for (int j = 0; j < tilemap->m_tilePictureIndex[i].size(); ++j)
			{
				rowStream << tilemap->m_tilePictureIndex[i][j] << ' ';
			}
			LOGGING_INFO("{}", rowStream.str().c_str());
		}
	}

	void debugGridChecks(ecs::GridComponent* grid)
	{
		for (int i = 0; i < grid->m_IsWall.size(); ++i)
		{
			std::ostringstream rowStream;

			rowStream << "Row " << i << ": ";
			for (int j = 0; j < grid->m_IsWall[i].size(); ++j)
			{
				rowStream << grid->m_IsWall[i][j] << ' ';
			}
			LOGGING_INFO("{}", rowStream.str().c_str());
		}
	}
}

