using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class EnemyController : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        InternalCall.m_InternalGetTransformComponent(EntityID, out startingEnemyPos, out startingEnemyScale, out startingEnemyRotate);
        InternalCall.m_InternalGetSpriteComponent(EntityID, out startingSprite, out startingLayer, out startingColor, out startingAlpha);
        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

        enemyDeathTexture = "img_scientistDeath.png";

        playerID = InternalCall.m_InternalCallGetTagID("Player");

        isDead = false;
        isChasing = true;
    }
    #endregion

    public static float enemySpeed;

    public bool isDead;
    private int playerID;
    private Vector2 playerScale;
    private float playerRotate;

    //Starting Variables
    private Vector2 startingEnemyPos;
    private Vector2 startingEnemyScale;
    private float startingEnemyRotate;

    //Sprite Variables
    private string startingSprite;
    private int startingLayer;
    private Vector3 startingColor;
    private float startingAlpha;

    //Animation Variables
    private int frameNumber;
    private int framesPerSecond;
    private float frameTimer;
    private bool isAnimating;
    private int stripCount;

    private bool isChasing;

    public static bool playerIsDead;

    public static bool forceDeath;

    private Vector2 movement;

    private string enemyDeathTexture;

    public static int totalEnemiesKilled;

    public override void Start()
    {
      
    }

    public override void Update()
    {
        if (isDead)
        {
            return;
        }

        if (playerIsDead)
        {
            InternalCall.m_InternalSetAnimationComponent(EntityID, frameNumber, 10, frameTimer, false, stripCount);

            return;
        }

        Vector2 playerPos;
        Vector2 enemyPos;

        InternalCall.m_InternalGetTransformComponent((uint)playerID, out playerPos, out playerScale, out playerRotate);
        InternalCall.m_InternalGetTransformComponent(EntityID, out enemyPos, out startingEnemyScale, out startingEnemyRotate);


        if (isChasing)
        {
            #region Enemy Movement
            Vector2 direction;

            //Gets direction to look towards
            direction.X = (playerPos.X - enemyPos.X);
            direction.Y = (playerPos.Y - enemyPos.Y);

            float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

            InternalCall.m_InternalSetTransformComponent(EntityID, enemyPos, startingEnemyScale, rotationFloat);

            //Convert into radians
            float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

            //Get forward vector X
            float forwardX = (float)(Math.Sin(rotationInRadians));

            //Get forward vector Y
            float forwardY = (float)(Math.Cos(rotationInRadians));

            if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
            {
                // return cause velocity -> rigidbody is not present in entity
                return;
            }

            movement.X = 0 + forwardX * enemySpeed;
            movement.Y = 0 + forwardY * enemySpeed;

            InternalCall.m_InternalSetVelocity(EntityID, movement);
            #endregion

            #region Animate Walking
            if (!isAnimating)
            {
                InternalCall.m_InternalSetAnimationComponent(EntityID, frameNumber, 10, frameTimer, isChasing, stripCount);
                isAnimating = true;
            }

            #endregion

        }

        #region Collision
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "MeleeKillZoneSpawn":
                    case "PlayerBullet":

                        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_humanDeath01");

                        totalEnemiesKilled++;

                        var collisionComponent = GetComponent.GetColliderComponent(EntityID);
                        collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
                        SetComponent.SetCollisionComponent(EntityID, collisionComponent);

                        InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 0, 0, false, 1);
                        InternalCall.m_InternalSetSpriteComponent(EntityID, enemyDeathTexture, 9, startingColor, startingAlpha);

                        Vector2 direction;

                        direction.X = (enemyPos.X - playerPos.X);
                        direction.Y = (enemyPos.Y - playerPos.Y);

                        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

                        InternalCall.m_InternalSetTransformComponent(EntityID, enemyPos, startingEnemyScale, rotationFloat);

                        //Convert into radians
                        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

                        //Get forward vector X
                        float forwardX = (float)(Math.Sin(rotationInRadians));

                        //Get forward vector Y
                        float forwardY = (float)(Math.Cos(rotationInRadians));

                        movement.X = 0 + forwardX * 0.4f;
                        movement.Y = 0 + forwardY * 0.4f;

                        InternalCall.m_InternalSetVelocity(EntityID, movement);

                        isDead = true;

                        Vector2 pos;
                        Vector2 scale;
                        float rotation;

                        InternalCall.m_InternalGetTransformComponent(EntityID, out pos, out scale, out rotation);

                        InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", pos.X, pos.Y, rotation);

                        break;

                    case "Player":

                        playerIsDead = true;
                        InternalCall.m_InternalSetAnimationComponent(EntityID, frameNumber, 10, frameTimer, !playerIsDead, stripCount);

                        movement.X = 0;
                        movement.Y = 0;

                        InternalCall.m_InternalSetVelocity(EntityID, movement);

                        break;

                    default:
                        break;
                }
            }
        }
        #endregion

        #region Cheat Code

        if (forceDeath)
        {
            totalEnemiesKilled++;

            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_humanDeath01");
            var collisionComponent = GetComponent.GetColliderComponent(EntityID);
            collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
            SetComponent.SetCollisionComponent(EntityID, collisionComponent);

            InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 0, 0, false, 1);
            InternalCall.m_InternalSetSpriteComponent(EntityID, enemyDeathTexture, 10, startingColor, startingAlpha);

            Vector2 direction;

            direction.X = (enemyPos.X - playerPos.X);
            direction.Y = (enemyPos.Y - playerPos.Y);

            float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

            InternalCall.m_InternalSetTransformComponent(EntityID, enemyPos, startingEnemyScale, rotationFloat);

            //Convert into radians
            float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

            //Get forward vector X
            float forwardX = (float)(Math.Sin(rotationInRadians));

            //Get forward vector Y
            float forwardY = (float)(Math.Cos(rotationInRadians));

            movement.X = 0 + forwardX * 0.4f;
            movement.Y = 0 + forwardY * 0.4f;

            InternalCall.m_InternalSetVelocity(EntityID, movement);

            isDead = true;

            Vector2 pos;
            Vector2 scale;
            float rotation;

            InternalCall.m_InternalGetTransformComponent(EntityID, out pos, out scale, out rotation);

            InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", pos.X, pos.Y, rotation);
        }

        #endregion

    }
}