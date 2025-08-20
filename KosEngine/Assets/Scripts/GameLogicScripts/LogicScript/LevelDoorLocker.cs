using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class LevelDoorLocker : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        doorLocked = true;
    }
    #endregion

    public static bool doorLocked;

    public override void Start()
    {

    }

    public override void Update()
    {
       if (doorLocked == false)
       {
            InternalCall.m_InternalCallDeleteEntity(EntityID);
       }
    }


}