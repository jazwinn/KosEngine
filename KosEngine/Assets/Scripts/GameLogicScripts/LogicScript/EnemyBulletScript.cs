using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.AccessControl;
using System.Text;
using System.Threading.Tasks;

public class EnemyBulletScript : ScriptBase
{
    #region Entity ID
    private uint EntityID;
    private float bulletSpeed = 4.5f;
    private float bulletLifetime = 5.0f;
    private float currentLifetime = 0.0f;
    private Vector2 movement;

    private bool bulletHasHit;

    // Animation Variables
    private int frameNumber;
    private int framesPerSecond;
    private float frameTimer;
    private bool isAnimating;
    private int stripCount;
    #endregion

    public override void Awake(uint id)
    {
        EntityID = id;
        bulletHasHit = false;
    }

    public override void Start()
    {
        InternalCall.m_InternalGetTransformComponent(EntityID, out Vector2 bulletPos, out Vector2 bulletScale, out float bulletRotation);

        float rotationInRadians = (float)(bulletRotation * Math.PI / 180.0);
        float directionX = (float)Math.Sin(rotationInRadians);
        float directionY = (float)Math.Cos(rotationInRadians);

        movement.X = directionX * bulletSpeed;
        movement.Y = directionY * bulletSpeed;

        InternalCall.m_InternalSetVelocity(EntityID, movement);
    }

    public override void Update()
    {
        if (bulletHasHit) { return; }

        //currentLifetime += InternalCall.m_InternalCallGetDeltaTime();
        //if (currentLifetime >= bulletLifetime)
        //{
        //    InternalCall.m_InternalCallDeleteEntity(EntityID);
        //    return;
        //}

        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

        if (isAnimating && frameNumber == stripCount - 1)
        {
            bulletHasHit = true;
            InternalCall.m_InternalCallDeleteEntity(EntityID);
        }

        CheckForCollisions();
    }

    private void CheckForCollisions()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);
            foreach (int collidedEntityID in collidedEntities)
            {
                string tag = InternalCall.m_InternalCallGetTag((uint)collidedEntityID);

                switch (tag)
                {
                    case "Player":
                    case "Wall":
                    case "Obstacle":
                    case "MeleeKillZoneSpawn":
                    case "PlayerBullet":
                    case "PlayerRailgunBullet":
                        if (!isAnimating)
                        {
                            isAnimating = true;
                            InternalCall.m_InternalSetAnimationComponent(EntityID, in frameNumber, in framesPerSecond, in frameTimer, in isAnimating, in stripCount);

                            movement.X = 0;
                            movement.Y = 0;
                            InternalCall.m_InternalSetVelocity(EntityID, movement);
                        }
                        break;
                }
            }
        }
    }
}
