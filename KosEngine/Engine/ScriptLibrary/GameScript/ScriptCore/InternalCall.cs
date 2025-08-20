using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text;
using System.Threading.Tasks;
using System.Reflection.Emit;
using System.Data.SqlClient;
using System.Runtime.InteropServices;

public static class InternalCall
{

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTransformComponent(uint entity, out Vector2 pos, out Vector2 scale, out float rotate);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTransformComponent(uint entity, in Vector2 pos, in Vector2 scale, in float rotate);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTranslate(uint entity, out Vector2 pos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTranslate(uint entity, in Vector2 pos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetColliderComponent(uint entity, out Vector2 size, out Vector2 offset, out bool drawDebug, out float radius, out uint bockflag, out float isCollided, out bool collisionCheck, out bool collisionresponse);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetColliderComponent(uint entity, in Vector2 size, in Vector2 offset, in bool drawDebug, in float radius, in uint bockflag, in float isCollided, in bool collisionCheck, in bool collisionresponse);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetEnemyComponent(uint entity, out int enemyTag, out int enemytype, out int enemybehaviour);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetEnemyComponent(uint entity, in int enemyTag, in int enemytype, in int enemybehaviour);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetRigidBodyComponent(uint entity, out Vector2 velocity, out Vector2 acceleration, out float rotation, out Vector2 previouspos, out Vector2 direction, out Vector2 force);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetRigidBodyComponent(uint entity, in Vector2 velocity, in Vector2 acceleration, in float rotation, in Vector2 previouspos, in Vector2 direction, in Vector2 force);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetTextComponent(uint entity, out string text, out string fileName, out int fontLayer, out float fontSize, out Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetTextComponent(uint entity, string text, string fileName, in int fontLayer, in float fontSize, in Vector3 color);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetAnimationComponent(uint entity, out int frameNumber, out int framesPerSecond, out float frameTimer, out bool isAnimating, out int stripcount);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetAnimationComponent(uint entity, in int frameNumber, in int framesPerSecond, in float frameTimer, in bool isAnimating, in int stripcount);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetSpriteComponent(uint entity, out string imageFile, out int layer, out Vector3 color, out float alpha);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetSpriteComponent(uint entity, string imageFile, in int layer, in Vector3 color, in float alpha);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetCameraComponent(uint entity, out float left, out float right, out float top, out float bottom, out float aspectRatio);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetCameraComponent(uint entity, in float left, in float right, in float top, in float bottom, in float aspectRatio);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetButtonComponent(uint entity, out Vector2 position, out Vector2 scale, out bool isClick);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetButtonComponent(uint entity, in Vector2 position, in Vector2 scale, bool isClick);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalIsButtonHovered(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string[] m_InternalGetScriptNames(uint entityID);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalAddScriptInstance(uint entityID, string scriptName, object instance);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetVelocity(uint entity, out Vector2 Velocity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetVelocity(uint entity, in Vector2 Velocity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetDeltaTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalCallGetTagID(string tag);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_InternalCallGetTagIDs(string tag);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_InternalCallGetCollidedEntities(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static string m_InternalCallGetTag(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetSceneActive(string sceneName);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_UnloadAllScene(); // ensure to follow up with a load scene, if no scene, engine will have undefine behaviour

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallLoadScene(string path);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalCallAddPrefab(string prefab, in float x, in float y, in float degree);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallDeleteEntity(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsKeyPressed(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallIsCollided(uint entity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallOverideMouseWithCursor(bool _boolean);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalGetMousePosition(out Vector2 mousepos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsKeyReleased(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsKeyTriggered(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsControllerTriggered(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsControllerPress(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsControllerReleased(keyCode key);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsControllerPresent();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float[] m_InternalCallGetJoyStickAxis();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetRightJoyStickRotation();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalGetWorldMousePosition(out Vector2 mousepos);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetTimeScale(float timescale);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetTimeScale();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallResetTimeScale();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallCloseWindow();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_InternalCallGetChildrenID(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallPlayAudio(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallStopAudio(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallStopAllAudio();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallPauseAudio(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallUnPauseAudio(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallAudioIsPlayingForEntity(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallPauseAllAudio();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallUnPauseAllAudio();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetGlobalBGMVolume(float volume);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetGlobalSFXVolume(float volume);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetGlobalBGMVolume();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetGlobalSFXVolume();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallCheckIsBGM(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallCheckIsSFX(uint id, string monoString);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetPanAudio(uint id, string monoString, float pan);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallIsWindowMinimise();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_getAccumulatedDeltaTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalCallGetSteps();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_InternalCallGetGameTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_EnableScript(uint id, string monostring);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_DisableScript(uint id, string monostring);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int[] m_RetrieveCollidableEntities(string monostring); // insert scene name

    [MethodImpl(MethodImplOptions.InternalCall)]
    private extern static bool m_InternalCallGetRayCast(uint id, string monoString, out bool isRaycasting, out Vector2 targetposition, out float m_distance, out bool targetReached, out Vector2 hitposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    private extern static bool m_InternalCallSetRayCast(uint id, string monoString, in bool isRaycasting, in Vector2 targetposition, in float m_distance, in bool targetReached, in Vector2 hitposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetGridComponent(uint id, out Vector2 anchor, out int gridRowLength, out int gridColumnLength, out bool setCollidable, out int gridKey);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetGridComponent(uint id, in Vector2 anchor, in int gridRowLength, in int gridColumnLength, in bool setCollidable, in int gridKey);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalGetParticleComponent(
        uint id,
        out bool willSpawn,
        out int noOfParticles,
        out float lifeSpan,
        out Vector2 size,
        out Vector2 velocity,
        out Vector2 acceleration,
        out Vector3 color,
        out float coneRotation,
        out float coneAngle,
        out float randomFactor,
        out string imageFile,
        out int stripCount,
        out int frameNumber,
        out int layer,
        out float friction,
        out int fps,
        out bool loopAnimation
    );

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalSetParticleComponent(
       uint id,
       bool willSpawn,
       in int noOfParticles,
       in float lifeSpan,
       in Vector2 size,
       in Vector2 velocity,
       in Vector2 acceleration,
       in Vector3 color,
       in float coneRotation,
       in float coneAngle,
       in float randomFactor,
       in string imageFile,
       in int stripCount,
       in int frameNumber,
       in int layer,
       in float friction,
       in int fps,
       bool loopAnimation
   );

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSpawnParticle(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallDespawnParticle(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetParticleLayer(uint id, in int layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetParticleConeRotation(uint id, in float angle);


    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static int m_InternalGetEntityIdFromGridKey(int gridkey);
    public static Raycast m_GetRay(uint id, string monoString)
    {
        Raycast ray = new Raycast();

        m_InternalCallGetRayCast(id, monoString, out ray.m_isRaycasting, out ray.m_targetposition, out ray.m_distance, out ray.m_targetReached, out ray.m_hitPosition);

        return ray;
    }

    public static void m_SetRay(uint id, string monoString, Raycast ray)
    {

        m_InternalCallSetRayCast(id, monoString, in ray.m_isRaycasting, in ray.m_targetposition, in ray.m_distance, in ray.m_targetReached, in ray.m_hitPosition);

    }
    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallGetPath(int gridkey, in int startX, in int startY, in int targetX, in int targetY, out int[] pathX, out int[] pathY);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static bool m_InternalCallGetPathfinding(uint id, out Vector2 m_startpos, out Vector2 m_startend, out int gridkey);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_EnableLayer(uint layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_DisableLayer(uint layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static void m_InternalCallSetTargetPathfinding(uint id, in Vector2 m_targetgridposition);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public extern static float m_GetUnfixedDeltaTime();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern void m_GetNameComponent(uint id, out IntPtr outPtr);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern void m_ChangeLayer(uint id, uint layerid);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern float m_getFPS();

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern void m_InternalCallGetLightingComponent(uint id, out Vector2 innerouterradius, out Vector3 color, out float intensity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern void m_InternalCallSetLightingComponent(uint id, in Vector2 innerouterradius, in Vector3 color, in float intensity);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern bool m_IsLayerVisable(int layer);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern void m_GetColliderDecomposedTRS(uint id, out Vector2 _translate, out Vector2 _rotate, out float _scale);

    [MethodImpl(MethodImplOptions.InternalCall)]
    public static extern int m_InternalCallGetParentID(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern float m_GetRandomFloat(float min, float max);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern int m_GetRandomInt(int min, int max);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern string m_GetScenefromID(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern void m_HideEntityandchildren(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern void m_UnHideEntityandchildren(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern void m_InternalCallHideCursor(bool _boolean);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern void m_StopVideo(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern void m_PauseVideo(uint id, bool _boolean);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern void m_StartVideo(uint id);

    [MethodImpl(MethodImplOptions.InternalCall)]
    static public extern bool m_HasVideoFinish(uint id);

}


public static class Component
{
    public static T Get<T>(uint id) where T : new()
    {
        T component = new T();

        if (typeof(T) == typeof(SpriteComponent))
        {
            var spriteComponent = component as SpriteComponent;
            InternalCall.m_InternalGetSpriteComponent(id, out spriteComponent.m_imageFile, out spriteComponent.m_layer, out spriteComponent.m_color, out spriteComponent.m_alpha);
        }
        else if (typeof(T) == typeof(TransformComponent))
        {
            var transformComponent = component as TransformComponent;
            InternalCall.m_InternalGetTransformComponent(id, out transformComponent.m_position, out transformComponent.m_scale, out transformComponent.m_rotation);
        }
        else if (typeof(T) == typeof(ColliderComponent))
        {
            var colliderComponent = component as ColliderComponent;
            InternalCall.m_InternalGetColliderComponent(id, out colliderComponent.m_Size, out colliderComponent.m_Offset, out colliderComponent.m_drawDebug, out colliderComponent.m_radius, out colliderComponent.m_blockedFlag, out colliderComponent.m_isCollided, out colliderComponent.m_collisionCheck, out colliderComponent.m_collisionResponse);
        }
        else if (typeof(T) == typeof(TextComponent))
        {
            var textComponent = component as TextComponent;
            InternalCall.m_InternalGetTextComponent(id, out textComponent.m_text, out textComponent.m_fileName, out textComponent.m_fontLayer, out textComponent.m_fontSize, out textComponent.m_color);
        }
        else if (typeof(T) == typeof(RigidBodyComponent))
        {
            var rigidComponent = component as RigidBodyComponent;
            InternalCall.m_InternalGetRigidBodyComponent(id, out rigidComponent.m_Velocity, out rigidComponent.m_Acceleration, out rigidComponent.m_Rotation, out rigidComponent.m_prevPos, out rigidComponent.m_direction, out rigidComponent.m_Force);
        }
        else if (typeof(T) == typeof(AnimationComponent))
        {
            var animationComponent = component as AnimationComponent;
            InternalCall.m_InternalGetAnimationComponent(id, out animationComponent.m_frameNumber, out animationComponent.m_framesPerSecond, out animationComponent.m_frameTimer, out animationComponent.m_isAnimating, out animationComponent.m_stripCount);
        }
        else if (typeof(T) == typeof(ButtomComponent))
        {
            var buttonComponent = component as ButtomComponent;
            InternalCall.m_InternalGetButtonComponent(id, out buttonComponent.m_Position, out buttonComponent.m_Scale, out buttonComponent.m_IsClick);
        }
        else if(typeof(T) == typeof(PathfindingComponent))
        {
            var pathfindingcomponent = component as PathfindingComponent;
            InternalCall.m_InternalCallGetPathfinding(id, out pathfindingcomponent.m_startPosition, out pathfindingcomponent.m_targetPosition, out pathfindingcomponent.m_gridkey);

        }
        else if (typeof(T) == typeof(EnemyComponent))
        {
            var enemyComponent = component as EnemyComponent;
            InternalCall.m_InternalGetEnemyComponent(id, out enemyComponent.m_tag, out enemyComponent.m_enemyTypeInt, out enemyComponent.m_enemyRoamBehaviourInt);
        }
        else if(typeof(T) == typeof(GridComponent))
        {
            var _component = component as GridComponent;
            InternalCall.m_InternalGetGridComponent(id, out _component.m_Anchor, out _component.m_GridRowLength, out _component.m_GridColumnLength, out _component.m_SetCollidable, out _component.m_GridKey);
        }
        else if (typeof(T) == typeof(LightComponent))
        {
            var _component = component as LightComponent;
            InternalCall.m_InternalCallGetLightingComponent(id, out _component.m_innerOuterRadius, out _component.m_colour, out _component.m_intensity);
        }
        else if (typeof(T) == typeof(ParticleComponent))
        {
            var _component = component as ParticleComponent;
            InternalCall.m_InternalGetParticleComponent(
            id,
            out _component.m_willSpawn,
            out _component.m_noOfParticles,
            out _component.m_lifeSpan,
            out _component.m_size,
            out _component.m_velocity,
            out _component.m_acceleration,
            out _component.m_color,
            out _component.m_coneRotation,
            out _component.m_coneAngle,
            out _component.m_randomFactor,
            out _component.m_imageFile,
            out _component.m_stripCount,
            out _component.m_frameNumber,
            out _component.m_layer,
            out _component.m_friction,
            out _component.m_fps,
            out _component.m_loopAnimation);
        }
        else
        {
            throw new NotSupportedException($"Component type {typeof(T).Name} is not supported.");
        }

        return component;
    }

    public static void Set<T>(uint id, T component)
    {
        if (component is SpriteComponent sprite)
        {
            InternalCall.m_InternalSetSpriteComponent(id, sprite.m_imageFile, in sprite.m_layer, in sprite.m_color, in sprite.m_alpha);
        }
        else if (component is TransformComponent transform)
        {
            InternalCall.m_InternalSetTransformComponent(id, in transform.m_position, in transform.m_scale, in transform.m_rotation);
        }
        else if (component is ColliderComponent collider)
        {
            InternalCall.m_InternalSetColliderComponent(id, in collider.m_Size, in collider.m_Offset, in collider.m_drawDebug, in collider.m_radius, in collider.m_blockedFlag, in collider.m_isCollided, in collider.m_collisionCheck, in collider.m_collisionResponse);
        }
        else if (component is TextComponent text)
        {
            InternalCall.m_InternalSetTextComponent(id, text.m_text, text.m_fileName, in text.m_fontLayer, in text.m_fontSize, in text.m_color);
        }
        else if (component is RigidBodyComponent rigid)
        {
            InternalCall.m_InternalSetRigidBodyComponent(id, in rigid.m_Velocity, in rigid.m_Acceleration, in rigid.m_Rotation, in rigid.m_prevPos, in rigid.m_direction, in rigid.m_Force);
        }
        else if (component is AnimationComponent animation)
        {
            InternalCall.m_InternalSetAnimationComponent(id, in animation.m_frameNumber, in animation.m_framesPerSecond, in animation.m_frameTimer, in animation.m_isAnimating, in animation.m_stripCount);
        }
        else if (component is EnemyComponent enemy)
        {
            InternalCall.m_InternalSetEnemyComponent(id, in enemy.m_tag, in enemy.m_enemyTypeInt, in enemy.m_enemyRoamBehaviourInt);
        }
        else if (component is GridComponent grid)
        {
            InternalCall.m_InternalSetGridComponent(id, in grid.m_Anchor, in grid.m_GridRowLength, in grid.m_GridColumnLength, in grid.m_SetCollidable, in grid.m_GridKey);
        }
        else if (component is LightComponent light)
        {
            InternalCall.m_InternalCallSetLightingComponent(id, in light.m_innerOuterRadius, in light.m_colour, in light.m_intensity);
        }
        else if (component is ParticleComponent particle)
        {
            InternalCall.m_InternalSetParticleComponent(
                         id,
                         particle.m_willSpawn,
                         in particle.m_noOfParticles,
                         in particle.m_lifeSpan,
                         in particle.m_size,
                         in particle.m_velocity,
                         in particle.m_acceleration,
                         in particle.m_color,
                         in particle.m_coneRotation,
                         in particle.m_coneAngle,
                         in particle.m_randomFactor,
                         in particle.m_imageFile,
                         in particle.m_stripCount,
                         in particle.m_frameNumber,
                         in particle.m_layer,
                         in particle.m_friction,
                         in particle.m_fps,
                         particle.m_loopAnimation);
        }
        else
        {
            throw new NotSupportedException($"Component type {typeof(T).Name} is not supported.");
        }
    }
}



public static class GetComponent
{
    public static SpriteComponent GetSpriteComponent(uint id)
    {
        SpriteComponent temp = new SpriteComponent();
        InternalCall.m_InternalGetSpriteComponent(id, out temp.m_imageFile, out temp.m_layer, out temp.m_color, out temp.m_alpha);

        return temp;
    }

    public static TransformComponent GetTransformComponent(uint id)
    {
        TransformComponent temp = new TransformComponent();
        InternalCall.m_InternalGetTransformComponent(id, out temp.m_position, out temp.m_scale, out temp.m_rotation);

        return temp;
    }

    public static ColliderComponent GetColliderComponent(uint id)
    {
        ColliderComponent temp = new ColliderComponent();
        InternalCall.m_InternalGetColliderComponent(id, out temp.m_Size, out temp.m_Offset, out temp.m_drawDebug, out temp.m_radius, out temp.m_blockedFlag, out temp.m_isCollided, out temp.m_collisionCheck, out temp.m_collisionResponse );
        return temp;
    }

    public static TextComponent GetTextComponent(uint id)
    {
        TextComponent temp = new TextComponent();
        InternalCall.m_InternalGetTextComponent(id, out temp.m_text, out temp.m_fileName, out temp.m_fontLayer, out temp.m_fontSize, out temp.m_color);
        return temp;
    }
}
public static class SetComponent
{
    public static void SetSpriteComponent(uint id, SpriteComponent sprite)
    {
        InternalCall.m_InternalSetSpriteComponent(id, sprite.m_imageFile, in sprite.m_layer, in sprite.m_color, in sprite.m_alpha);
    }

    public static void SetTransformComponent(uint id, TransformComponent transform)
    {
        InternalCall.m_InternalSetTransformComponent(id, in transform.m_position, in transform.m_scale, in transform.m_rotation);
    }

    public static void SetCollisionComponent(uint id, ColliderComponent transform)
    {
        InternalCall.m_InternalSetColliderComponent(id, in transform.m_Size, in transform.m_Offset, in transform.m_drawDebug, in transform.m_radius, in transform.m_blockedFlag, in transform.m_isCollided, in transform.m_collisionCheck, in transform.m_collisionResponse);
    }

    public static void SetTextComponent(uint id, TextComponent text)
    {
        InternalCall.m_InternalSetTextComponent(id, text.m_text, text.m_fileName, in text.m_fontLayer, in text.m_fontSize, in text.m_color);
    }
}
