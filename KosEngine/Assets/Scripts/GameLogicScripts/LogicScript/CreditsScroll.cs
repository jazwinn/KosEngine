using System;
using System.Collections;
using System.Runtime.InteropServices.ComTypes;

public class CreditsScroll : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion
    public float endPointY;
    public int speed;
    RigidBodyComponent rbComp;
    TransformComponent transformComp;
    

    public override void Start()
    {
        endPointY = -222.5f;
        rbComp = Component.Get<RigidBodyComponent>(EntityID);
        rbComp.m_Velocity.Y = speed;
        Component.Set<RigidBodyComponent>(EntityID, rbComp);
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_mainMenuLoop");

    }

    public override void Update()
    {
        rbComp = Component.Get<RigidBodyComponent>(EntityID);
        Vector2 currentTranslate;
        InternalCall.m_InternalGetTranslate(EntityID, out currentTranslate);

        if (currentTranslate.Y > endPointY)
        {
            rbComp.m_Velocity.Y = -1;
            Component.Set<RigidBodyComponent>(EntityID, rbComp);

            if (InternalCall.m_InternalCallIsKeyPressed(keyCode.LMB))
            {
                rbComp = Component.Get<RigidBodyComponent>(EntityID);
                rbComp.m_Velocity.Y = -5;
                Component.Set<RigidBodyComponent>(EntityID, rbComp);
            }

        }

        else
        {
            transformComp = Component.Get<TransformComponent>(EntityID);
            transformComp.m_position = new Vector2(13, endPointY);
            rbComp.m_Velocity = new Vector2(0, 0);

            Component.Set<TransformComponent>(EntityID, transformComp);
            Component.Set<RigidBodyComponent>(EntityID, rbComp);

            CoroutineManager.Instance.StartCoroutine(QuitAfterDelay(), "QuitAfterDelay");

        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            CoroutineManager.Instance.StartCoroutine(Quit(), "Quit");
        }

    }
    private IEnumerator Quit()
    {
        InternalCall.m_DisableLayer(8); //Disables Loadout Menu UI
        InternalCall.m_DisableLayer(7); //Disables Pause Menu UI
        InternalCall.m_DisableLayer(6); //Disable How To Play UI
        InternalCall.m_DisableLayer(9); //Disable Death Screen UI
        InternalCall.m_DisableLayer(15); //Disable FPS counter UI

        InternalCall.m_InternalCallStopAllAudio();
        CoroutineManager.Instance.StopAllCoroutines();
        InternalCall.m_InternalCallSetTimeScale(1);
        InternalCall.m_UnloadAllScene();
        InternalCall.m_InternalCallLoadScene("MainMenu");

        yield return null;
    }

    private IEnumerator QuitAfterDelay()
    {
        yield return new CoroutineManager.WaitForSeconds(5f);

        InternalCall.m_DisableLayer(8); //Disables Loadout Menu UI
        InternalCall.m_DisableLayer(7); //Disables Pause Menu UI
        InternalCall.m_DisableLayer(6); //Disable How To Play UI
        InternalCall.m_DisableLayer(9); //Disable Death Screen UI
        InternalCall.m_DisableLayer(15); //Disable FPS counter UI

        InternalCall.m_InternalCallStopAllAudio();
        CoroutineManager.Instance.StopAllCoroutines();
        InternalCall.m_InternalCallSetTimeScale(1);
        InternalCall.m_UnloadAllScene();
        InternalCall.m_InternalCallLoadScene("MainMenu");

        yield return null;
    }
}