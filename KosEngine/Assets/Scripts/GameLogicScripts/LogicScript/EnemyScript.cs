using System;
using System.Collections;
using System.Collections.Generic;
using System.Data;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;
using System.Threading;
using static EnemyScript;


public class EnemyScript : ScriptBase //Enemy Script, not state machine
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!

    private uint playerID; //Store player ID
    #endregion

    #region Enemy State Machine Variables
    public enum EnemySelection //Enemy type enum, add here for more enemy types
    {
        Helpless,
        Melee,
        Ranged,
        AlertMelee,
        AlertRanged
    };

    public enum EnemyRoamType //Enemy roaming type enum, add here for more roaming behaviours
    {
        Static,
        Patrolling
    };

    public EnemySelection enemyType; //Current Enemy Type

    public EnemyRoamType enemyRoamBehaviour; //Current Enemy Roaming Behaviour

    public EnemyState currentState; //Current Enemy State

    #endregion

    #region Component Variables
    private SpriteComponent spriteComp;
    private AnimationComponent animComp;
    private EnemyComponent enemyComp;
    private PathfindingComponent pathFindComp;
    private ColliderComponent collComp;
    private TransformComponent transformComp;
    private TransformComponent playerTransformComp;
    private ParticleComponent particleComp;
    //private GridComponent gridComp;
    #endregion

    #region Sprite Variables
    private string enemyScientistDeathTexture; //Enemy Scientist Death Texture
    private string enemyRobotDeathTexture; //Enemy Robot Death Texture
    #endregion

    #region Audio Variables
    private float panRange = 5.0f;

    private string bodyDeathAud1 = "aud_bodyDeath01.wav";

    //Body Falls
    private string bodyFallAud1 = "aud_bodyFall01.wav";
    private string bodyFallAud2 = "aud_bodyFall02.wav";
    private string bodyFallAud3 = "aud_bodyFall03.wav";
    private string bodyFallAud4 = "aud_bodyFall04.wav";
    private string bodyFallAud5 = "aud_bodyFall05.wav";
    private string bodyFallAud6 = "aud_bodyFall06.wav";
    private string bodyFallAud7 = "aud_bodyFall07.wav";
    private string bodyFallAud8 = "aud_bodyFall08.wav";
    private string bodyFallAud9 = "aud_bodyFall09.wav";

    private string bodyStabAud1 = "aud_bodyStab01.wav";
    private string bodyStabAud2 = "aud_bodyStab02.wav";

    private string rangedEnemyDeathAud1 = "aud_playerDeath01.wav";
    private string enemyShootAud1       = "aud_railgunShot01.wav";

    private List<string> bodyDeathAudList = new List<string>();
    private List<string> bodyFallAudList = new List<string>();
    private List<string> bodyStabAudList = new List<string>();

    #endregion

    //TO BE COMMENTED
    public bool isDead;
    private bool isChasing;
    public bool isPatrolling;
    private bool isAnimating;
    private Vector2 movement = new Vector2();
    private float enemyDeathKnockbackMultiplier;
    private float enemyBloodPoolSpawnDelay = 0.5f;
    private float enemySpeed = 1.9f;
    private float patrolSpeed = 1.9f;
    private float enemyFOVangle = 300.0f;
    private float enemyFOVdistance = 15.0f;

    private float scanTime = 0f;
    private bool scanning = false;
    private float scanDuration = 3.0f;
    private float initialRotation;
    private bool scanningRight = true;
    private bool animateRobotDeath = false;
    private bool spewGuts = false;

    #region Waypoint Variables
    private int initialPatrolWaypoint = 0;
    private int currentPatrolWaypoint = 0;
    private int[] childrenIDList;
    private List<Vector2> waypoints;
    private int currentPatrolPath = 0;
    private int currentChasePath = 0;

    private List<Vector2> Paths;
    private List<Vector2> ChasePaths;
    #endregion

    #region Pathfinding Variables
    private bool pFReachedEndOfPath;
    #endregion

    #region Enemy Variables
    private Vector2 originalPosition;

    private float fireRate = 2.5f;
    private float fireFirstDelay = 0.5f;
    private float fireTimer = 0f;
    private float shuffleDistance = 0.20f;
    private bool  rangedShuffleLeft = true;
    private float rangedShuffleRate = 1.1f;
    private float rangedShuffleTimer = 0f;
    private float targetCheckInterval = 0.3f;
    private float targetCheckTimer = 0f;

    private float searchDuration = 5.0f;
    private float searchTimer;
    private float scanInterval = 0.5f;
    private float scanTimer = 0f;
    private Vector2 lastKnownPlayerPosition;
    #endregion

    #region Particle Variables
    private bool willSpawn = false;          // Whether the particle system should spawn particles
    //private int noOfParticles = 1;       // Number of particles in the system
    //private float lifeSpan = 3f;          // Lifespan of each particle
    //private Vector2 velocity;        // Initial velocity of the particles
    //private Vector2 acceleration;    // Acceleration affecting the particles
    //private Vector3 color;           // Color of the particles
    //private float coneRotation = 0;      // Rotation of the emission cone
    //private float coneAngle = 360f;         // Angle of the emission cone
    //private float randomFactor = 0;      // Randomness applied to particle properties
    //private string imageFile;        // Texture file for the particle
    //private int stripCount = 0;          // Number of strips in the particle animation
    //private int frameNumber = 0;         // Current frame in animation
    //private int layer = 500;               // Layer the particle system belongs to
    //private float friction = 0;          // Friction affecting particle movement
    //private int fps = 0;                 // Frames per second for particle animation
    #endregion

    public override void Awake(uint id) //Called everytime instance is created
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH

        //UpdateRayCastToPlayerPosition(); //Updates Raycast Position when spawned, for objecting pooling, etc.
    }

    public override void Start() //Called once at the start of the game
    {
        AddAudioToLists();

        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID
        UpdateComponentValues();
        enemyScientistDeathTexture = "ani_scientistDeathPose_strip5.png";
        enemyRobotDeathTexture = "ani_rangedEnemyDeath_strip11.png"; //Set to ranged enemy death texture

        originalPosition = transformComp.m_position;
        enemyDeathKnockbackMultiplier = 0.4f;


        switch (enemyComp.m_enemyTypeInt) //Sets enemy type based on EnemyComponent enemy type int ID
        {
            //Helpless Melee
            case 0:
                enemyType = EnemySelection.Helpless; //Set enemy type to helpless
                break;

            //Normal Melee
            case 1:
                enemyType = EnemySelection.Melee; //Set enemy type to melee
                break;

            //Ranged
            case 2:
                enemyType = EnemySelection.Ranged; //Set enemy type to ranged
                break;

            case 3:
                enemyType = EnemySelection.AlertMelee; //Set enemy type to AlertMelee
                break;

            case 4:
                enemyType = EnemySelection.AlertRanged; //Set enemy type to AlertRanged
                break;

            default:
                break;
        }

        switch (enemyComp.m_enemyRoamBehaviourInt) //Sets enemy roaming behaviour based on EnemyComponent roam behaviour int ID
        {
            //Static, non moving
            case 0:
                enemyRoamBehaviour = EnemyRoamType.Static; //Set enemy type to static turrets
                break;

            //Roamers
            case 1:
                enemyRoamBehaviour = EnemyRoamType.Patrolling; //Set enemy type to roamers, with pathing
                break;

            default:
                break;
        }

        UpdateRayCastToPlayerPosition(); //Updates Raycast Position at the start to lock onto the Player

        currentState = CheckEnemyType(); // Checks enemy type to start off new state

        if (enemyRoamBehaviour == EnemyRoamType.Patrolling)
        {
            childrenIDList = InternalCall.m_InternalCallGetChildrenID(EntityID);
            StoreWaypoints();

            if (waypoints == null || waypoints.Count == 0)
            {
                //Console.WriteLine("[WARNING] No waypoints found. Enemy will remain static.");
                enemyRoamBehaviour = EnemyRoamType.Static;
                return;
            }

            // Ensure `currentPatrolWaypoint` is within a valid range
            if (currentPatrolWaypoint < 0 || currentPatrolWaypoint >= waypoints.Count)
            {
                //Console.WriteLine($"[ERROR] Invalid waypoint index {currentPatrolWaypoint}. Resetting to 0.");
                currentPatrolWaypoint = 0;
            }

            Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];
            Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);
            pathFindComp.m_targetPosition = gridTargetPos;

            // Ensure GetPath() is only called if there is a valid target
            Paths = GetPath(
               pathFindComp.m_gridkey,
               (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).X,
               (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).Y,
               (int)pathFindComp.m_targetPosition.X,
               (int)pathFindComp.m_targetPosition.Y
            );

            if (Paths == null || Paths.Count == 0)
            {
                //Console.WriteLine("[WARNING] No valid path found! Enemy may remain idle.");
            }
        }
    }

    public override void Update() //Runs every frame
    {
        if (animateRobotDeath)
        {
            int frameNumber;
            int fps;
            float timer;
            bool isAnimating;
            int stripCount;
            InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out fps, out timer, out isAnimating, out stripCount);
            if (frameNumber == 8 && spewGuts)
            {
                int[] childs = InternalCall.m_InternalCallGetChildrenID(EntityID);
                for (int i = 0; i < 11; i++)
                {
                    uint childID = (uint)childs[i];
                   InternalCall.m_InternalCallSpawnParticle(childID);
                }
                spewGuts = false;
            }
            if (frameNumber == stripCount - 1)
            {
                InternalCall.m_InternalSetAnimationComponent(EntityID, 10, 8, 0, false, 11); //Stops animation of enemy
               
                animateRobotDeath = false;
            }
        }
        if (isDead || PlayerController.isDead || GameControllerLevel1.gameIsPaused) return;
        CheckForCollisions(); //Checks for collisions in the event an enemy touches the player
        CheckWalking();
        currentState.DoActionUpdate(InternalCall.m_InternalCallGetDeltaTime()); //Update the current state's DoActionUpdate function, such as patrolling, chasing etc, with delta time
    }

    #region Enemy State Manager Functions
    public void SetCurrentState(EnemyState nextState) //Sets state for the current enemy type
    {
        currentState = nextState; //Set current state
    }

    public EnemyState CheckEnemyType()
    {
        switch (enemyType)
        {
            case EnemySelection.Helpless:
                return new EnemyStatePatrol(this);

            case EnemySelection.Melee:
                return new EnemyStateScan(this); // Start with scanning before chasing

            case EnemySelection.Ranged:
                return new EnemyStateScan(this); // Scanning before attacking

            case EnemySelection.AlertMelee:
                return new EnemyStateChase(this);

            case EnemySelection.AlertRanged:
                return new EnemyStateRangedSearch(this);

            default:
                //Console.WriteLine("[ERROR] Invalid enemy type!");
                return null;
        }
    }


    #endregion

    #region Raycasting Functions
    public void UpdateRayCastToPlayerPosition() //Do before using Raycast
    {

        Raycast rC = InternalCall.m_GetRay(EntityID, "RaycastToPlayer"); //Gets current ray

        UpdateComponentValues(); //Updates components to get latest values

        rC.m_targetposition = playerTransformComp.m_position; //Set ray.Targetposition to player position through transform component, have to use Component.Get due to every frame position being updated

        InternalCall.m_SetRay(EntityID, "RaycastToPlayer", rC); //Sets ray
    }
    public bool CheckPlayerWithinSight() //Checks if able to Raycast to player, prevents enemies from seeing player through walls
    {
        //Draws raycast from enemy to player
        Raycast rC = InternalCall.m_GetRay(EntityID, "RaycastToPlayer"); //Gets current ray, ensure UpdateRaycastToPlayerPosition was called before this

        if (rC.m_targetReached == true) //Checks if able to reach the "target"/player
        {
            return true; //Returns true if able
        }

        else { return false; } //Returns false if unable
    }
    #endregion

    #region Collisions
    private void CheckForCollisions()
    {

        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {

            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);


            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (isDead) return;
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "MeleeKillZoneSpawn":
                        CoroutineManager.Instance.StartCoroutine(EnemyDeath("Katana"), "EnemyDeath");
                        break;
                    case "PlayerRailgunBullet":
                        CoroutineManager.Instance.StartCoroutine(EnemyDeath("Railgun"), "EnemyDeath"); //Runs coroutine to spawn blood pool
                        break;
                    case "PlayerShotgunBullet":
                        CoroutineManager.Instance.StartCoroutine(EnemyDeath("Shotgun"), "EnemyDeath"); //Runs coroutine to spawn blood pool
                        break;
                    case "PlayerBullet":
                        CoroutineManager.Instance.StartCoroutine(EnemyDeath("Gun"), "EnemyDeath"); //Runs coroutine to spawn blood pool
                        break;

                    case "Player":

                        //playerIsDead = true;
                        //InternalCall.m_InternalSetAnimationComponent(EntityID, frameNumber, 10, frameTimer, !playerIsDead, stripCount);
                        if (PlayerGun.playerBoost) //kill enemy if player is boosting
                        {
                            CoroutineManager.Instance.StartCoroutine(EnemyDeath("Booster"), "EnemyDeath"); //Runs coroutine to spawn blood pool
                            break;
                        }
                        movement.X = 0;
                        movement.Y = 0;

                        InternalCall.m_InternalSetVelocity(EntityID, in movement);

                        break;

                    default:
                        break;
                }
            }
        }
    }
    #endregion

    #region Death Handlers
    private void SetDeadEnemySprite(string causeOfDeath) //Function to set dead enemy Sprite
    {
        UpdateComponentValues(); //Get latest sprite component

        spriteComp.m_layer = 9; //Sets layer to prevent clipping

        switch (enemyType)
        {
            case EnemySelection.Helpless: 
            case EnemySelection.Melee:
            case EnemySelection.AlertMelee:
                spriteComp.m_imageFile = enemyScientistDeathTexture;
                InternalCall.m_InternalSetAnimationComponent(EntityID, GetDeathFrameNumber(causeOfDeath), 0, 0, false, 5); //Stops animation of enemy
               

                break;

            case EnemySelection.Ranged: //Set dead robot image
            case EnemySelection.AlertRanged:
                spriteComp.m_imageFile = enemyRobotDeathTexture;
                InternalCall.m_InternalSetAnimationComponent(EntityID, 0, 16, 0, true, 11); //Stops animation of enemy
               // InternalCall.m_InternalCallSpawnParticle(EntityID);
                animateRobotDeath = true;
                spewGuts = true;
                break;

            default:
                break;
        }

        Component.Set<SpriteComponent>(EntityID, spriteComp); //Sets sprite component


    }

    private int GetDeathFrameNumber(string causeOfDeath)
    {
        switch (causeOfDeath)
        {
            case "Gun":
                return 0;

            case "Shotgun":
                return 1;

            case "Railgun":
                return 2;

            case "Katana":
                
                //Spawn prefab
                

                return 3;

            default:
                return 0;
        }
    }


    private IEnumerator EnemyDeath(string causeOfDeath) //Coroutine for enemy death
    {
        if (isDead) yield break;
        CoroutineManager.Instance.StartCoroutine(PlayEnemyDeathAudio(causeOfDeath), "EnemyDeathAudio");
       
        isDead = true;
        currentState.EnemyDead();

       

        //collComp.m_collisionCheck = !collComp.m_collisionCheck; //Disables collision check
        //collComp.m_collisionResponse = false;
        InternalCall.m_ChangeLayer(EntityID, 12);


        // SetComponent.SetCollisionComponent(EntityID, collComp); //Sets collider of enemy
        SetDeadEnemySprite(causeOfDeath); //Sets dead sprite

       // willSpawn = true;
        //InternalCall.m_InternalCallSpawnParticle(EntityID);



        transformComp = Component.Get<TransformComponent>(EntityID);

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X towards player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y towards player

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation towards player


        InternalCall.m_InternalCallSpawnParticle(EntityID);
        InternalCall.m_InternalCallSetParticleConeRotation(EntityID, rotationFloat);
        InternalCall.m_InternalCallSetParticleLayer(EntityID, 4);// 1 Below the sprite layer;

        transformComp.m_rotation = rotationFloat; //Sets rotation values

        SetComponent.SetTransformComponent(EntityID, transformComp); //Sets transform component

        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X away from player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y away from player

        rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation away player

        if (causeOfDeath == "Katana")
        {
            Random random = new Random();
            rotationFloat += (float)(random.NextDouble() * 150 - 75);
        }


        //Convert into radians
        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);

        //Get forward vector X
        float forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        float forwardY = (float)(Math.Cos(rotationInRadians));

        //+ forwardX * 0.4f
        //movement.X = 0.0f; //Pushes enemy back for "knockback effect"
        //movement.Y = 0.0f; //Pushes enemy back for "knockback effect"

        //Pushes enemy back "Knockback effect" with varying force
        PushbackCorpse(forwardX, forwardY, rotationInRadians, causeOfDeath);

        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        rb.m_Acceleration.X = 0;
        rb.m_Acceleration.Y = 0;
        Component.Set<RigidBodyComponent>(EntityID, rb);

        transformComp = Component.Get<TransformComponent>(EntityID);


        yield return null;
        //yield return new CoroutineManager.WaitForSeconds(enemyBloodPoolSpawnDelay); //Waits for time before moving to next line;


        int poolId = InternalCall.m_InternalCallAddPrefab("prefab_enemyBloodPool", transformComp.m_position.X, transformComp.m_position.Y, transformComp.m_rotation); //Spawns blood pool
        if (enemyType == EnemySelection.Ranged || enemyType == EnemySelection.AlertRanged)
        {
            SpriteComponent sc = Component.Get<SpriteComponent>((uint)poolId);
            sc.m_color.R = 0;
            sc.m_color.G = 0;
            sc.m_color.B = 0;
            Component.Set<SpriteComponent>((uint)poolId, sc);
            //sc.m_color =

            //Turn off light
            if (enemyType == EnemySelection.Ranged || enemyType == EnemySelection.AlertRanged)
            {
                CoroutineManager.Instance.StartCoroutine(TurnLightOff(), "EnemyLights");
            }
        }

        KillCounter.killCount++;
        //Console.WriteLine($"Kill Count: {KillCounter.killCount}");
    }

    private IEnumerator TurnLightOff()
    {
        LightComponent lc = Component.Get<LightComponent>(EntityID);

        float start = lc.m_intensity;   // Start value
        float end = 0;    // Target value
        float duration = 10.0f; // Time in seconds
        double t = 0;

        while (t < 1.0f)
        {
            t += InternalCall.m_InternalCallGetDeltaTime() / duration; // Adjusting t based on deltaTime
            t = Math.Min(t, 1.0f);

            lc = Component.Get<LightComponent>(EntityID);

            lc.m_intensity = Lerp(lc.m_intensity, end, t);

            Component.Set<LightComponent>(EntityID, lc);

        }
        
        yield return null;
    }

    private void PushbackCorpse(float forwardX, float forwardY, float rotationInRadians, string causeOfDeath)
    {
        switch (enemyType)
        {
            case EnemySelection.Helpless:
            case EnemySelection.Melee:
            case EnemySelection.AlertMelee:
                float temp = 0;
                switch (causeOfDeath)
                {
                    case "Pistol":
                        temp = 0.7f;
                        break;
                    case "Railgun":
                        temp = 0f;
                        break;
                    case "Shotgun":
                    case "Booster":
                        temp = 0.9f;
                        break;
                    case "Katana":
                        temp = 0;
                        InternalCall.m_InternalCallAddPrefab("EnemyTopHalf", transformComp.m_position.X  * 1.01f, transformComp.m_position.Y * 1.01f, rotationInRadians);
                        break;
                }
                Random random = new Random();
                float randomForce = 0.05f + (float)random.NextDouble() * temp; // 0.05 to 0.3

                movement.X = forwardX * randomForce;
                movement.Y = forwardY * randomForce;

                InternalCall.m_InternalSetVelocity(EntityID, in movement); //Sets velocity for rigidbody to move

                break;
            default:
                movement.X = 0;
                movement.Y = 0;

                InternalCall.m_InternalSetVelocity(EntityID, in movement); //Sets velocity for rigidbody to move

                break;
        }

    }

    #endregion

    #region Patrolling Behaviour
    public void PatrolSetup()
    {
        if (enemyRoamBehaviour == EnemyScript.EnemyRoamType.Patrolling)
        {
            SetInitialPatrolPaths();

            if (!AreWaypointsAvailable())
            {
                //Console.WriteLine("[WARNING] No valid waypoints for patrol!");
                return;
            }

            StartPatrol();
        }
    }



    public void StartPatrol()
    {
        if (!AreWaypointsAvailable())
        {
            //Console.WriteLine("[ERROR] No waypoints available! Calling StoreWaypoints...");
            //StoreWaypoints();

            if (waypoints == null || waypoints.Count == 0)
            {
                //Console.WriteLine("[ERROR] Waypoints still empty after StoreWaypoints(). Enemy will stop.");
                return;
            }
        }

        if (Paths == null || Paths.Count == 0)
        {
            //Console.WriteLine("[ERROR] Paths list is empty! Resetting to waypoints.");
            SetInitialPatrolPaths();
            return;
        }

        if (currentPatrolPath < 0 || currentPatrolPath >= Paths.Count)
        {
            //Console.WriteLine("[ERROR] Invalid patrol path index: " + currentPatrolPath);
            currentPatrolPath = 0;
        }

        if (!Vector2DistanceChecker(transformComp.m_position, Grid2WorldCoordinates((int)Paths[currentPatrolPath].X, (int)Paths[currentPatrolPath].Y, pathFindComp.m_gridkey), 0.8f))
        {
            MoveToTarget(Paths[currentPatrolPath], patrolSpeed);
        }
        else
        {
            SetToNextPath();
        }

        if (childrenIDList.Length > 0 && !isPatrolling)
        {
            isPatrolling = true;
            CoroutineManager.Instance.StartCoroutine(PatrolRoutine(), "Patrol");
        }
    }

    private IEnumerator PatrolRoutine()
    {
        if (waypoints == null || waypoints.Count == 0)
        {
            //Console.WriteLine("[ERROR] No waypoints found! Stopping patrol.");
            isPatrolling = false;
            yield break;
        }

        while (isPatrolling)
        {
            if (Paths == null || Paths.Count == 0)
            {
                //Console.WriteLine("[ERROR] No waypoints available for fallback movement. Stopping patrol.");
                isPatrolling = false;
                yield break;
            }

            Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];
            Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);
            pathFindComp.m_targetPosition = gridTargetPos;

            yield return new CoroutineManager.WaitForSeconds(5.0f);
        }
    }

    public void SetInitialPatrolPaths()
    {
        if (!AreWaypointsAvailable())
        {
            //Console.WriteLine("[ERROR] No waypoints set for patrolling!");
            enemyRoamBehaviour = EnemyRoamType.Static;
            return;
        }

        //Console.WriteLine("[DEBUG] Setting initial patrol paths...");

        Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];
        Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);
        pathFindComp.m_targetPosition = gridTargetPos;

        Paths = GetPath(
            pathFindComp.m_gridkey,
            (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).X,
            (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).Y,
            (int)pathFindComp.m_targetPosition.X, (int)pathFindComp.m_targetPosition.Y
        );

        if (Paths == null || Paths.Count == 0)
        {
            //Console.WriteLine("[WARNING] No valid path found! Enemy will remain idle.");
            enemyRoamBehaviour = EnemyRoamType.Static;
        }
    }

    public bool AreWaypointsAvailable()
    {
        return waypoints != null && waypoints.Count > 0;
    }


    public void SetToNextWaypoint()
    {
        currentPatrolWaypoint += 1;
        if (currentPatrolWaypoint >= waypoints.Count) // Ensure it wraps correctly
            currentPatrolWaypoint = 0;
    }

    public void SetToNextPath()
    {
        currentPatrolPath += 1;
        if (currentPatrolPath >= Paths.Count) // Ensure it wraps correctly
        {
            Paths.Clear();
            currentPatrolPath = 0;
            SetToNextWaypoint();
            Vector2 targetWaypoint = waypoints[currentPatrolWaypoint];
            Vector2 gridTargetPos = World2GridCoordinates(targetWaypoint.X, targetWaypoint.Y, pathFindComp.m_gridkey);
            pathFindComp.m_targetPosition = gridTargetPos;
            Paths = GetPath(
               pathFindComp.m_gridkey,
               (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).X, (int)World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey).Y,
               (int)pathFindComp.m_targetPosition.X, (int)pathFindComp.m_targetPosition.Y
            );
        }
    }

    public void SetToNextChasePath()
    {
        currentChasePath += 1;
        if (currentChasePath >= ChasePaths.Count) // Ensure it wraps correctly
        {
            ChasePaths.Clear();
            currentChasePath = 0;
            SetCurrentState(new EnemyStateScan(this));
            return;
        }
    }
    #endregion

    #region Component Handlers  
    private void UpdateComponentValues()
    {
        spriteComp = Component.Get<SpriteComponent>(EntityID);
        animComp = Component.Get<AnimationComponent>(EntityID);
        enemyComp = Component.Get<EnemyComponent>(EntityID);
        collComp = Component.Get<ColliderComponent>(EntityID);
        transformComp = Component.Get<TransformComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>(playerID);
        pathFindComp = Component.Get<PathfindingComponent>(EntityID);

    }
    #endregion

    #region Pathfinding
    private void StoreWaypoints()
    {
        //Console.WriteLine("[DEBUG] Storing Waypoints!");

        if (childrenIDList == null || childrenIDList.Length == 0)
        {
            //Console.WriteLine("[WARNING] No child waypoints found!");
            return;
        }

        waypoints = new List<Vector2>(); // Ensure list is fresh

        foreach (var waypointID in childrenIDList)
        {
            Vector2 waypointPos;
            bool success = InternalCall.m_InternalGetTranslate((uint)waypointID, out waypointPos);

            if (!success)
            {
                //Console.WriteLine($"[ERROR] Failed to get translation for waypoint ID {waypointID}!");
                continue;
            }

            waypoints.Add(waypointPos);
        }

        if (waypoints.Count == 0)
        {
            //Console.WriteLine("[WARNING] All waypoints failed to load!");
        }
        else
        {
            //Console.WriteLine($"[INFO] Successfully stored {waypoints.Count} waypoints.");
        }
    }

    public bool ReachedEndOfPathChecker()
    {
        return false;
    }

    public bool Vector2DistanceChecker(Vector2 v1, Vector2 v2, float tolerance)
    {
        float distance = (float)Math.Sqrt(Math.Pow(v2.X - v1.X, 2) + Math.Pow(v2.Y - v1.Y, 2));
        return distance <= tolerance;

    }

    public Vector2 Grid2WorldCoordinates(int gridx, int gridy, int gridkey)
    {
        int gridid = InternalCall.m_InternalGetEntityIdFromGridKey(gridkey);
        GridComponent gridComponent = Component.Get<GridComponent>((uint)gridid);
        float worldX = gridComponent.m_Anchor.X + gridx + 0.5f;
        float worldY = gridComponent.m_Anchor.Y - gridy - 0.5f;
        return new Vector2(worldX, worldY); //This is the World Coordinates from the Grid Coordinates
    }

    public Vector2 World2GridCoordinates(float worldX, float worldY, int gridkey)
    {
        int gridid = InternalCall.m_InternalGetEntityIdFromGridKey(gridkey);
        GridComponent gridComponent = Component.Get<GridComponent>((uint)gridid);
        int gridX = (int)Math.Floor(worldX) - (int)gridComponent.m_Anchor.X;
        int gridY = (int)gridComponent.m_Anchor.Y - (int)Math.Floor(worldY) - 1;

        return new Vector2(gridX, gridY); //This is the Grid Coordinates from the World Coordinates
    }
    public List<Vector2> GetPath(int gridKey, int startX, int startY, int endX, int endY)
    {
        bool success = InternalCall.m_InternalCallGetPath(gridKey, startX, startY, endX, endY, out int[] pathx, out int[] pathy);

        List<Vector2> path = new List<Vector2>();

        if (success && pathx.Length == pathy.Length) // Ensure arrays are valid
        {
            for (int i = 0; i < pathx.Length; i++)
            {
                path.Add(new Vector2(pathx[i], pathy[i]));
            }
        }

        return path;
    }

    #endregion

    #region Enemy Behaviour
    public void ReturnHomeStart()
    {
        currentChasePath = 0;

        // Convert positions to grid coordinates
        Vector2 gridLastPos = World2GridCoordinates(originalPosition.X, originalPosition.Y, pathFindComp.m_gridkey);
        Vector2 gridCurrentPos = World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey);

        // Attempt pathfinding
        ChasePaths = GetPath(pathFindComp.m_gridkey,
                             (int)gridCurrentPos.X, (int)gridCurrentPos.Y,
                             (int)gridLastPos.X, (int)gridLastPos.Y);

        if (ChasePaths.Count == 0)
        {
            HandleFailedPathfinding();
        }
    }

    public void ReturnHomeUpdate()
    {
        if (isDead) return;

        UpdateRayCastToPlayerPosition();
        if (CheckPlayerWithinSight() && IsPlayerInFOV())
        {
            SetCurrentState(new EnemyStateChase(this));
            return;
        }
        MoveToLastKnownPosition(lastKnownPlayerPosition);

    }

    public void RunFromPlayer()
    {
        UpdateRayCastToPlayerPosition();
        if (!CheckPlayerWithinSight())
        {
            currentState.LostTarget();
        }

        //UpdateComponentValues();
        transformComp = Component.Get<TransformComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>(playerID);

        Vector2 direction;

        //Gets direction to look towards
        direction.X = (playerTransformComp.m_position.X - transformComp.m_position.X); //Gets Vector.X towards player
        direction.Y = (playerTransformComp.m_position.Y - transformComp.m_position.Y); //Gets Vector.Y towards player

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation towards player

        transformComp.m_rotation = rotationFloat; //Sets rotation values

        SetComponent.SetTransformComponent(EntityID, transformComp); //Sets transform component

        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X away from player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y away from player

        rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation away player

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

        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        //rb.m_Acceleration = movement;
        rb.m_Velocity = movement;
        Component.Set<RigidBodyComponent>(EntityID, rb);

        //InternalCall.m_InternalSetVelocity(EntityID, in movement); //BANE OF MY EXISTENCE
    }

    public void MoveToTarget(Vector2 targetPosition, float maxSpeed)
    {
        //Console.WriteLine($"Moving to: {targetPosition.X}, {targetPosition.Y}");

        // Convert target grid position to world coordinates
        Vector2 targetPos = Grid2WorldCoordinates((int)targetPosition.X, (int)targetPosition.Y, pathFindComp.m_gridkey);

        // Calculate direction vector
        Vector2 direction = new Vector2(
            targetPos.X - transformComp.m_position.X,
            targetPos.Y - transformComp.m_position.Y
        );

        // Calculate squared magnitude (to avoid sqrt for comparison)
        float squaredMagnitude = (direction.X * direction.X) + (direction.Y * direction.Y);

        // Define a threshold squared distance
        float thresholdSquared = 0.1f;  // (0.1f * 0.1f)

        // If target is reached (magnitude squared is below the threshold), stop
        if (squaredMagnitude < thresholdSquared)
        {
            transformComp.m_position = targetPos;
            movement.X = 0;
            movement.Y = 0;
        }
        else
        {
            // Normalize direction vector
            float magnitude = (float)Math.Sqrt(squaredMagnitude);  // magnitude is used just for normalization
            direction.X /= magnitude;
            direction.Y /= magnitude;

            transformComp.m_rotation = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));
            SetComponent.SetTransformComponent(EntityID, transformComp);

            float dynamicSpeed = maxSpeed * (1 - Math.Min(squaredMagnitude / 100.0f, 1.0f)); // Easing in deceleration

            // Set the movement velocity based on dynamic speed
            movement.X = direction.X * dynamicSpeed;
            movement.Y = direction.Y * dynamicSpeed;
        }


        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        rb.m_Velocity = movement;
        Component.Set<RigidBodyComponent>(EntityID, rb);
        // Set velocity only once
        // InternalCall.m_InternalSetVelocity(EntityID, in movement);
    }

    public void RunAtPlayer()
    {
        UpdateRayCastToPlayerPosition();

        if (!CheckPlayerWithinSight())
        {
            currentState.LostTarget();
            return;
        }

        // Fetch enemy and player transform components
        transformComp = Component.Get<TransformComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>(playerID);

        // Compute direction vector to the player
        Vector2 direction;
        direction.X = playerTransformComp.m_position.X - transformComp.m_position.X;
        direction.Y = playerTransformComp.m_position.Y - transformComp.m_position.Y;

        // Normalize direction to maintain consistent movement speed
        float magnitude = (float)Math.Sqrt(direction.X * direction.X + direction.Y * direction.Y);
        if (magnitude > 0)
        {
            direction.X /= magnitude;
            direction.Y /= magnitude;
        }

        // Compute rotation angle towards the player
        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));
        transformComp.m_rotation = rotationFloat;

        // Apply updated transform
        SetComponent.SetTransformComponent(EntityID, transformComp);

        // Get velocity reference
        if (!InternalCall.m_InternalGetVelocity(EntityID, out movement))
        {
            //Console.WriteLine($"[ERROR] Entity {EntityID} has no Rigidbody!");
            return;
        }

        // Compute movement based on direction
        movement.X = direction.X * enemySpeed;
        movement.Y = direction.Y * enemySpeed;
        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        //rb.m_Acceleration = movement;
        rb.m_Velocity = movement;
        Component.Set<RigidBodyComponent>(EntityID, rb);
        // Apply movement
        //InternalCall.m_InternalSetVelocity(EntityID, in movement);
    }


    //public void PatrolToWaypoint()
    //{
    //    // Call MoveToTarget method with the next waypoint's position
    //    Vector2 waypointPosition = new Vector2(
    //        transformComp.m_position.X + GetComponent.GetTransformComponent((uint)childrenIDList[currentPatrolWaypoint]).m_position.X,
    //        transformComp.m_position.Y + GetComponent.GetTransformComponent((uint)childrenIDList[currentPatrolWaypoint]).m_position.Y
    //    );

    //    InternalCall.m_InternalGetTranslate((uint)childrenIDList[currentPatrolWaypoint], out waypointPosition);

    //    MoveToTarget(waypointPosition, patrolSpeed);
    //}

    public void CheckWithinFOV()
    {
        transformComp = Component.Get<TransformComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>(playerID);

        float enemydirection = transformComp.m_rotation;
        float enemyLeftBound = enemydirection - enemyFOVangle / 2;
        float enemyRightBound = enemydirection + enemyFOVangle / 2;


    }

    #endregion

    #region Audio Handlers
    private void AddAudioToLists()
    {
        //Enemy Death
        bodyDeathAudList.AddRange(new List<string>
        {
            bodyDeathAud1
        });

        //Enemy Body Fall
        bodyFallAudList.AddRange(new List<string>
        {
            bodyFallAud1, bodyFallAud2, bodyFallAud3, bodyFallAud4, bodyFallAud5, bodyFallAud6, bodyFallAud7, bodyFallAud8, bodyFallAud9,
        });

        //Enemy Stab
        bodyStabAudList.AddRange(new List<string>
        {
            bodyStabAud1, bodyStabAud2
        });

        //Console.WriteLine("Sizes are " + bodyDeathAudList.Count + " " + bodyFallAudList.Count + " "+ bodyStabAudList.Count);
    }
    private float CalculateAudioPan(float bulletX)
    {
        uint cameraEntityID = (uint)InternalCall.m_InternalCallGetTagID("Camera");
        TransformComponent camTransformComp = GetComponent.GetTransformComponent(cameraEntityID);

        float camX = camTransformComp.m_position.X;

        float pan = (bulletX - camX) / panRange;
        pan = Clamp(pan, -1.0f, 1.0f);
        return pan;
    }


    private float Clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
    }

    private string ReturnRandomAudio(string name)
    {
        Random random = new Random();

        switch (name)
        {
            case "Death":
                return bodyDeathAudList[(random.Next(bodyDeathAudList.Count - 1))];

            case "Fall":
                return bodyFallAudList[(random.Next(bodyFallAudList.Count - 1))];

            case "Stab":
                return bodyStabAudList[(random.Next(bodyStabAudList.Count - 1))];

            default:
                return null;
        }
    }

    private IEnumerator PlayEnemyDeathAudio(string causeOfDeath)
    {
        switch (enemyType)
        {
            case EnemySelection.Helpless:
            case EnemySelection.Melee:
            case EnemySelection.AlertMelee:
                switch (causeOfDeath)
                {
                    case "Booster":
                    case "Gun":
                    case "Railgun":
                    case "Shotgun":
                        InternalCall.m_InternalCallPlayAudio(EntityID, ReturnRandomAudio("Death")); //Plays enemy death sound
                        yield return new CoroutineManager.WaitForSeconds(0.1f);
                        InternalCall.m_InternalCallPlayAudio(EntityID, ReturnRandomAudio("Fall")); //Plays enemy fall sound
                        break;

                    case "Katana":
                        InternalCall.m_InternalCallPlayAudio(EntityID, ReturnRandomAudio("Stab")); //Plays enemy death sound
                        yield return new CoroutineManager.WaitForSeconds(0.1f);
                        InternalCall.m_InternalCallPlayAudio(EntityID, ReturnRandomAudio("Death")); //Plays enemy fall sound
                        yield return new CoroutineManager.WaitForSeconds(0.2f);
                        InternalCall.m_InternalCallPlayAudio(EntityID, ReturnRandomAudio("Fall")); //Plays enemy fall sound
                        break;

                    default:
                        break;

                }
                break;
            case EnemySelection.Ranged:
            case EnemySelection.AlertRanged:
             InternalCall.m_InternalCallPlayAudio(EntityID, rangedEnemyDeathAud1); //Plays enemy death sound
                break;

            default :
                break;

        }

    }
    #endregion

    #region Animation Handler
    private void CheckWalking()
    {
        Vector2 temp;
        InternalCall.m_InternalGetVelocity(EntityID, out temp);

        if (Magnitude(temp) != 0 && isAnimating == false)
        {

            animComp = Component.Get<AnimationComponent>(EntityID);
            animComp.m_isAnimating = true;
            Component.Set<AnimationComponent>(EntityID, animComp);
            isAnimating = true;
        }

        else if (Magnitude(temp) == 0 && isAnimating == true)
        {
            animComp = Component.Get<AnimationComponent>(EntityID);
            animComp.m_isAnimating = false;
            animComp.m_frameNumber = 0;
            Component.Set<AnimationComponent>(EntityID, animComp);
            isAnimating = false;
        }
    }

    public float Magnitude(Vector2 v2)
    {
        return (float)Math.Sqrt(v2.X * v2.X + v2.Y * v2.Y);
    }

    #endregion

    public bool IsPlayerInFOV()
    {
        UpdateComponentValues();

        Vector2 directionToPlayer = new Vector2(
            playerTransformComp.m_position.X - transformComp.m_position.X,
            playerTransformComp.m_position.Y - transformComp.m_position.Y
        );

        float distanceToPlayer = (float)Math.Sqrt(
            directionToPlayer.X * directionToPlayer.X +
            directionToPlayer.Y * directionToPlayer.Y
        );

        if (distanceToPlayer > enemyFOVdistance)
            return false;

        float angleToPlayer = (float)(Math.Atan2(directionToPlayer.X, directionToPlayer.Y) * (180 / Math.PI));

        float enemyAngle = transformComp.m_rotation;

        float angleDifference = angleToPlayer - enemyAngle;
        while (angleDifference > 180) angleDifference -= 360;
        while (angleDifference < -180) angleDifference += 360;
        angleDifference = Math.Abs(angleDifference);

        return angleDifference <= enemyFOVangle / 2;
    }

    public void FaceTarget()
    {
        UpdateComponentValues();

        Vector2 direction = new Vector2(
            playerTransformComp.m_position.X - transformComp.m_position.X,
            playerTransformComp.m_position.Y - transformComp.m_position.Y
        );

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

        transformComp.m_rotation = rotationFloat;
        SetComponent.SetTransformComponent(EntityID, transformComp);
    }

    public Vector2 GetPlayerPosition()
    {
        UpdateComponentValues();
        return playerTransformComp.m_position;
    }

    public void MoveToLastKnownPosition(Vector2 lastPosition)
    {
        UpdateComponentValues();

        // Ensure ChasePaths is valid before clearing it
        if (ChasePaths == null || ChasePaths.Count == 0)
        {
            //Console.WriteLine("[DEBUG] No path found. Switching to fallback behavior.");

            // Stop movement
            movement.X = 0;
            movement.Y = 0;
            InternalCall.m_InternalSetVelocity(EntityID, in movement);

            // Clear paths only if ChasePaths is not null
            ChasePaths?.Clear();
            currentChasePath = 0;

            // Ensure melee enemies properly return to patrolling
            HandleLostPath();
            return;
        }

        // Convert the current path point to world coordinates
        Vector2 targetPos = Grid2WorldCoordinates((int)ChasePaths[currentChasePath].X, (int)ChasePaths[currentChasePath].Y, pathFindComp.m_gridkey);

        // Move to the next waypoint if not close enough
        if (!Vector2DistanceChecker(transformComp.m_position, targetPos, 0.5f))
        {
            MoveToTarget(ChasePaths[currentChasePath], enemySpeed);
        }
        else
        {
            SetToNextChasePath();
        }
    }

    private void HandleLostPath()
    {
        //Console.WriteLine("[DEBUG] Enemy lost path. Checking behavior...");

        if (enemyRoamBehaviour == EnemyRoamType.Patrolling)
        {
            //Console.WriteLine("[DEBUG] Returning to patrol.");
            SetCurrentState(new EnemyStatePatrol(this));
        }
        else if (enemyRoamBehaviour == EnemyRoamType.Static)
        {
           // Console.WriteLine("[DEBUG] Returning to home position.");
            SetCurrentState(new EnemyStateReturnToHome(this));
        }
        else
        {
            //Console.WriteLine("[DEBUG] Defaulting to scan state.");
            SetCurrentState(new EnemyStateScan(this));
        }
    }


    public void FireAtPlayer()
    {
        UpdateComponentValues();

        float rotationInRadians = (float)(transformComp.m_rotation * Math.PI / 180.0);
        float offsetX = (float)Math.Sin(rotationInRadians) * 1.0f;
        float offsetY = (float)Math.Cos(rotationInRadians) * 1.0f;

        Vector2 bulletSpawnPosition = new Vector2(
            transformComp.m_position.X + offsetX,
            transformComp.m_position.Y + offsetY
        );

        uint bulletID = (uint)InternalCall.m_InternalCallAddPrefab(
            "prefab_enemyBullet",
            bulletSpawnPosition.X,
            bulletSpawnPosition.Y,
            transformComp.m_rotation
        );

        float pan = CalculateAudioPan(bulletSpawnPosition.X);

        InternalCall.m_InternalCallSetPanAudio(EntityID, enemyShootAud1, pan);

        InternalCall.m_InternalCallPlayAudio(EntityID, enemyShootAud1);
    }


    public void MeleeSearchStart()
    {
        //Console.WriteLine("startsearch");
        searchTimer = searchDuration;
        lastKnownPlayerPosition = GetPlayerPosition();
        currentChasePath = 0;

        // Convert last known position to grid coordinates
        Vector2 gridLastPos = World2GridCoordinates(lastKnownPlayerPosition.X, lastKnownPlayerPosition.Y, pathFindComp.m_gridkey);
        Vector2 gridCurrentPos = World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey);

        // Generate path to last known position
        ChasePaths = GetPath(
            pathFindComp.m_gridkey,
            (int)gridCurrentPos.X, (int)gridCurrentPos.Y,
            (int)gridLastPos.X, (int)gridLastPos.Y
        );
        //Console.WriteLine(ChasePaths.Count);
        if (ChasePaths.Count > 0)
        {
            return;
        }
        else
        {
            if (enemyRoamBehaviour == EnemyRoamType.Patrolling)
            {
                SetCurrentState(new EnemyStatePatrol(this));
            }
            else if(enemyRoamBehaviour == EnemyRoamType.Static)
            {
                SetCurrentState(new EnemyStateReturnToHome(this));
            }
        }
    }

    public void MeleeSearchUpdate()
    {
        if (isDead) return;

        UpdateRayCastToPlayerPosition();

        // Check if player is back in sight
        if (CheckPlayerWithinSight() && IsPlayerInFOV())
        {
            // Player spotted again, switch back to attack
            SetCurrentState(new EnemyStateChase(this));
            return;
        }

        // Move toward last known player position
        MoveToLastKnownPosition(lastKnownPlayerPosition);

        searchTimer -= InternalCall.m_InternalCallGetDeltaTime();
        if (searchTimer <= 0)
        {
            SetCurrentState(new EnemyStatePatrol(this));
        }
    }

    public void RangedSearchStart()
    {
        //searchTimer = searchDuration;
        //lastKnownPlayerPosition = GetPlayerPosition();
        //currentChasePath = 0;

        //// Convert positions to grid coordinates
        //Vector2 gridLastPos = World2GridCoordinates(lastKnownPlayerPosition.X, lastKnownPlayerPosition.Y, pathFindComp.m_gridkey);
        //Vector2 gridCurrentPos = World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey);

        //// Attempt pathfinding
        //ChasePaths = GetPath(pathFindComp.m_gridkey,
        //                     (int)gridCurrentPos.X, (int)gridCurrentPos.Y,
        //                     (int)gridLastPos.X, (int)gridLastPos.Y);

        //if (ChasePaths.Count == 0)
        //{
        //    HandleFailedPathfinding();
        //}
        //Console.WriteLine("startsearch");
        searchTimer = searchDuration;
        lastKnownPlayerPosition = GetPlayerPosition();
        currentChasePath = 0;

        // Convert last known position to grid coordinates
        Vector2 gridLastPos = World2GridCoordinates(lastKnownPlayerPosition.X, lastKnownPlayerPosition.Y, pathFindComp.m_gridkey);
        Vector2 gridCurrentPos = World2GridCoordinates(transformComp.m_position.X, transformComp.m_position.Y, pathFindComp.m_gridkey);

        // Generate path to last known position
        ChasePaths = GetPath(
            pathFindComp.m_gridkey,
            (int)gridCurrentPos.X, (int)gridCurrentPos.Y,
            (int)gridLastPos.X, (int)gridLastPos.Y
        );
        //Console.WriteLine(ChasePaths.Count);
        if (ChasePaths.Count > 0)
        {
            return;
        }
        else
        {
            if (enemyRoamBehaviour == EnemyRoamType.Patrolling)
            {
                SetCurrentState(new EnemyStatePatrol(this));
            }
            else if (enemyRoamBehaviour == EnemyRoamType.Static)
            {
                SetCurrentState(new EnemyStateReturnToHome(this));
            }
        }
    }

    public void RangedSearchUpdate()
    {
        if (isDead) return;

        // Update vision check and transition if player is found
        UpdateRayCastToPlayerPosition();
        if (CheckPlayerWithinSight() && IsPlayerInFOV())
        {
            SetCurrentState(new EnemyStateRangedAttack(this));
            return;
        }
            MoveToLastKnownPosition(lastKnownPlayerPosition);

        // Countdown search timer
        searchTimer -= InternalCall.m_InternalCallGetDeltaTime();
        if (searchTimer <= 0)
        {
            SetCurrentState(new EnemyStatePatrol(this));
        }
    }


    public void RangedAttackStart()
    {
        // Stop moving
        movement.X = 0;
        movement.Y = 0;
        InternalCall.m_InternalSetVelocity(EntityID, in movement);
        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        rb.m_Acceleration = movement;
        Component.Set<RigidBodyComponent>(EntityID, rb);
        // Initial delay before first shot
        fireTimer = fireRate * fireFirstDelay;
        rangedShuffleTimer = fireRate * 0.6f;

    }

    public void RangedAttackUpdate()
    {
        if (isDead) return;

        //// Check if player is still in sight periodically
        //targetCheckTimer -= InternalCall.m_InternalCallGetDeltaTime();
        //if (targetCheckTimer <= 0)
        //{
        FaceTarget(); //To facee the player

        //targetCheckTimer = targetCheckInterval;
        UpdateRayCastToPlayerPosition();

        if (!CheckPlayerWithinSight() || !IsPlayerInFOV())
        {
            // Lost sight of player, switch to search state
            SetCurrentState(new EnemyStateRangedSearch(this));
            return;
        }
        //}


        // Fire at player when timer expires
        float timescale = InternalCall.m_InternalCallGetTimeScale();
        fireTimer -= InternalCall.m_InternalCallGetDeltaTime() * timescale;
        rangedShuffleTimer -= InternalCall.m_InternalCallGetDeltaTime() * timescale;
        if (fireTimer <= 0)
        {
            fireTimer = fireRate;
            FireAtPlayer();
        }
        if (rangedShuffleTimer <= 0)
        {
            rangedShuffleTimer = rangedShuffleRate;
            if (rangedShuffleLeft)
            {
                ShuffleLeft();
                rangedShuffleLeft = !rangedShuffleLeft;
            }
            else
            {
                ShuffleRight();
                rangedShuffleLeft = !rangedShuffleLeft;

            }
        }
    }

    private void ShuffleLeft()
    {
        UpdateComponentValues();

        // Get direction to player
        Vector2 directionToPlayer = new Vector2(
            playerTransformComp.m_position.X - transformComp.m_position.X,
            playerTransformComp.m_position.Y - transformComp.m_position.Y
        );

        // Get perpendicular direction for left shuffle
        Vector2 leftShuffleDir = new Vector2(-directionToPlayer.Y, directionToPlayer.X);

        // Apply movement (multiply components separately)
        Vector2 newPosition = new Vector2(
            transformComp.m_position.X + (leftShuffleDir.X * shuffleDistance),
            transformComp.m_position.Y + (leftShuffleDir.Y * shuffleDistance)
        );

        movement.X = newPosition.X - transformComp.m_position.X;
        movement.Y = newPosition.Y - transformComp.m_position.Y;

        InternalCall.m_InternalSetVelocity(EntityID, in movement);
    }

    private void ShuffleRight()
    {
        UpdateComponentValues();

        // Get direction to player
        Vector2 directionToPlayer = new Vector2(
            playerTransformComp.m_position.X - transformComp.m_position.X,
            playerTransformComp.m_position.Y - transformComp.m_position.Y
        );

        // Get perpendicular direction for right shuffle
        Vector2 rightShuffleDir = new Vector2(directionToPlayer.Y, -directionToPlayer.X);

        // Apply movement (multiply components separately)
        Vector2 newPosition = new Vector2(
            transformComp.m_position.X + (rightShuffleDir.X * shuffleDistance),
            transformComp.m_position.Y + (rightShuffleDir.Y * shuffleDistance)
        );

        movement.X = newPosition.X - transformComp.m_position.X;
        movement.Y = newPosition.Y - transformComp.m_position.Y;

        InternalCall.m_InternalSetVelocity(EntityID, in movement);

    }


    public void EnemyScanStart()
    {
        // Stop moving
        movement.X = 0;
        movement.Y = 0;
        InternalCall.m_InternalSetVelocity(EntityID, in movement);
        RigidBodyComponent rb = Component.Get<RigidBodyComponent>(EntityID);
        rb.m_Acceleration = movement;
        Component.Set<RigidBodyComponent>(EntityID, rb);
        scanning = true;
        scanTime = 0f;
        initialRotation = transformComp.m_rotation - 180f;
    }

    public void EnemyScanUpdate()
    {
        if (!scanning) return;

        scanTime += InternalCall.m_InternalCallGetDeltaTime();

        if (scanTime >= scanDuration)
        {
            HandleLostPath();
            return;
        }

        UpdateRayCastToPlayerPosition();
        if (CheckPlayerWithinSight() && IsPlayerInFOV() && enemyType == EnemySelection.Ranged)
        {
            SetCurrentState(new EnemyStateRangedAttack(this));
            return;
        }
        else if(CheckPlayerWithinSight() && IsPlayerInFOV() && enemyType == EnemySelection.Melee)
        {
            SetCurrentState(new EnemyStateChase(this));
            return;
        }

        // Define left and right limits
        float leftRotation = initialRotation - enemyFOVangle / 2;
        float rightRotation = initialRotation + enemyFOVangle / 2;

        // Use time-based interpolation for smooth back-and-forth motion
        float t = (float)(Math.Sin(scanTime * Math.PI / scanDuration)); // Oscillates between -1 and 1

        // Map `t` from (-1,1) to (leftRotation, rightRotation)
        transformComp.m_rotation = leftRotation + (t + 1) / 2 * (rightRotation - leftRotation);
        SetComponent.SetTransformComponent(EntityID, transformComp);
    }

    private void HandleFailedPathfinding()
    {
        // Example fallback strategy: enemy roams near the last known position before giving up
        if (enemyRoamBehaviour == EnemyRoamType.Patrolling)
        {
            SetCurrentState(new EnemyStatePatrol(this));
        }
        else if (enemyRoamBehaviour == EnemyRoamType.Static)
        {
            SetCurrentState(new EnemyStateReturnToHome(this));
        }
    }

    private float Lerp(float a, float b, double t)
    {
        return (float)(a + (b - a) * t);
    }

}


