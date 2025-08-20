/******************************************************************/
/*!
\file      PhysicsLayer.h
\author    Rayner Tan, raynerweichen.tan , 2301449
\par       raynerweichen.tan@digipen.edu
\date      Oct 29, 2024
\brief     This header file defines the PhysicsLayer class, which manages
           layer-based collision settings within the physics pipeline.

The PhysicsLayer class provides functionality for managing collisions
between different layers in the physics system. It maintains a
collision matrix where each entry represents whether collisions are
enabled between two specific layers. The class follows a singleton
pattern to ensure a single instance is accessible throughout the
system. Key methods include setting and retrieving collision statuses
between layers, as well as printing the collision matrix for debugging
purposes.

Copyright (C) 2024 DigiPen Institute of Technology.
Reproduction or disclosure of this file or its contents without the
prior written consent of DigiPen Institute of Technology is prohibited.
*/
/******************************************************************/
#ifndef PHYSICSLAYERR_H
#define PHYSICSLAYERR_H

#include "../ECS/Layers.h"
#include "../Config/pch.h"

namespace physicslayer {
    constexpr int size = layer::LAYERS::MAXLAYER;
	class PhysicsLayer {
	public:
    /******************************************************************/
    /*!
    \fn        static PhysicsLayer* PhysicsLayer::getInstance()
    \brief     Provides access to the single instance of the PhysicsLayer
                class, following the Singleton pattern.
    \return    A pointer to the single instance of the `PhysicsLayer` class.
    */
    /******************************************************************/
    static PhysicsLayer* m_GetInstance() {
        if (!instance) {
            instance = std::make_unique<PhysicsLayer>();
        }
        return instance.get();
    }

    /******************************************************************/
    /*!
    \fn        PhysicsLayer::PhysicsLayer()
    \brief     Default constructor for the PhysicsLayer class, initializing
                the collision matrix for layer-based collision detection.
    */
    /******************************************************************/

    PhysicsLayer();
	PhysicsLayer(const PhysicsLayer&) = delete;
	PhysicsLayer& operator=(const PhysicsLayer&) = delete;

    /******************************************************************/
    /*!
    \fn        void PhysicsLayer::setCollision(int layer1, int layer2, bool value)
    \brief     Sets the collision status between two specified layers in
               the collision matrix.
    \param[in] layer1 The first layer involved in the collision check.
    \param[in] layer2 The second layer involved in the collision check.
    \param[in] value  The collision status to set (true for collision enabled, false otherwise).
    */
    /******************************************************************/
	void m_SetCollision(int layer1, int layer2, bool value);
    /******************************************************************/
    /*!
    \fn        void PhysicsLayer::printCollisionMatrix() const
    \brief     Prints the current state of the collision matrix, showing
               which layers are set to collide with each other.
    */
    /******************************************************************/
	void m_PrintCollisionMatrix() const;
    /******************************************************************/
    /*!
    \fn        bool PhysicsLayer::getCollide(int layer1, int layer2)
    \brief     Retrieves the collision status between two specified layers
               from the collision matrix.
    \param[in] layer1 The first layer to check for collision.
    \param[in] layer2 The second layer to check for collision.
    \return    True if a collision is set between the two layers, false otherwise.
    */
    /******************************************************************/
    bool m_GetCollide(int layer1, int layer2);
    /******************************************************************/
    /*!
    \fn        std::vector<std::vector<bool>> PhysicsLayer::getMatrix() const
    \brief     Returns the entire collision matrix for inspection or modification.
    \return    A 2D vector of booleans representing the collision matrix.
    */
    /******************************************************************/
    std::vector<std::vector<bool>> m_GetMatrix() const;
    std::bitset<size>* m_GetMat();
    const std::bitset<size>* m_GetMat() const;
    static const int m_GetSize()  {
        return size;
    }

	private:
		 std::vector<std::vector<bool>> collisionMatrix; // 2D collision matrix
         static std::unique_ptr<PhysicsLayer> instance;
         std::bitset<size> layerCollisions[size];
	};
   
}

#endif