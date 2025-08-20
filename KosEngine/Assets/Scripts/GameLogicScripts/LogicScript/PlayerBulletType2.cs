using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerBulletType2 : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        speed = 20;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingBulletPos, out startingBulletScale, out startingBulletRotate);

        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

        rotation = startingBulletRotate;

        //Convert into radians
        rotationInRadians = (float)((rotation) * Math.PI / 180.0);

        //Get forward vector X
        forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        forwardY = (float)(Math.Cos(rotationInRadians));

        BulletRichochet = 3;
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

    private int BulletRichochet; // delete bullet once it reaches 0

    private int prevWall; //ignore previous wall entity

    private bool bulletHasHit;

    public override void Start()
    {
        prevWall = -1;
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

        else
        {
            

            InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

            if (frameNumber == stripCount - 1)
            {
                bulletHasHit = true;
                BulletRichochet = 0;
                InternalCall.m_InternalCallDeleteEntity(EntityID);
                return;
            }
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
                        if (prevWall == collidedEntitiesID) break;
                        prevWall = collidedEntitiesID; //ignore 6 frames

                        BulletRichochet--;
                        //richoceht logic

                        if(BulletRichochet > 0 )
                        {

                            Vector2 bulletpos, wallcenter;
                            InternalCall.m_InternalGetTranslate(EntityID, out bulletpos);
                            InternalCall.m_InternalGetTranslate((uint)collidedEntitiesID, out wallcenter);

                            ColliderComponent wallCC = Component.Get<ColliderComponent>((uint)collidedEntitiesID);
                            float halfWallWidth = wallCC.m_Size.X / 2f;
                            float halfWallHeight = wallCC.m_Size.Y / 2f;

                            Vector2 normal = GetRectangleNormal(bulletpos, wallcenter, halfWallWidth, halfWallHeight);
                                //Console.WriteLine(normal.X);
                                //Console.WriteLine(normal.Y);
                            Vector2 bulletVelocity;
                            InternalCall.m_InternalGetVelocity(EntityID, out bulletVelocity);

                            float newRotation = 0;

                            // Calculate the wall normal angle in degrees
                            float wallNormalAngle = (float)Math.Atan2(normal.Y, normal.X) * (180f / (float)Math.PI);

                            // Reflect the rotation using the general reflection formula
                            newRotation = 2 * wallNormalAngle - rotation;

                            //Console.WriteLine(rotation);
                            //set rotation to transform component

                            TransformComponent tc = Component.Get<TransformComponent>(EntityID);
                            tc.m_rotation = newRotation;
                            Component.Set<TransformComponent>(EntityID, tc);    

                            //Convert into radians
                            rotationInRadians = (float)((newRotation) * Math.PI / 180.0);

                            //Get forward vector X
                            forwardX = (float)(Math.Sin(rotationInRadians));

                            //Get forward vector Y
                            forwardY = (float)(Math.Cos(rotationInRadians));

                            rotation = newRotation;


                        }

   


                        break;

                    case "Enemy":
                    case "Boss":
                        BulletRichochet--;
                        break;

                    case "Player":
                        bulletHasHit = true;
                        InternalCall.m_InternalCallDeleteEntity(EntityID);

                        break;

                    default:
                        break;
                }

                if (bulletHasHit) { return; }

                if (!isAnimating && BulletRichochet <= 0)
                {
                    isAnimating = true;
                    InternalCall.m_InternalSetAnimationComponent(EntityID, in frameNumber, in framesPerSecond, in frameTimer, in isAnimating, in stripCount);

                    Vector2 movement;

                    movement.X = 0;
                    movement.Y = 0;

                    InternalCall.m_InternalSetVelocity(EntityID, movement);

                    return;
                }
            }
        }

        #endregion

    }

    public Vector2 GetRectangleNormal(Vector2 bulletPos, Vector2 rectCenter, float halfWidth, float halfHeight)
    {
        float left = rectCenter.X - halfWidth;
        float right = rectCenter.X + halfWidth;
        float top = rectCenter.Y + halfHeight;
        float bottom = rectCenter.Y - halfHeight;

        float dxLeft = Math.Abs(bulletPos.X - left);
        float dxRight = Math.Abs(bulletPos.X - right);
        float dyTop = Math.Abs(bulletPos.Y - top);
        float dyBottom = Math.Abs(bulletPos.Y - bottom);

        float minDx = Math.Min(dxLeft, dxRight);
        float minDy = Math.Min(dyTop, dyBottom);

        if (Math.Abs(minDx - minDy) < 0.001f) // Bullet hit close to a corner (floating-point precision check)
        {
            return new Vector2(
                (bulletPos.X < rectCenter.X) ? -1 : 1,
                (bulletPos.Y < rectCenter.Y) ? -1 : 1
            ); // Diagonal normal (corner hit)
        }
        else if (minDx < minDy) // Closer to left/right edge
        {
            return (dxLeft < dxRight) ? new Vector2(-1, 0) : new Vector2(1, 0); // Left or Right normal
        }
        else // Closer to top/bottom edge
        {
            return (dyBottom < dyTop) ? new Vector2(0, -1) : new Vector2(0, 1); // Bottom or Top normal
        }
    }

}