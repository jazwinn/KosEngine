using System;

public class KillCounter : ScriptBase
{
    #region ID Variables
    private uint EntityID;
    private uint playerID;
    #endregion

    public static int killCount = 0;
    private bool hasSpawned = false;
    private string exitPrefab = "Exit Spawn";
    public override void Awake(uint id) //Called everytime instance is created
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH

    }

    public override void Start() //Called once at the start of the game
    {
        ResetKillCount();

    }

    public override void Update()
    {
        if (LevelSelection.SceneName == "Level5")
        {
            //Console.WriteLine($"Kill Count: {killCount}");

            if (killCount >= 20 && hasSpawned == false)
            {
                //Console.WriteLine("spawn");
                TransformComponent spawnPoint = Component.Get<TransformComponent>(EntityID);

                InternalCall.m_InternalCallAddPrefab(exitPrefab, spawnPoint.m_position.X, spawnPoint.m_position.Y, 0.0f);
                hasSpawned = true;
            }
        }
    }

    public void IncrementKillCount()
    {
        killCount++;
        //Console.WriteLine($"Kill Count: {killCount}"); 
    }

    public int GetKillCount()
    {
        return killCount;
    }

    public void ResetKillCount()
    {
        killCount = 0;
        //Console.WriteLine("Kill Counter Reset"); // Help for debbugging if Ted need
    }

}
