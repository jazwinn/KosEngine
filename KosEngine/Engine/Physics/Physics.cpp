/******************************************************************/
/*!
\file      Physics.cpp
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Sept 28, 2024
\brief     This file implements the physics pipeline, including
		   entity creation, bounding box calculations, and
		   collision detection for various entity types such as
		   circles and rectangles.

The file provides functionality to send, retrieve, and check
physics data for entities, using collision algorithms for
circle-circle, circle-rectangle, and rectangle-rectangle interactions.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#include "../Config/pch.h"
#include "Physics.h"
namespace physicspipe {

	std::vector<std::shared_ptr<PhysicsData>> Physics::m_physicsEntities;
	std::vector<std::shared_ptr<PhysicsData>> Physics::m_collidedEntities;
	std::map<layer::LAYERS, std::vector<std::shared_ptr<PhysicsData>>> Physics::m_layerToEntities;
	std::vector <std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> Physics::m_collidedEntitiesPair;
	std::vector < std::pair < std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float >>> Physics::m_collidedEntitiesPairWithVector;
	std::unique_ptr<Physics> Physics::m_instance = nullptr;
	physicslayer::PhysicsLayer* physicsLayer = physicslayer::PhysicsLayer::m_GetInstance(); // Get the PhysicsLayer instance
	std::vector<int> Physics::m_checker{};

	Circle::Circle(float radius, vector2::Vec2 shape_position, vector2::Vec2 prevposition, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID, int layer_ID)
		: m_radius(radius)   // Initialize radius
	{
		m_position = shape_position;
		m_prevPosition = prevposition;
		m_scale = shape_scale;
		m_velocity = shape_velocity;
		m_ID = entity_ID;
		type = EntityType::CIRCLE;  // Set type to Circle
		m_layerID = layer_ID;
	}


	Rectangle::Rectangle(float rect_height, float rect_width, float rect_angle, vector2::Vec2 shape_position, vector2::Vec2 prevposition, vector2::Vec2 shape_scale, vector2::Vec2 shape_velocity, int entity_ID, int layer_ID)
		: m_height(rect_height), m_width(rect_width)  // Initialize height and width
	{
		m_position = shape_position;
		m_prevPosition = prevposition;
		m_scale = shape_scale;
		m_scale = shape_scale;
		m_velocity = shape_velocity;
		m_ID = entity_ID;
		m_rotAngle = rect_angle;
		type = EntityType::RECTANGLE;  // Set type to Rectangle
		m_layerID = layer_ID;
	}

	bool Physics::m_Static_CollisionCheck(const AABB aabb1, const AABB aabb2) {
		//std::cout << "BOUNDING BOX1 MIN X " << aabb1.m_min.m_x << " Y " << aabb1.m_min.m_y << std::endl;
		//std::cout << "BOUNDING BOX1 MAX X " << aabb1.m_max.m_x << " Y " << aabb1.m_max.m_y << std::endl;
		//std::cout << "BOUNDING BOX2 MIN X " << aabb2.m_min.m_x << " Y " << aabb2.m_min.m_y << std::endl;
		//std::cout << "BOUNDING BOX2 MAX X " << aabb2.m_max.m_x << " Y " << aabb2.m_max.m_y << std::endl;

		if (aabb2.m_max.m_x < aabb1.m_min.m_x) return 0;
		if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
		if (aabb2.m_max.m_y < aabb1.m_min.m_y) return 0;
		if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;
		//std::cout << "Static Collision fail " << std::endl;
		return 1;
	}



	void Physics::m_SendPhysicsData(float rect_height, float rect_width, float rect_angle, vector2::Vec2 position, vector2::Vec2 prevposition, vector2::Vec2 scale, vector2::Vec2 velocity, int ID, layer::LAYERS layerID) {
		m_physicsEntities.push_back(std::make_shared<Rectangle>(rect_height, rect_width, rect_angle, position, prevposition, scale, velocity, ID, static_cast<int>(layerID)));
		//	m_layerToEntities[layerID].push_back(std::make_shared<Rectangle>(rect_height, rect_width, rect_angle, position, scale, velocity, ID));
	}

	void Physics::m_SendPhysicsData(float radius, vector2::Vec2 position, vector2::Vec2 prevposition, vector2::Vec2 scale, vector2::Vec2 velocity, int ID, layer::LAYERS layerID) {
		m_physicsEntities.push_back(std::make_shared<Circle>(radius, position, prevposition, scale, velocity, ID, static_cast<int>(layerID)));
		//	m_layerToEntities[layerID].push_back(std::make_shared<Circle>(radius, position, scale, velocity, ID));
	}

	void Physics::m_CollisionCheck(float dt) {

		//calculate boarding box
		m_CalculateBoundingBox();

		for (size_t i = 0; i < m_physicsEntities.size(); ++i) {
			for (size_t j = 0; j < m_physicsEntities.size(); ++j) {
				if (m_physicsEntities[i]->m_ID != m_physicsEntities[j]->m_ID) {
					/*************************************
						CHECK RECT V RECT
					*************************************/
					if ((m_physicsEntities[i]->m_GetEntity() == EntityType::RECTANGLE) && (m_physicsEntities[j]->m_GetEntity() == EntityType::RECTANGLE)) {
						if (m_CollisionIntersection_RectRect_AABB(*dynamic_cast<Rectangle*>(m_physicsEntities[i].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[j].get()), dt)) {
							//checking whether if entity is alr added inside
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED"); // Dont LOG uses up too much process
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK CIRCLE V RECT
					*************************************/
					else if ((m_physicsEntities[i]->m_GetEntity() == EntityType::CIRCLE) && (m_physicsEntities[j]->m_GetEntity() == EntityType::RECTANGLE)) {
						if (m_CollisionIntersection_CircleRect_AABB(*dynamic_cast<Circle*>(m_physicsEntities[i].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[j].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED");// Dont LOG uses up too much process
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}

					/*************************************
						CHECK RECT V CIRCLE
					*************************************/
					else if (m_physicsEntities[j]->m_GetEntity() == EntityType::CIRCLE && m_physicsEntities[i]->m_GetEntity() == EntityType::RECTANGLE) {
						if (m_CollisionIntersection_CircleRect_AABB(*dynamic_cast<Circle*>(m_physicsEntities[j].get()), *dynamic_cast<Rectangle*>(m_physicsEntities[i].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED"); // Dont LOG uses up too much process
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					/*************************************
						CHECK CIRCLE V CIRCLE
					*************************************/
					else if (m_physicsEntities[i]->m_GetEntity() == EntityType::CIRCLE && m_physicsEntities[j]->m_GetEntity() == EntityType::CIRCLE) {
						if (m_CollisionIntersection_CircleCircle(*dynamic_cast<Circle*>(m_physicsEntities[i].get()), *dynamic_cast<Circle*>(m_physicsEntities[j].get()))) {
							if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), m_physicsEntities[i]) == m_collidedEntities.end()) {
								//std::cout << "Collided" << std::endl;
								//LOGGING_DEBUG("COLLIDED");// Dont LOG uses up too much process
								m_collidedEntities.push_back(m_physicsEntities[i]);
							}
						}
					}
					else {
						LOGGING_ERROR("NO CASE FOR CHECKING");
					}
				}
				//std::cout << "********************************************************************************" << std::endl;
			}
		}
		//std::cout << "Physics Entities size " << m_physicsEntities.size() << " collideEntities " << m_collidedEntities.size() << std::endl;
	}

	std::vector<std::shared_ptr<PhysicsData>> Physics::m_RetrievePhysicsData() {
		std::vector<std::shared_ptr	<PhysicsData>> TempCollidedEntities = m_collidedEntities;

		//this->m_ClearEntites();
		return TempCollidedEntities;
	}

	void Physics::m_ClearEntites() {
		m_physicsEntities.clear();
		m_collidedEntities.clear();
		m_layerToEntities.clear();
		m_checker.clear();
	}


	void Physics::m_CalculateBoundingBox() {
		//std::cout << "********************************************************************************" << std::endl;
		//std::cout << "UPDATING BOUNDING BOX" << std::endl;
		for (size_t i = 0; i < m_physicsEntities.size(); ++i) {
			if (m_physicsEntities[i]->m_GetEntity() == EntityType::RECTANGLE) {
				AABB boundingBox;
				float width = dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_width;
				float height = dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_height;
				boundingBox.m_min = { m_physicsEntities[i]->m_position.m_x - (width * 0.5f), m_physicsEntities[i]->m_position.m_y - (height * 0.5f) };
				boundingBox.m_max = { m_physicsEntities[i]->m_position.m_x + (width * 0.5f), m_physicsEntities[i]->m_position.m_y + (height * 0.5f) };
				dynamic_cast<Rectangle*>(m_physicsEntities[i].get())->m_boundingBox = boundingBox;
				//std::cout << "ID " << m_physicsEntities[i].get()->m_ID << " BOUNDING BOX MIN X" << boundingBox.m_min.m_x << " MIN Y " << boundingBox.m_max.m_y << " MAX X " << boundingBox.m_max.m_x << " MAX Y" << boundingBox.m_max.m_y << std::endl;
			}
		}

		//std::cout << "********************************************************************************" << std::endl;
	}

	bool Physics::m_CollisionIntersection_RectRect_AABB(const Rectangle& obj1, const Rectangle& obj2, float dt) {
		//static collision
		AABB aabb1 = obj1.m_boundingBox;
		AABB aabb2 = obj2.m_boundingBox;
		vector2::Vec2 obj1_Velocity = obj1.m_velocity;
		vector2::Vec2 obj2_Velocity = obj2.m_velocity;
		vector2::Vec2 rel_Velocity{};
		float Tfirst{}, Tlast = dt;
		rel_Velocity = obj1_Velocity - obj2_Velocity;


		/*std::cout << "CHECKING" << std::endl;
		std::cout << "ID " << obj1.ID << " ID " << obj2.ID << std::endl;
		std::cout << "********************************************************************************" << std::endl;
		std::cout << "obj1_Velocity x " << obj1_Velocity.x << " y " << obj1_Velocity.y << std::endl;
		std::cout << "obj2_Velocity x " << obj2_Velocity.x << " y " << obj2_Velocity.y << std::endl;
		std::cout << "rel_Velocity x " << rel_Velocity.x << " y " << rel_Velocity.y << std::endl;
		std::cout << "********************************************************************************" << std::endl;
		std::cout << "AABB1 min x " << aabb1.min.x << " y " << aabb1.min.y << std::endl;
		std::cout << "AABB1 max x " << aabb1.max.x << " y " << aabb1.max.x << std::endl;
		std::cout << "AABB2 min x " << aabb2.min.x << " y " << aabb2.min.y << std::endl;
		std::cout << "AABB2 max x " << aabb2.max.x << " y " << aabb2.max.x << std::endl;
		std::cout << "********************************************************************************" << std::endl;*/


		//static collision
		if (m_Static_CollisionCheck(aabb1, aabb2)) return true;
		//dynamic collision
		if (rel_Velocity.m_x < 0) {
			if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
			if (aabb2.m_max.m_x < aabb1.m_min.m_x) {
				Tfirst = std::max((aabb2.m_max.m_x - aabb1.m_min.m_x) / rel_Velocity.m_x, Tfirst);
			}
			if (aabb2.m_min.m_x < aabb1.m_max.m_x) {// case 4
				Tlast = std::min((aabb2.m_min.m_x - aabb1.m_max.m_x) / rel_Velocity.m_x, Tlast);
			}
		}
		else if (rel_Velocity.m_x > 0) {
			if (aabb2.m_min.m_x > aabb1.m_max.m_x) {// case 2
				Tfirst = std::max((aabb2.m_min.m_x - aabb1.m_max.m_x) / rel_Velocity.m_x, Tfirst);
			}
			if (aabb2.m_max.m_x > aabb1.m_min.m_x) {// case 2
				Tlast = std::min((aabb2.m_max.m_x - aabb1.m_min.m_x) / rel_Velocity.m_x, Tlast);
			}
			if (aabb2.m_max.m_x < aabb1.m_min.m_x)return 0;//case 3
		}
		else if (rel_Velocity.m_x == 0) {//case 5
			if (aabb2.m_max.m_x < aabb1.m_min.m_x) return 0;
			else if (aabb2.m_min.m_x > aabb1.m_max.m_x) return 0;
		}

		if (Tfirst > Tlast) return 0;


		if (rel_Velocity.m_y < 0) {
			if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;//case 1
			if (aabb2.m_max.m_y < aabb1.m_min.m_y) {// case 4
				Tfirst = std::max((aabb2.m_max.m_y - aabb1.m_min.m_y) / rel_Velocity.m_y, Tfirst);
			}
			if (aabb2.m_min.m_y < aabb1.m_max.m_y) {// case 4
				Tlast = std::min((aabb2.m_min.m_y - aabb1.m_max.m_y) / rel_Velocity.m_y, Tlast);
			}
		}
		else if (rel_Velocity.m_y > 0) {
			if (aabb2.m_min.m_y > aabb1.m_max.m_y) {// case 2
				Tfirst = std::max((aabb2.m_min.m_y - aabb1.m_max.m_y) / rel_Velocity.m_y, Tfirst);
			}
			if (aabb2.m_max.m_y > aabb1.m_min.m_y) {// case 2
				Tlast = std::min((aabb2.m_max.m_y - aabb1.m_min.m_y) / rel_Velocity.m_y, Tlast);
			}
			if (aabb2.m_max.m_y < aabb1.m_min.m_y)return 0;//case 3
		}
		else if (rel_Velocity.m_y == 0) {
			if (aabb2.m_max.m_y < aabb1.m_min.m_y) return 0;
			else if (aabb2.m_min.m_y > aabb1.m_max.m_y) return 0;
		}
		if (Tfirst > Tlast) return 0;

		return true;

	}

	bool Physics::m_CollisionIntersection_CircleCircle(const Circle& circle1, const Circle& circle2) {
		float dx = circle2.m_position.m_x - circle1.m_position.m_x;
		float dy = circle2.m_position.m_y - circle1.m_position.m_y;
		float distance_Square = dx * dx + dy * dy;

		float combineRadius = circle1.m_radius + circle2.m_radius;
		vector2::Vec2 delta = circle2.m_position - circle1.m_position;

		//return distance_Square <= (combineRadius * combineRadius);

		if (distance_Square >= (combineRadius * combineRadius)) {
			return false; // no collision
		}

		vector2::Vec2 collisionNormal = delta;
		vector2::Vec2::m_funcVec2Normalize(collisionNormal, collisionNormal);
		if (std::abs(collisionNormal.m_x) > std::abs(collisionNormal.m_y)) {
			// Horizontal collision
			if (collisionNormal.m_x > 0) {
				const_cast<Circle&>(circle1).m_AddCollisionFlag(RIGHT);
				const_cast<Circle&>(circle2).m_AddCollisionFlag(LEFT);
			}
			else {
				const_cast<Circle&>(circle1).m_AddCollisionFlag(LEFT);
				const_cast<Circle&>(circle2).m_AddCollisionFlag(RIGHT);
			}
		}
		else {
			// Vertical collision
			if (collisionNormal.m_y > 0) {
				const_cast<Circle&>(circle1).m_AddCollisionFlag(DOWN);
				const_cast<Circle&>(circle2).m_AddCollisionFlag(UP);
			}
			else {
				const_cast<Circle&>(circle1).m_AddCollisionFlag(UP);
				const_cast<Circle&>(circle2).m_AddCollisionFlag(DOWN);
			}
		}

		return true;
	}

	void m_BuildLineSegment(LineSegment& lineSegment, const vector2::Vec2& p0, const vector2::Vec2& p1) {
		lineSegment.m_pt0 = p0;
		lineSegment.m_pt1 = p1;
		vector2::Vec2 dVector = p1 - p0;
		vector2::Vec2 nVector = { dVector.m_y, -dVector.m_x };
		vector2::Vec2::m_funcVec2Normalize(nVector, nVector);
		lineSegment.m_normal = nVector;
	}


	int Physics::m_FindClosestPointOnPolygon(vector2::Vec2 circle_pos, std::vector<vector2::Vec2> vertices) {
		int result = -1;
		float minDistance = std::numeric_limits<float>::max();
		for (int i = 0; i < static_cast<int>(vertices.size()); ++i) {
			vector2::Vec2 vert = vertices[i];
			float dist = vector2::Vec2::m_funcVec2DDistance(circle_pos, vert);
			if (dist < minDistance) {
				minDistance = dist;
				result = i;
			}
		}
		return result;
	}

	void Physics::m_ProjectOntoCircle(vector2::Vec2 center, float radius, vector2::Vec2 axis, float& min, float& max) {
		vector2::Vec2 dVector = axis;
		vector2::Vec2::m_funcVec2Normalize(dVector, dVector);
		vector2::Vec2 dVectorAndRadius = dVector * radius;

		vector2::Vec2 p1 = center + dVectorAndRadius;
		vector2::Vec2 p2 = center - dVectorAndRadius;

		min = vector2::Vec2::m_funcVec2DDotProduct(p1, axis);
		max = vector2::Vec2::m_funcVec2DDotProduct(p2, axis);

		if (min > max) {
			std::swap(min, max);
		}
	}
	std::pair<bool, std::pair<vector2::Vec2, float>> Physics::m_CollisionIntersection_CircleRect_SAT_TEST(const Circle& circle, const Rectangle& rect) {
		vector2::Vec2 normal{};
		float depth = FLT_MAX;
		bool flag = true;
		std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val;
		std::vector<vector2::Vec2> rect_Vertices = rect.m_GetRotatedVertices();
		std::vector<vector2::Vec2> rect_Edges = rect.m_GetEdges();
		float minA{}, maxA{}, minB{}, maxB{};
		float axisdepth;
		for (const auto& edge : rect_Edges) {
			vector2::Vec2 axis = { -edge.m_y, edge.m_x };
			vector2::Vec2::m_funcVec2Normalize(axis, axis);
			m_ProjectOntoAxis(rect_Vertices, axis, minA, maxA);
			m_ProjectOntoCircle(circle.m_position, circle.m_radius, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA)
			{
				flag = false;
				ret_Val.first = flag;
				ret_Val.second.first = {};
				ret_Val.second.second = {};
			
			}
			axisdepth = std::min(maxA, maxB) - std::max(minA, minB);
			if (axisdepth < depth) {
				depth = axisdepth;
				normal = axis;
			}
		}

		int closePointIndex = m_FindClosestPointOnPolygon(circle.m_position, rect_Vertices);
		vector2::Vec2 closePoint = rect_Vertices[closePointIndex];

		vector2::Vec2 axis = closePoint - circle.m_position;
		vector2::Vec2::m_funcVec2Normalize(axis, axis);
		m_ProjectOntoAxis(rect_Vertices, axis, minA, maxA);
		m_ProjectOntoCircle(circle.m_position, circle.m_radius, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA) {
			flag = false;
			ret_Val.first = flag;
			ret_Val.second.first = {};
			ret_Val.second.second = {};
			
		}
		axisdepth = std::min(maxB - minA, maxA - minB);
		if (axisdepth < depth) {
			depth = axisdepth;
			normal = axis;
		}

		vector2::Vec2 direction = rect.m_position - circle.m_position;
		if (vector2::Vec2::m_funcVec2DDotProduct(direction, normal) < 0.f)
		{
			normal = -normal;
		}

		ret_Val.first = flag;
		ret_Val.second.first = normal;
		ret_Val.second.second = depth;

		return ret_Val;

	}


	std::pair<bool, std::pair<vector2::Vec2, float>> Physics::m_CollisionIntersection_CircleCircle_SAT_TEST(const Circle& circleA, const Circle& circleB) {
		vector2::Vec2 normal{};
		float depth = FLT_MAX;
		bool flag = true;
		std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val;
		// Compute vector between circle centers
		vector2::Vec2 ab = circleB.m_position - circleA.m_position;

		// Compute squared distance between centers
		float distanceSquared = vector2::Vec2::m_funcVec2DDotProduct(ab, ab);

		// Compute squared sum of radii
		float combinedRadius = circleA.m_radius + circleB.m_radius;
		float combinedRadiusSquared = combinedRadius * combinedRadius;

		// Check if circles overlap
		if (distanceSquared >= combinedRadiusSquared) {
			flag = false;
			ret_Val.first = flag;
			ret_Val.second.first = {};
			ret_Val.second.second = {};
			return ret_Val;
		}

		// Compute actual distance between centers
		float distance = sqrt(distanceSquared);

		// Normal is the direction vector between centers
		normal = ab;
		//vector2::Vec2::m_funcVec2Normalize(normal, normal);

		// Compute penetration depth
		depth = combinedRadius - distance;

		// Return collision information
		ret_Val.first = flag;
		ret_Val.second.first = normal;
		ret_Val.second.second = depth;

		return ret_Val;

	}

	bool Physics::m_CollisionIntersection_CircleRect_SAT(const Circle& circle, const Rectangle& rect) {
		//need the vertices of the rectangle
		vector2::Vec2 normal{};
		float depth = FLT_MAX;
		std::vector<vector2::Vec2> rect_Vertices = rect.m_GetRotatedVertices();
		std::vector<vector2::Vec2> rect_Edges = rect.m_GetEdges();
		float minA{}, maxA{}, minB{}, maxB{};
		float axisdepth;
		for (const auto& edge : rect_Edges) {
			vector2::Vec2 axis = { -edge.m_y, edge.m_x };
			vector2::Vec2::m_funcVec2Normalize(axis, axis);
			m_ProjectOntoAxis(rect_Vertices, axis, minA, maxA);
			m_ProjectOntoCircle(circle.m_position, circle.m_radius, axis, minB, maxB);

			if (minA >= maxB || minB >= maxA)
			{
				return false;
			}
			axisdepth = std::min(maxA, maxB) - std::max(minA, minB);
			if (axisdepth < depth) {
				depth = axisdepth;
				normal = axis;
			}
		}

		int closePointIndex = m_FindClosestPointOnPolygon(circle.m_position, rect_Vertices);
		vector2::Vec2 closePoint = rect_Vertices[closePointIndex];

		vector2::Vec2 axis = closePoint - circle.m_position;
		vector2::Vec2::m_funcVec2Normalize(axis, axis);
		m_ProjectOntoAxis(rect_Vertices, axis, minA, maxA);
		m_ProjectOntoCircle(circle.m_position, circle.m_radius, axis, minB, maxB);

		if (minA >= maxB || minB >= maxA) return false;
		axisdepth = std::min(maxB - minA, maxA - minB);
		if (axisdepth < depth) {
			depth = axisdepth;
			normal = axis;
		}

		vector2::Vec2 direction = rect.m_position - circle.m_position;
		if (vector2::Vec2::m_funcVec2DDotProduct(direction, normal) < 0.f)
		{
			normal = -normal;
		}

		return true;

	}

	bool Physics::m_CollisionIntersection_CircleRect_AABB(const Circle& circle, const Rectangle& rect) {
		vector2::Vec2 shortestDistance{};

		if (circle.m_position.m_x < rect.m_boundingBox.m_min.m_x) shortestDistance.m_x = rect.m_boundingBox.m_min.m_x;
		else if (circle.m_position.m_x > rect.m_boundingBox.m_max.m_x) shortestDistance.m_x = rect.m_boundingBox.m_max.m_x;
		else shortestDistance.m_x = circle.m_position.m_x;

		if (circle.m_position.m_y < rect.m_boundingBox.m_min.m_y) shortestDistance.m_y = rect.m_boundingBox.m_min.m_y;
		else if (circle.m_position.m_y > rect.m_boundingBox.m_max.m_y) shortestDistance.m_y = rect.m_boundingBox.m_max.m_y;
		else shortestDistance.m_y = circle.m_position.m_y;

		float Y_Square = static_cast<float>(pow((circle.m_position.m_x - shortestDistance.m_x), 2));
		float X_Square = static_cast<float>(pow((circle.m_position.m_y - shortestDistance.m_y), 2));
		float distance_Square = Y_Square + X_Square;

		return distance_Square <= (circle.m_radius * circle.m_radius);
	}


	void Physics::m_Init() {

	}

	void Physics::m_Update() {

		m_CollisionCheckUpdate();
	}

	void Physics::m_LogCollision(int entityID) {
		collidedEntities.set(entityID); // Mark entity as collided
	}

	bool Physics::m_HasCollided(int entityID) const {
		return collidedEntities.test(entityID); // Check if it has collided already
	}

	float Circle::m_GetBoundingRadius() const {
		return m_radius;
	}
	float Rectangle::m_GetBoundingRadius() const {
		return 0.5f * std::sqrt(m_width * m_width + m_height * m_height);
	}



	bool Physics::m_WithinBoundingRadius(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2) {
		float dx = entity1->m_position.m_x - entity2->m_position.m_x;
		float dy = entity1->m_position.m_y - entity2->m_position.m_y;
		float distanceSquared = dx * dx + dy * dy;
		float combinedRadius = entity1->m_GetBoundingRadius() + entity2->m_GetBoundingRadius();
		return distanceSquared <= combinedRadius * combinedRadius;
	}

	void Physics::m_AddCollidedEntity(const std::shared_ptr<PhysicsData>& entity) {
		if (std::find(m_collidedEntities.begin(), m_collidedEntities.end(), entity) == m_collidedEntities.end()) {
			m_collidedEntities.push_back(entity);
		}
	}

	void Physics::m_CollisionCheckUpdate() {
		if (m_physicsEntities.empty()) return;

		// Reset blocked directions for all entities
		for (auto& entity : m_physicsEntities) {
			entity->m_ClearCollisionFlags();
		}

		m_CalculateBoundingBox();
		std::set<std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> pair;
		for (size_t i = 0; i < m_physicsEntities.size(); ++i) {
			for (size_t j = i + 1; j < m_physicsEntities.size(); ++j) {
				int layer1 = m_physicsEntities[i]->m_layerID;
				int layer2 = m_physicsEntities[j]->m_layerID;

				if (m_physicsEntities[i]->m_ID == m_physicsEntities[j]->m_ID) continue;
				// Check if these layers should collide
				if (physicsLayer->m_GetCollide(layer1, layer2) && m_WithinBoundingRadius(m_physicsEntities[i], m_physicsEntities[j])) {
					if (m_CheckCollision(m_physicsEntities[i], m_physicsEntities[j])) {
						pair.emplace(m_physicsEntities[i], m_physicsEntities[j]);
						m_AddCollidedEntity(m_physicsEntities[i]);
						m_AddCollidedEntity(m_physicsEntities[j]);

					}
				}
			}
		}
		std::vector<std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> tmp(pair.begin(), pair.end());
		m_collidedEntitiesPair = tmp;

	}

	bool Physics::m_CheckCollision(const std::shared_ptr<PhysicsData>& entity1, const std::shared_ptr<PhysicsData>& entity2) {
		// Check for collision based on the types of entities.
		if (entity1->m_GetEntity() == EntityType::RECTANGLE && entity2->m_GetEntity() == EntityType::RECTANGLE) {
			//return m_CollisionIntersection_RectRect_SAT(*static_cast<Rectangle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
			std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val = m_CollisionIntersection_RectRect_SAT_TEST(*static_cast<Rectangle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
			if (ret_Val.first) {
				std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>> obj_Pair;
				obj_Pair.first = entity1;
				obj_Pair.second = entity2;
				std::pair< std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float>> store_Pairs;
				store_Pairs.first = obj_Pair;
				store_Pairs.second.first = ret_Val.second.first;
				store_Pairs.second.second = ret_Val.second.second;
				m_collidedEntitiesPairWithVector.push_back(store_Pairs);

			}
			return ret_Val.first;
			//return m_CollisionIntersection_RectRect_SAT(*static_cast<Rectangle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
		}
		else if (entity1->m_GetEntity() == EntityType::CIRCLE && entity2->m_GetEntity() == EntityType::CIRCLE) {
			std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val = m_CollisionIntersection_CircleCircle_SAT_TEST(*static_cast<Circle*>(entity1.get()), *static_cast<Circle*>(entity2.get()));
			if (ret_Val.first) {
				std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>> obj_Pair;
				obj_Pair.first = entity1;
				obj_Pair.second = entity2;
				std::pair< std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float>> store_Pairs;
				store_Pairs.first = obj_Pair;
				store_Pairs.second.first = ret_Val.second.first;
				store_Pairs.second.second = ret_Val.second.second;
				m_collidedEntitiesPairWithVector.push_back(store_Pairs);
			}
			return ret_Val.first;
		}
		else if (entity1->m_GetEntity() == EntityType::CIRCLE && entity2->m_GetEntity() == EntityType::RECTANGLE) {
			//return m_CollisionIntersection_CircleRect_SAT(*static_cast<Circle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
			std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val = m_CollisionIntersection_CircleRect_SAT_TEST(*static_cast<Circle*>(entity1.get()), *static_cast<Rectangle*>(entity2.get()));
			if (ret_Val.first) {
				std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>> obj_Pair;
				obj_Pair.first = entity1;
				obj_Pair.second = entity2;
				std::pair< std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float>> store_Pairs;
				store_Pairs.first = obj_Pair;
				store_Pairs.second.first = ret_Val.second.first;
				store_Pairs.second.second = ret_Val.second.second;
				m_collidedEntitiesPairWithVector.push_back(store_Pairs);

			}
			return ret_Val.first;
		}
		else if (entity1->m_GetEntity() == EntityType::RECTANGLE && entity2->m_GetEntity() == EntityType::CIRCLE) {
			//return m_CollisionIntersection_CircleRect_SAT(*static_cast<Circle*>(entity2.get()), *static_cast<Rectangle*>(entity1.get()));
			std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val = m_CollisionIntersection_CircleRect_SAT_TEST(*static_cast<Circle*>(entity2.get()), *static_cast<Rectangle*>(entity1.get()));
			if (ret_Val.first) {
				std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>> obj_Pair;
				obj_Pair.first = entity1;
				obj_Pair.second = entity2;
				std::pair< std::pair< std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float>> store_Pairs;
				store_Pairs.first = obj_Pair;
				store_Pairs.second.first = ret_Val.second.first;
				store_Pairs.second.second = -ret_Val.second.second;
				m_collidedEntitiesPairWithVector.push_back(store_Pairs);

			}
			return ret_Val.first;
		}

		return false;  // If no valid collision type, return false.
	}

	std::vector<vector2::Vec2> Rectangle::m_GetRotatedVertices() const {
		std::vector<vector2::Vec2> vertices(4);
		float hw{}, hh{};

		//Calculate the angle
		float cosTheta = mathlibrary::mathlib::funcCos(mathlibrary::mathlib::funcDegreeToRadian(m_rotAngle));
		float sinTheta = mathlibrary::mathlib::funcSin(mathlibrary::mathlib::funcDegreeToRadian(m_rotAngle));

		//the scale of collider box
		hw = m_width * 0.5f;
		hh = m_height * 0.5f;

		// Center of the rectangle
		vector2::Vec2 center = m_position;

		//calculate the scale and rotate
		vertices[0] = { ((-hw) * cosTheta) - ((hh)*sinTheta), ((-hw) * sinTheta) + ((hh)*cosTheta) };	 // Top - Left
		vertices[1] = { ((hw)*cosTheta) - ((hh)*sinTheta), ((hw)*sinTheta) + ((hh)*cosTheta) };		 // Top - Right
		vertices[2] = { ((hw)*cosTheta) - ((-hh) * sinTheta), ((hw)*sinTheta) + ((-hh) * cosTheta) };	 // Bottom - Right
		vertices[3] = { ((-hw) * cosTheta) - ((-hh) * sinTheta), ((-hw) * sinTheta) + ((-hh) * cosTheta) };  // Bottom - Left

		//translate the vertices
		for (auto& vert : vertices) {
			vert += center;
		}
		return vertices;
	}

	std::vector<vector2::Vec2> Rectangle::m_GetEdges() const {
		std::vector<vector2::Vec2> vertices = m_GetRotatedVertices();
		std::vector<vector2::Vec2> edges(4);
		for (size_t i = 0; i < 4; ++i) {
			vector2::Vec2 p1 = vertices[i];
			vector2::Vec2 p2 = vertices[(i + 1) % 4];
			edges[i] = { p2.m_x - p1.m_x, p2.m_y - p1.m_y };
		}
		return edges;
	}

	void Physics::m_ProjectOntoAxis(const std::vector<vector2::Vec2>& vertices, const vector2::Vec2& axis, float& min, float& max) const {
		//min = max = (vertices[0].m_x * axis.m_x + vertices[0].m_y * axis.m_y);
		min = max = vector2::Vec2::m_funcVec2DDotProduct(vertices[0], axis);
		for (const auto& vertex : vertices) {
			//float projection = vertex.m_x * axis.m_x + vertex.m_y * axis.m_y;
			float projection = vector2::Vec2::m_funcVec2DDotProduct(vertex, axis);
			if (projection < min) min = projection;
			if (projection > max) max = projection;
		}
	}

	vector2::Vec2 Rectangle::m_TransformToLocalSpace(const vector2::Vec2& globalVector) const
	{
		float cosTheta = mathlibrary::mathlib::funcCos(-mathlibrary::mathlib::funcDegreeToRadian(m_rotAngle));
		float sinTheta = mathlibrary::mathlib::funcSin(-mathlibrary::mathlib::funcDegreeToRadian(m_rotAngle));
		return {
			globalVector.m_x * cosTheta - globalVector.m_y * sinTheta,  // x' calculation
		   globalVector.m_x * sinTheta + globalVector.m_y * cosTheta // y' calculation
		};
	}
	std::pair<bool, std::pair<vector2::Vec2, float>> Physics::m_CollisionIntersection_RectRect_SAT_TEST(const Rectangle& obj1, const Rectangle& obj2) {
		vector2::Vec2 normal{};
		//float maxAllowedDepth = 1.f;
		float depth = std::numeric_limits<float>::max();
		bool flag = true;
		std::pair<bool, std::pair<vector2::Vec2, float>> ret_Val;
		std::vector<vector2::Vec2> verticesA = obj1.m_GetRotatedVertices();
		std::vector<vector2::Vec2> verticesB = obj2.m_GetRotatedVertices();
		std::vector<vector2::Vec2> edgesA = obj1.m_GetEdges();
		std::vector<vector2::Vec2> edgesB = obj2.m_GetEdges();


		for (size_t i = 0; i < verticesA.size(); ++i) {
			float minA{}, maxA{}, minB{}, maxB{};
			vector2::Vec2 axis = { -edgesA[i].m_y, edgesA[i].m_x };
			vector2::Vec2::m_funcVec2Normalize(axis, axis);
			m_ProjectOntoAxis(verticesA, axis, minA, maxA);
			m_ProjectOntoAxis(verticesB, axis, minB, maxB);
			if (minA >= maxB || minB >= maxA) {
				flag = false;
				ret_Val.first = flag;
				ret_Val.second.first = {};
				ret_Val.second.second = {};
				return ret_Val;
			}

			float axisDepth = std::min(maxB - minA, maxA - minB);
			if (axisDepth < depth) {
				depth = axisDepth;
				//depth = std::max(0.0f, std::min(depth, maxAllowedDepth));
				normal = axis;
			}
		}

		for (size_t i = 0; i < verticesB.size(); ++i) {
			float minA{}, maxA{}, minB{}, maxB{};
			vector2::Vec2 axis = { -edgesB[i].m_y, edgesB[i].m_x };
			vector2::Vec2::m_funcVec2Normalize(axis, axis);
			m_ProjectOntoAxis(verticesA, axis, minA, maxA);
			m_ProjectOntoAxis(verticesB, axis, minB, maxB);
			if (minA >= maxB || minB >= maxA) {
				flag = false;
				ret_Val.first = flag;
				ret_Val.second.first = {};
				ret_Val.second.second = {};
				return ret_Val;
			}

			float axisDepth = std::min(maxB - minA, maxA - minB);
			if (axisDepth < depth) {
				depth = axisDepth;
				//depth = std::max(0.0f, std::min(depth, maxAllowedDepth));
				normal = axis;
			}
		}

		depth = depth / vector2::Vec2::m_funcVec2DLength(normal);
		vector2::Vec2::m_funcVec2Normalize(normal, normal);
		vector2::Vec2 direction = obj2.m_position - obj1.m_position;
		if (vector2::Vec2::m_funcVec2DDotProduct(direction, normal) < 0.f) {
			normal = -normal;
		}

		ret_Val.first = flag;
		ret_Val.second.first = normal;
		ret_Val.second.second = depth;


		return ret_Val;

	}
	bool Physics::m_CollisionIntersection_RectRect_SAT(const Rectangle& obj1, const Rectangle& obj2) {

		// Get rotated vertices and edges
		std::vector<vector2::Vec2> verticesA = obj1.m_GetRotatedVertices();
		std::vector<vector2::Vec2> verticesB = obj2.m_GetRotatedVertices();
		std::vector<vector2::Vec2> edgesA = obj1.m_GetEdges();
		std::vector<vector2::Vec2> edgesB = obj2.m_GetEdges();

		// List of all the axes (normals of edges)
		std::vector<vector2::Vec2> axes;
		for (const auto& edge : edgesA) {
			axes.push_back({ -edge.m_y, edge.m_x }); // Perpendicular vector
		}
		for (const auto& edge : edgesB) {
			axes.push_back({ -edge.m_y, edge.m_x });
		}

		//float minOverlap = std::numeric_limits<float>::max();
		vector2::Vec2 smallestAxis{};
		//bool isEntity1Blocked = false;
		// Check for overlap on all axes
		for (const auto& axis : axes) {
			float minA, maxA, minB, maxB;
			//using dot product to determine
			m_ProjectOntoAxis(verticesA, axis, minA, maxA);
			m_ProjectOntoAxis(verticesB, axis, minB, maxB);

			// If there's a gap, no collision
			if (maxA < minB || maxB < minA) {
				return false;
			}

			//float axisDepth = std::min(maxB - minA, maxA - minB); //unused

		}

		// No separating axis found, so there's a collision
		return true;

		/*vector2::Vec2 normal{};
		float depth{};
		std::vector<vector2::Vec2> verticesA = obj1.m_GetRotatedVertices();
		std::vector<vector2::Vec2> verticesB = obj2.m_GetRotatedVertices();
		std::vector<vector2::Vec2> edgesA = obj1.m_GetEdges();
		std::vector<vector2::Vec2> edgesB = obj2.m_GetEdges();


		for (size_t i = 0; i < verticesA.size(); ++i) {
			float minA{}, maxA{}, minB{}, maxB{};
			vector2::Vec2 axis = { -edgesA[i].m_y, edgesA[i].m_x };
			m_ProjectOntoAxis(verticesA, axis, minA, maxA);
			m_ProjectOntoAxis(verticesB, axis, minB, maxB);
			if (minA >= maxB || minB >= maxA) return false;

			float axisDepth = std::min(maxB - minA, maxA - minB);
			if (axisDepth < depth) {
				depth = axisDepth;
				normal = axis;
			}
		}

		for (size_t i = 0; i < verticesB.size(); ++i) {
			float minA{}, maxA{}, minB{}, maxB{};
			vector2::Vec2 axis = { -edgesB[i].m_y, edgesB[i].m_x };
			m_ProjectOntoAxis(verticesA, axis, minA, maxA);
			m_ProjectOntoAxis(verticesB, axis, minB, maxB);
			if (minA >= maxB || minB >= maxA) return false;

			float axisDepth = std::min(maxB - minA, maxA - minB);
			if (axisDepth < depth) {
				depth = axisDepth;
				normal = axis;
			}
		}

		depth = depth / vector2::Vec2::m_funcVec2DLength(normal);
		vector2::Vec2::m_funcVec2Normalize(normal, normal);
		vector2::Vec2 direction = obj2.m_position - obj1.m_position;
		if (vector2::Vec2::m_funcVec2DDotProduct(direction, normal) < 0.f) {
			normal = -normal;
		}*/

	}

	std::vector <std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> Physics::m_RetrievePhysicsDataPair() {
		std::vector <std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>> TempCollidedEntities = m_collidedEntitiesPair;
		//this->m_clearPair();
		return TempCollidedEntities;
	}

	std::vector < std::pair < std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float>>>  Physics::m_RetrievePhysicsDataPairWithVector() {
		std::vector < std::pair < std::pair<std::shared_ptr<PhysicsData>, std::shared_ptr<PhysicsData>>, std::pair<vector2::Vec2, float>>>  TempCollidedEntities = m_collidedEntitiesPairWithVector;
		//this->m_clearPair();
		return TempCollidedEntities;
	}

	void Physics::m_ClearPair() {
		m_collidedEntitiesPair.clear();
		m_collidedEntitiesPairWithVector.clear();
	}

	bool IsPointOnSegment(const vector2::Vec2& point, const vector2::Vec2& segStart, const vector2::Vec2& segEnd, float epsilon) {
		return point.m_x >= std::min(segStart.m_x, segEnd.m_x) - epsilon &&
			point.m_x <= std::max(segStart.m_x, segEnd.m_x) + epsilon &&
			point.m_y >= std::min(segStart.m_y, segEnd.m_y) - epsilon &&
			point.m_y <= std::max(segStart.m_y, segEnd.m_y) + epsilon;
	}

	bool Physics::LineIntersect(const vector2::Vec2& p1, const vector2::Vec2& p2, const vector2::Vec2& q1, const vector2::Vec2& q2, vector2::Vec2& intersection) {
		const float epsilon = 1e-4f;

		// Line equations: Ax + By = C
		float a1 = p2.m_y - p1.m_y;
		float b1 = p1.m_x - p2.m_x;
		float c1 = a1 * p1.m_x + b1 * p1.m_y;

		float a2 = q2.m_y - q1.m_y;
		float b2 = q1.m_x - q2.m_x;
		float c2 = a2 * q1.m_x + b2 * q1.m_y;

		float determinant = a1 * b2 - a2 * b1;

		// Check if lines are parallel
		if (std::abs(determinant) < epsilon) {
			// Check if lines are collinear
			if (std::abs(a1 * q1.m_x + b1 * q1.m_y - c1) < epsilon) {
				// Check if collinear segments overlap
				if (std::max(p1.m_x, p2.m_x) >= std::min(q1.m_x, q2.m_x) &&
					std::max(q1.m_x, q2.m_x) >= std::min(p1.m_x, p2.m_x) &&
					std::max(p1.m_y, p2.m_y) >= std::min(q1.m_y, q2.m_y) &&
					std::max(q1.m_y, q2.m_y) >= std::min(p1.m_y, p2.m_y)) {

					// Find the intersection point as the midpoint of overlapping range
					intersection.m_x = (std::max(std::min(p1.m_x, p2.m_x), std::min(q1.m_x, q2.m_x)) +
						std::min(std::max(p1.m_x, p2.m_x), std::max(q1.m_x, q2.m_x))) / 2.0f;
					intersection.m_y = (std::max(std::min(p1.m_y, p2.m_y), std::min(q1.m_y, q2.m_y)) +
						std::min(std::max(p1.m_y, p2.m_y), std::max(q1.m_y, q2.m_y))) / 2.0f;
					return true;  // Overlapping collinear segments
				}
			}
			return false; // Parallel but not collinear
		}

		// Find intersection point
		intersection.m_x = (b2 * c1 - b1 * c2) / determinant;
		intersection.m_y = (a1 * c2 - a2 * c1) / determinant;

		// Check if the intersection point lies on both line segments
		if (!IsPointOnSegment(intersection, p1, p2, epsilon) || !IsPointOnSegment(intersection, q1, q2, epsilon)) {
			return false;
		}

		return true;
	}

	float Distance(const vector2::Vec2& p1, const vector2::Vec2& p2) {
		return std::sqrt((p2.m_x - p1.m_x) * (p2.m_x - p1.m_x) + (p2.m_y - p1.m_y) * (p2.m_y - p1.m_y));
	}

	bool Physics::LineRectangleIntersect(const vector2::Vec2& p1, const vector2::Vec2& p2, const vector2::Vec2& rectBottomLeft, const vector2::Vec2& rectTopRight, vector2::Vec2& intersectionpoint)
	{
		vector2::Vec2 rectTopLeft = { rectBottomLeft.m_x, rectTopRight.m_y };
		vector2::Vec2 rectBottomRight = { rectTopRight.m_x, rectBottomLeft.m_y };

		// Rectangle edges
		vector2::Vec2 edges[4][2] = {
			{rectBottomLeft, rectBottomRight},
			{rectBottomRight, rectTopRight},
			{rectTopRight, rectTopLeft},
			{rectTopLeft, rectBottomLeft}
		};

		// Check the line against each rectangle edge
		bool foundIntersection = false;
		float minDistance = std::numeric_limits<float>::max();

		// Iterate through each rectangle edge to find the nearest intersection point
		for (int i = 0; i < 4; ++i) {
			vector2::Vec2 intersection;
			if (LineIntersect(p1, p2, edges[i][0], edges[i][1], intersection)) {
				float dist = Distance(p1, intersection);
				if (dist < minDistance) {
					minDistance = dist;
					intersectionpoint = intersection;
					foundIntersection = true;
				}
			}
		}

		if (!foundIntersection) {
			intersectionpoint = { p2.m_x, p2.m_y }; // intersection point be target position
		}
		return foundIntersection;

	}

	void Physics::IsLineIntersecting(const ecs::EntityID id, const vector2::Vec2& p1, const vector2::Vec2& p2, const std::vector<layer::LAYERS>& layer, bool& isHit, vector2::Vec2& hitPosition)
	{
		//int count{};
		for (const auto entity : m_physicsEntities) {

			//std::cout << count++ << std::endl;

			if (entity->type == EntityType::RECTANGLE) {

				if (std::find(layer.begin(), layer.end(), (layer::LAYERS)entity->m_layerID) == layer.end())continue; // return any entity that is not a part of the req layer
				if (id == static_cast<ecs::EntityID>(entity->m_ID)) continue;

				auto entityshape = std::dynamic_pointer_cast<Rectangle>(entity);
				if (LineRectangleIntersect(p1, p2, entityshape->m_boundingBox.m_min, entityshape->m_boundingBox.m_max, hitPosition)) {
					isHit = false;
					return;
				}
				else {
					isHit = true;
				}
			}



		}



	}


}