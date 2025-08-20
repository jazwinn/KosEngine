using System;

public class VideoScript : ScriptBase
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
        InternalCall.m_StartVideo(EntityID);
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_cutsceneStartup01.wav");
    }

    public override void Update()
    {

        if (InternalCall.m_HasVideoFinish(EntityID))
        {
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("MainMenu");
        }

    }
}