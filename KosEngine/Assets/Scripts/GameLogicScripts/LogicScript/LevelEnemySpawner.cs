using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class LevelEnemySpawner : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        spawnDelay = 2;
        enemyPrefab = "prefab_enemy";
        startSpawning = false;
        isSpawning = false;
        totalEnemiesToKill = 15;
        maxEnemiesSpawned = 15;
    }
    #endregion

    private int totalEnemiesToKill;
    private int totalEnemiesSpawned;
    private int maxEnemiesSpawned;

    private bool isSpawning;

    private bool startSpawning;

    private string enemyPrefab;

    private float spawnDelay;

    public override void Start()
    {

    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                if (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID) == "Player")
                {
                    //var collisionComponent = GetComponent.GetColliderComponent(EntityID);
                    var collisionComponent = Component.Get<ColliderComponent>(EntityID);
                    collisionComponent.m_collisionCheck = !collisionComponent.m_collisionCheck;
                    //SetComponent.SetCollisionComponent(EntityID, collisionComponent);
                    Component.Set<ColliderComponent>(EntityID, collisionComponent);
                    startSpawning = true;

                    foreach (int id in InternalCall.m_InternalCallGetTagIDs("Door"))
                    {
                        Vector2 tempPos;
                        Vector2 tempScale;
                        float tempRotate;

                        InternalCall.m_InternalGetTransformComponent((uint)id, out tempPos, out tempScale, out tempRotate);

                        InternalCall.m_InternalCallAddPrefab("prefab_wallDoor", tempPos.X, tempPos.Y, 0f);
                    }

                    foreach (int id in InternalCall.m_InternalCallGetTagIDs("DoorSpawnPos"))
                    {
                        Vector2 tempPos;
                        Vector2 tempScale;
                        float tempRotate;

                        InternalCall.m_InternalGetTransformComponent((uint)id, out tempPos, out tempScale, out tempRotate);

                        InternalCall.m_InternalCallAddPrefab("prefab_door", tempPos.X, tempPos.Y, tempRotate);
                    }


                }
            }
        }

        if (EnemyController.totalEnemiesKilled >= totalEnemiesToKill)
        {
            LevelEndPoint.isWinnable = true;

            if (LevelDoorLocker.doorLocked == true)
            {
                foreach (int id in InternalCall.m_InternalCallGetTagIDs("Door"))
                {
                    InternalCall.m_InternalCallDeleteEntity((uint)id);
                }

                LevelDoorLocker.doorLocked = false;
            }
            
        }

        else if (totalEnemiesSpawned >= maxEnemiesSpawned)
        {
            startSpawning = false;
        }

        else
        {
            TrySpawningEnemies();
        }
    }

    private void TrySpawningEnemies()
    {

        if (!isSpawning && !EnemyController.playerIsDead && startSpawning && !GameController.gameIsPaused)
        {

            isSpawning = true;

            int[] spawnPoints = InternalCall.m_InternalCallGetTagIDs("EnemySpawnPoint");

            foreach (int spawnPointID in spawnPoints)
            {

                Vector2 spawnPointTransform;
                InternalCall.m_InternalGetTranslate((uint)spawnPointID, out spawnPointTransform);

                Vector2 playerPos;
                InternalCall.m_InternalGetTranslate((uint)InternalCall.m_InternalCallGetTagID("Player"), out playerPos);

                Vector2 direction;

                direction.X = (spawnPointTransform.X - playerPos.X);
                direction.Y = (spawnPointTransform.Y - playerPos.Y);

                float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI));

                InternalCall.m_InternalCallPlayAudio((uint)spawnPointID, "aud_enemySpawn01");

                InternalCall.m_InternalCallAddPrefab(enemyPrefab, spawnPointTransform.X, spawnPointTransform.Y, rotationFloat);
                totalEnemiesSpawned++;
                // Delay before moving to the next spawn point


            }

            _ = ToggleSpawningAfterDelay(spawnDelay);

        }
    }

    private async Task ToggleSpawningAfterDelay(float delaySeconds)
    {
        await Task.Delay(TimeSpan.FromSeconds(delaySeconds));
        if (!GameController.isSceneActive || EnemyController.playerIsDead || GameController.gameIsPaused)
        {
            return;
        }
        isSpawning = false;
    }

}