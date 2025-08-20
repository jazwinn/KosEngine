using System;

public class CutsceneMenuButtons : ScriptBase
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
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            InternalCall.m_InternalCallLoadScene(LevelSelection.SceneName);
        }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.SPACE) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.ENTER) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_A))
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
            switch (buttonFunctionNo)
            {
                case 1:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("TutorialCutscene02");
                    break;
                case 2:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("TutorialCutscene03");
                    break;
                case 3:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene01");
                    break;
                case 10:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene02");
                    break;
                case 11:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene03");
                    break;
                case 12:
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Cutscene04");
                    break;
                case 13:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level1");
                    break;
                case -1:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("LevelSelect");
                    break;
                case 21:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level2");
                    break;
                case 22:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level3");
                    break;
                case 23:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level4");
                    break;
                case 24:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level5");
                    break;
                case 25:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Level6");
                    break;
                case 100:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStop01");
                    InternalCall.m_InternalCallStopAllAudio();
                    InternalCall.m_UnloadAllScene();
                    InternalCall.m_InternalCallLoadScene("Credits");
                    break;
                default:
                    break;
            }
        }
    }
}