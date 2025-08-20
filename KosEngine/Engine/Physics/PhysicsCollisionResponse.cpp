/******************************************************************/
/*!
\file      PhysicsCollisionResponse.cpp
\author    Elijah Teo(teo.e, 2301530)[50%], Rayner Tan(raynerweichen.tan, 2301449)[50%]
\date      Jan 14, 2024
\brief     This file contains the definitions of the functions used to find contact points between two collided entities
		   and the functions used to find the directional collision flags



Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#include "../Config/pch.h"
#include "Physics.h"
#include "../ECS/ECS.h"
#include "PhysicsCollisionResponse.h"
#include "../Math/mathlib.h"

namespace physicspipe {

	void m_FindContactPoints() {
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();

		for (int i = 0; i < vecCollisionEntityPair.size(); i++) {
			std::shared_ptr<physicspipe::PhysicsData> entA = vecCollisionEntityPair[i].first;
			std::shared_ptr<physicspipe::PhysicsData> entB = vecCollisionEntityPair[i].second;
			ecs::EntityID first = entA.get()->m_ID;
			ecs::EntityID second = entB.get()->m_ID;
			const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(first));
			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(second));
			if (entA.get()->m_GetEntity() == EntityType::RECTANGLE && entB.get()->m_GetEntity() == EntityType::RECTANGLE) {
				std::tuple<int, vector2::Vec2, vector2::Vec2> contact = m_FindSquareSquareContact(static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices());
				
				if (std::get<0>(contact) == 1) {
					colComp->m_contactPoints.emplace_back(std::get<1>(contact), second,1);
					colComp2->m_contactPoints.emplace_back(std::get<1>(contact), first,1);
				}
				else {
					colComp->m_contactPoints.emplace_back(std::get<1>(contact), second,2);
					colComp2->m_contactPoints.emplace_back(std::get<1>(contact), first,2);
					colComp->m_contactPoints.emplace_back(std::get<2>(contact), second,2);
					colComp2->m_contactPoints.emplace_back(std::get<2>(contact), first,2);
				}
			}
			else if (entA.get()->m_GetEntity() == EntityType::RECTANGLE && entB.get()->m_GetEntity() == EntityType::CIRCLE) {
				vector2::Vec2 contact = m_FindCircleSquareContact(entB.get()->m_position, static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices());
				colComp->m_contactPoints.emplace_back(contact, second,1);
				colComp2->m_contactPoints.emplace_back(contact, first,1);
			}
			else if (entA.get()->m_GetEntity() == EntityType::CIRCLE && entB.get()->m_GetEntity() == EntityType::RECTANGLE) {
				vector2::Vec2 contact = m_FindCircleSquareContact(entA.get()->m_position, static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices());
				colComp->m_contactPoints.emplace_back(contact, second,1);
				colComp2->m_contactPoints.emplace_back(contact, first,1);
			}
			else if (entA.get()->m_GetEntity() == EntityType::CIRCLE && entB.get()->m_GetEntity() == EntityType::CIRCLE) {
				vector2::Vec2 contact = m_FindCircleCirleContact(entA.get()->m_position, static_cast<physicspipe::Circle*>(entA.get())->m_radius, entB.get()->m_position);
				colComp->m_contactPoints.emplace_back(contact,second,1);
				colComp2->m_contactPoints.emplace_back(contact, first,1);
			}
		}
	}

	vector2::Vec2 m_FindCircleCirleContact(const vector2::Vec2& cirleAPos, const float& cirlceARadius, const vector2::Vec2& circleBPos) {
		vector2::Vec2 ab = circleBPos - cirleAPos;
		vector2::Vec2 dir = {};
		vector2::Vec2 ret{};
		vector2::Vec2::m_funcVec2Normalize(dir, ab);
		ret = cirleAPos + dir * cirlceARadius;
		return ret;

	}

	std::pair<vector2::Vec2, float> m_PointSegDist(const vector2::Vec2& circlePos, const vector2::Vec2& startingPoint, const vector2::Vec2& endPoint) {
		vector2::Vec2 lineSeg = endPoint - startingPoint;
		vector2::Vec2 startToCircle = circlePos - startingPoint;
		std::pair<vector2::Vec2, float> ret;
		float proj = vector2::Vec2::m_funcVec2DDotProduct(startToCircle, lineSeg);
		float lineSegLenSq = vector2::Vec2::m_funcVec2DSquareLength(lineSeg);
		float dist = proj / lineSegLenSq;
		dist = std::max(0.f, std::min(1.f, dist));
		if (lineSegLenSq <= 0.0005f) {
			ret.first = startingPoint;
		}
		else if (dist<= 0.0005f) {
			ret.first = startingPoint;
		}
		else if (dist >= 1.f) {
			ret.first = endPoint;
			
		}
		else {
			ret.first = startingPoint + lineSeg * dist;
		}
		//ret.second = (ret.first.m_x - circlePos.m_x) * (ret.first.m_x - circlePos.m_x) + (ret.first.m_y - circlePos.m_y) * (ret.first.m_y - circlePos.m_y);
		ret.second = vector2::Vec2::m_funcVec2DSquareDistance(circlePos, ret.first);
		return ret;
	}

	vector2::Vec2 m_FindCircleSquareContact(const vector2::Vec2& circlePos, const std::vector<vector2::Vec2>& vertices) {

		float minDistSq = std::numeric_limits<float>::max();
		vector2::Vec2 contactRet{};
		for (int i = 0; i < static_cast<int>(vertices.size()); i++) {
			vector2::Vec2 va = vertices[i];
			vector2::Vec2 vb = vertices[(i + 1) % vertices.size()];
			std::pair<vector2::Vec2, float> optionOfContact = m_PointSegDist(circlePos, va, vb);

			if (optionOfContact.second < minDistSq) {
				minDistSq = optionOfContact.second;
				contactRet = optionOfContact.first;
			}
		}
		return contactRet;
	}
	
	bool m_AlmostEqual(float lhs, float rhs) {
		const float minDist = 0.0005f;
		return abs(lhs - rhs) < minDist;
	}

	bool m_AlmostEqual(vector2::Vec2 lhs, vector2::Vec2 rhs) {
		return m_AlmostEqual(lhs.m_x, rhs.m_x) && m_AlmostEqual(lhs.m_y, rhs.m_y);
	}

	bool m_AlmostEqualCP(float lhs, float rhs) {
		const float minDist = 0.05f;
		return abs(lhs - rhs) < minDist;
	}

	bool m_AlmostEqualCP(vector2::Vec2 lhs, vector2::Vec2 rhs) {
		return m_AlmostEqualCP(lhs.m_x, rhs.m_x) && m_AlmostEqualCP(lhs.m_y, rhs.m_y);
	}

	bool m_AlmostEqualAng(float lhs, float rhs) {
		const float minDist = 0.0005f;
		return abs(lhs - rhs) < minDist;
	}

	std::tuple<int, vector2::Vec2, vector2::Vec2> m_FindSquareSquareContact(const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& verticesB) {
		vector2::Vec2 contact1 = { 0,0 };
		vector2::Vec2 contact2 = { 0,0 };
		int points = 0;
		float minDistSq = std::numeric_limits<float>::max();
		for (int i = 0; i < static_cast<int>(verticesA.size()); i++) {
			vector2::Vec2 pointA = verticesA[i];
			for (int j = 0; j < static_cast<int>(verticesB.size()); j++) {
				vector2::Vec2 va = verticesB[j];
				vector2::Vec2 vb = verticesB[(j + 1) % static_cast<int>(verticesB.size())];

				std::pair<vector2::Vec2, float> optionOfContact = m_PointSegDist(pointA, va, vb);

				if (m_AlmostEqual(optionOfContact.second, minDistSq)) {
					if (!m_AlmostEqualCP(contact1, optionOfContact.first) && !m_AlmostEqualCP(contact2, optionOfContact.first)) {
						contact2 = optionOfContact.first;
						points = 2;
					}
				}
				else if (optionOfContact.second < minDistSq) {
					minDistSq = optionOfContact.second;
					contact1 = optionOfContact.first;
					points = 1;
				}

			}
		}

		for (int i = 0; i < static_cast<int>(verticesB.size()); i++) {
			vector2::Vec2 pointB = verticesB[i];
			for (int j = 0; j < static_cast<int>(verticesA.size()); j++) {
				vector2::Vec2 va = verticesA[j];
				vector2::Vec2 vb = verticesA[(j + 1) % static_cast<int>(verticesA.size())];

				std::pair<vector2::Vec2, float> optionOfContact = m_PointSegDist(pointB, va, vb);

				if (m_AlmostEqual(optionOfContact.second, minDistSq)) {
					if (!m_AlmostEqualCP(contact1, optionOfContact.first) && !m_AlmostEqualCP(contact2, optionOfContact.first)) {
						contact2 = optionOfContact.first;
						points = 2;
					}
				}
				else if (optionOfContact.second < minDistSq) {
					minDistSq = optionOfContact.second;
					contact1 = optionOfContact.first;
					points = 1;
				}

			}
		}
		std::tuple<int, vector2::Vec2, vector2::Vec2> contactRet = { points, contact1, contact2 };
		return contactRet;
	}

	void m_FindCollisionFlags() {
		physicspipe::Physics* PhysicsPipeline = physicspipe::Physics::m_GetInstance();
		std::vector <std::pair<std::shared_ptr<physicspipe::PhysicsData>, std::shared_ptr<physicspipe::PhysicsData>>> vecCollisionEntityPair = PhysicsPipeline->m_RetrievePhysicsDataPair();
		for (int i = 0; i < vecCollisionEntityPair.size(); i++) {
			std::shared_ptr<physicspipe::PhysicsData> entA = vecCollisionEntityPair[i].first;
			std::shared_ptr<physicspipe::PhysicsData> entB = vecCollisionEntityPair[i].second;
			ecs::EntityID first = entA.get()->m_ID;
			ecs::EntityID second = entB.get()->m_ID;
			const auto& colComp = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(first));
			const auto& colComp2 = static_cast<ecs::ColliderComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPECOLLIDERCOMPONENT]->m_GetEntityComponent(second));
			const auto& rigComp = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(first));
			const auto& rigComp2 = static_cast<ecs::RigidBodyComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPERIGIDBODYCOMPONENT]->m_GetEntityComponent(second));
			//if (rigComp == NULL || rigComp2 == NULL) continue;
			//const auto& transComp = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ColComp->m_Entity));
			if (colComp->m_type == EntityType::CIRCLE && colComp2->m_type == EntityType::CIRCLE) {
				if (rigComp == NULL || rigComp2 == NULL) continue;
				vector2::Vec2 dirA = rigComp->m_PrevDirVec;
				vector2::Vec2 dirB = rigComp2->m_PrevDirVec;
				vector2::Vec2::m_funcVec2Normalize(dirA,dirA);
				vector2::Vec2::m_funcVec2Normalize(dirB, dirB);
				std::pair<std::bitset<4>, std::bitset<4>> flags = m_FindCircleCircleFlags(colComp->m_contactPoints,first, second,entA.get()->m_position, static_cast<physicspipe::Circle*>(entA.get())->m_radius,
										entB.get()->m_position, static_cast<physicspipe::Circle*>(entB.get())->m_radius,dirA,dirB);
				colComp->m_bits |= flags.first;
				colComp2->m_bits |= flags.second;
				colComp->m_blockedFlag = colComp->m_bits.to_ulong();
				colComp2->m_blockedFlag = colComp2->m_bits.to_ulong();
			}
			else if (colComp->m_type == EntityType::CIRCLE && colComp2->m_type == EntityType::RECTANGLE) {
				if (rigComp == NULL) continue;
				vector2::Vec2 dirA = rigComp->m_PrevDirVec;
				vector2::Vec2::m_funcVec2Normalize(dirA, dirA);
				std::pair<std::bitset<4>, std::bitset<4>> flags = m_FindCircleSquareFlags(colComp->m_contactPoints,first,second,entA.get()->m_position,static_cast<physicspipe::Circle*>(entA.get())->m_radius, dirA,
																	entB.get()->m_position, static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entB.get())->m_GetEdges());
				colComp->m_bits |= flags.first;
				colComp2->m_bits |= flags.second;
				colComp->m_blockedFlag = colComp->m_bits.to_ulong();
				colComp2->m_blockedFlag = colComp2->m_bits.to_ulong();
			}
			else if (colComp->m_type == EntityType::RECTANGLE && colComp2->m_type == EntityType::CIRCLE) {
				if (rigComp2 == NULL) continue;
				//m_FindCircleSquareFlags(colComp2, colComp);
				vector2::Vec2 dirB = rigComp2->m_PrevDirVec;
				vector2::Vec2::m_funcVec2Normalize(dirB, dirB);
				std::pair<std::bitset<4>, std::bitset<4>> flags = m_FindCircleSquareFlags(colComp2->m_contactPoints, second, first, entB.get()->m_position, static_cast<physicspipe::Circle*>(entB.get())->m_radius, dirB,
					entA.get()->m_position, static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entA.get())->m_GetEdges());
				colComp2->m_bits |= flags.first;
				colComp->m_bits |= flags.second;
				colComp->m_blockedFlag = colComp->m_bits.to_ulong();
				colComp2->m_blockedFlag = colComp2->m_bits.to_ulong();
			}
			else {
				std::pair<std::bitset<4>, std::bitset<4>> collFlags = m_FindSquareSquareFlags(colComp->m_contactPoints,first, second, entA.get()->m_position, static_cast<physicspipe::Rectangle*>(entA.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entA.get())->m_GetEdges(),
					entB.get()->m_position, static_cast<physicspipe::Rectangle*>(entB.get())->m_GetRotatedVertices(), static_cast<physicspipe::Rectangle*>(entB.get())->m_GetEdges());
				colComp->m_bits |= collFlags.first;
				colComp2->m_bits |= collFlags.second;
				colComp->m_blockedFlag = colComp->m_bits.to_ulong();
				colComp2->m_blockedFlag = colComp2->m_bits.to_ulong();
			}
			//const auto& colComp2 = static_cast<ecs::TransformComponent*>(ecs::ECS::m_GetInstance()->m_ECS_CombinedComponentPool[ecs::TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(second));
		}
	}

	std::pair<std::bitset<4>, std::bitset<4>> m_FindSquareSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, [[maybe_unused]] ecs::EntityID entA, [[maybe_unused]] ecs::EntityID entB, const vector2::Vec2& centerA, const std::vector<vector2::Vec2>& verticesA, const std::vector<vector2::Vec2>& edgesA, const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB) {
		vector2::Vec2 topNormA{ -edgesA[0].m_y, edgesA[0].m_x };
		vector2::Vec2 rightNormA{ -edgesA[1].m_y, edgesA[1].m_x };
		vector2::Vec2 bottomNormA{ -edgesA[2].m_y, edgesA[2].m_x };
		vector2::Vec2 leftNormA{ -edgesA[3].m_y, edgesA[3].m_x };
		vector2::Vec2::m_funcVec2Normalize(topNormA, topNormA);
		vector2::Vec2::m_funcVec2Normalize(rightNormA, rightNormA);
		vector2::Vec2::m_funcVec2Normalize(bottomNormA, bottomNormA);
		vector2::Vec2::m_funcVec2Normalize(leftNormA, leftNormA);
		std::vector<vector2::Vec2> normalsA;
		normalsA.push_back(topNormA);
		normalsA.push_back(rightNormA);
		normalsA.push_back(bottomNormA);
		normalsA.push_back(leftNormA);

		vector2::Vec2 topNormB{ -edgesB[0].m_y, edgesB[0].m_x };
		vector2::Vec2 rightNormB{ -edgesB[1].m_y, edgesB[1].m_x };
		vector2::Vec2 bottomNormB{ -edgesB[2].m_y, edgesB[2].m_x };
		vector2::Vec2 leftNormB{ -edgesB[3].m_y, edgesB[3].m_x };
		vector2::Vec2::m_funcVec2Normalize(topNormB, topNormB);
		vector2::Vec2::m_funcVec2Normalize(rightNormB, rightNormB);
		vector2::Vec2::m_funcVec2Normalize(bottomNormB, bottomNormB);
		vector2::Vec2::m_funcVec2Normalize(leftNormB, leftNormB);
		std::vector<vector2::Vec2> normalsB;
		normalsB.push_back(topNormB);
		normalsB.push_back(rightNormB);
		normalsB.push_back(bottomNormB);
		normalsB.push_back(leftNormB);
		vector2::Vec2 contactPoint1;
		vector2::Vec2 contactPoint2{ 0,0 };
		int numOfContacts = 0;
		std::pair<std::bitset<4>, std::bitset<4>> ret{};
		for (int i = 0; i < contactPoints.size(); i++) {
			if (contactPoints[i].m_contactPointEnt.second == entB) {
				if (contactPoints[i].m_numOfContacts == 1) {
					contactPoint1 = contactPoints[i].m_contactPointEnt.first;
					numOfContacts = 1;
					break;
				}
				else if(contactPoints[i].m_numOfContacts == 2){
					contactPoint1 = contactPoints[i].m_contactPointEnt.first;
					contactPoint2 = contactPoints[i + 1].m_contactPointEnt.first;
					//midPoint = (contactPoint1 + contactPoint2) / 2;
					numOfContacts = 2;
					break;
				}
			}
		}

		float min = std::numeric_limits<float>::max();
		float max = -9999.f;
		for (int i = 0; i < verticesA.size(); i++) {
			if (numOfContacts == 2) {
				vector2::Vec2 midpoint = contactPoint1 + contactPoint2;
				midpoint /= 2;
				vector2::Vec2 centerToVert = verticesA[i] - centerA;
				vector2::Vec2 centerToCP = midpoint - centerA;
				//vector2::Vec2 centerToCP2 = contactPoint2 - centerA;
				float dotVertA = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToVert);
				float dotCPA = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToCP);
				//float dotCPA2 = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToCP2);
				float eq = dotCPA - dotVertA;
				//float eq2 = dotCPA2 - dotVertA;
				if (m_AlmostEqualCP(eq,0.f) && eq < min) {
					min = std::min(eq, min);
					if (i == 0) {
						ret.first.reset();
						ret.first.set(0);
						//break;
					}
					if (i == 1) {
						ret.first.reset();
						ret.first.set(1);
						//break;
					}
					if (i == 2) {
						ret.first.reset();
						ret.first.set(2);
						//break;
					}
					if (i == 3) {
						ret.first.reset();
						ret.first.set(3);
						//break;
					}
				}
			}
			else {
				if (m_AlmostEqualCP(contactPoint1, verticesA[i])) {
					if (i == 0) {
						ret.first.set(0);
						ret.first.set(3);
						break;
					}
					if (i == 1) {
						ret.first.set(0);
						ret.first.set(1);
						break;
					}
					if (i == 2) {
						ret.first.set(1);
						ret.first.set(2);
						break;
					}
					if (i == 3) {
						ret.first.set(2);
						ret.first.set(3);
						break;
					}
				}
				else {
					vector2::Vec2 centerToVert = verticesA[i] - centerA;
					vector2::Vec2 centerToCP = contactPoint1 - centerA;
					float dotVertA = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToVert);
					float dotCPA = vector2::Vec2::m_funcVec2DDotProduct(normalsA[i], centerToCP);
					float eq = dotCPA - dotVertA;
					if (m_AlmostEqualCP(eq, 0.f)) {
						max = std::max(dotCPA, max);
						min = std::min(eq, min);
						if (i == 0) {
							ret.first.reset();
							ret.first.set(0);							//break;
						}
						if (i == 1) {
							ret.first.reset();
							ret.first.set(1);							//break;
						}
						if (i == 2) {
							ret.first.reset();
							ret.first.set(2);							//break;
						}
						if (i == 3) {
							ret.first.reset();
							ret.first.set(3);							//break;
						}
					}else if(dotCPA <= 0.f && max < dotCPA && eq < min) {
						max = std::max(dotCPA, max);
						if (i == 0) {
							ret.first.set(0);
							ret.first.set(3);
							break;
						}
						if (i == 1) {
							ret.first.set(0);
							ret.first.set(1);
							break;
						}
						if (i == 2) {
							ret.first.set(1);
							ret.first.set(2);
							break;
						}
						if (i == 3) {
							ret.first.set(2);
							ret.first.set(3);
							break;
						}
					}
				}
			}
		}
		min = std::numeric_limits<float>::max();
		max = -9999.f;
		for (int i = 0; i < verticesB.size(); i++) {
			if (numOfContacts == 2) {
				vector2::Vec2 midpoint = contactPoint1 + contactPoint2;
				midpoint /= 2;
				vector2::Vec2 centerToVert = verticesB[i] - centerB;
				vector2::Vec2 centerToCP = midpoint - centerB;
				float dotVertB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToVert);
				float dotCPB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToCP);
				float eq = dotCPB - dotVertB;
				if (m_AlmostEqualCP(eq,0.f) && eq < min) {
					min = std::min(eq, min);
					if (i == 0) {
						ret.second.reset();
						ret.second.set(0);
						//break;
					}
					if (i == 1) {
						ret.second.reset();
						ret.second.set(1);						//break;
					}
					if (i == 2) {
						ret.second.reset();
						ret.second.set(2);						//break;
					}
					if (i == 3) {
						ret.second.reset();
						ret.second.set(3);						//break;
					}
				}
			}
			else {
				if (m_AlmostEqualCP(contactPoint1, verticesB[i])) {
					if (i == 0) {
						ret.second.set(0);
						ret.second.set(3);
						break;
					}
					if (i == 1) {
						ret.second.set(0);
						ret.second.set(1);
						break;
					}
					if (i == 2) {
						ret.second.set(2);
						ret.second.set(1);
						break;
					}
					if (i == 3) {
						ret.second.set(2);
						ret.second.set(3);
						break;
					}
				}
				else {
					vector2::Vec2 centerToVert = verticesB[i] - centerB;
					vector2::Vec2 centerToCP = contactPoint1 - centerB;
					float dotVertB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToVert);
					float dotCPB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToCP);
					float eq = dotCPB - dotVertB;
					if (m_AlmostEqual(eq, 0.f)) {
						max = std::max(dotCPB, max);
						min = std::min(eq, min);
						if (i == 0) {
							ret.second.reset();
							ret.second.set(0);
							//break;
						}
						if (i == 1) {
							ret.second.reset();
							ret.second.set(1);						//break;
						}
						if (i == 2) {
							ret.second.reset();
							ret.second.set(2);						//break;
						}
						if (i == 3) {
							ret.second.reset();
							ret.second.set(3);						//break;
						}
					}
					else if (dotCPB <= 0.f && max < dotCPB && eq < min) {
						max = std::max(dotCPB, max);
						if (i == 0) {
							ret.second.reset();
							ret.second.set(0);
							//break;
						}
						if (i == 1) {
							ret.second.reset();
							ret.second.set(1);						//break;
						}
						if (i == 2) {
							ret.second.reset();
							ret.second.set(2);						//break;
						}
						if (i == 3) {
							ret.second.reset();
							ret.second.set(3);						//break;
						}
					}
				}
			}
		}

		return ret;
	}



	std::pair<std::bitset<4>, std::bitset<4>> m_FindCircleCircleFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, [[maybe_unused]] ecs::EntityID entA, [[maybe_unused]] ecs::EntityID entB, const vector2::Vec2& centerA, [[maybe_unused]] const float& radA, const vector2::Vec2& centerB, [[maybe_unused]] const float& radB, const vector2::Vec2& dirVecA, const vector2::Vec2& dirVecB) {
		vector2::Vec2 contactPoint1;
		std::pair<std::bitset<4>, std::bitset<4>> ret{};
		for (int i = 0; i < contactPoints.size(); i++) {
			if (contactPoints[i].m_contactPointEnt.second == entB) {
				if (contactPoints[i].m_numOfContacts == 1) {
					contactPoint1 = contactPoints[i].m_contactPointEnt.first;
					break;
				}
			}
		}

		vector2::Vec2 centerAToCP = contactPoint1 - centerA;
		vector2::Vec2 centerBToCP = contactPoint1 - centerB;
		vector2::Vec2::m_funcVec2Normalize(centerAToCP, centerAToCP);
		vector2::Vec2::m_funcVec2Normalize(centerBToCP, centerBToCP);
		vector2::Vec2 dirA = dirVecA;
		vector2::Vec2 dirB = dirVecB;
		float dotProdDirACP = vector2::Vec2::m_funcVec2DDotProduct(centerAToCP, dirA);
		float dotProdDirBCP = vector2::Vec2::m_funcVec2DDotProduct(centerBToCP, dirB);
		float crossProdDirACP = vector2::Vec2::m_funcVec2CrossProduct(centerAToCP, dirA);
		float crossProdDirBCP = vector2::Vec2::m_funcVec2CrossProduct(centerBToCP, dirB);
		float angBetDirACP = mathlibrary::mathlib::funcRadianToDegree(atan2f(crossProdDirACP,dotProdDirACP));
		float angBetDirBCP = mathlibrary::mathlib::funcRadianToDegree(atan2f(crossProdDirBCP, dotProdDirBCP));
		if (angBetDirACP < 0.f) {
			angBetDirACP += 360.f;
		}
		if (angBetDirBCP < 0.f) {
			angBetDirBCP += 360.f;
		}
		if (angBetDirACP >= 315.f ||( angBetDirACP >= 0.f && angBetDirACP <= 45.f)) {
			if (m_AlmostEqualAng(angBetDirACP, 315.f)) {
				ret.first.set(0);
				ret.first.set(3);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 45.f)) {
				ret.first.set(0);
				ret.first.set(1);
			}
			else {
				ret.first.set(0);
			}

		} else if (angBetDirACP >= 45.f && angBetDirACP <= 135.f) {
			if (m_AlmostEqualAng(angBetDirACP, 45.f)) {
				ret.first.set(0);
				ret.first.set(1);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 135.f)) {
				ret.first.set(1);
				ret.first.set(2);
			}
			else {
				ret.first.set(1);
			}

		}
		else if (angBetDirACP >= 135.f && angBetDirACP <= 225.f) {
			if (m_AlmostEqualAng(angBetDirACP, 135.f)) {
				ret.first.set(1);
				ret.first.set(2);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 225.f)) {
				ret.first.set(2);
				ret.first.set(3);
			}
			else {
				ret.first.set(2);
			}

		}
		else if (angBetDirACP >= 225.f && angBetDirACP <= 315.f) {
			if (m_AlmostEqualAng(angBetDirACP, 225.f)) {
				ret.first.set(2);
				ret.first.set(3);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 315.f)) {
				ret.first.set(0);
				ret.first.set(3);
			}
			else {
				ret.first.set(3);
			}

		}

		if (angBetDirBCP >= 315.f || (angBetDirBCP >= 0.f && angBetDirBCP <= 45.f)) {
			if (m_AlmostEqualAng(angBetDirBCP, 315.f)) {
				ret.second.set(0);
				ret.second.set(3);
			}
			else if (m_AlmostEqualAng(angBetDirBCP, 45.f)) {
				ret.second.set(0);
				ret.second.set(1);
			}
			else {
				ret.second.set(0);
			}

		}
		else if (angBetDirBCP >= 45.f && angBetDirBCP <= 135.f) {
			if (m_AlmostEqualAng(angBetDirBCP, 45.f)) {
				ret.second.set(0);
				ret.second.set(1);
			}
			else if (m_AlmostEqualAng(angBetDirBCP, 135.f)) {
				ret.second.set(1);
				ret.second.set(2);
			}
			else {
				ret.second.set(1);
			}

		}
		else if (angBetDirBCP >= 135.f && angBetDirBCP <= 225.f) {
			if (m_AlmostEqualAng(angBetDirBCP, 135.f)) {
				ret.second.set(1);
				ret.second.set(2);
			}
			else if (m_AlmostEqualAng(angBetDirBCP, 225.f)) {
				ret.second.set(2);
				ret.second.set(3);
			}
			else {
				ret.second.set(2);
			}

		}
		else if (angBetDirBCP >= 225.f && angBetDirBCP <= 315.f) {
			if (m_AlmostEqualAng(angBetDirBCP, 225.f)) {
				ret.second.set(2);
				ret.second.set(3);
			}
			else if (m_AlmostEqualAng(angBetDirBCP, 315.f)) {
				ret.second.set(0);
				ret.second.set(3);
			}
			else {
				ret.second.set(3);
			}

		}
		return ret;

	}

	std::pair<std::bitset<4>, std::bitset<4>> m_FindCircleSquareFlags(const std::vector<physicspipe::CollisionResponseData>& contactPoints, [[maybe_unused]] ecs::EntityID entA, [[maybe_unused]] ecs::EntityID entB, const vector2::Vec2& centerA, [[maybe_unused]] const float& radA, const vector2::Vec2& dirVecA, const vector2::Vec2& centerB, const std::vector<vector2::Vec2>& verticesB, const std::vector<vector2::Vec2>& edgesB) {
		vector2::Vec2 contactPoint1;
		vector2::Vec2 topNormB{ -edgesB[0].m_y, edgesB[0].m_x };
		vector2::Vec2 rightNormB{ -edgesB[1].m_y, edgesB[1].m_x };
		vector2::Vec2 bottomNormB{ -edgesB[2].m_y, edgesB[2].m_x };
		vector2::Vec2 leftNormB{ -edgesB[3].m_y, edgesB[3].m_x };
		vector2::Vec2::m_funcVec2Normalize(topNormB, topNormB);
		vector2::Vec2::m_funcVec2Normalize(rightNormB, rightNormB);
		vector2::Vec2::m_funcVec2Normalize(bottomNormB, bottomNormB);
		vector2::Vec2::m_funcVec2Normalize(leftNormB, leftNormB);
		std::vector<vector2::Vec2> normalsB;
		normalsB.push_back(topNormB);
		normalsB.push_back(rightNormB);
		normalsB.push_back(bottomNormB);
		normalsB.push_back(leftNormB);
		std::pair<std::bitset<4>, std::bitset<4>> ret{};
		for (int i = 0; i < contactPoints.size(); i++) {
			if (contactPoints[i].m_contactPointEnt.second == entB) {
				if (contactPoints[i].m_numOfContacts == 1) {
					contactPoint1 = contactPoints[i].m_contactPointEnt.first;
					break;
				}
			}
		}

		vector2::Vec2 centerAToCP = contactPoint1 - centerA;
		vector2::Vec2::m_funcVec2Normalize(centerAToCP, centerAToCP);
		vector2::Vec2 dirA = dirVecA;
		float dotProdDirACP = vector2::Vec2::m_funcVec2DDotProduct(centerAToCP, dirA);
		float crossProdDirACP = vector2::Vec2::m_funcVec2CrossProduct(centerAToCP, dirA);
		float angBetDirACP = mathlibrary::mathlib::funcRadianToDegree(atan2f(crossProdDirACP, dotProdDirACP));
		if (angBetDirACP < 0.f) {
			angBetDirACP += 360.f;
		}

		if (angBetDirACP >= 315.f || (angBetDirACP >= 0.f && angBetDirACP <= 45.f)) {
			if (m_AlmostEqualAng(angBetDirACP, 315.f)) {
				ret.first.set(0);
				ret.first.set(3);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 45.f)) {
				ret.first.set(0);
				ret.first.set(1);
			}
			else {
				ret.first.set(0);
			}

		}
		else if (angBetDirACP >= 45.f && angBetDirACP <= 135.f) {
			if (m_AlmostEqualAng(angBetDirACP, 45.f)) {
				ret.first.set(0);
				ret.first.set(1);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 135.f)) {
				ret.first.set(1);
				ret.first.set(2);
			}
			else {
				ret.first.set(1);
			}

		}
		else if (angBetDirACP >= 135.f && angBetDirACP <= 225.f) {
			if (m_AlmostEqualAng(angBetDirACP, 135.f)) {
				ret.first.set(1);
				ret.first.set(2);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 225.f)) {
				ret.first.set(2);
				ret.first.set(3);
			}
			else {
				ret.first.set(2);
			}

		}
		else if (angBetDirACP >= 225.f && angBetDirACP <= 315.f) {
			if (m_AlmostEqualAng(angBetDirACP, 225.f)) {
				ret.first.set(2);
				ret.first.set(3);
			}
			else if (m_AlmostEqualAng(angBetDirACP, 315.f)) {
				ret.first.set(0);
				ret.first.set(3);
			}
			else {
				ret.first.set(3);
			}

		}

		float min = std::numeric_limits<float>::max();
		float max = -9999.f;
		for (int i = 0; i < verticesB.size(); i++) {
			
			if (m_AlmostEqualCP(contactPoint1, verticesB[i])) {
				if (i == 0) {
					ret.second.set(0);
					ret.second.set(3);
					break;
				}
				if (i == 1) {
					ret.second.set(0);
					ret.second.set(1);
					break;
				}
				if (i == 2) {
					ret.second.set(2);
					ret.second.set(1);
					break;
				}
				if (i == 3) {
					ret.second.set(2);
					ret.second.set(3);
					break;
				}
			}
			else {
				vector2::Vec2 centerToVert = verticesB[i] - centerB;
				vector2::Vec2 centerToCP = contactPoint1 - centerB;
				float dotVertB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToVert);
				float dotCPB = vector2::Vec2::m_funcVec2DDotProduct(normalsB[i], centerToCP);
				float eq = dotCPB - dotVertB;
				if (m_AlmostEqual(eq, 0.f)) {
					max = std::max(dotCPB, max);
					min = std::min(eq, min);
					if (i == 0) {
						ret.second.reset();
						ret.second.set(0);
						//break;
					}
					if (i == 1) {
						ret.second.reset();
						ret.second.set(1);						//break;
					}
					if (i == 2) {
						ret.second.reset();
						ret.second.set(2);						//break;
					}
					if (i == 3) {
						ret.second.reset();
						ret.second.set(3);						//break;
					}
				}
				else if (dotCPB <= 0.f && max < dotCPB && eq < min) {
					max = std::max(dotCPB, max);
					if (i == 0) {
						ret.second.reset();
						ret.second.set(0);
						//break;
					}
					if (i == 1) {
						ret.second.reset();
						ret.second.set(1);						//break;
					}
					if (i == 2) {
						ret.second.reset();
						ret.second.set(2);						//break;
					}
					if (i == 3) {
						ret.second.reset();
						ret.second.set(3);						//break;
					}
				}
			}
			
		}


		return ret;
	}
}