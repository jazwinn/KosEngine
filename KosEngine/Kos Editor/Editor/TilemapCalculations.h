/******************************************************************/
/*!
\file      TilemapCalculations.h
\author    Sean Tiu
\par       s.tiu@digipen.edu
\date      29th Nov, 2024
\brief     Header file for tilemap calculations and utilities, providing
		   functions for resizing tiles, setting individual tiles, and
		   debugging tile indices as well as similar purposes for grids
		   components.

\details   This file includes:
		   - Functionality to dynamically resize tilemaps and grids.
		   - Tools to modify and debug specific tiles within the map.
		   - Integration with ECS components for tilemap management.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/

#ifndef TILEMAPCALC_H
#define TILEMAPCALC_H


#include "Graphics/GraphicsPipe.h"
#include "ECS/ECS.h"
#include "Math/vector2.h"

namespace Tilemap

{
	/******************************************************************/
	/*!
	\fn      void Tilemap::resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength)
	\brief   Resizes the tiles in the tilemap to the specified row and column lengths.
	\param   tilemap - Pointer to the tilemap component to be resized.
	\param   rowLength - The new number of rows in the tilemap.
	\param   colLength - The new number of columns in the tilemap.
	\details Adjusts the size of the tilemap grid by modifying its rows and columns.
	*/
	/******************************************************************/
	void resizeTiles(ecs::TilemapComponent* tilemap, int rowLength, int colLength);


	/******************************************************************/
	/*!
	\fn      void Tilemap::resizeCollidableGrid(ecs::GridComponent* grid, int rowLength, int colLength)
	\brief   Resizes the collidable grid to the specified row and column lengths.
	\param   grid - Pointer to the grid component to be resized.
	\param   rowLength - The new number of rows in the grid.
	\param   colLength - The new number of columns in the grid.
	\details Adjusts the size of the collidable grid by modifying its rows and columns.
	*/
	/******************************************************************/
	void resizeCollidableGrid(ecs::GridComponent* grid, int rowLength, int colLength);

	/******************************************************************/
	/*!
	\fn      void Tilemap::setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap)
	\brief   Sets an individual tile in the tilemap based on given coordinates.
	\param   originCoordinates - Reference to the origin coordinates of the tile to be set.
	\param   tileCoordinates - The coordinates of the tile within the tilemap.
	\param   tilemap - Pointer to the tilemap component.
	\details Updates the tilemap with a specific tile based on the provided coordinates.
	*/
	/******************************************************************/
	void setIndividualTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::TilemapComponent* tilemap);


	/******************************************************************/
	/*!
	\fn      void Tilemap::setCollidableTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::GridComponent* grid)
	\brief   Sets a collidable tile in the grid based on given coordinates.
	\param   originCoordinates - Reference to the origin coordinates of the tile to be set.
	\param   tileCoordinates - The coordinates of the tile within the grid.
	\param   grid - Pointer to the grid component.
	\details Updates the grid with a specific collidable tile based on the provided coordinates.
			 This function modifies the grid at the specified tile's coordinates.
	*/
	/******************************************************************/
	void setCollidableTile(vector2::Vec2& originCoordinates, const vector2::Vec2& tileCoordinates, ecs::GridComponent* grid);

	/******************************************************************/
	/*!
	\fn      void Tilemap::debugTileIndex(ecs::TilemapComponent* tilemap)
	\brief   Outputs debugging information for a specific tile in the tilemap.
	\param   tilemap - Pointer to the tilemap component to debug.
	\details Prints the tilemap index and relevant data for debugging purposes.
	*/
	/******************************************************************/
	void debugTileIndex(ecs::TilemapComponent* tilemap);

	/******************************************************************/
	/*!
	\fn      void Tilemap::debugGridChecks(ecs::GridComponent* grid)
	\brief   Outputs debugging information for grid collision checks.
	\param   grid - Pointer to the grid component to debug.
	\details Prints information about the grid's collision checks, including any
			 relevant data or states that can help in debugging the grid's behavior.
	*/
	/******************************************************************/
	void debugGridChecks(ecs::GridComponent* grid);

}


#endif TILEMAPCALC_H
