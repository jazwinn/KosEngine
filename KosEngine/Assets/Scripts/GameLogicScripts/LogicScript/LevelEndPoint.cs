using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelEndPoint : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        isWinnable = false;
    }
    #endregion

    public static bool isWinnable;

    public override void Start()
    {

    }

    public override void Update()
    {
        if (isWinnable)
        {
            if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
            {
                int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

                foreach (int collidedEntitiesID in collidedEntities)
                {
                    if (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID) == "Player")
                    {
                        //Console.WriteLine("Closing Game");
                        InternalCall.m_InternalCallCloseWindow();
                    }
                }
            }
        }
        
    }
}