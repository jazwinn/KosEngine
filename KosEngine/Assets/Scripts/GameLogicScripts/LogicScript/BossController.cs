using System;
using System.Collections;
using System.Collections.Generic;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Text.RegularExpressions;

public class BossController : ScriptBase
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!
    private uint forceFieldID;
    #endregion

    #region Component Variable
    private SpriteComponent spriteComp;
    //private ColliderComponent collComp;

    private AnimationComponent animComp;
    //private bool isAnimating = false;

    private string respawnForcefieldTexture;
    private string despawnForcefieldTexture;
    private string bossDamageTexture;
    private string bossDestroyedTexture;

    private bool isDying = false;
    private bool isTakingDamage = false;
    private bool isForceFieldSpawning = false;
    private bool isForceFieldDeactivating = false;

    private bool isAttacking = false;
    private string bossDeathSound = "aud_bossDeath01.wav";
    private string shieldRegenSound = "aud_bossShieldRegen01.wav";
    private string shieldBreakSound = "aud_bossShieldBreak01.wav";
    private string shieldDamageSound01 = "aud_railgunShot01.wav";
    private string shieldDamageSound02 = "aud_katana01.wav";
    private string bossDamageSound01 = "aud_playerDeath01.wav";
    private string bossDamageSound02 = "aud_bossHit01.wav";
    private string bossShootingSound = "aud_bossBullet01.wav";

    #endregion

    #region Boss Variable
    private BossAttackPattern currentPattern;
    private long seed = DateTime.Now.Ticks; // seed for randomizer

    private Vector2 bossPosition;
    //private bool canAttack = true;
    private float attackCooldown = 3.0f;
    private BossAttackPattern lastPattern;

    private int repeatCount = 0;
    public int forceFieldHealth = 3;
    public int bossHealth = 24;
    public bool isForceFieldActive = true;
    private bool forceFieldStart = false;
    private string forceFieldPrefab = "Boss_Forcefield";

    private string bossBulletPrefab;
    private string bossClusterBulletPrefab;

    private bool isInvincible = false;
    private float invicibilityTimer = 0.2f;

    private int bossVulnerableHealth = 3;

    private static int challengeCounter;
    #endregion

    #region Enemy Spawning
    private uint[] enemySpawnPoints; // Store spawn point entity IDs
    private string[] enemyPrefabs = { "prefab_Enemy_Alert" };

    //private string[] enemyPrefabs = { "FearlessEnemy" };
    private List<uint> activeEnemies = new List<uint>();

    //private float enemySpawnCooldown = 10.0f;
    private int maxEnemies = 4;
    #endregion


    #region Boss Attack Pattern
    public enum BossAttackPattern // Attack pattern
    {
        AlternatingBullet,
        BulletSpread,
        BulletDisperse,
        BulletSpiral
    };
    #endregion

    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH
        bossBulletPrefab = "prefab_bossBullet";
        bossClusterBulletPrefab = "prefab_bossClusterBullet";

        //SpawnForceField();

        int[] childIDs = InternalCall.m_InternalCallGetChildrenID(EntityID);
        enemySpawnPoints = Array.ConvertAll(childIDs, item => (uint)item);
        animComp = Component.Get<AnimationComponent>(EntityID);

        //forcefield animation Texture
        respawnForcefieldTexture = "ani_bossShield_strip6.png";
        despawnForcefieldTexture = "ani_bossShieldDeactivate_strip7.png";

        //boss animation texture
        bossDamageTexture = "ani_bossHit_strip4.png";
        bossDestroyedTexture = "ani_bossDeath_strip15.png";

        challengeCounter = 0;
    }
    public override void Start()
    {
        
    }

    public override void Update()
    {
        if (GameControllerLevel1.isBossDead || !GameControllerLevel1.isActivated || PlayerController.isDead) return;

        if (isDying)
        {
            animComp = Component.Get<AnimationComponent>(EntityID);

            if (animComp.m_frameNumber >= animComp.m_stripCount - 1)
            {

                animComp.m_isAnimating = false;

                animComp.m_frameNumber = 14;
                Component.Set<AnimationComponent>(EntityID, animComp);

                isDying = false;
                
                if (LevelSelection.SceneName == "LevelChallenge")
                {
                    challengeCounter++;
                    if (challengeCounter == 3)
                    {
                        InternalCall.m_InternalCallStopAudio((uint)InternalCall.m_InternalCallGetTagID("GameController"), "aud_bossLevelLoop.wav");
                        GameControllerLevel1.isBossDead = true;
                    }
                }

                else
                {
                    GameControllerLevel1.isBossDead = true;
                    InternalCall.m_InternalCallStopAudio((uint)InternalCall.m_InternalCallGetTagID("GameController"), "aud_bossLevelLoop.wav");
                }

                return;
            }

            return;
        }

        if (isTakingDamage)
        {
            animComp = Component.Get<AnimationComponent>(EntityID);

            if (animComp.m_frameNumber >= animComp.m_stripCount - 1) // Check for last frame
            {
                animComp.m_isAnimating = false;
                animComp.m_framesPerSecond = 11;
                animComp.m_frameNumber = 0;
                Component.Set<AnimationComponent>(EntityID, animComp);
                isTakingDamage = false;
            }
        }

        


        if (isForceFieldActive)
        {
            AnimationComponent forceFieldAnim = Component.Get<AnimationComponent>(forceFieldID);

            switch (forceFieldHealth)
            {
                case 3:
                    forceFieldAnim.m_isAnimating = false;
                    forceFieldAnim.m_frameNumber = 3;
                    Component.Set<AnimationComponent>(forceFieldID, forceFieldAnim);
                    break;
                case 2:
                    forceFieldAnim.m_isAnimating = false;
                    forceFieldAnim.m_frameNumber = 4;
                    Component.Set<AnimationComponent>(forceFieldID, forceFieldAnim);
                    break;
                case 1:
                    forceFieldAnim.m_isAnimating = false;
                    forceFieldAnim.m_frameNumber = 5;
                    Component.Set<AnimationComponent>(forceFieldID, forceFieldAnim);
                    break;


                default:
                    break;
            }
            
        }

        if (isForceFieldSpawning)
        {
            AnimationComponent forceFieldAnim = Component.Get<AnimationComponent>(forceFieldID);

            if (forceFieldAnim.m_frameNumber >= forceFieldAnim.m_stripCount - 1)
            {
                forceFieldAnim.m_isAnimating = false;
                forceFieldAnim.m_frameNumber = 3;
                Component.Set<AnimationComponent>(forceFieldID, forceFieldAnim);
                isForceFieldSpawning = false;
            }
        }

        if (isForceFieldDeactivating)
        {
            AnimationComponent forceFieldAnim = Component.Get<AnimationComponent>(forceFieldID);

            if (forceFieldAnim.m_frameNumber >= forceFieldAnim.m_stripCount - 1)
            {
                InternalCall.m_InternalCallDeleteEntity(forceFieldID);
                isForceFieldDeactivating = false;
            }
        }
        CheckCollision();

        if (GameControllerLevel1.isActivated == true && isAttacking == false)
        {
            isAttacking = true;
            CoroutineManager.Instance.StartCoroutine(AttackLoop()); // Start attacks
            CoroutineManager.Instance.StartCoroutine(EnemySpawnLoop()); // Start enemy spawns

        }

    }

    #region Animation

    private IEnumerator StartBossDamageAnimation()
    {
        isTakingDamage = true;

        spriteComp = GetComponent.GetSpriteComponent(EntityID);
        spriteComp.m_imageFile = bossDamageTexture;
        Component.Set<SpriteComponent>(EntityID, spriteComp);

        animComp = Component.Get<AnimationComponent>(EntityID);

        animComp.m_frameNumber = 0;
        animComp.m_framesPerSecond = 7;
        animComp.m_isAnimating = true;

        Component.Set<AnimationComponent>(EntityID, animComp);

        yield return null;

    }

    private void StartBossDestroyedAnimation()
    {

        LightComponent lightComp = Component.Get<LightComponent>(EntityID);
        lightComp.m_intensity = 0;
        Component.Set<LightComponent>(EntityID, lightComp);

        ColliderComponent tempColComp = Component.Get<ColliderComponent>(EntityID);

        tempColComp.m_collisionCheck = false;
        tempColComp.m_collisionResponse = false;

        Component.Set<ColliderComponent>(EntityID, tempColComp);

        spriteComp = GetComponent.GetSpriteComponent(EntityID);
        spriteComp.m_imageFile = bossDestroyedTexture;
        Component.Set<SpriteComponent>(EntityID, spriteComp);

        animComp = Component.Get<AnimationComponent>(EntityID);
        animComp.m_frameNumber = 0;
        animComp.m_isAnimating = true;
        animComp.m_stripCount = 15; // Set frame strip count to 15
        animComp.m_frameTimer = 0;

        Component.Set<AnimationComponent>(EntityID, animComp);

        isDying = true;  // Track that death animation is playing
    }
    #endregion Animation


    private int GenerateRandom(int min, int max)
    {
        seed = (seed * 1664525 + 1013904223) & 0x7FFFFFFF; // Update seed
        return (int)(min + (seed % (max - min))); // Map to range
    }

    #region AttackPattern Execution
    public void AttackRandomizer()
    {
        BossAttackPattern newPattern;

        do
        {
            //Console.WriteLine("Randomizing");
            int randomIndex = GenerateRandom(0, Enum.GetValues(typeof(BossAttackPattern)).Length);
            newPattern = (BossAttackPattern)randomIndex;

        }
        while (repeatCount >= 1 && newPattern == lastPattern);

        if (newPattern == lastPattern)
        {
            repeatCount++;
        }
        else
        {
            repeatCount = 0;
            lastPattern = newPattern;
        }

        currentPattern = newPattern;
        ExecutePattern(currentPattern);

        //Console.WriteLine($"Selected Pattern: {currentPattern}");
    }

    private void ExecutePattern(BossAttackPattern pattern)
    {
        InternalCall.m_InternalGetTranslate(EntityID, out bossPosition);
        switch (pattern)
        {
            case BossAttackPattern.AlternatingBullet:
                //Console.WriteLine("Firing Alternating Bullets!");
                CoroutineManager.Instance.StartCoroutine(FireAlternatingWaves());

                break;
            case BossAttackPattern.BulletSpread:
                //Console.WriteLine("Firing Bullet Spread!");
                SpawnBullets(bossPosition, 8, 0.2f, 180f, 360f);

                break;
            case BossAttackPattern.BulletDisperse:
                //Console.WriteLine("Firing Bullet Disperse!");
                SpawnBulletDisperse(bossPosition);

                break;
            case BossAttackPattern.BulletSpiral:
                //Console.WriteLine("Firing Bullet Spiral!");
                CoroutineManager.Instance.StartCoroutine(FireSweepingCurvingBulletVolley(bossPosition, 10, 0.2f, 0.1f));

                break;
        }
    }

    private IEnumerator AttackLoop()
    {
        while (bossHealth > 0)
        {
            AttackRandomizer();
            attackCooldown = GenerateRandom(200, 350) / 100f; 
            yield return new CoroutineManager.WaitForSeconds(attackCooldown);
        }
    }
    #endregion

    #region Enemy Spawner
    private IEnumerator EnemySpawnLoop()
    {
        while (bossHealth > 0)
        {
            ////Only stop spawning if enemie count reaches max enemies count
            //if (activeEnemies.Count < maxEnemies)
            //{
            //    SpawnEnemy();
            //}
            SpawnEnemy();

            float randomDelay = GenerateRandom(3, 5);
            yield return new CoroutineManager.WaitForSeconds(randomDelay);
        }
    }

    private void SpawnEnemy()
    {
        if (enemySpawnPoints.Length == 0) return; // No spawn points found

        // Select a random spawn point
        uint spawnPointID = enemySpawnPoints[GenerateRandom(0, enemySpawnPoints.Length)];

        Vector2 spawnPosition;
        InternalCall.m_InternalGetTranslate(spawnPointID, out spawnPosition);

        // Select a random enemy type
        string enemyType = enemyPrefabs[GenerateRandom(0, enemyPrefabs.Length)];

        // Spawn the enemy
        uint enemyID = (uint)InternalCall.m_InternalCallAddPrefab(enemyType, spawnPosition.X, spawnPosition.Y, 0);

        //Console.WriteLine($"[Boss] Spawned {enemyType} at {spawnPosition.X}, {spawnPosition.Y}");
        activeEnemies.Add(enemyID);
    }
    #endregion

    #region Boss Attack Pattern
    private IEnumerator FireAlternatingWaves()
    {
        int waveCount = 3; // Number of waves 
        float waveDelay = 0.3f; // Time between waves
        float positionOffset = 10f; // Angle offset to shift bullets 

        for (int i = 0; i < waveCount; i++)
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);

            float waveAngleOffset = (i % 2 == 0) ? positionOffset : -positionOffset;
            SpawnBullets(bossPosition, 8, 0.2f, 180f + waveAngleOffset, 360f + waveAngleOffset);
            yield return new CoroutineManager.WaitForSeconds(waveDelay);
        }
    }

    private void SpawnBullets(Vector2 position, int bulletCount, float radius, float startAngle, float endAngle)
    {
        InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);

        float angleStep = (endAngle - startAngle) / (bulletCount - 1);

        for (int i = 0; i < bulletCount; i++)
        {
            float angle = startAngle + (angleStep * i);
            float radian = angle * (float)(Math.PI / 180f);
            float spawnX = position.X + (float)Math.Cos(radian) * radius;
            float spawnY = position.Y + (float)Math.Sin(radian) * radius;

            InternalCall.m_InternalCallAddPrefab(bossBulletPrefab, spawnX, spawnY, angle);
        }
    }

    private void SpawnBulletDisperse(Vector2 position)
    {
        float bigBulletAngle = GenerateRandom(240, 300); // 270 ± 30

        InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);

        uint largeBullet = (uint)InternalCall.m_InternalCallAddPrefab(bossClusterBulletPrefab, position.X, position.Y, bigBulletAngle);

        CoroutineManager.Instance.StartCoroutine(ExplodeBigBullet(largeBullet));
    }

    private IEnumerator ExplodeBigBullet(uint largeBullet)
    {
        yield return new CoroutineManager.WaitForSeconds(1.5f);

        InternalCall.m_InternalGetTranslate(largeBullet, out Vector2 explosionPosition);

        List<uint> mediumBullets = new List<uint>();

        // First explosion (i = number of bullet)
        for (int i = 0; i < 6; i++)
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);


            float angle = i * (360f / 6); // Spread evenly
            uint mediumBullet = (uint)InternalCall.m_InternalCallAddPrefab(bossClusterBulletPrefab, explosionPosition.X, explosionPosition.Y, angle);
            mediumBullets.Add(mediumBullet);
        }



        InternalCall.m_InternalCallDeleteEntity(largeBullet);

        // Delay 
        yield return new CoroutineManager.WaitForSeconds(1f);

        // Second explosion
        foreach (uint mediumBullet in mediumBullets)
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);

            InternalCall.m_InternalGetTranslate(mediumBullet, out Vector2 mediumBulletPosition);

            for (int j = 0; j < 8; j++)
            {
                float spreadAngle = (j * 60f) - 30f;
                float radian = (float)(spreadAngle * (Math.PI / 180f));
                float offsetX = (float)(mediumBulletPosition.X + Math.Cos(radian) * 0.3f);
                float offsetY = (float)(mediumBulletPosition.Y + Math.Sin(radian) * 0.3f);

                //InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);

                InternalCall.m_InternalCallAddPrefab(bossBulletPrefab, offsetX, offsetY, spreadAngle);
            }

            InternalCall.m_InternalCallDeleteEntity(mediumBullet);
        }

    }

    private IEnumerator FireSweepingCurvingBulletVolley(Vector2 position, int bulletCount, float radius, float bulletInterval)
    {

        for (int direction = 0; direction < 2; direction++) // 0 = Left to Right, 1 = Right to Left
        {
            bool reverse = (direction == 1);

            for (int i = 0; i < bulletCount; i++)
            {
                int index = reverse ? (bulletCount - 1 - i) : i;
                float baseAngle = 180f + ((180f / (bulletCount - 1)) * index);
                float radian = (float)(baseAngle * (Math.PI / 180f));

                float spawnX = position.X + (float)Math.Cos(radian) * radius;
                float spawnY = position.Y + (float)Math.Sin(radian) * radius;

                uint bullet = (uint)InternalCall.m_InternalCallAddPrefab(bossBulletPrefab, spawnX, spawnY, baseAngle);
                InternalCall.m_InternalCallPlayAudio(EntityID, bossShootingSound);

                yield return new CoroutineManager.WaitForSeconds(bulletInterval);
            }
            yield return new CoroutineManager.WaitForSeconds(0.2f);
        }
    }


    #endregion

    #region Boss Forcefield
    public void SpawnForceField()
    {
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 bossPosition);
        forceFieldID = (uint)InternalCall.m_InternalCallAddPrefab(forceFieldPrefab, bossPosition.X, bossPosition.Y, 0);
        AnimationComponent forceFieldAnim = Component.Get<AnimationComponent>(forceFieldID);
        forceFieldAnim.m_stripCount = 6; // Set frame strip count to 7
        Component.Set<AnimationComponent>(forceFieldID, forceFieldAnim);

        isForceFieldActive = true;
        forceFieldHealth = 3;
    }


    private void StartForceFieldDeactivateAnimation()
    {
        if (forceFieldID == 0) return;

        spriteComp = GetComponent.GetSpriteComponent(forceFieldID);
        InternalCall.m_InternalSetSpriteComponent(forceFieldID, despawnForcefieldTexture, spriteComp.m_layer, spriteComp.m_color, spriteComp.m_alpha);

        AnimationComponent forceFieldAnim = Component.Get<AnimationComponent>(forceFieldID);
        forceFieldAnim.m_frameNumber = 0;
        forceFieldAnim.m_isAnimating = true;
        forceFieldAnim.m_stripCount = 7; // Set frame strip count to 7
        Component.Set<AnimationComponent>(forceFieldID, forceFieldAnim);

        isForceFieldDeactivating = true;
    }

    private IEnumerator InvincibilityTimer()
    {
        isInvincible = true;
        yield return new CoroutineManager.WaitForSeconds(invicibilityTimer);
        isInvincible = false;
    }
    private void CheckCollision()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntityID in collidedEntities)
            {
                uint collidedEntity = (uint)collidedEntityID;
                string entityTag = InternalCall.m_InternalCallGetTag(collidedEntity);

                // Ignore damage if forcefield is deactivating or boss is dying
                if (isForceFieldDeactivating || isDying || isInvincible)
                    continue;

                if (!forceFieldStart)
                {
                    // First time boss is hit, trigger force field
                    forceFieldStart = true;
                    SpawnForceField();
                    return; // Skip damage for this hit
                }

                // Apply damage to forcefield or boss
                if (isForceFieldActive)
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, shieldDamageSound01);
                    InternalCall.m_InternalCallPlayAudio(EntityID, shieldDamageSound02);

                    if (entityTag == "MeleeKillZoneSpawn")
                    {
                        forceFieldHealth -= 2;
                    }
                    else
                    {
                        forceFieldHealth -= 1;
                    }
                    
                    //Console.WriteLine($"[Boss] Force Field hit! Remaining Health: {forceFieldHealth}");

                    if (forceFieldHealth <= 0)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, shieldBreakSound);

                        //Console.WriteLine("[Boss] Force Field Destroyed!");
                        isForceFieldActive = false;
                        StartForceFieldDeactivateAnimation();
                    }
                    else
                    {
                        if (entityTag != "MeleeKillZoneSpawn")
                        {
                            CoroutineManager.Instance.StartCoroutine(InvincibilityTimer());
                        }
                    }
                }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, bossDamageSound01);
                    InternalCall.m_InternalCallPlayAudio(EntityID, bossDamageSound02);

                    bossHealth--;
                    //Console.WriteLine($"[Boss] Hit! Remaining Health: {bossHealth}");

                    if (bossHealth <= 0)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, bossDeathSound);
                        StartBossDestroyedAnimation();
                    }
                    else
                    {
                        bossVulnerableHealth--;

                        CoroutineManager.Instance.StartCoroutine(StartBossDamageAnimation(), "BossAnimation");

                        if (bossVulnerableHealth == 0)
                        {
                            InternalCall.m_InternalCallPlayAudio(EntityID, shieldRegenSound);
                            SpawnForceField();
                            bossVulnerableHealth = 3;
                        }

                        if (entityTag != "MeleeKillZoneSpawn")
                        {
                            CoroutineManager.Instance.StartCoroutine(InvincibilityTimer());
                        }
                    }
                }
            }

        }
    }
    #endregion

}