#include "../Config/pch.h"
#include "../ECS.h"
#include "PathfindingSystem.h"
#include "../Pathfinding/AStarPathfinding.h"
#include "../Graphics/GraphicsPipe.h"
#include "../Asset Manager/AssetManager.h"

namespace ecs {

    void PathfindingSystem::m_RegisterSystem(EntityID ID) {
        ECS* ecs = ECS::m_GetInstance();

        if (std::find_if(m_vecTransformComponentPtr.begin(), m_vecTransformComponentPtr.end(),
            [ID](const auto& obj) { return obj->m_Entity == ID; }) == m_vecTransformComponentPtr.end()) {

            m_vecTransformComponentPtr.push_back(
                static_cast<TransformComponent*>(ecs->m_ECS_CombinedComponentPool[TYPETRANSFORMCOMPONENT]->m_GetEntityComponent(ID))
            );
            m_vecPathfindingComponentPtr.push_back(
                static_cast<PathfindingComponent*>(ecs->m_ECS_CombinedComponentPool[TYPEPATHFINDINGCOMPONENT]->m_GetEntityComponent(ID))
            );
            m_vecNameComponentPtr.push_back((NameComponent*)ecs->m_ECS_CombinedComponentPool[TYPENAMECOMPONENT]->m_GetEntityComponent(ID));
        }
    }

    void PathfindingSystem::m_DeregisterSystem(EntityID ID) {
        size_t IndexID = 0;

        for (auto& pathfindingComponent : m_vecPathfindingComponentPtr) {
            if (pathfindingComponent->m_Entity == ID) {
                break;
            }
            IndexID++;
        }

        size_t IndexLast = m_vecPathfindingComponentPtr.size() - 1;

        std::swap(m_vecPathfindingComponentPtr[IndexID], m_vecPathfindingComponentPtr[IndexLast]);
        std::swap(m_vecTransformComponentPtr[IndexID], m_vecTransformComponentPtr[IndexLast]);
        std::swap(m_vecNameComponentPtr[IndexID], m_vecNameComponentPtr[IndexLast]);
        
        m_vecPathfindingComponentPtr.pop_back();
        m_vecTransformComponentPtr.pop_back();
        m_vecNameComponentPtr.pop_back();
    }

    void PathfindingSystem::m_Init() {
        m_SystemSignature.set(TYPETRANSFORMCOMPONENT);
        m_SystemSignature.set(TYPEPATHFINDINGCOMPONENT);
    }

    void PathfindingSystem::m_Update(const std::string& scene) {
        if (m_vecPathfindingComponentPtr.size() != m_vecTransformComponentPtr.size() ||
            m_vecTransformComponentPtr.size() != m_vecPathfindingComponentPtr.size()) {
            LOGGING_ERROR("Error: Vectors container size does not match");
            return;
        }

        AStarPathfinding pathfinder;
        ECS* ecs = ECS::m_GetInstance();
        for (size_t n = 0; n < m_vecPathfindingComponentPtr.size(); ++n) {
            TransformComponent* transform = m_vecTransformComponentPtr[n];
            PathfindingComponent* pathfinding = m_vecPathfindingComponentPtr[n];
            NameComponent* NameComp = m_vecNameComponentPtr[n];
            GridComponent* grid = NULL;

            // Skip components not of the scene
            if ((pathfinding->m_scene != scene) || !ecs->m_layersStack.m_layerBitSet.test(NameComp->m_Layer) || NameComp->m_hide) continue;

            const auto& ids = ecs->m_ECS_SceneMap.find(pathfinding->m_scene);
            if (ids != ecs->m_ECS_SceneMap.end()) {
                for (auto x : ids->second.m_sceneIDs) {
                    auto itSignature = ecs->m_ECS_EntityMap.find(x);
                    if (itSignature->second.test(TYPEGRIDCOMPONENT)) {
                        // retrieve grid component
                        ecs::GridComponent* gc = static_cast<ecs::GridComponent*>(ecs->m_ECS_CombinedComponentPool[ecs::TYPEGRIDCOMPONENT]->m_GetEntityComponent(x));
                        if (gc) {
                            if (gc->m_GridKey == pathfinding->m_GridKey)
                            {
                                grid = gc;
                                break;
                            }
                        }
                    }
                }
            }
            if (grid == NULL)
            {
                LOGGING_WARN("Error: Grid not found");
                return;
            }

            if ((transform->m_position.m_x < grid->m_Anchor.m_x || transform->m_position.m_y > grid->m_Anchor.m_y) ||
                (transform->m_position.m_x > (grid->m_Anchor.m_x + grid->m_GridRowLength) || transform->m_position.m_y < (grid->m_Anchor.m_y - grid->m_GridColumnLength)))
            {
                LOGGING_WARN("Error: Pathfinding Component -> Outside grid component");
                return;
            }
            else
            { 
                pathfinding->m_StartPos.m_x = floor(transform->m_position.m_x) - grid->m_Anchor.m_x;
                pathfinding->m_StartPos.m_y = grid->m_Anchor.m_y - floor(transform->m_transformation.m_e21) - 1.0f;
            }


            /*int startX = static_cast<int>(pathfinding->m_StartPos.m_x);
            int startY = static_cast<int>(pathfinding->m_StartPos.m_y);
            int targetX = static_cast<int>(pathfinding->m_TargetPos.m_x);
            int targetY = static_cast<int>(pathfinding->m_TargetPos.m_y);

            auto path = pathfinder.FindPath(grid, startX, startY, targetX, targetY);*/

            //if (!path.empty()) {
            //    pathfinding->m_Path = path;
            //    //LOGGING_INFO("Path found");
            //    /*for (const auto& node : path) {*/
            //       //std::cout << "Step: (" << node.x << ", " << node.y << ")\n";

            //       /* if (!pathfinding->m_Path.empty())
            //        {
            //            std::cout << "has path" << "\n";
            //        }*/
            //   // }
            //}
            //else {
            //    //LOGGING_INFO("No path found");
            //}
        }
    }

} // namespace ecs
