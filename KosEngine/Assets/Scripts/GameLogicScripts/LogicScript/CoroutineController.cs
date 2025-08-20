// Final CoroutineManagerFunctions.cs
using System;
using System.Collections;
using System.Collections.Generic;

public class CoroutineController : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public override void Start()
    {
    }

    public override void Update()
    {
        CoroutineManager.Instance.Update();

        //if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.NUM0))
        //{
        //    CoroutineManager.Instance.StopAllCoroutines();
        //    Console.WriteLine("Stopped all coroutines.");
        //}

        //// Pause/Resume coroutines for testing 
        //if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.NUM1))
        //{
        //    CoroutineManager.Instance.PauseAllCoroutines();
        //    Console.WriteLine("Paused all coroutines.");
        //}

        //if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.NUM2))
        //{
        //    CoroutineManager.Instance.ResumeAllCoroutines();
        //    Console.WriteLine("Resumed all coroutines.");
        //}
    }
    
}