using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

public class BossBullet : ScriptBase
{
    #region Entity ID
    private uint EntityID;
    private AnimationComponent animComp;
    private bool isAnimating = false;
    private bool waitingToDelete = false;

    public override void Awake(uint id)
    {
        EntityID = id;

        speed = 2;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);

        rotation = startingBulletRotate;

        rotationInRadians = (float)(rotation * Math.PI / 180.0);

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);


        // Calculate forward movement
        forwardX = (float)Math.Cos(rotationInRadians);
        forwardY = (float)Math.Sin(rotationInRadians);

        //Console.WriteLine($"[Bullet Spawned] Position: X={startingBulletPos.X}, Y={startingBulletPos.Y}, Rotation={rotation}°");
        //Console.WriteLine($"[Bullet Direction] ForwardX={forwardX}, ForwardY={forwardY}");

        animComp = Component.Get<AnimationComponent>(EntityID);

    }
    #endregion

    public float speed;
    private Vector2 startingBulletPos;
    private Vector2 startingBulletScale;
    private float startingBulletRotate;

    private float forwardX;
    private float forwardY;
    private float rotation, rotationInRadians;

    private bool bulletHasHit;

    public override void Start()
    {
        bulletHasHit = false;

    }

    public override void Update()
    {
        if (waitingToDelete)
        {
            InternalCall.m_InternalCallDeleteEntity(EntityID);
            return;
        }

        if (bulletHasHit)
        {
            AnimationComponent bulletAnimComp = Component.Get<AnimationComponent>(EntityID);

            // Check if animation has finished
            if (bulletAnimComp.m_frameNumber >= bulletAnimComp.m_stripCount - 1)
            {
                // Once the animation is done, mark it for deletion
                waitingToDelete = true;
            }



            return;
        }

        #region Movement in Forward Direction
        Vector2 movement;

        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            return;
        }

        movement.X = forwardX * speed;
        movement.Y = forwardY * speed;

        InternalCall.m_InternalSetVelocity(EntityID, movement);
        #endregion

        #region Collision Handling
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if(bulletHasHit) return;
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "PropGlassWall":
                    case "Wall":
                        bulletHasHit = true;
                        animComp.m_isAnimating = true;
                        animComp.m_framesPerSecond = 20;
                        Component.Set<AnimationComponent>(EntityID, animComp);
                        InternalCall.m_InternalSetVelocity(EntityID, new Vector2(0, 0));
                        break;
                    case "Player": // If it hits the player, delete it
                        
                        InternalCall.m_InternalCallDeleteEntity(EntityID);

                        break;

                    default:
                        break;
                }
            }
        }
        #endregion
    }

}

