/*!
\file      ActionEvent.h
\author    Elijah Teo, teo.e , 2301530
\par       teo.e@digipen.edu
\date      Sept 28, 2024
\brief     This file holds the declaration and definitions of the specialised action event classes for undo/redo

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/
#pragma once
#include "EventsBaseEvent.h"
#include "../Math/vector2.h"
#include "../Math/Mat3x3.h"
#include "../Actions/ModifyAction.h"

namespace events {

	/******************************************************************/
	/*!
	\class     TransformComponentChanged
	\brief     A derived class from BaseEvent class that holds which entityID, a pointer to the transform component, all the previous member variables in the transform component
	*/
	/******************************************************************/
	class TransformComponentChanged : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityChanged;
		ecs::TransformComponent* m_changedComponent;
		vector2::Vec2 m_oldPos;
		float m_oldRot;
		vector2::Vec2 m_oldScale;
		mat3x3::Mat3x3 m_oldTrans;
		//ecs::TransformComponent oldVal, newVal;
	public:
		// Constructor for TransformComponentChanged
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::TransformComponent* inComp, ecs::TransformComponent inOld)
		\brief     Constructor for creating a transform component changed event
		\param[in] inType	Type of component that was changed
		\param[in] inID		Entitys' ID
		\param[in] inComp	Pointer to the Entitys' transform component
		\param[in] inOld	Copy of previous transform component
		*/
		/******************************************************************/
		TransformComponentChanged(ecs::ComponentType inType, ecs::EntityID inID, ecs::TransformComponent* inComp, ecs::TransformComponent inOld) : BaseEvent<Actions>(Actions::TRANSFORMCOMP),
			m_changedType(inType), m_entityChanged(inID), m_changedComponent(inComp),m_oldPos(inOld.m_position), m_oldRot(inOld.m_rotation), m_oldScale(inOld.m_scale), m_oldTrans(inOld.m_transformation){}

		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetComponentType()
		\brief     Getter for the changed component type
		\return	   Component type of the entity
		*/
		/******************************************************************/
		ecs::ComponentType m_GetComponentType() { return m_changedType; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetID()
		\brief     Getter for the entitys' ID
		\return	   Entitys' ID
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityChanged; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetComp()
		\brief     Getter for the pointer to the transform component
		\return	   Pointer to the entity's transform component
		*/
		/******************************************************************/
		ecs::TransformComponent* m_GetComp() { return m_changedComponent; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldPos()
		\brief     Getter for the previous position
		\return	   Previous position of entity
		*/
		/******************************************************************/
		vector2::Vec2 m_GetOldPos() { return m_oldPos; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldScale()
		\brief     Getter for the previous scale
		\return	   Previous scale of entity
		*/
		/******************************************************************/
		vector2::Vec2 m_GetOldScale() { return m_oldScale; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldRot()
		\brief     Getter for the previous rotation
		\return	   Previous rotation of entity
		*/
		/******************************************************************/
		float m_GetOldRot() { return m_oldRot; }
		/******************************************************************/
		/*!
		\fn        TransformComponentChanged::m_GetOldTrans()
		\brief     Getter for the previous transform matrix
		\return    Previous computed Transform matrix
		*/
		/******************************************************************/
		mat3x3::Mat3x3 m_GetOldTrans() { return m_oldTrans; }

	};

	/******************************************************************/
	/*!
	\class     AddComponent
	\brief     A derived class from BaseEvent class that represents adding a component to an entity.
	*/
	/******************************************************************/
	class AddComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged; /*!< The ID of the entity to which the component is added. */
		ecs::ComponentType m_addedType; /*!< The type of component being added. */
	public:
		/******************************************************************/
		/*!
		\fn        AddComponent::AddComponent(ecs::EntityID inID, ecs::ComponentType inType)
		\brief     Constructor for creating an add component event.
		\param[in] inID      The ID of the entity.
		\param[in] inType    The type of the component to be added.
		*/
		/******************************************************************/
		AddComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::ADDCOMP), m_entityChanged(inID), m_addedType(inType) {}

		/******************************************************************/
		/*!
		\fn        AddComponent::m_GetComponentType()
		\brief     Getter for the type of the component added.
		\return    The type of the added component.
		*/
		/******************************************************************/
		ecs::ComponentType m_GetComponentType() { return m_addedType; }

		/******************************************************************/
		/*!
		\fn        AddComponent::m_GetID()
		\brief     Getter for the entity's ID.
		\return    The ID of the entity to which the component is added.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityChanged; }
	};

	/******************************************************************/
	/*!
	\class     RemoveComponent
	\brief     A derived class from BaseEvent class that represents removing a component from an entity.
	*/
	/******************************************************************/
	class RemoveComponent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityChanged; /*!< The ID of the entity from which the component is removed. */
		ecs::ComponentType m_removedType; /*!< The type of component being removed. */
	public:
		/******************************************************************/
		/*!
		\fn        RemoveComponent::RemoveComponent(ecs::EntityID inID, ecs::ComponentType inType)
		\brief     Constructor for creating a remove component event.
		\param[in] inID      The ID of the entity.
		\param[in] inType    The type of the component to be removed.
		*/
		/******************************************************************/
		RemoveComponent(ecs::EntityID inID, ecs::ComponentType inType) : BaseEvent<Actions>(Actions::REMOVECOMP), m_entityChanged(inID), m_removedType(inType) {}

		/******************************************************************/
		/*!
		\fn        RemoveComponent::m_GetComponentType()
		\brief     Getter for the type of the component removed.
		\return    The type of the removed component.
		*/
		/******************************************************************/
		ecs::ComponentType m_GetComponentType() { return m_removedType; }

		/******************************************************************/
		/*!
		\fn        RemoveComponent::m_GetID()
		\brief     Getter for the entity's ID.
		\return    The ID of the entity from which the component is removed.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityChanged; }
	};

	/******************************************************************/
	/*!
	\class     AddEntity
	\brief     A derived class from BaseEvent class that represents adding a new entity.
	*/
	/******************************************************************/
	class AddEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the newly added entity. */
	public:
		/******************************************************************/
		/*!
		\fn        AddEntity::AddEntity(ecs::EntityID inID)
		\brief     Constructor for creating an add entity event.
		\param[in] inID      The ID of the newly added entity.
		*/
		/******************************************************************/
		AddEntity(ecs::EntityID inID) : BaseEvent<Actions>(Actions::ADDENT), m_entityID(inID) {}

		/******************************************************************/
		/*!
		\fn        AddEntity::m_GetID()
		\brief     Getter for the ID of the added entity.
		\return    The ID of the newly added entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }
	};

	/******************************************************************/
	/*!
	\class     RemoveEntity
	\brief     A derived class from BaseEvent class that represents removing an entity.
	*/
	/******************************************************************/
	class RemoveEntity : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity to be removed. */
	public:
		/******************************************************************/
		/*!
		\fn        RemoveEntity::RemoveEntity(ecs::EntityID inID)
		\brief     Constructor for creating a remove entity event.
		\param[in] inID      The ID of the entity to be removed.
		*/
		/******************************************************************/
		RemoveEntity(ecs::EntityID inID) : BaseEvent<Actions>(Actions::DELENT), m_entityID(inID) {}

		/******************************************************************/
		/*!
		\fn        RemoveEntity::m_GetID()
		\brief     Getter for the ID of the removed entity.
		\return    The ID of the removed entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }
	};

	/******************************************************************/
	/*!
	\class     MoveEntityChildToChild
	\brief     A derived class from BaseEvent class that represents moving an entity from one parent entity to another parent entity.
	*/
	/******************************************************************/
	class MoveEntityChildToChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity being moved. */
		ecs::EntityID m_prevParent; /*!< The ID of the previous parent entity. */
		ecs::EntityID m_newParent; /*!< The ID of the new parent entity. */
	public:
		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::MoveEntityChildToChild(ecs::EntityID inID, ecs::EntityID inOld, ecs::EntityID inNew)
		\brief     Constructor for creating a move entity child-to-child event.
		\param[in] inID       The ID of the entity being moved.
		\param[in] inOld      The ID of the previous parent entity.
		\param[in] inNew      The ID of the new parent entity.
		*/
		/******************************************************************/
		MoveEntityChildToChild(ecs::EntityID inID, ecs::EntityID inOld, ecs::EntityID inNew) : BaseEvent<Actions>(Actions::MOVECTC), m_entityID(inID), m_prevParent(inOld), m_newParent(inNew) {}

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::m_GetID()
		\brief     Getter for the ID of the moved entity.
		\return    The ID of the moved entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::m_GetOldParentID()
		\brief     Getter for the ID of the previous parent entity.
		\return    The ID of the previous parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetOldParentID() { return m_prevParent; }

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToChild::m_GetNewParentID()
		\brief     Getter for the ID of the new parent entity.
		\return    The ID of the new parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetNewParentID() { return m_newParent; }
	};

	/******************************************************************/
	/*!
	\class     MoveEntityChildToParent
	\brief     A derived class from BaseEvent class that represents moving an entity from a child-parent relationship to a standalone parent entity.
	*/
	/******************************************************************/
	class MoveEntityChildToParent : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity being moved. */
		ecs::EntityID m_prevParent; /*!< The ID of the previous parent entity. */
	public:
		/******************************************************************/
		/*!
		\fn        MoveEntityChildToParent::MoveEntityChildToParent(ecs::EntityID inID, ecs::EntityID inOld)
		\brief     Constructor for creating a move entity child-to-parent event.
		\param[in] inID       The ID of the entity being moved.
		\param[in] inOld      The ID of the previous parent entity.
		*/
		/******************************************************************/
		MoveEntityChildToParent(ecs::EntityID inID, ecs::EntityID inOld) : BaseEvent<Actions>(Actions::MOVECTP), m_entityID(inID), m_prevParent(inOld) {}

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToParent::m_GetID()
		\brief     Getter for the ID of the moved entity.
		\return    The ID of the moved entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }

		/******************************************************************/
		/*!
		\fn        MoveEntityChildToParent::m_GetOldParentID()
		\brief     Getter for the ID of the previous parent entity.
		\return    The ID of the previous parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetOldParentID() { return m_prevParent; }
	};


	/******************************************************************/
	/*!
	\class     MoveEntityParentToChild
	\brief     A derived class from BaseEvent class that represents moving a standalone parent entity to become a child entity of another parent.
	*/
	/******************************************************************/
	class MoveEntityParentToChild : public BaseEvent<Actions> {
	private:
		ecs::EntityID m_entityID; /*!< The ID of the entity being moved. */
		ecs::EntityID m_newParent; /*!< The ID of the new parent entity. */
	public:
		/******************************************************************/
		/*!
		\fn        MoveEntityParentToChild::MoveEntityParentToChild(ecs::EntityID inID, ecs::EntityID inNew)
		\brief     Constructor for creating a move entity parent-to-child event.
		\param[in] inID       The ID of the entity being moved.
		\param[in] inNew      The ID of the new parent entity.
		*/
		/******************************************************************/
		MoveEntityParentToChild(ecs::EntityID inID, ecs::EntityID inNew) : BaseEvent<Actions>(Actions::MOVEPTC), m_entityID(inID), m_newParent(inNew) {}

		/******************************************************************/
		/*!
		\fn        MoveEntityParentToChild::m_GetID()
		\brief     Getter for the ID of the moved entity.
		\return    The ID of the moved entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetID() { return m_entityID; }

		/******************************************************************/
		/*!
		\fn        MoveEntityParentToChild::m_GetNewParentID()
		\brief     Getter for the ID of the new parent entity.
		\return    The ID of the new parent entity.
		*/
		/******************************************************************/
		ecs::EntityID m_GetNewParentID() { return m_newParent; }
	};

	/******************************************************************/
	/*!
	\class     UndoLatest
	\brief     A derived class from BaseEvent class that represents an undo action for the most recent change.
	*/
	/******************************************************************/
	class UndoLatest : public BaseEvent<Actions> {
	public:
		/******************************************************************/
		/*!
		\fn        UndoLatest::UndoLatest()
		\brief     Constructor for creating an undo action event.
		*/
		/******************************************************************/
		UndoLatest() : BaseEvent<Actions>(Actions::UNDO) {}
	};


	/******************************************************************/
	/*!
	\class     RedoPrevious
	\brief     A derived class from BaseEvent class that represents a redo action for the last undone change.
	*/
	/******************************************************************/
	class RedoPrevious : public BaseEvent<Actions> {
	public:
		/******************************************************************/
		/*!
		\fn        RedoPrevious::RedoPrevious()
		\brief     Constructor for creating a redo action event.
		*/
		/******************************************************************/
		RedoPrevious() : BaseEvent<Actions>(Actions::REDO) {}
	};

	class ModifyAnim : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::AnimationComponent* m_changedComp;
		int m_oldFrameNum;
		int m_oldFPS;
		float m_oldFrameTimer;
		bool m_oldIsAnim;
		int m_oldStrip;
	public:
		ModifyAnim(ecs::ComponentType inType, ecs::EntityID inID, ecs::AnimationComponent* inComp, ecs::AnimationComponent inOld) : BaseEvent<Actions>(Actions::MODIFYANIM),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp), m_oldFrameNum(inOld.m_frameNumber), m_oldFPS(inOld.m_framesPerSecond),
			m_oldFrameTimer(inOld.m_frameTimer), m_oldIsAnim(inOld.m_isAnimating),m_oldStrip(inOld.m_stripCount) {}
		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::AnimationComponent* m_GetComp() { return m_changedComp; }
		int m_GetOldFrameNum() { return m_oldFrameNum; }
		int m_GetOldFPS() { return m_oldFPS; }
		float m_GetOldFT() { return m_oldFrameTimer; }
		bool m_GetOldIsAnim() { return m_oldIsAnim; }
		int m_GetOldStrip() { return m_oldStrip; }
	};

	class ModifyCamera : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::CameraComponent* m_changedComp;
		float m_oldLeft, m_oldRight, m_oldTop, m_oldBottom, m_oldAspectRatio;

	public:
		ModifyCamera(ecs::ComponentType inType, ecs::EntityID inID, ecs::CameraComponent* inComp, ecs::CameraComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYCAMERA),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldLeft(inOld.m_left), m_oldRight(inOld.m_right),
			m_oldTop(inOld.m_top), m_oldBottom(inOld.m_bottom),
			m_oldAspectRatio(inOld.m_aspectRatio) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::CameraComponent* m_GetComp() { return m_changedComp; }
		float m_GetOldLeft() { return m_oldLeft; }
		float m_GetOldRight() { return m_oldRight; }
		float m_GetOldTop() { return m_oldTop; }
		float m_GetOldBottom() { return m_oldBottom; }
		float m_GetOldAspectRatio() { return m_oldAspectRatio; }
	};

	class ModifyCollider : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::ColliderComponent* m_changedComp;
		vector2::Vec2 m_oldSize;
		vector2::Vec2 m_oldOffset;
		bool m_oldDrawDebug, m_oldCollisionResponse, m_oldCollisionCheck;
		float m_oldRadius;
		physicspipe::EntityType m_oldShape;

	public:
		ModifyCollider(ecs::ComponentType inType, ecs::EntityID inID, ecs::ColliderComponent* inComp, ecs::ColliderComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYCOLL),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldSize(inOld.m_Size), m_oldOffset(inOld.m_OffSet), m_oldDrawDebug(inOld.m_drawDebug),
			m_oldRadius(inOld.m_radius), m_oldCollisionResponse(inOld.m_collisionResponse),
			m_oldCollisionCheck(inOld.m_collisionCheck), m_oldShape(inOld.m_type) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::ColliderComponent* m_GetComp() { return m_changedComp; }
		vector2::Vec2 m_GetOldSize() { return m_oldSize; }
		vector2::Vec2 m_GetOldOffset() { return m_oldOffset; }
		bool m_GetOldDrawDebug() { return m_oldDrawDebug; }
		float m_GetOldRadius() { return m_oldRadius; }
		bool m_GetOldCollisionResponse() { return m_oldCollisionResponse; }
		bool m_GetOldCollisionCheck() { return m_oldCollisionCheck; }
		physicspipe::EntityType m_GetOldShape() { return m_oldShape; }
	};

	class ModifyEnemy : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::EnemyComponent* m_changedComp;
		int m_oldTag, m_oldType, m_oldBehavior;

	public:
		ModifyEnemy(ecs::ComponentType inType, ecs::EntityID inID, ecs::EnemyComponent* inComp, ecs::EnemyComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYENEMY),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldTag(inOld.m_enemyTag), m_oldType(inOld.m_enemyTypeInt),
			m_oldBehavior(inOld.m_enemyRoamBehaviourInt) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::EnemyComponent* m_GetComp() { return m_changedComp; }
		int m_GetOldTag() { return m_oldTag; }
		int m_GetOldType() { return m_oldType; }
		int m_GetOldBehavior() { return m_oldBehavior; }
	};

	class ModifyGrid : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::GridComponent* m_changedComp;
		vector2::Vec2 m_oldAnchor;
		int m_oldRowLength, m_oldColumnLength, m_oldGridKey;
		bool m_oldCollidable;

	public:
		ModifyGrid(ecs::ComponentType inType, ecs::EntityID inID, ecs::GridComponent* inComp, ecs::GridComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYGRID),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldAnchor(inOld.m_Anchor), m_oldRowLength(inOld.m_GridRowLength),
			m_oldColumnLength(inOld.m_GridColumnLength), m_oldCollidable(inOld.m_SetCollidable),
			m_oldGridKey(inOld.m_GridKey) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::GridComponent* m_GetComp() { return m_changedComp; }
		vector2::Vec2 m_GetOldAnchor() { return m_oldAnchor; }
		int m_GetOldRowLength() { return m_oldRowLength; }
		int m_GetOldColumnLength() { return m_oldColumnLength; }
		bool m_GetOldCollidable() { return m_oldCollidable; }
		int m_GetOldGridKey() { return m_oldGridKey; }
	};

	class ModifyLight : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::LightingComponent* m_changedComp;
		graphicpipe::LightType m_oldType;
		float m_oldIntensity, m_oldRot;
		vector2::Vec2 m_oldScale;
		vector2::Vec2 m_oldOffset;
		vector2::Vec2 m_oldInnerOuterRadius;
		vector3::Vec3 m_oldColor;

	public:
		ModifyLight(ecs::ComponentType inType, ecs::EntityID inID, ecs::LightingComponent* inComp, ecs::LightingComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYLIGHT),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldType(inOld.m_lightType), m_oldIntensity(inOld.m_intensity), m_oldRot(inOld.m_light_rotation), m_oldOffset(inOld.m_light_OffSet), m_oldScale(inOld.m_light_scale),
			m_oldInnerOuterRadius(inOld.m_innerOuterRadius), m_oldColor(inOld.m_colour) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::LightingComponent* m_GetComp() { return m_changedComp; }
		graphicpipe::LightType m_GetOldType() { return m_oldType; }
		float m_GetOldIntensity() { return m_oldIntensity; }
		float m_GetOldRot() { return m_oldRot; }
		vector2::Vec2 m_GetOldInnerOuterRadius() { return m_oldInnerOuterRadius; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }
		vector2::Vec2 m_GetOldScale() { return m_oldScale; }
		vector2::Vec2 m_GetOldOffset() { return m_oldOffset; }
	};

	class ModifyPathfinding : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::PathfindingComponent* m_changedComp;
		vector2::Vec2 m_oldStartPos, m_oldEndPos;
		int m_oldGridKey;

	public:
		ModifyPathfinding(ecs::ComponentType inType, ecs::EntityID inID, ecs::PathfindingComponent* inComp, ecs::PathfindingComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYPATHFINDING),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldStartPos(inOld.m_StartPos), m_oldEndPos(inOld.m_TargetPos),
			m_oldGridKey(inOld.m_GridKey) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::PathfindingComponent* m_GetComp() { return m_changedComp; }
		ecs::EntityID m_GetID() { return m_entityID; }
		vector2::Vec2 m_GetOldStartPos() { return m_oldStartPos; }
		vector2::Vec2 m_GetOldEndPos() { return m_oldEndPos; }
		int m_GetOldGridKey() { return m_oldGridKey; }
	};

	class ModifyText : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::TextComponent* m_changedComp;
		std::string m_oldFile;
		std::string m_oldText;
		int m_oldLayer;
		float m_oldSize;
		vector3::Vec3 m_oldColor;

	public:
		ModifyText(ecs::ComponentType inType, ecs::EntityID inID, ecs::TextComponent* inComp, ecs::TextComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYTEXT),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldFile(inOld.m_fileName), m_oldText(inOld.m_text),
			m_oldLayer(inOld.m_fontLayer), m_oldSize(inOld.m_fontSize),
			m_oldColor(inOld.m_color) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetOldFile() { return m_oldFile; }
		std::string m_GetOldText() { return m_oldText; }
		int m_GetOldLayer() { return m_oldLayer; }
		float m_GetOldSize() { return m_oldSize; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }

		// New function
		ecs::TextComponent* m_GetComp() { return m_changedComp; }
	};

	class ModifyRigid : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::RigidBodyComponent* m_changedComp;
		vector2::Vec2 m_oldVelocity;
		vector2::Vec2 m_oldAcceleration;
		float m_oldRotation;
		float m_oldAngularVelocity;
		float m_oldAngularAcceleration;
		float m_oldMass;
		float m_oldInverseMass;
		float m_oldLinearDamping;
		float m_oldAngularDamping;
		vector2::Vec2 m_oldForce;
		float m_oldTorque;
		bool m_oldKinematic;
		bool m_oldStatic;

	public:
		ModifyRigid(ecs::ComponentType inType, ecs::EntityID inID, ecs::RigidBodyComponent* inComp, ecs::RigidBodyComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYRIGID),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldVelocity(inOld.m_Velocity), m_oldAcceleration(inOld.m_Acceleration),
			m_oldRotation(inOld.m_Rotation), m_oldAngularVelocity(inOld.m_AngularVelocity),
			m_oldAngularAcceleration(inOld.m_AngularAcceleration), m_oldMass(inOld.m_Mass),
			m_oldInverseMass(inOld.m_InverseMass), m_oldLinearDamping(inOld.m_LinearDamping),
			m_oldAngularDamping(inOld.m_AngularDamping), m_oldForce(inOld.m_Force),
			m_oldTorque(inOld.m_Torque), m_oldKinematic(inOld.m_IsKinematic), m_oldStatic(inOld.m_IsStatic) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		vector2::Vec2 m_GetOldVelocity() { return m_oldVelocity; }
		vector2::Vec2 m_GetOldAcceleration() { return m_oldAcceleration; }
		float m_GetOldRotation() { return m_oldRotation; }
		float m_GetOldAngularVelocity() { return m_oldAngularVelocity; }
		float m_GetOldAngularAcceleration() { return m_oldAngularAcceleration; }
		float m_GetOldMass() { return m_oldMass; }
		float m_GetOldInverseMass() { return m_oldInverseMass; }
		float m_GetOldLinearDamping() { return m_oldLinearDamping; }
		float m_GetOldAngularDamping() { return m_oldAngularDamping; }
		vector2::Vec2 m_GetOldForce() { return m_oldForce; }
		float m_GetOldTorque() { return m_oldTorque; }
		bool m_GetOldKinematic() { return m_oldKinematic; }
		bool m_GetOldStatic() { return m_oldStatic; }

		// New function
		ecs::RigidBodyComponent* m_GetComp() { return m_changedComp; }
	};

	class ModifySprite : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::SpriteComponent* m_changedComp;
		std::string m_oldFile;
		int m_oldLayer;
		vector3::Vec3 m_oldColor;
		float m_oldAlpha;
		bool m_oldIlluminated;

	public:
		ModifySprite(ecs::ComponentType inType, ecs::EntityID inID, ecs::SpriteComponent* inComp, ecs::SpriteComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYSPRITE),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldFile(inOld.m_imageFile), m_oldLayer(inOld.m_layer),
			m_oldColor(inOld.m_color), m_oldAlpha(inOld.m_alpha),
			m_oldIlluminated(inOld.m_isIlluminated) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetOldFile() { return m_oldFile; }
		int m_GetOldLayer() { return m_oldLayer; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }
		float m_GetOldAlpha() { return m_oldAlpha; }
		bool m_GetOldIlluminated() { return m_oldIlluminated; }

		// New function
		ecs::SpriteComponent* m_GetComp() { return m_changedComp; }
	};

	class ModifyButton : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::ButtonComponent* m_changedComp;
		vector2::Vec2 m_oldPos;
		vector2::Vec2 m_oldScale;
		bool m_oldClick;

	public:
		ModifyButton(ecs::ComponentType inType, ecs::EntityID inID, ecs::ButtonComponent* inComp, ecs::ButtonComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYSPRITE),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldPos(inOld.m_Position), m_oldScale(inOld.m_Scale),
			m_oldClick(inOld.m_IsClick){}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		vector2::Vec2 m_GetOldPos() { return m_oldPos; }
		vector2::Vec2 m_GetOldScale() { return m_oldScale; }
		bool m_GetOldClick() { return m_oldClick; }

		// New function
		ecs::ButtonComponent* m_GetComp() { return m_changedComp; }
	};
	class ModifyVideo : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::VideoComponent* m_changedComp;
		std::string m_oldFilename;
		bool m_oldPause;
		bool m_oldLoop;
		int m_oldLayer;

	public:
		ModifyVideo(ecs::ComponentType inType, ecs::EntityID inID, ecs::VideoComponent* inComp, ecs::VideoComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYVIDEO),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldFilename(inOld.filename), m_oldPause(inOld.pause),
			m_oldLoop(inOld.loop), m_oldLayer(inOld.layer) {}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::VideoComponent* m_GetComp() { return m_changedComp; }
		std::string m_GetOldFilename() { return m_oldFilename; }
		bool m_GetOldPause() { return m_oldPause; }
		bool m_GetOldLoop() { return m_oldLoop; }
		int m_GetOldLayer() { return m_oldLayer; }
	};

	class ModifyTilemap : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::TilemapComponent* m_changedComp;
		std::string m_oldTilemapFile;
		int m_oldTileLayer;
		vector3::Vec3 m_oldColor;
		float m_oldAlpha;
		int m_oldTileIndex, m_oldRowLength, m_oldColumnLength, m_oldPictureRowLength, m_oldPictureColumnLength;

	public:
		ModifyTilemap(ecs::ComponentType inType, ecs::EntityID inID, ecs::TilemapComponent* inComp, ecs::TilemapComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYTILEMAP),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldTilemapFile(inOld.m_tilemapFile), m_oldTileLayer(inOld.m_tileLayer),
			m_oldColor(inOld.m_color), m_oldAlpha(inOld.m_alpha), m_oldTileIndex(inOld.m_tileIndex),
			m_oldRowLength(inOld.m_rowLength), m_oldColumnLength(inOld.m_columnLength), m_oldPictureRowLength(inOld.m_pictureRowLength), m_oldPictureColumnLength(inOld.m_pictureColumnLength){}

		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::TilemapComponent* m_GetComp() { return m_changedComp; }
		std::string m_GetOldTilemapFile() { return m_oldTilemapFile; }
		int m_GetOldTileLayer() { return m_oldTileLayer; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }
		float m_GetOldAlpha() { return m_oldAlpha; }
		int m_GetOldTileIndex() { return m_oldTileIndex; }
		int m_GetOldRowLength() { return m_oldRowLength; }
		int m_GetOldColumnLength() { return m_oldColumnLength; }
		int m_GetOldPictureRowLength() { return m_oldPictureRowLength; }
		int m_GetOldPictureColumnLength() { return m_oldPictureColumnLength; }
	};

	class ModifyParticle : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::ParticleComponent* m_changedComp;
		bool m_oldWillSpawn;
		int m_oldNoOfParticles;
		float m_oldLifeSpan;
		vector2::Vec2 m_oldVelocity;
		vector2::Vec2 m_oldAcceleration;
		vector3::Vec3 m_oldColor;
		float m_oldConeRotation;
		float m_oldConeAngle;
		float m_oldRandomFactor;
		std::string m_oldImageFile;
		int m_oldStripCount;
		int m_oldFrameNumber;
		int m_oldLayer;
		float m_oldFriction;
		int m_oldFps;

	public:
		ModifyParticle(ecs::ComponentType inType, ecs::EntityID inID, ecs::ParticleComponent* inComp, ecs::ParticleComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYPARTICLE),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldWillSpawn(inOld.m_willSpawn), m_oldNoOfParticles(inOld.m_noOfParticles),
			m_oldLifeSpan(inOld.m_lifeSpan), m_oldVelocity(inOld.m_velocity),
			m_oldAcceleration(inOld.m_acceleration), m_oldColor(inOld.m_color),
			m_oldConeRotation(inOld.m_coneRotation), m_oldConeAngle(inOld.m_coneAngle),
			m_oldRandomFactor(inOld.m_randomFactor), m_oldImageFile(inOld.m_imageFile),
			m_oldStripCount(inOld.m_stripCount), m_oldFrameNumber(inOld.m_frameNumber),
			m_oldLayer(inOld.m_layer), m_oldFriction(inOld.m_friction), m_oldFps(inOld.m_fps) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		bool m_GetOldWillSpawn() { return m_oldWillSpawn; }
		int m_GetOldNoOfParticles() { return m_oldNoOfParticles; }
		float m_GetOldLifeSpan() { return m_oldLifeSpan; }
		vector2::Vec2 m_GetOldVelocity() { return m_oldVelocity; }
		vector2::Vec2 m_GetOldAcceleration() { return m_oldAcceleration; }
		vector3::Vec3 m_GetOldColor() { return m_oldColor; }
		float m_GetOldConeRotation() { return m_oldConeRotation; }
		float m_GetOldConeAngle() { return m_oldConeAngle; }
		float m_GetOldRandomFactor() { return m_oldRandomFactor; }
		std::string m_GetOldImageFile() { return m_oldImageFile; }
		int m_GetOldStripCount() { return m_oldStripCount; }
		int m_GetOldFrameNumber() { return m_oldFrameNumber; }
		int m_GetOldLayer() { return m_oldLayer; }
		float m_GetOldFriction() { return m_oldFriction; }
		int m_GetOldFps() { return m_oldFps; }

		ecs::ParticleComponent* m_GetComp() { return m_changedComp; }
	};

	class ModifyName : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::NameComponent* m_changedComp;
		std::string m_oldName;
		std::string m_oldTag;
		bool m_oldIsPrefab;
		std::string m_oldPrefabName;
		layer::LAYERS m_oldLayer;

	public:
		ModifyName(ecs::ComponentType inType, ecs::EntityID inID, ecs::NameComponent* inComp, ecs::NameComponent inOld)
			: BaseEvent<Actions>(Actions::MODIFYNAME),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_oldName(inOld.m_entityName), m_oldTag(inOld.m_entityTag),
			m_oldIsPrefab(inOld.m_isPrefab), m_oldPrefabName(inOld.m_prefabName),
			m_oldLayer(inOld.m_Layer) {}

		ecs::ComponentType m_GetType() { return m_changedType; }
		ecs::EntityID m_GetID() { return m_entityID; }
		std::string m_GetOldName() { return m_oldName; }
		std::string m_GetOldTag() { return m_oldTag; }
		bool m_GetOldIsPrefab() { return m_oldIsPrefab; }
		std::string m_GetOldPrefabName() { return m_oldPrefabName; }
		layer::LAYERS m_GetOldLayer() { return m_oldLayer; }

		ecs::NameComponent* m_GetComp() { return m_changedComp; }
	};

	class AddAudio : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::AudioComponent* m_changedComp;
		std::string m_name;
		std::string m_filePath;
		float m_volume;
		bool m_loop;
		bool m_playOnStart, m_hasPlayed;
		float m_pan;
		bool m_isSFX, m_isBGM;

	public:
		AddAudio(ecs::ComponentType inType, ecs::EntityID inID, ecs::AudioComponent* inComp, ecs::AudioComponent inOld)
			: BaseEvent<Actions>(Actions::ADDAUDIO),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_name(inComp->m_AudioFiles.back().m_Name), m_filePath(inComp->m_AudioFiles.back().m_FilePath),
			m_volume(inComp->m_AudioFiles.back().m_Volume), m_loop(inComp->m_AudioFiles.back().m_Loop),
			m_playOnStart(inComp->m_AudioFiles.back().m_PlayOnStart), m_hasPlayed(inComp->m_AudioFiles.back().m_HasPlayed), m_pan(inComp->m_AudioFiles.back().m_Pan),
			m_isSFX(inComp->m_AudioFiles.back().m_IsSFX), m_isBGM(inComp->m_AudioFiles.back().m_IsBGM){}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::AudioComponent* m_GetComp() { return m_changedComp; }
		std::string m_GetOldFilename() { return m_name; }
		std::string m_GetOldFilePath() { return m_filePath; }
		bool m_GetOldPlayOnStart() { return m_playOnStart; }
		bool m_GetOldLoop() { return m_loop; }
		bool m_GetOldHasPlayed() { return m_hasPlayed; }
		bool m_GetOldSFX() { return m_isSFX; }
		bool m_GetOldBGM() { return m_isBGM; }
		float m_GetOldVol() { return m_volume; }
		float m_GetOldPan() { return m_pan; }
	};

	class RemoveAudio : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::AudioComponent* m_changedComp;
		std::string m_name;
		std::string m_filePath;
		float m_volume;
		bool m_loop;
		bool m_playOnStart, m_hasPlayed;
		float m_pan;
		bool m_isSFX, m_isBGM;


	public:
		RemoveAudio(ecs::ComponentType inType, ecs::EntityID inID, ecs::AudioComponent* inComp, ecs::AudioComponent inOld)
			: BaseEvent<Actions>(Actions::REMOVEAUDIO),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_name(inOld.m_AudioFiles.back().m_Name), m_filePath(inOld.m_AudioFiles.back().m_FilePath),
			m_volume(inOld.m_AudioFiles.back().m_Volume), m_loop(inOld.m_AudioFiles.back().m_Loop),
			m_playOnStart(inOld.m_AudioFiles.back().m_PlayOnStart), m_hasPlayed(inOld.m_AudioFiles.back().m_HasPlayed), m_pan(inOld.m_AudioFiles.back().m_Pan),
			m_isSFX(inOld.m_AudioFiles.back().m_IsSFX), m_isBGM(inOld.m_AudioFiles.back().m_IsBGM){}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::AudioComponent* m_GetComp() { return m_changedComp; }
		std::string m_GetOldFilename() { return m_name; }
		std::string m_GetOldFilePath() { return m_filePath; }
		bool m_GetOldPlayOnStart() { return m_playOnStart; }
		bool m_GetOldLoop() { return m_loop; }
		bool m_GetOldHasPlayed() { return m_hasPlayed; }
		float m_GetOldVol() { return m_volume; }
		float m_GetOldPan() { return m_pan; }
		bool m_GetOldSFX() { return m_isSFX; }
		bool m_GetOldBGM() { return m_isBGM; }
	};

	class ModifyAudio : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		std::vector<ecs::AudioFile>::iterator m_changedComp;
		std::string m_name;
		std::string m_filePath;
		float m_volume;
		bool m_loop;
		bool m_playOnStart, m_hasPlayed;
		float m_pan;
		bool m_isSFX, m_isBGM;


	public:
		ModifyAudio(ecs::ComponentType inType, ecs::EntityID inID, std::vector<ecs::AudioFile>::iterator inComp, std::vector<ecs::AudioFile>::iterator inOld)
			: BaseEvent<Actions>(Actions::MODIFYAUDIO),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_name(inOld->m_Name), m_filePath(inOld->m_FilePath),
			m_volume(inOld->m_Volume), m_loop(inOld->m_Loop),
			m_playOnStart(inOld->m_PlayOnStart), m_hasPlayed(inOld->m_HasPlayed), m_pan(inOld->m_Pan),
			m_isSFX(inOld->m_IsSFX), m_isBGM(inOld->m_IsBGM) {}
		ecs::EntityID m_GetID() { return m_entityID; }
		std::vector<ecs::AudioFile>::iterator m_GetComp() { return m_changedComp; }
		std::string m_GetOldFilename() { return m_name; }
		std::string m_GetOldFilePath() { return m_filePath; }
		bool m_GetOldPlayOnStart() { return m_playOnStart; }
		bool m_GetOldLoop() { return m_loop; }
		bool m_GetOldHasPlayed() { return m_hasPlayed; }
		float m_GetOldVol() { return m_volume; }
		float m_GetOldPan() { return m_pan; }
		bool m_GetOldSFX() { return m_isSFX; }
		bool m_GetOldBGM() { return m_isBGM; }
	};

	class AddRay : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::RaycastComponent* m_changedComp;
		std::string m_rayID;

		bool m_isRaycasting;

		vector2::Vec2 m_targetPosition;

		std::vector<layer::LAYERS> m_Layers;



	public:
		AddRay(ecs::ComponentType inType, ecs::EntityID inID, ecs::RaycastComponent* inComp, ecs::RaycastComponent inOld)
			: BaseEvent<Actions>(Actions::ADDRAY),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_rayID(inComp->m_raycast.back().m_rayID), m_isRaycasting(inComp->m_raycast.back().m_isRaycasting),
			m_targetPosition(inComp->m_raycast.back().m_targetPosition), m_Layers(inComp->m_raycast.back().m_Layers){}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::RaycastComponent* m_GetComp() { return m_changedComp; }
		std::string m_GetOldRayID() { return m_rayID; }
		bool m_GetOldIsRayCasting() { return m_isRaycasting; }
		vector2::Vec2 m_GetOldTargetPos() { return m_targetPosition; }
		std::vector<layer::LAYERS> m_GetOldLayers() { return m_Layers; }

	};

	class RemoveRay : public BaseEvent<Actions> {
	private:
		ecs::ComponentType m_changedType;
		ecs::EntityID m_entityID;
		ecs::RaycastComponent* m_changedComp;
		std::string m_rayID;

		bool m_isRaycasting;

		vector2::Vec2 m_targetPosition;

		std::vector<layer::LAYERS> m_Layers;



	public:
		RemoveRay(ecs::ComponentType inType, ecs::EntityID inID, ecs::RaycastComponent* inComp, ecs::RaycastComponent inOld)
			: BaseEvent<Actions>(Actions::REMOVERAY),
			m_changedType(inType), m_entityID(inID), m_changedComp(inComp),
			m_rayID(inOld.m_raycast.back().m_rayID), m_isRaycasting(inOld.m_raycast.back().m_isRaycasting),
			m_targetPosition(inOld.m_raycast.back().m_targetPosition), m_Layers(inOld.m_raycast.back().m_Layers) {}
		ecs::EntityID m_GetID() { return m_entityID; }
		ecs::RaycastComponent* m_GetComp() { return m_changedComp; }
		std::string m_GetOldRayID() { return m_rayID; }
		bool m_GetOldIsRayCasting() { return m_isRaycasting; }
		vector2::Vec2 m_GetOldTargetPos() { return m_targetPosition; }
		std::vector<layer::LAYERS> m_GetOldLayers() { return m_Layers; }

	};

}
