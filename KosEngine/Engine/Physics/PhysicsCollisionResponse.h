/******************************************************************/
/*!
\file      PhysicsCollisionResponse.h
\author    Elijah Teo(teo.e, 2301530)[50%], Rayner Tan(raynerweichen.tan, 2301449)[50%]
\date      Jan 14, 2024
\brief     This file contains the declarations of the functions used to find contact points between two collided entities
		   and the functions used to find the directional collision flags



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "Physics.h"
#include "../ECS/ECS.h"

namespace physicspipe {
	//This is an enum that specifies the current state of the collision of an entity
	enum CollisionState {
		ENTERED,
		CONTINUOUS,
		EXIT
	};

	//This is a class used to store data in the collider component
	class CollisionResponseData {
	public:
		std::pair<vector2::Vec2, ecs::EntityID> m_contactPointEnt{};
		int m_numOfContacts;
		CollisionResponseData(vector2::Vec2 inVec, ecs::EntityID inID, int inNum) {
			m_contactPointEnt.first = inVec;
			m_contactPointEnt.second = inID;
			m_numOfContacts = inNum;
		}
	};

	/******************************************************************/
		/*!
		\fn        void m_FindContactPoints
		\brief     Finds the contact points between all colided entities
		*/
	/******************************************************************/
	void m_FindContactPoints();

	/******************************************************************/
	/*!
	\fn        void m_FindCollisionFlags
	\brief     Finds the directional collision flags between all colided entities
	*/
	/******************************************************************/
	void m_FindCollisionFlags();


		/******************************************************************/
		/*!
		\fn        void m_FindContactPoints
		\brief     Finds the contact points between all colided entities
		\param[in] contactPoints The contact points for the entity A
		\param[in] entA The entity ID for the first collided entity
		\param[in] entB The entity ID for the second collided entity
		\param[in] centerA The position of the first collided entity
		\param[in] radA The radius of the first collided entity
		\param[in] centerB The position of the second collided entity
		\param[in] radB The radius of the second collided entity
		\param[in] dirVecA The direction vector of the first collided entity
		\param[in] dirVecB The direction vector of the second collided entity
		\return    returns the bitset that represents the directional flags for both entities
		*/
		/******************************************************************/
	std::pair<std::bitset<4>, std::bitset<4>> m_FindCircleCircleFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints,[[maybe_unused]] ecs::EntityID entA, [[maybe_unused]] ecs::EntityID entB, const vector2::Vec2& centerA, [[maybe_unused]] const float& radA, const vector2::Vec2& centerB, [[maybe_unused]] const float& radB, const vector2::Vec2& dirVecA, const vector2::Vec2& dirVecB);


	/******************************************************************/
	/*!
	\fn        std::pair<std::bitset<4>, std::bitset<4>> m_FindCircleSquareFlags
	\brief     Determines the directional collision flags when a circle collides with a square.
	\param[in] contactPoints The contact points between the circle and the square.
	\param[in] entA The entity ID of the circle.
	\param[in] entB The entity ID of the square.
	\param[in] centerA The position of the circle.
	\param[in] radA The radius of the circle.
	\param[in] dirVecA The direction vector of the circle's movement.
	\param[in] centerB The position of the square.
	\param[in] verticesB The vertices of the square.
	\param[in] edgesB The edges of the square.
	\return    Returns two bitsets representing the directional flags for both entities.
	*/
	/******************************************************************/
	std::pair<std::bitset<4>, std::bitset<4>> m_FindCircleSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, [[maybe_unused]] ecs::EntityID entA, [[maybe_unused]] ecs::EntityID entB, const vector2::Vec2& centerA, [[maybe_unused]] const float& radA, const vector2::Vec2& dirVecA,const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB);

	/******************************************************************/
	/*!
	\fn        std::pair<std::bitset<4>, std::bitset<4>> m_FindSquareSquareFlags
	\brief     Determines the directional collision flags when two squares collide.
	\param[in] contactPoints The contact points between the two squares.
	\param[in] entA The entity ID of the first square.
	\param[in] entB The entity ID of the second square.
	\param[in] centerA The position of the first square.
	\param[in] verticesA The vertices of the first square.
	\param[in] edgesA The edges of the first square.
	\param[in] centerB The position of the second square.
	\param[in] verticesB The vertices of the second square.
	\param[in] edgesB The edges of the second square.
	\return    Returns two bitsets representing the directional flags for both squares.
	*/
	/******************************************************************/
	std::pair<std::bitset<4>, std::bitset<4>> m_FindSquareSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, [[maybe_unused]] ecs::EntityID entA, [[maybe_unused]] ecs::EntityID entB,const vector2::Vec2& centerA, const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& edgesA, const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB);

	/******************************************************************/
	/*!
	\fn        std::pair<vector2::Vec2, float> m_PointSegDist
	\brief     Computes the shortest distance from a circle's center to a line segment.
	\param[in] circlePos The position of the circle.
	\param[in] startingPoint The start of the line segment.
	\param[in] endPoint The end of the line segment.
	\return    Returns a pair containing the closest point on the segment and the distance.
	*/
	/******************************************************************/
	std::pair<vector2::Vec2, float> m_PointSegDist(const vector2::Vec2& circlePos, const vector2::Vec2& startingPoint, const vector2::Vec2& endPoint);

	/******************************************************************/
	/*!
	\fn        vector2::Vec2 m_FindCircleCirleContact
	\brief     Computes the contact point between two colliding circles.
	\param[in] cirleAPos The position of the first circle.
	\param[in] cirlceARadius The radius of the first circle.
	\param[in] circleBPos The position of the second circle.
	\return    Returns the contact point between the two circles.
	*/
	/******************************************************************/
	vector2::Vec2 m_FindCircleCirleContact(const vector2::Vec2& cirleAPos, const float& cirlceARadius, const vector2::Vec2& circleBPos);

	/******************************************************************/
	/*!
	\fn        vector2::Vec2 m_FindCircleSquareContact
	\brief     Computes the contact point between a circle and a square.
	\param[in] circlePos The position of the circle.
	\param[in] vertices The vertices of the square.
	\return    Returns the closest contact point between the circle and the square.
	*/
	/******************************************************************/
	vector2::Vec2 m_FindCircleSquareContact(const vector2::Vec2& circlePos, const std::vector<vector2::Vec2>& vertices);

	/******************************************************************/
	/*!
	\fn        std::tuple<int, vector2::Vec2, vector2::Vec2> m_FindSquareSquareContact
	\brief     Finds the contact points between two colliding squares.
	\param[in] verticesA The vertices of the first square.
	\param[in] verticesB The vertices of the second square.
	\return    Returns a tuple containing the number of contact points and the contact point itself.
	*/
	/******************************************************************/
	std::tuple<int, vector2::Vec2, vector2::Vec2> m_FindSquareSquareContact(const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& verticesB);

}