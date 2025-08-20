using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class PlayerController : ScriptBase
{
    #region Entity ID
    private uint EntityID;
    private uint eyeID;
    public override void Awake(uint id)
    {
        EntityID = id;

        //Dead Player Texture Allocation
        playerDeathTexture = "img_roombertDeath.png";

        animComp = Component.Get<AnimationComponent>(EntityID);

        //Get starting component values
        InternalCall.m_InternalGetTransformComponent(EntityID, out startingRoombaPos, out startingRoombaScale, out startingRoombaRotate);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startingSprite, out startingLayer, out startingColor, out startingAlpha);

        //Ensures player isnt dead at the start of the game
        isDead = false;

        //Set speed of player
        speed = 4;

        //Set tolerance to prevent jittering, higher values = more rigid rotation, but no more jittering due to micro changes
        angleTolerance = 4f;

        InternalCall.m_InternalGetWorldMousePosition(out previousMousePos);

        eyeID = (uint)InternalCall.m_InternalCallGetTagID("PlayerEyeGlow");

    }
    #endregion

    private bool godMode = false;

    //Player Speed
    public float speed;

    //Starting Values Holder
    private Vector2 startingRoombaPos;
    private Vector2 startingRoombaScale;
    private float startingRoombaRotate;

    //For rotation, angleTolerance to ensure no jittering
    private float previousRotationFloat;
    private float rotationFloat;
    private float angleTolerance;

    //Movement Vector
    private Vector2 movement;

    //prev mouse pos
    private Vector2 previousMousePos;

    //Collision
    private int[] collidedEntities;

    //Sprite Variables
    private string startingSprite;
    private int startingLayer;
    private Vector3 startingColor;
    private float startingAlpha;

    //Stop playing from doing anything if dead
    public static bool isDead;

    //Dead player texture, swap once dead
    private string playerDeathTexture;

    //Components
    private AnimationComponent animComp;
    private bool isAnimating;

    private string movementStartAudio = "aud_vacuumStart01.wav";
    private string movementLoopAudio = "aud_vacuumLoop01.wav";
    private string movementStopAudio = "aud_vacuumEnd01.wav";


    //controller/cursor
    public static bool isControllerLastUsed = false;
    private bool cursoroff = false;

    //godmode color
    private Vector2 innterOuterRadius;
    private Vector3 color;
    private static float intensity;
    public override void Start()
    {
        InternalCall.m_InternalCallGetLightingComponent(eyeID, out innterOuterRadius, out color, out intensity);

    }

    public override void Update()
    {
        //Checks if game is paused and prevents player from doing anything
        if (GameControllerLevel1.gameIsPaused) { return; }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.LeftControl) && (InternalCall.m_InternalCallIsKeyTriggered(keyCode.L))
)
        {
            
                if (godMode == true)
                {

                    color.R = 1f;
                    color.G = 0f;
                    color.B = 0f;

                godMode = false;
                }
                else if (godMode == false)
                {
                    color.R = 0f;
                    color.G = 1f;
                    color.B = 0.4f;

                    godMode = true;
                }          
            

            InternalCall.m_InternalCallSetLightingComponent(eyeID, innterOuterRadius, color, intensity);

        }

        //Dead player, return to prevent actions
        if (isDead) { return; }

        #region Movement WASD

        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            // return cause velocity -> rigidbody is not present in entity
            return;
        }

        movement.X = 0;
        movement.Y = 0;

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.W))
        {
            movement.Y = speed; // Move up if not blocked
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.S))
        {
            movement.Y = -speed; // Move down if not blocked
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.A))
        {
            movement.X = -speed; // Move left if not blocked
        }

        if (InternalCall.m_InternalCallIsKeyPressed(keyCode.D))
        {
            movement.X = speed; // Move right if not blocked
        }

        if (InternalCall.m_InternalCallIsControllerPresent())
        {
            float[] axis = InternalCall.m_InternalCallGetJoyStickAxis();

            if(axis != null)
            {
                float deadzone = 0.2f;
                if (Math.Abs(axis[0]) > deadzone)
                {
                   // Console.WriteLine(axis[0]);
                    movement.X = speed * axis[0];
                }
                if (Math.Abs(axis[1]) > deadzone)
                {
                    movement.Y = speed * axis[1];
                }
            }

        }

        

        //Normalize to prevent diagonal movement from adding speed

        movement = NormalizeAndScale(movement.X, movement.Y, speed);

        if (!PlayerGun.playerBoost)
        {
            InternalCall.m_InternalSetVelocity(EntityID, movement);
        }

        if (PlayerLoadoutManager.isSortieing)
        {
            movement.X = 0;
            movement.Y = 0;
            InternalCall.m_InternalSetVelocity(EntityID, movement);

        }


        #endregion

        #region Collision
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f && !godMode)
        {
            collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (isDead) return;
                if ( !PlayerGun.playerBoost)
                {
                    switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                    {
                        case "Enemy":
                        case "EnemyBullet":
                        case "PlayerRailgunBullet":
                        case "Boss":
                            CameraFollowPlayerScript.Shake(10f, 1f);

                            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_playerDeath01");

                            var collisionComponent = GetComponent.GetColliderComponent(EntityID);
                            collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
                            SetComponent.SetCollisionComponent(EntityID, collisionComponent);

                            InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 0, 0, false, 1);
                            InternalCall.m_InternalSetSpriteComponent(EntityID, playerDeathTexture, startingLayer, startingColor, startingAlpha);

                            movement.X = 0;
                            movement.Y = 0;

                            InternalCall.m_InternalSetVelocity(EntityID, movement);

                            isDead = true;

                            CoroutineManager.Instance.PauseAllCoroutines();

                            InternalCall.m_InternalCallSetTimeScale(0);
                            break;

                        default:
                            break;

                    }
                }

                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "Wall":
                        if (PlayerGun.playerBoost)
                        {
                            CameraFollowPlayerScript.Shake(10f, 1f);
                            PlayerGun.playerBoost = false;
                        }
                        break;

                    case "Boss":
                    case "LaserWall":

                        CameraFollowPlayerScript.Shake(10f, 1f);

                        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_playerDeath01");

                        var collisionComponent = GetComponent.GetColliderComponent(EntityID);
                        collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
                        SetComponent.SetCollisionComponent(EntityID, collisionComponent);

                        InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 0, 0, false, 1);
                        InternalCall.m_InternalSetSpriteComponent(EntityID, playerDeathTexture, startingLayer, startingColor, startingAlpha);

                        movement.X = 0;
                        movement.Y = 0;

                        InternalCall.m_InternalSetVelocity(EntityID, movement);

                        isDead = true;

                        InternalCall.m_InternalCallSetTimeScale(0);

                        CoroutineManager.Instance.PauseAllCoroutines();

                        break;

                    default:
                        break;
                }
            }
        }

        #endregion

        if (PlayerGun.playerBoost || PlayerLoadoutManager.isSortieing)
        {
            return;
        }


        #region Mouse Rotation

        Vector2 mousePos;
        Vector2 roombaPos;
        //Get pos of player
        InternalCall.m_InternalGetTransformComponent(EntityID, out roombaPos, out startingRoombaScale, out startingRoombaRotate);
        //Get pos of mouse
        InternalCall.m_InternalGetWorldMousePosition(out mousePos);
        Vector2 screenMousePos;
        InternalCall.m_InternalGetMousePosition(out screenMousePos);
        if (previousMousePos.X != screenMousePos.X && previousMousePos.Y != screenMousePos.Y)
        {

            Vector2 direction;

            //Gets direction to look towards
            direction.X = (mousePos.X - roombaPos.X);
            direction.Y = (mousePos.Y - roombaPos.Y);

            rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

            previousMousePos = screenMousePos;

            isControllerLastUsed = false;

            if (cursoroff)
            {
                cursoroff = false;
                InternalCall.m_InternalCallHideCursor(false);
            }
           // Console.WriteLine("Mouse");
        }
       

        float controllerRotation = InternalCall.m_InternalCallGetRightJoyStickRotation();

        if(controllerRotation > 0)
        {
            rotationFloat = controllerRotation;
            isControllerLastUsed = true;

            if (!cursoroff)
            {
                cursoroff = true;
                InternalCall.m_InternalCallHideCursor(true);
            }

            // Console.WriteLine("Controller");
        }


        if ((rotationFloat > previousRotationFloat + angleTolerance) || (rotationFloat < previousRotationFloat - angleTolerance))
        {
            InternalCall.m_InternalSetTransformComponent(EntityID, roombaPos, startingRoombaScale, rotationFloat);

            previousRotationFloat = rotationFloat;

        }

        #endregion

        CheckMovement();

    }

    public void LateUpdate()
    {
        //Checks if game is paused and prevents player from doing anything
        if (GameControllerLevel1.gameIsPaused) { return; }

        if (isDead) { return; }

    }

    private void CheckMovement()
    {
        Vector2 temp;
        InternalCall.m_InternalGetVelocity(EntityID, out temp);
        RigidBodyComponent rigidBodyComponent = Component.Get<RigidBodyComponent>(EntityID);

        if ((Magnitude(temp) != 0 && isAnimating == false)||(Magnitude(rigidBodyComponent.m_Acceleration) != 0 && isAnimating == false))
        {
            if (isAnimating == false)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, movementStartAudio);
                InternalCall.m_InternalCallPlayAudio(EntityID, movementLoopAudio);
                animComp = Component.Get<AnimationComponent>(EntityID);
                animComp.m_isAnimating = true;
                Component.Set<AnimationComponent>(EntityID, animComp);
                isAnimating = true;


            }

            
        }

        else if ((Magnitude(temp) == 0 && Magnitude(rigidBodyComponent.m_Acceleration) == 0 && isAnimating == true))
        {
            InternalCall.m_InternalCallStopAudio(EntityID, movementLoopAudio);
            InternalCall.m_InternalCallPlayAudio(EntityID, movementStopAudio);
            animComp = Component.Get<AnimationComponent>(EntityID);
            animComp.m_isAnimating = false;
            animComp.m_frameNumber = 0;
            Component.Set<AnimationComponent>(EntityID, animComp);
            isAnimating = false;
        }

    }

    #region Vec2 Functions
    public Vector2 NormalizeAndScale(float x, float y, float speed)
    {
        // Calculate the magnitude of the vector
        double magnitude = Math.Sqrt(x * x + y * y);

        // Handle the zero vector case
        if (magnitude == 0)
        {
            Vector2 empty;

            empty.X = 0;
            empty.Y = 0;

            return empty;
        }

        // Normalize the vector
        double normalizedX = x / magnitude;
        double normalizedY = y / magnitude;

        // Scale the normalized vector by the desired speed
        double scaledX = normalizedX * speed;
        double scaledY = normalizedY * speed;

        Vector2 temp;

        temp.X = (float)scaledX;
        temp.Y = (float)scaledY;

        return temp;
    }

    public float Magnitude(Vector2 v2)
    {
        return (float)Math.Sqrt(v2.X * v2.X + v2.Y * v2.Y);
    }

    public Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistance)
    {
        float dx = target.X - current.X;
        float dy = target.Y - current.Y;
        float distance = (float)Math.Sqrt(dx * dx + dy * dy);

        // If the distance to the target is less than maxDistance, move directly to the target
        if (distance <= maxDistance || distance == 0f)
        {
            return target;
        }

        // Otherwise, move a fraction towards the target
        float ratio = maxDistance / distance;
        return new Vector2(
            current.X + dx * ratio,
            current.Y + dy * ratio
        );
    }
    #endregion
}