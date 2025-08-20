using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;

public class Keycard : ScriptBase
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!
    private string door = "keyDoor";
    public bool keyDoorCollected = false;

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

        #region Collision Handling
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "Player": // If it hits the player
                        if (keyDoorCollected == false)
                        {
                            InternalCall.m_InternalCallDeleteEntity(EntityID);

                        }
                        break;

                    default:
                        break;
                }
            }
        }
        #endregion
    }

}