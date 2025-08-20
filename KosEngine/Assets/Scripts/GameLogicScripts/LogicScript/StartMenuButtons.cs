using System;

public class StartMenuButtons : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public int buttonFunctionNo;

    private bool isHovering;

    private AnimationComponent animComp;
    public static bool isReading;

    public override void Start()
    {
        animComp = Component.Get<AnimationComponent>(EntityID);
    }

    public override void Update()
    {
        CheckForCollisions();
        CheckForHover();
        CheckForClicks();

    }

    private void CheckForCollisions()
    {
        if (InternalCall.m_InternalIsButtonHovered(EntityID))
        {
            if (isHovering)
            {
                return;
            }
            isHovering = true;
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonHover01");
        }
        else
        {
            isHovering = false;
        }
    }

    private void CheckForHover()
    {
        if (isHovering)
        {
            animComp.m_frameNumber = 1;
            Component.Set<AnimationComponent>(EntityID, animComp);
        }

        else
        {
            animComp.m_frameNumber = 0;
            Component.Set<AnimationComponent>(EntityID, animComp);
        }
    }

    private void CheckForClicks()
    {
        if ((InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_A) ) && isHovering == true)
        {
            
            switch (buttonFunctionNo)
            {
                case 10:
                    if (isReading)
                    {
                        isReading = false;
                        InternalCall.m_DisableLayer(9);
                    }
                    else
                    {
                        isReading = true;
                        InternalCall.m_EnableLayer(9);
                    }
                    
                    break;
                case 11:
                    isReading = false;
                    InternalCall.m_DisableLayer(9);
                    break;
                case 12:
                    CoroutineManager.Instance.StopAllCoroutines();
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceQuit01");
                    StartMenuButtons.isReading = false;
                    InternalCall.m_DisableLayer(9);
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Credits");
                    break;
                case -1:
                    break;
                default:
                    break;
            }
        }

        if(InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_B))
        {
            switch (buttonFunctionNo)
            {
                case 10:
                case 11:
                    isReading = false;
                    InternalCall.m_DisableLayer(9);
                    break;
                case -1:
                    break;
                default:
                    break;
            }
        }
    }
}