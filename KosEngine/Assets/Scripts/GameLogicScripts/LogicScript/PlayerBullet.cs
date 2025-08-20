using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerBullet : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        speed = 12;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);

        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

        rotation = startingBulletRotate;

        //Convert into radians
        rotationInRadians = (float)((rotation) * Math.PI / 180.0);

        //Get forward vector X
        forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        forwardY = (float)(Math.Cos(rotationInRadians));
    }
    #endregion

    public float speed;

    private Vector2 startingBulletPos;
    private Vector2 startingBulletScale;
    private float startingBulletRotate;

    private float forwardX;
    private float forwardY;
    private float rotation, rotationInRadians;

    //Animation Variables
    private int frameNumber;
    private int framesPerSecond;
    private float frameTimer;
    private bool isAnimating;
    private int stripCount;

    private bool bulletHasHit;

    public override void Start()
    {
        bulletHasHit = false;
        //Console.WriteLine(EntityID);
    }

    public override void Update()
    {
        if (bulletHasHit) { return; }
        #region Movement in forward direction
        
        if (!isAnimating)
        {
            Vector2 movement;

            if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }

            movement.X = 0 + forwardX * speed;
            movement.Y = 0 + forwardY * speed;

            InternalCall.m_InternalSetVelocity(EntityID, movement);


        }

        else if (isAnimating)
        {
            

            InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

            if (frameNumber == stripCount - 1)
            {
                bulletHasHit = true;
                InternalCall.m_InternalCallDeleteEntity(EntityID);
                return;
            }

            return;
        }

        
        #endregion

        #region Collision

        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (bulletHasHit) { return; }
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "PropGlassWall":
                    case "Wall":
                       
                        if (!isAnimating)
                        {
                            isAnimating = true;
                            InternalCall.m_InternalSetAnimationComponent(EntityID, in frameNumber, in framesPerSecond, in frameTimer, in isAnimating, in stripCount);

                            Vector2 movement;

                            movement.X = 0;
                            movement.Y = 0;

                            InternalCall.m_InternalSetVelocity(EntityID, movement);

                        }

                        break;

                    case "Enemy":
                    case "Boss":
                        if (!isAnimating)
                        {
                            isAnimating = true;
                            InternalCall.m_InternalSetAnimationComponent(EntityID, in frameNumber, in framesPerSecond, in frameTimer, in isAnimating, in stripCount);

                            Vector2 movement;

                            movement.X = 0;
                            movement.Y = 0;

                            InternalCall.m_InternalSetVelocity(EntityID, movement);

                        }

                        return;

                    default:
                        break;
                }
            }
        }

        #endregion


    }
}