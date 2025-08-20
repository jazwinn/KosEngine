using System;

public class EnemySpawner : ScriptBase
{
    #region Entity ID

    
    private uint EntityID;

    private float timeSinceLastSpawn;
    public float minSpawnInterval;
    public float maxSpawnInterval;
    public int spawnlimit;
    private bool stopspawn;

    float spawntime;
    EnemyType enemyType;

    private int spawnedEnemies;

    private enum EnemyType
    {
        ENEMY1,
        RANGEENEMY,

        TOTALENEMY
    }

    public override void Awake(uint id)
    {
        EntityID = id;

        spawnedEnemies = 0;
        stopspawn = false;

        timeSinceLastSpawn = 0f;
    }
    #endregion

    private void getRandomSpawnTime()
    {
        spawntime = InternalCall.m_GetRandomFloat(minSpawnInterval, maxSpawnInterval); 
    }

    private void getRandomEnemyType()
    {
        enemyType = (EnemyType)InternalCall.m_GetRandomInt((int)EnemyType.ENEMY1, (int)EnemyType.TOTALENEMY - 1);
    }

    private Vector2 getRandomPositionFromColider()
    {  
        Vector2 returnvec = new Vector2 (0, 0);

        Vector2 trans, scale;
        float rotate;
        InternalCall.m_GetColliderDecomposedTRS(EntityID, out trans, out scale, out rotate);
        float xmin = trans.X - scale.X / 2;
        float xmax = trans.X + scale.X / 2;

        returnvec.X = InternalCall.m_GetRandomFloat(xmin, xmax);

        float Ymin = trans.Y - scale.Y / 2;
        float Ymax = trans.Y + scale.Y / 2;
        returnvec.Y = InternalCall.m_GetRandomFloat(Ymin, Ymax);

        return returnvec;
    }

    public override void Start()
    {
        getRandomSpawnTime();
        getRandomEnemyType();
    }

    public override void Update()
    {
        if (GameControllerLevel1.gameIsPaused || stopspawn || PlayerController.isDead) return;

        //update time
        timeSinceLastSpawn += InternalCall.m_GetUnfixedDeltaTime();

        if (timeSinceLastSpawn > spawntime)
        {
            Vector2 spawnpos = getRandomPositionFromColider();
            int spawnrot = InternalCall.m_GetRandomInt(1, 360);

            switch ((EnemyType)enemyType)
            {
                case EnemyType.ENEMY1:

                    InternalCall.m_InternalCallAddPrefab("prefab_Enemy_Alert", spawnpos.X, spawnpos.Y, spawnrot);

                    break;
                case EnemyType.RANGEENEMY:
                    InternalCall.m_InternalCallAddPrefab("prefab_Enemy_RangedAlert", spawnpos.X, spawnpos.Y, spawnrot);
                    break;
                default:


                    break;
            }
            getRandomSpawnTime();
            getRandomEnemyType();
            spawnedEnemies++;
            timeSinceLastSpawn = 0f;
        }

        //check if spawn limit is reached
        if(spawnlimit > 0)
        {
            if(spawnedEnemies >= spawnlimit)
            {
                stopspawn = true;
            }

        }

    }
}