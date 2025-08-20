using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class PlayerMeleeKillZone : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        timer = 0;
    }
    #endregion

    private float timer;

    public override void Start()
    {
        
    }

    public override void Update()
    {
        timer += InternalCall.m_InternalCallGetDeltaTime();

        if (timer > 0.1f || InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            InternalCall.m_InternalCallDeleteEntity(EntityID);
        }
    }
}