using System;
using System.Collections;

public class CutsceneManager : ScriptBase
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

        CoroutineManager.Instance.StartCoroutine(Wait(), "Wait");
         
    }

    private IEnumerator Wait()
    {
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStart01");
        yield return new CoroutineManager.WaitForSeconds(0.3f);
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceLoop01");
    }

    public override void Update()
    {
    }
}