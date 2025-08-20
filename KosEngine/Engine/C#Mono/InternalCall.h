/********************************************************************/
/*!
\file      InternalCall.h
\author    Chiu Jun Jie, junjie.c , 2301524
\par       junjie.c@digipen.edu
\date      Nov 27, 2024
\brief     This header file defines the `InternalCall` class, which contains
		   functions that allow C# scripts to interact with ECS components.

Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/********************************************************************/

#ifndef INTCALL_H
#define INTCALL_H

#include"../ECS/ECS.h"
#include "../Inputs/Keycodes.h"

namespace script {

	class InternalCall {
		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
			\brief     Retrieves the transform component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the transform from.
			\param[out] trans  Pointer to the entity's position vector.
			\param[out] scale  Pointer to the entity's scale vector.
			\param[out] rotate Pointer to the entity's rotation value.
			\return    True if the transform component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate)
			\brief     Sets the transform component of a specified entity.
			\param[in] entity The ID of the entity to set the transform for.
			\param[in] trans  Pointer to the position vector to set.
			\param[in] scale  Pointer to the scale vector to set.
			\param[in] rotate Pointer to the rotation value to set.
			\return    True if the transform component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetTransformComponent(ecs::EntityID entity, vector2::Vec2* trans, vector2::Vec2* scale, float* rotate);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
			\brief     Retrieves the position of a specified entity.
			\param[in] entity The ID of the entity to retrieve the position from.
			\param[out] trans  Pointer to the position vector to store the retrieved position.
			\return    True if the position is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetTranslate(ecs::EntityID entity, vector2::Vec2* trans);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans)
			\brief     Sets the position of a specified entity.
			\param[in] entity The ID of the entity to set the position for.
			\param[in] trans  Pointer to the position vector to set.
			\return    True if the position is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetTranslate(ecs::EntityID entity, vector2::Vec2* trans);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, int* m_blockedFlag, float* isCollided, bool* collisionCheck)
			\brief     Retrieves the collider component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the collider from.
			\param[out] size   Pointer to the collider size vector.
			\param[out] offset Pointer to the collider offset vector.
			\param[out] drawDebug Pointer to whether debug drawing is enabled.
			\param[out] radius Pointer to the collider's radius.
			\param[out] m_blockedFlag Pointer to the entity's blocked flag.
			\param[out] isCollided Pointer to store the collision state.
			\param[out] collisionCheck Pointer to enable or disable collision checking.
			\return    True if the collider is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, unsigned int* m_blockedFlag, float* isCollided, bool* collisionCheck, bool* collisionresponse);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, int* m_blockedFlag, float* isCollided, bool* collisionCheck)
			\brief     Sets the collider component of a specified entity.
			\param[in] entity The ID of the entity to set the collider for.
			\param[in] size   Pointer to the collider size vector to set.
			\param[in] offset Pointer to the collider offset vector to set.
			\param[in] drawDebug Pointer to enable or disable debug drawing.
			\param[in] radius Pointer to the collider's radius to set.
			\param[in] m_blockedFlag Pointer to the entity's blocked flag to set.
			\param[in] isCollided Pointer to set the collision state.
			\param[in] collisionCheck Pointer to enable or disable collision checking.
			\return    True if the collider is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetColliderComponent(ecs::EntityID entity, vector2::Vec2* size, vector2::Vec2* offset, bool* drawDebug, float* radius, unsigned int* m_blockedFlag, float* isCollided, bool* collisionCheck, bool* collisionresponse);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetPlayerComponent(ecs::EntityID entity, bool* control)
			\brief     Retrieves the player component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the player component from.
			\param[out] control Pointer to the player control flag.
			\return    True if the player component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetEnemyComponent(ecs::EntityID entity, int* enemytag, int* enemytype, int* enemybehaviour);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetPlayerComponent(ecs::EntityID entity, bool control)
			\brief     Sets the player component of a specified entity.
			\param[in] entity The ID of the entity to set the player component for.
			\param[in] control The player control flag to set.
			\return    True if the player component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetEnemyComponent(ecs::EntityID entity, int* enemytag, int* enemytype, int* enemybehaviour);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation)
			\brief     Retrieves the rigid body component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the rigid body from.
			\param[out] velocity Pointer to the velocity vector.
			\param[out] acceleration Pointer to the acceleration vector.
			\param[out] rotation Pointer to the rotation value.
			\return    True if the rigid body component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation, vector2::Vec2* previouspos, vector2::Vec2* directionvector, vector2::Vec2* force);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation)
			\brief     Sets the rigid body component of a specified entity.
			\param[in] entity The ID of the entity to set the rigid body for.
			\param[in] velocity Pointer to the velocity vector to set.
			\param[in] acceleration Pointer to the acceleration vector to set.
			\param[in] rotation Pointer to the rotation value to set.
			\return    True if the rigid body component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetRigidBodyComponent(ecs::EntityID entity, vector2::Vec2* velocity, vector2::Vec2* acceleration, float* rotation, vector2::Vec2* previouspos, vector2::Vec2* directionvector, vector2::Vec2* force);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetTextComponent(ecs::EntityID entity, MonoString** text, MonoString** imageFile, int* fontLayer, float* fontSize, vector3::Vec3* color)
			\brief     Retrieves the text component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the text component from.
			\param[out] text Pointer to the text string.
			\param[out] imageFile Pointer to the image file name.
			\param[out] fontLayer Pointer to the font layer.
			\param[out] fontSize Pointer to the font size.
			\param[out] color Pointer to the text color vector.
			\return    True if the text component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetTextComponent(ecs::EntityID entity, MonoString** text, MonoString** imageFile, int* fontLayer, float* fontSize, vector3::Vec3* color);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetTextComponent(ecs::EntityID entity, MonoString* text, MonoString* fileName, int* fontLayer, float* fontSize, const vector3::Vec3* color)
			\brief     Sets the text component of a specified entity.
			\param[in] entity The ID of the entity to set the text component for.
			\param[in] text Pointer to the text string to set.
			\param[in] fileName Pointer to the image file name to set.
			\param[in] fontLayer Pointer to the font layer to set.
			\param[in] fontSize Pointer to the font size to set.
			\param[in] color Pointer to the text color vector to set.
			\return    True if the text component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetTextComponent(ecs::EntityID entity, MonoString* text, MonoString* fileName, int* fontLayer, float* fontSize, const vector3::Vec3* color);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount)
			\brief     Retrieves the animation component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the animation component from.
			\param[out] frameNumber Pointer to the frame number.
			\param[out] framesPerSecond Pointer to the frames per second.
			\param[out] frameTimer Pointer to the frame timer.
			\param[out] isAnimating Pointer to the animation state.
			\param[out] stripcount Pointer to the strip count.
			\return    True if the animation component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount)
			\brief     Sets the animation component of a specified entity.
			\param[in] entity The ID of the entity to set the animation component for.
			\param[in] frameNumber Pointer to the frame number to set.
			\param[in] framesPerSecond Pointer to the frames per second to set.
			\param[in] frameTimer Pointer to the frame timer to set.
			\param[in] isAnimating Pointer to the animation state to set.
			\param[in] stripcount Pointer to the strip count to set.
			\return    True if the animation component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetAnimationComponent(ecs::EntityID entity, int* frameNumber, int* framesPerSecond, float* frameTimer, bool* isAnimating, int* stripcount);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetSpriteComponent(ecs::EntityID entity, MonoString** imageFile, int* layer, vector3::Vec3* color, float* alpha)
			\brief     Retrieves the sprite component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the sprite component from.
			\param[out] imageFile Pointer to the image file name.
			\param[out] layer Pointer to the layer.
			\param[out] color Pointer to the color vector.
			\param[out] alpha Pointer to the alpha value.
			\return    True if the sprite component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetSpriteComponent(ecs::EntityID entity, MonoString** imageFile, int* layer, vector3::Vec3* color, float* alpha);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetSpriteComponent(ecs::EntityID entity, MonoString* imageFile, const int* layer, const vector3::Vec3* color, const float* alpha)
			\brief     Sets the sprite component of a specified entity.
			\param[in] entity The ID of the entity to set the sprite component for.
			\param[in] imageFile Pointer to the image file name to set.
			\param[in] layer Pointer to the layer to set.
			\param[in] color Pointer to the color vector to set.
			\param[in] alpha Pointer to the alpha value to set.
			\return    True if the sprite component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetSpriteComponent(ecs::EntityID entity, MonoString* imageFile, const int* layer, const vector3::Vec3* color, const float* alpha);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetCameraComponent(ecs::EntityID entity, float* left, float* right, float* top, float* bottom, float* aspectRatio)
			\brief     Retrieves the camera component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the camera component from.
			\param[out] left Pointer to the left boundary value.
			\param[out] right Pointer to the right boundary value.
			\param[out] top Pointer to the top boundary value.
			\param[out] bottom Pointer to the bottom boundary value.
			\param[out] aspectRatio Pointer to the aspect ratio value.
			\return    True if the camera component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetCameraComponent(ecs::EntityID entity, float* left, float* right, float* top, float* bottom, float* aspectRatio);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetCameraComponent(ecs::EntityID entity, float left, float right, float top, float bottom, float aspectRatio)
			\brief     Sets the camera component of a specified entity.
			\param[in] entity The ID of the entity to set the camera component for.
			\param[in] left The left boundary value to set.
			\param[in] right The right boundary value to set.
			\param[in] top The top boundary value to set.
			\param[in] bottom The bottom boundary value to set.
			\param[in] aspectRatio The aspect ratio value to set.
			\return    True if the camera component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetCameraComponent(ecs::EntityID entity, float left, float right, float top, float bottom, float aspectRatio);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalGetButtonComponent(ecs::EntityID entity, vector2::Vec2* position, vector2::Vec2* scale, bool* isClick)
			\brief     Retrieves the button component of a specified entity.
			\param[in] entity The ID of the entity to retrieve the button component from.
			\param[out] position Pointer to the button position vector.
			\param[out] scale Pointer to the button scale vector.
			\param[out] isClick Pointer to the button click state.
			\return    True if the button component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetButtonComponent(ecs::EntityID entity, vector2::Vec2* position, vector2::Vec2* scale, bool* isClick);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalIsButtonHovered(ecs::EntityID entity)
		\brief   Checks whether the button associated with the specified entity is currently hovered.
		\param   entity - The ID of the entity to check for hover status.
		\return  True if the button is hovered by the mouse cursor, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalIsButtonHovered(ecs::EntityID entity);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalSetButtonComponent(ecs::EntityID entity, const vector2::Vec2& position, const vector2::Vec2& scale, bool isClick)
			\brief     Sets the button component of a specified entity.
			\param[in] entity The ID of the entity to set the button component for.
			\param[in] position Reference to the position vector to set.
			\param[in] scale Reference to the scale vector to set.
			\param[in] isClick The button click state to set.
			\return    True if the button component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetButtonComponent(ecs::EntityID entity, const vector2::Vec2& position, const vector2::Vec2& scale, bool isClick);

		/******************************************************************/
		/*! \fn        MonoArray* InternalCall::m_InternalGetScriptNames(ecs::EntityID entity)
			\brief     Retrieves the script names associated with a specified entity.
			\param[in] entity The ID of the entity to retrieve script names from.
			\return    An array of script names if found; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalGetScriptNames(ecs::EntityID entity);

		/******************************************************************/
		/*! \fn        bool InternalCall::m_InternalAddScriptInstance(ecs::EntityID entity, MonoString* monoScriptName, MonoObject* instance)
			\brief     Adds a script instance to a specified entity.
			\param[in] entity The ID of the entity to add the script to.
			\param[in] monoScriptName Pointer to the name of the script.
			\param[in] instance Pointer to the script instance to add.
			\return    True if the script instance is added successfully; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalAddScriptInstance(ecs::EntityID entity, MonoString* monoScriptName, MonoObject* instance);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
			\brief     Retrieves the velocity of a specified entity.
			\param[in] entity The ID of the entity to retrieve the velocity from.
			\param[out] vec   Pointer to the entity's velocity vector.
			\return    True if the velocity component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetVelocity(ecs::EntityID entity, vector2::Vec2* vec);
		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec)
			\brief     Sets the velocity of a specified entity.
			\param[in] entity The ID of the entity to set the velocity for.
			\param[in] vec    Pointer to the velocity vector to set.
			\return    True if the velocity component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetVelocity(ecs::EntityID entity, vector2::Vec2* vec);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallGetDeltaTime(float* deltatime)
			\brief     Retrieves the fixed delta time.
			\param[out] deltatime Pointer to store the retrieved delta time value.
			\return    Always returns false for consistency in call handling.
		*/
		/******************************************************************/
		static float m_InternalCallGetDeltaTime();

		/******************************************************************/
		/*! \fn        int InternalCall::m_InternalCallGetTagID(MonoString* monostring)
			\brief     Retrieves the ID of the first entity associated with a specific tag.
			\param[in] monostring Pointer to the tag as a MonoString.
			\return    The ID of the first entity associated with the tag if found; otherwise, -1.
		*/
		/******************************************************************/
		static int m_InternalCallGetTagID(MonoString* tag);

		/******************************************************************/
		/*! \fn        MonoArray* InternalCall::m_InternalCallGetTagIDs(MonoString* monostring)
			\brief     Retrieves all entity IDs associated with a specific tag.
			\param[in] monostring Pointer to the tag as a MonoString.
			\return    An array of entity IDs associated with the tag if found; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetTagIDs(MonoString* tag);

		/******************************************************************/
		/*! \fn        MonoArray* InternalCall::m_InternalCallGetCollidedEntities(ecs::EntityID entity)
			\brief     Retrieves the IDs of entities collided with a specified entity.
			\param[in] entity The ID of the entity to check for collisions.
			\return    An array of entity IDs that the specified entity has collided with if any; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetCollidedEntities(ecs::EntityID entity);

		/******************************************************************/
		/*! \fn        MonoString* InternalCall::m_InternalCallGetTag(ecs::EntityID entity)
			\brief     Retrieves the tag associated with a specified entity.
			\param[in] entity The ID of the entity to retrieve the tag for.
			\return    A MonoString containing the tag of the specified entity if found; otherwise, null.
		*/
		/******************************************************************/
		static MonoString* m_InternalCallGetTag(ecs::EntityID entity);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallSetSceneActive(MonoString* monoString)
			\brief     Activates the specified scene in the application.
			\param[in] monoString Pointer to the MonoString containing the name of the scene to activate.
		*/
		/******************************************************************/
		static void m_InternalCallSetSceneActive(MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_UnloadAllScene()
			\brief     Unloads all currently loaded scenes from the application.
		*/
		/******************************************************************/
		static void m_UnloadAllScene();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallLoadScene(MonoString* monoString)
			\brief     Loads a specified scene into the application.
			\param[in] monoString Pointer to the MonoString containing the name of the scene to load.
		*/
		/******************************************************************/
		static void m_InternalCallLoadScene(MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        int InternalCall::m_InternalCallAddPrefab(MonoString* prefab, const float* x, const float* y, const float* rotation)
			\brief     Adds a prefab instance at the specified position and rotation.
			\param[in] prefab Pointer to the MonoString containing the prefab name.
			\param[in] x Pointer to the x-coordinate for the prefab's position.
			\param[in] y Pointer to the y-coordinate for the prefab's position.
			\param[in] rotation Pointer to the rotation value for the prefab.
			\return    An integer representing the ID of the newly added prefab instance.
		*/
		/******************************************************************/
		static int m_InternalCallAddPrefab(MonoString* prefab, const float* x, const float* y, const float* rotation);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallDeleteEntity(ecs::EntityID entity)
			\brief     Deletes the specified entity from the ECS system.
			\param[in] entity The ID of the entity to delete.
		*/
		/******************************************************************/
		static void m_InternalCallDeleteEntity(ecs::EntityID entity);

		/******************************************************************/
		/*!
			\fn        float InternalCall::m_InternalCallIsCollided(ecs::EntityID entity)
			\brief     Checks whether the specified entity is involved in a collision.
			\param[in] entity The ID of the entity to check for collisions.
			\return    A float representing collision status: 1.0 for collided, 0.0 otherwise.
		*/
		/******************************************************************/
		static float m_InternalCallIsCollided(ecs::EntityID entity);

		/******************************************************************/
		/*!
			\fn        vector2::Vec2 InternalCall::m_InternalGetMousePosition()
			\brief     Retrieves the current position of the mouse in world coordinates.
			\return    A vector2::Vec2 object containing the mouse's position.
		*/
		/******************************************************************/
		static void m_InternalGetMousePosition(vector2::Vec2*);


		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyPressed(keyCode key)
			\brief     Checks if a specific key is currently pressed.
			\param[in] key The key code to check.
			\return    True if the specified key is pressed; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyPressed(keyCode key);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyReleased(keyCode key)
			\brief     Checks if a specific key has been released.
			\param[in] key The key code to check.
			\return    True if the specified key has been released; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyReleased(keyCode key);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallIsKeyTriggered(keyCode key)
			\brief     Checks if a specific key has been triggered (pressed and then released).
			\param[in] key The key code to check.
			\return    True if the specified key has been triggered; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsKeyTriggered(keyCode key);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallIsControllerTriggered(keyCode key)
		\brief   Checks if the specified controller key was triggered (pressed this frame).
		\param   key - The controller key to check.
		\return  True if the key was triggered, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallIsControllerTriggered(keyCode key);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallIsControllerPress(keyCode key)
		\brief   Checks if the specified controller key is currently being held down.
		\param   key - The controller key to check.
		\return  True if the key is being held down, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallIsControllerPress(keyCode key);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallIsControllerReleased(keyCode key)
		\brief   Checks if the specified controller key was released this frame.
		\param   key - The controller key to check.
		\return  True if the key was released, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallIsControllerReleased(keyCode key);

		/******************************************************************/
		/*!
		\fn      static float m_InternalCallGetRightJoyStickRotation()
		\brief   Retrieves the rotation angle of the right joystick.
		\return  The angle of the right joystick in degrees.
		*/
		/******************************************************************/
		static float m_InternalCallGetRightJoyStickRotation();

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallIsControllerPresent()
		\brief   Checks if a controller is currently connected and active.
		\return  True if a controller is detected, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallIsControllerPresent();

		/******************************************************************/
		/*!
		\fn      static void m_InternalCallOverideMouseWithCursor(bool _boolean)
		\brief   Enables or disables the override of mouse input with the controller cursor.
		\param   _boolean - True to enable override, false to disable.
		*/
		/******************************************************************/
		static void m_InternalCallOverideMouseWithCursor(bool _boolean);

		/******************************************************************/
		/*!
		\fn      static void m_InternalCallHideCursor(bool _boolean)
		\brief   Shows or hides the mouse cursor.
		\param   _boolean - True to hide the cursor, false to show it.
		*/
		/******************************************************************/
		static void m_InternalCallHideCursor(bool _boolean);

		/******************************************************************/
		/*!
		\fn      static MonoArray* m_InternalCallGetJoyStickAxis()
		\brief   Retrieves the joystick axis data as a MonoArray for scripting integration.
		\return  A MonoArray containing the axis values of the connected controller.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetJoyStickAxis();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallSetTimeScale(const float x)
			\brief     Sets the time scale for the application, affecting the speed of time progression.
			\param[in] x The time scale multiplier to set (e.g., 1.0 for normal speed, 0.5 for half speed).
		*/
		/******************************************************************/
		static void m_InternalCallSetTimeScale(const float x);

		/******************************************************************/
		/*!
		\fn      static float m_InternalCallGetTimeScale()
		\brief   Retrieves the current time scale of the game.
		\return  The time scale value (1.0f is normal speed, 0.0f is paused, etc.).
		*/
		/******************************************************************/
		static float m_InternalCallGetTimeScale();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallResetTimeScale()
			\brief     Resets the time scale to its default value (e.g., 1.0 for normal speed).
		*/
		/******************************************************************/
		static void m_InternalCallResetTimeScale();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalGetWorldMousePosition(vector2::Vec2* mousecord)
			\brief     Retrieves the mouse position in world coordinates.
			\param[out] mousecord Pointer to store the world coordinates of the mouse.
		*/
		/******************************************************************/
		static void m_InternalGetWorldMousePosition(vector2::Vec2* mousecord);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallCloseWindow()
			\brief     Closes the application window.
		*/
		/******************************************************************/
		static void m_InternalCallCloseWindow();

		/******************************************************************/
		/*!
			\fn        MonoArray* InternalCall::m_InternalCallGetChildrenID(ecs::EntityID id, bool* have_children)
			\brief     Retrieves the IDs of child entities for a specified entity.
			\param[in] id The ID of the parent entity.
			\param[out] have_children Pointer to a boolean indicating whether the entity has children.
			\return    A MonoArray containing the IDs of child entities if any; otherwise, null.
		*/
		/******************************************************************/
		static MonoArray* m_InternalCallGetChildrenID(ecs::EntityID);

		static int m_InternalCallGetParentID(ecs::EntityID);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallPlayAudio(ecs::EntityID id, MonoString* monoString)
			\brief     Plays the specified audio associated with an entity.
			\param[in] id The ID of the entity playing the audio.
			\param[in] monoString Pointer to the MonoString containing the audio file name.
		*/
		/******************************************************************/
		static void m_InternalCallPlayAudio(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallStopAudio(ecs::EntityID id, MonoString* monoString)
			\brief     Stops the specified audio associated with an entity.
			\param[in] id The ID of the entity stopping the audio.
			\param[in] monoString Pointer to the MonoString containing the audio file name.
		*/
		/******************************************************************/
		static void m_InternalCallStopAudio(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallStopAllAudio()
			\brief     Stops all audio currently playing in the application.
		*/
		/******************************************************************/
		static void m_InternalCallStopAllAudio();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallPauseAudio(ecs::EntityID id, MonoString* monoString)
			\brief     Pauses the audio associated with a specified entity.
			\param[in] id The ID of the entity whose audio is to be paused.
			\param[in] monoString Pointer to the MonoString containing the name of the audio clip.
		*/
		/******************************************************************/
		static void m_InternalCallPauseAudio(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallUnPauseAudio(ecs::EntityID id, MonoString* monoString)
			\brief     Unpauses the audio associated with a specified entity.
			\param[in] id The ID of the entity whose audio is to be unpaused.
			\param[in] monoString Pointer to the MonoString containing the name of the audio clip.
		*/
		/******************************************************************/
		static void m_InternalCallUnPauseAudio(ecs::EntityID id, MonoString* monoString);


		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallPauseAllAudio()
			\brief     Pauses all currently playing audio within the application.
		*/
		/******************************************************************/
		static void m_InternalCallPauseAllAudio();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallUnPauseAllAudio()
			\brief     Unpauses all currently paused audio within the application.
		*/
		/******************************************************************/
		static void m_InternalCallUnPauseAllAudio();

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallAudioIsPlayingForEntity(ecs::EntityID id, MonoString* monoString)
		\brief   Checks if the specified audio is currently playing for the given entity.
		\param   id - The ID of the entity.
		\param   monoString - The name of the audio clip to check.
		\return  True if the audio is playing, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallAudioIsPlayingForEntity(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
		\fn      static void m_InternalCallSetGlobalBGMVolume(float volume)
		\brief   Sets the global volume for all background music (BGM).
		\param   volume - The desired volume level (0.0 to 1.0).
		*/
		/******************************************************************/
		static void m_InternalCallSetGlobalBGMVolume(float volume);


		/******************************************************************/
		/*!
		\fn      static void m_InternalCallSetGlobalSFXVolume(float volume)
		\brief   Sets the global volume for all sound effects (SFX).
		\param   volume - The desired volume level (0.0 to 1.0).
		*/
		/******************************************************************/
		static void m_InternalCallSetGlobalSFXVolume(float volume);

		/******************************************************************/
		/*!
		\fn      static float m_InternalCallGetGlobalBGMVolume()
		\brief   Retrieves the current global volume level for background music (BGM).
		\return  The current BGM volume (0.0 to 1.0).
		*/
		/******************************************************************/
		static float m_InternalCallGetGlobalBGMVolume();


		/******************************************************************/
		/*!
		\fn      static float m_InternalCallGetGlobalSFXVolume()
		\brief   Retrieves the current global volume level for sound effects (SFX).
		\return  The current SFX volume (0.0 to 1.0).
		*/
		/******************************************************************/
		static float m_InternalCallGetGlobalSFXVolume();

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallCheckIsBGM(ecs::EntityID id, MonoString* monoString)
		\brief   Checks if the specified audio for an entity is marked as background music (BGM).
		\param   id - The ID of the entity.
		\param   monoString - The name of the audio clip.
		\return  True if the audio is categorized as BGM, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallCheckIsBGM(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallCheckIsSFX(ecs::EntityID id, MonoString* monoString)
		\brief   Checks if the specified audio for an entity is marked as a sound effect (SFX).
		\param   id - The ID of the entity.
		\param   monoString - The name of the audio clip.
		\return  True if the audio is categorized as SFX, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallCheckIsSFX(ecs::EntityID id, MonoString* monoString);


		/******************************************************************/
		/*!
		\fn      static void m_InternalCallSetPanAudio(ecs::EntityID id, MonoString* monoString, float pan)
		\brief   Sets the stereo pan for the specified audio on an entity.
		\param   id - The ID of the entity.
		\param   monoString - The name of the audio clip.
		\param   pan - Stereo balance (-1.0 for full left, 1.0 for full right).
		*/
		/******************************************************************/
		static void m_InternalCallSetPanAudio(ecs::EntityID id, MonoString* monoString, float pan);


		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallIsWindowMinimise()
		\brief   Checks if the application window is minimized.
		\return  True if the window is minimized; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallIsWindowMinimise();

		/******************************************************************/
		/*!
		\fn      static float m_getAccumulatedDeltaTime()
		\brief   Retrieves the accumulated delta time for the application.
		\return  The accumulated delta time as a float.
		*/
		/******************************************************************/
		static float m_getAccumulatedDeltaTime();

		/******************************************************************/
		/*!
		\fn      static int m_InternalCallGetSteps()
		\brief   Retrieves the number of steps taken by the application.
		\return  The number of steps as an integer.
		*/
		/******************************************************************/
		static int m_InternalCallGetSteps();

		/******************************************************************/
		/*!
			\fn        float InternalCall::m_InternalCallGetGameTime()
			\brief     Retrieves the game time elapsed since the start of the application.
			\return    The game time as a float, measured in seconds.
		*/
		/******************************************************************/
		static float m_InternalCallGetGameTime();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_EnableScript(ecs::EntityID id, MonoString* monoString)
			\brief     Enables a script component for a specified entity.
			\param[in] id The ID of the entity whose script is to be enabled.
			\param[in] monoString Pointer to the MonoString containing the script name.
		*/
		/******************************************************************/
		static void m_EnableScript(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_DisableScript(ecs::EntityID id, MonoString* monoString)
			\brief     Disables a script component for a specified entity.
			\param[in] id The ID of the entity whose script is to be disabled.
			\param[in] monoString Pointer to the MonoString containing the script name.
		*/
		/******************************************************************/
		static void m_DisableScript(ecs::EntityID id, MonoString* monoString);

		/******************************************************************/
		/*!
			\fn        MonoArray* InternalCall::m_RetrieveCollidableEntities(MonoString* scene)
			\brief     Retrieves all entities with collidable components within a specified scene.
			\param[in] scene Pointer to the MonoString containing the scene name.
			\return    A MonoArray containing the IDs of entities with collidable components; null if none are found.
		*/
		/******************************************************************/
		static MonoArray* m_RetrieveCollidableEntities(MonoString* scene);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallGetRayCast(ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition)
			\brief     Performs a raycast from a specified entity towards a target position, checking for hits.
			\param[in] id The ID of the entity performing the raycast.
			\param[in] monoString Pointer to additional data for raycast.
			\param[out] isRaycasting Pointer to the boolean storing if the raycast is active.
			\param[out] targetposition Pointer to the target position vector.
			\param[out] m_distance Pointer to the distance to the target.
			\param[out] targetReached Pointer to the boolean indicating if the target is reached.
			\param[out] hitposition Pointer to store the hit position if a collision occurs.
			\return    True if the raycast hits a target; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallGetRayCast(ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallSetRayCast(ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition)
			\brief     Sets the parameters for a raycast from a specified entity towards a target position.
			\param[in] id The ID of the entity setting the raycast.
			\param[in] monoString Pointer to additional data for raycast setup.
			\param[in] isRaycasting Pointer to the boolean to activate or deactivate raycasting.
			\param[in] targetposition Pointer to the target position vector to set.
			\param[in] m_distance Pointer to the distance to the target to set.
			\param[in] targetReached Pointer to the boolean to set if the target is reached.
			\param[in] hitposition Pointer to set the hit position if a collision occurs.
			\return    True if the raycast parameters are successfully set; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallSetRayCast(ecs::EntityID id, MonoString* monoString, bool* isRaycasting, vector2::Vec2* targetposition, float* m_distance, bool* targetReached, vector2::Vec2* hitposition);;

		/******************************************************************/
		/*!
		\fn      static bool m_InternalGetParticleComponent(ecs::EntityID entity, bool* willSpawn, int* noOfParticles, float* lifeSpan, vector2::Vec2* size, vector2::Vec2* velocity, vector2::Vec2* acceleration, vector3::Vec3* color, float* coneRotation, float* coneAngle, float* randomFactor, MonoString** imageFile, int* stripCount, int* frameNumber, int* layer, float* friction, int* fps, bool* loopAnimation)
		\brief   Retrieves particle component data for the specified entity.
		\param   entity - The ID of the entity.
		\param   willSpawn - Whether the particle system is currently set to spawn particles.
		\param   noOfParticles - Number of particles to spawn.
		\param   lifeSpan - Lifetime of each particle.
		\param   size - Size of each particle.
		\param   velocity - Initial velocity of the particles.
		\param   acceleration - Acceleration applied to particles over time.
		\param   color - Color of the particles.
		\param   coneRotation - Rotation angle of the spawn cone.
		\param   coneAngle - Spread angle of the cone.
		\param   randomFactor - Degree of randomness applied to particle properties.
		\param   imageFile - Texture image file for the particles.
		\param   stripCount - Number of strips in the animation sheet.
		\param   frameNumber - Current frame number in animation.
		\param   layer - Rendering layer.
		\param   friction - Friction applied to particle movement.
		\param   fps - Frames per second for animation playback.
		\param   loopAnimation - Whether the animation should loop.
		\return  True if the component was successfully retrieved, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalGetParticleComponent(ecs::EntityID entity, bool* willSpawn, int* noOfParticles, float* lifeSpan, vector2::Vec2* size ,vector2::Vec2* velocity, vector2::Vec2* acceleration, vector3::Vec3* color, float* coneRotation, float* coneAngle, float* randomFactor, MonoString** imageFile, int* stripCount, int* frameNumber, int* layer, float* friction, int* fps, bool* loopAnimation);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalSetParticleComponent(ecs::EntityID entity, bool* willSpawn, int* noOfParticles, float* lifeSpan, vector2::Vec2* size, vector2::Vec2* velocity, vector2::Vec2* acceleration, vector3::Vec3* color, float* coneRotation, float* coneAngle, float* randomFactor, MonoString* imageFile, int* stripCount, int* frameNumber, int* layer, float* friction, int* fps, bool* loopAnimation)
		\brief   Sets the particle component data for the specified entity.
		\param   entity - The ID of the entity.
		\param   willSpawn - Whether the particle system should spawn particles.
		\param   noOfParticles - Number of particles to spawn.
		\param   lifeSpan - Lifetime of each particle.
		\param   size - Size of each particle.
		\param   velocity - Initial velocity of the particles.
		\param   acceleration - Acceleration applied to particles.
		\param   color - Color of the particles.
		\param   coneRotation - Rotation angle of the spawn cone.
		\param   coneAngle - Spread angle of the cone.
		\param   randomFactor - Randomness factor for particle properties.
		\param   imageFile - Texture image file for the particles.
		\param   stripCount - Number of strips in the animation sheet.
		\param   frameNumber - Current frame of the animation.
		\param   layer - Rendering layer.
		\param   friction - Friction applied to particles.
		\param   fps - Animation playback speed.
		\param   loopAnimation - Whether the animation should loop.
		\return  True if the component was successfully set, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalSetParticleComponent(ecs::EntityID entity, bool* willSpawn, int* noOfParticles, float* lifeSpan, vector2::Vec2* size, vector2::Vec2* velocity, vector2::Vec2* acceleration, vector3::Vec3* color, float* coneRotation, float* coneAngle, float* randomFactor, MonoString* imageFile, int* stripCount, int* frameNumber, int* layer, float* friction, int* fps, bool* loopAnimation);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallSpawnParticle(ecs::EntityID entity)
		\brief   Triggers the particle system to start spawning particles for the specified entity.
		\param   entity - The ID of the entity.
		\return  True if the particle system was successfully started, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallSpawnParticle(ecs::EntityID entity);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallSetParticleLayer(ecs::EntityID entity, int* layer)
		\brief   Sets the rendering layer of the particle system for the specified entity.
		\param   entity - The ID of the entity.
		\param   layer - Pointer to the desired rendering layer.
		\return  True if the layer was set successfully, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallSetParticleLayer(ecs::EntityID entity, int* layer);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallSetParticleConeRotation(ecs::EntityID entity, float* angle)
		\brief   Sets the cone rotation angle for the particle system of the specified entity.
		\param   entity - The ID of the entity.
		\param   angle - Pointer to the desired rotation angle.
		\return  True if the angle was set successfully, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallSetParticleConeRotation(ecs::EntityID entity, float* angle);

		/******************************************************************/
		/*!
		\fn      static bool m_InternalCallDespawnParticle(ecs::EntityID entity)
		\brief   Stops the particle system from spawning further particles for the specified entity.
		\param   entity - The ID of the entity.
		\return  True if the particle system was successfully stopped, false otherwise.
		*/
		/******************************************************************/
		static bool m_InternalCallDespawnParticle(ecs::EntityID entity);


		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalGetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey)
			\brief     Retrieves the grid component details of a specified entity.
			\param[in] entity The ID of the entity to retrieve the grid component from.
			\param[out] anchor Pointer to the anchor position vector.
			\param[out] gridRowLength Pointer to the number of rows in the grid.
			\param[out] gridColumnLength Pointer to the number of columns in the grid.
			\param[out] setCollidable Pointer to the boolean indicating if the grid is collidable.
			\param[out] gridKey Pointer to the grid key identifier.
			\return    True if the grid component is found and retrieved; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalGetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalSetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey)
			\brief     Sets the grid component details of a specified entity.
			\param[in] entity The ID of the entity to set the grid component for.
			\param[in] anchor Pointer to the anchor position vector to set.
			\param[in] gridRowLength Pointer to the number of rows in the grid to set.
			\param[in] gridColumnLength Pointer to the number of columns in the grid to set.
			\param[in] setCollidable Pointer to the boolean to set if the grid is collidable.
			\param[in] gridKey Pointer to the grid key identifier to set.
			\return    True if the grid component is found and updated; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalSetGridComponent(ecs::EntityID entity, vector2::Vec2* anchor, int* gridRowLength, int* gridColumnLength, bool* setCollidable, int* gridKey);

		/******************************************************************/
		/*!
			\fn        bool InternalCall::m_InternalCallGetPathfinding(ecs::EntityID id, vector2::Vec2* m_startpos, vector2::Vec2* m_startend, int* gridkey)
			\brief     Retrieves the pathfinding information from a specified entity.
			\param[in] id The ID of the entity to retrieve pathfinding details from.
			\param[out] m_startpos Pointer to the start position for pathfinding.
			\param[out] m_startend Pointer to the end position for pathfinding.
			\param[out] gridkey Pointer to the grid key used in pathfinding.
			\return    True if pathfinding details are found; otherwise, false.
		*/
		/******************************************************************/
		static bool m_InternalCallGetPathfinding(ecs::EntityID id, vector2::Vec2* m_startpos, vector2::Vec2* m_startend, int* gridkey);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallSetTargetPathfinding(ecs::EntityID id, vector2::Vec2* m_targetgridposition)
			\brief     Sets the target position for pathfinding for a specified entity.
			\param[in] id The ID of the entity to set the target for pathfinding.
			\param[in] m_targetgridposition Pointer to the target grid position.
		*/
		/******************************************************************/
		static void m_InternalCallSetTargetPathfinding(ecs::EntityID id, vector2::Vec2* m_targetgridposition);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_InternalCallGetPath(int gridKey, int* startX, int* startY, int* targetX, int* targetY, MonoArray** nodeArray_x, MonoArray** nodeArray_y)
			\brief     Retrieves a path based on the specified grid key and coordinates.
			\param[in] gridKey The grid key identifier.
			\param[in] startX Pointer to the start x-coordinate.
			\param[in] startY Pointer to the start y-coordinate.
			\param[in] targetX Pointer to the target x-coordinate.
			\param[in] targetY Pointer to the target y-coordinate.
			\param[out] nodeArray_x Pointer to the MonoArray to store the x-coordinates of the path nodes.
			\param[out] nodeArray_y Pointer to the MonoArray to store the y-coordinates of the path nodes.
		*/
		/******************************************************************/
		static void m_InternalCallGetPath(int gridKey, int* startX, int* startY, int* targetX, int* targetY, MonoArray** nodeArray_x, MonoArray** nodeArray_y);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_EnableLayer(unsigned int layer)
			\brief     Enables the specified layer in the application.
			\param[in] layer The layer identifier to enable.
		*/
		/******************************************************************/
		static void m_EnableLayer(unsigned int layer);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_DisableLayer(unsigned int layer)
			\brief     Disables the specified layer in the application.
			\param[in] layer The layer identifier to disable.
		*/
		/******************************************************************/
		static void m_DisableLayer(unsigned int layer);

		/******************************************************************/
		/*!
			\fn        float InternalCall::m_GetUnfixedDeltaTime()
			\brief     Retrieves the unfixed delta time, which is not adjusted by the time scale.
			\return    The unfixed delta time as a float.
		*/
		/******************************************************************/
		static float m_GetUnfixedDeltaTime();

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_GetNameComponent(ecs::EntityID id, void** outptr)
			\brief     Retrieves the name component of a specified entity.
			\param[in] id The ID of the entity to retrieve the name from.
			\param[out] outptr Pointer to store the name component.
		*/
		/******************************************************************/
		static void m_GetNameComponent(ecs::EntityID id, void** outptr);

		/******************************************************************/
		/*!
			\fn        int InternalCall::m_InternalGetEntityIdFromGridKey(int gridkey)
			\brief     Retrieves the entity ID associated with a specific grid key.
			\param[in] gridkey The grid key identifier.
			\return    The entity ID associated with the grid key; returns -1 if no entity is found.
		*/
		/******************************************************************/
		static int m_InternalGetEntityIdFromGridKey(int gridkey);

		/******************************************************************/
		/*!
			\fn        void InternalCall::m_ChangeLayer(ecs::EntityID id, unsigned int layerid)
			\brief     Changes the layer of a specified entity.
			\param[in] id The ID of the entity whose layer is to be changed.
			\param[in] layerid The new layer identifier to set for the entity.
		*/
		/******************************************************************/
		static void m_ChangeLayer(ecs::EntityID id, unsigned int layerid);

		/******************************************************************/
		/*!
			\fn        float InternalCall::m_getFPS()
			\brief     Retrieves the current frames per second (FPS) of the application.
			\return    The current FPS as a float.
		*/
		/******************************************************************/
		static float m_getFPS();

		/******************************************************************/
		/*!
		\fn      static void m_InternalCallGetLightingComponent(ecs::EntityID id, vector2::Vec2* innerouterradius, vector3::Vec3* color, float* intensity)
		\brief   Retrieves the lighting component data for a given entity.
		\param   id - The ID of the entity.
		\param   innerouterradius - Pointer to store the inner and outer radius values.
		\param   color - Pointer to store the light color.
		\param   intensity - Pointer to store the light intensity.
		*/
		/******************************************************************/
		static void m_InternalCallGetLightingComponent(ecs::EntityID id, vector2::Vec2* innerouterradius, vector3::Vec3* color, float* intensity);

		/******************************************************************/
		/*!
		\fn      static void m_InternalCallSetLightingComponent(ecs::EntityID id, vector2::Vec2* innerouterradius, vector3::Vec3* color, float* intensity)
		\brief   Sets the lighting component values for a given entity.
		\param   id - The ID of the entity.
		\param   innerouterradius - Pointer to the inner and outer radius values.
		\param   color - Pointer to the light color.
		\param   intensity - Pointer to the light intensity.
		*/
		/******************************************************************/
		static void m_InternalCallSetLightingComponent(ecs::EntityID id, vector2::Vec2* innerouterradius, vector3::Vec3* color, float* intensity);

		/******************************************************************/
		/*!
		\fn      static bool m_IsLayerVisable(int layer)
		\brief   Checks if the specified rendering layer is currently visible.
		\param   layer - The layer index to check.
		\return  True if the layer is visible, false otherwise.
		*/
		/******************************************************************/
		static bool m_IsLayerVisable(int layer);

		/******************************************************************/
		/*!
		\fn      static void m_GetColliderDecomposedTRS(ecs::EntityID id, vector2::Vec2* translate, vector2::Vec2* rotate, float* scale)
		\brief   Retrieves the decomposed translation, rotation, and scale of a collider.
		\param   id - The ID of the entity.
		\param   translate - Pointer to store the translation values.
		\param   rotate - Pointer to store the rotation values.
		\param   scale - Pointer to store the scale value.
		*/
		/******************************************************************/
		static void m_GetColliderDecomposedTRS(ecs::EntityID id, vector2::Vec2* translate, vector2::Vec2* rotate, float* scale);

		/******************************************************************/
		/*!
		\fn      static float m_GetRandomFloat(float min, float max)
		\brief   Generates a random floating-point number between min and max.
		\param   min - Minimum value.
		\param   max - Maximum value.
		\return  Random float between min and max.
		*/
		/******************************************************************/
		static float m_GetRandomFloat(float min, float max);

		/******************************************************************/
		/*!
		\fn      static int m_GetRandomInt(int min, int max)
		\brief   Generates a random integer between min and max.
		\param   min - Minimum integer value.
		\param   max - Maximum integer value.
		\return  Random integer between min and max.
		*/
		/******************************************************************/
		static int m_GetRandomInt(int min, int max);

		/******************************************************************/
		/*!
		\fn      static MonoString* m_GetScenefromID(ecs::EntityID id)
		\brief   Retrieves the scene name associated with the given entity ID.
		\param   id - The ID of the entity.
		\return  A MonoString containing the scene name.
		*/
		/******************************************************************/
		static MonoString* m_GetScenefromID(ecs::EntityID id);

		/******************************************************************/
		/*!
		\fn      static void m_HideEntityandchildren(ecs::EntityID id)
		\brief   Hides the specified entity and all its child entities.
		\param   id - The ID of the parent entity.
		*/
		/******************************************************************/
		static void m_HideEntityandchildren(ecs::EntityID id);

		/******************************************************************/
		/*!
		\fn      static void m_UnHideEntityandchildren(ecs::EntityID id)
		\brief   Unhides the specified entity and all its child entities.
		\param   id - The ID of the parent entity.
		*/
		/******************************************************************/
		static void m_UnHideEntityandchildren(ecs::EntityID id);

		/******************************************************************/
		/*!
		\fn      static void m_StopVideo(ecs::EntityID id)
		\brief   Stops the video playback for the given entity.
		\param   id - The ID of the entity playing the video.
		*/
		/******************************************************************/
		static void m_StopVideo(ecs::EntityID);

		/******************************************************************/
		/*!
		\fn      static void m_PauseVideo(ecs::EntityID id, bool _boolean)
		\brief   Pauses or resumes the video playback for the given entity.
		\param   id - The ID of the entity.
		\param   _boolean - True to pause, false to resume.
		*/
		/******************************************************************/
		static void m_PauseVideo(ecs::EntityID, bool _boolean);

		/******************************************************************/
		/*!
		\fn      static void m_StartVideo(ecs::EntityID id)
		\brief   Starts video playback for the specified entity.
		\param   id - The ID of the entity.
		*/
		/******************************************************************/
		static void m_StartVideo(ecs::EntityID);

		/******************************************************************/
		/*!
		\fn      static bool m_HasVideoFinish(ecs::EntityID id)
		\brief   Checks if the video has finished playing for the specified entity.
		\param   id - The ID of the entity.
		\return  True if the video has completed playback, false otherwise.
		*/
		/******************************************************************/
		static bool m_HasVideoFinish(ecs::EntityID);

	public:
		/******************************************************************/
		/*!
			\fn        void InternalCall::m_RegisterInternalCalls()
			\brief     Registers internal call functions for Mono scripting,
					   allowing C# scripts to access C++ ECS functions.
		*/
		/******************************************************************/
		static void m_RegisterInternalCalls();
	};
}

#endif