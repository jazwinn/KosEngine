using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;

public class BossDoor : ScriptBase
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!
    #endregion

    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH
    }
    public override void Start()
    {

    }

    public override void Update()
    {

        if (GameControllerLevel1.isBossDead == true)
        {
            InternalCall.m_InternalCallDeleteEntity(EntityID);

        }
    }

}