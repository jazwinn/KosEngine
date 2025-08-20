using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelTransition : ScriptBase
{
    private uint EntityID;
    private uint playerID; //Store player ID
    public string NextLevelName;

    public override void Awake(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player"); //Get Player ID
}

public override void Update()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "Player":
                        if (NextLevelName != null)
                        {
                            InternalCall.m_InternalCallStopAllAudio();

                            InternalCall.m_UnloadAllScene();
                            InternalCall.m_InternalCallLoadScene(NextLevelName);
                        }
                        break;

                    default:
                        break;
                }
            }
        }
    }
}
