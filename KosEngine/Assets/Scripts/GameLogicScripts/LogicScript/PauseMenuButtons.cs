using System;

public class PauseMenuButtons : ScriptBase
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
    
    public static bool confirmQuit;

    private TextComponent quitConfirmTextComp;
    public override void Start()
    {
        animComp = Component.Get<AnimationComponent>(EntityID);

        quitConfirmTextComp = Component.Get<TextComponent>((uint)InternalCall.m_InternalCallGetTagID("UIConfirmQuit"));
        
        confirmQuit = false;
    }

    public override void Update()
    {
        if (true)
        {
            CheckForCollisions();
            CheckForHover();
            CheckForClicks();

            if (confirmQuit == true && (InternalCall.m_InternalCallIsKeyTriggered(keyCode.Y) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_Y)))
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                QuitToMainMenu();
            }
            if (confirmQuit == true && (InternalCall.m_InternalCallIsKeyTriggered(keyCode.N) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_B)))
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                confirmQuit = false;
            }
        }
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
        if ((InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_A)) && isHovering == true)
        {
            switch (buttonFunctionNo)
            {
                case 10:
                    //Resume
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    ResumeGame();
                    break;
                case 20:
                    //Restart
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    RestartGame();
                    break;
                case 30:
                    //HowToplay
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    HowToPlay();
                    break;
                case 31:
                    //HowToPlayBack
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    HowToPlayBack();
                    break;
                case 32:
                    //enter sound menu
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PauseToSoundMenu();
                    break;
                case 35:
                    //leave sound menu
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    SoundToPauseMenu();
                    break;
                case 80:
                    //increase/decrease SFX, BGM
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    AddSFXBGM(0.1f, "BGM");
                    break;
                case 81:
                    //increase/decrease SFX, BGM
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    AddSFXBGM(-0.1f, "BGM");
                    break;
                case 82:
                    //increase/decrease SFX, BGM
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    AddSFXBGM(0.1f, "SFX");
                    break;
                case 83:
                    //increase/decrease SFX, BGM
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    AddSFXBGM(-0.1f, "SFX");
                    break;

                case -1:
                    //Quit Game
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    ConfirmQuit();
                    break;

                default:
                    break;
            }
        }

        if (InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_B)){
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
            ResumeGame();

        }
    }

    private void ResumeGame()
    {
        UnconfirmQuit();

        //Hide Pause Menu Layer
        InternalCall.m_DisableLayer(7);
        //Hide sound layer
        InternalCall.m_DisableLayer(16);
        //Hide how to play
        InternalCall.m_DisableLayer(6);

        //Resumes all coroutines
        CoroutineManager.Instance.ResumeAllCoroutines();

        //Sets delta timescale to 1
        InternalCall.m_InternalCallSetTimeScale(1f);

        GameControllerLevel1.gameIsPaused = false;
        GameControllerLevel1.runOnce = false;

        //continue sound
        InternalCall.m_InternalCallUnPauseAllAudio();

        //disable controller cursor
        InternalCall.m_InternalCallOverideMouseWithCursor(false);

    }

    private void RestartGame()
    {
        UnconfirmQuit();

        if (LevelSelection.SceneName != null)
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
            InternalCall.m_InternalCallLoadScene(LevelSelection.SceneName);
        }

        else
        {
            //Console.WriteLine("Level Selection Script Has No SceneName!");

            InternalCall.m_InternalCallStopAllAudio();

            CoroutineManager.Instance.StopAllCoroutines();
            InternalCall.m_InternalCallSetTimeScale(1);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("Level1");
        }
    }

    private void HowToPlay()
    {
        InternalCall.m_DisableLayer(7);
        InternalCall.m_EnableLayer(6);
    }

    private void HowToPlayBack()
    {
        InternalCall.m_DisableLayer(6);
        InternalCall.m_EnableLayer(7);
    }

    private void ConfirmQuit()
    {
        if (confirmQuit == false)
        {
            confirmQuit = true;

            quitConfirmTextComp.m_text = "Confirm";

            //Component.Set<TextComponent>((uint)InternalCall.m_InternalCallGetTagID("UIConfirmQuit"), quitConfirmTextComp);
            //Console.WriteLine($"Bool {confirmQuit}");

        }

        if (confirmQuit == true && (InternalCall.m_InternalCallIsKeyTriggered(keyCode.Y) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_Y)))
        {
            QuitToMainMenu();
        }

        if (confirmQuit == true && (InternalCall.m_InternalCallIsKeyTriggered(keyCode.N) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_B)))
        {
            confirmQuit = false;
        }
    }

    private void UnconfirmQuit()
    {
        confirmQuit = false;

        quitConfirmTextComp.m_text = "Quit";

        Component.Set<TextComponent>((uint)InternalCall.m_InternalCallGetTagID("UIConfirmQuit"), quitConfirmTextComp);
    }

    private void QuitToMainMenu()
    {
        InternalCall.m_DisableLayer(8); //Disables Loadout Menu UI
        InternalCall.m_DisableLayer(7); //Disables Pause Menu UI
        InternalCall.m_DisableLayer(6); //Disable How To Play UI
        InternalCall.m_DisableLayer(9); //Disable Death Screen UI
        InternalCall.m_DisableLayer(15); //Disable FPS counter UI

        InternalCall.m_DisableLayer(1);
        InternalCall.m_DisableLayer(2);
        InternalCall.m_DisableLayer(3);
        InternalCall.m_DisableLayer(4);
        InternalCall.m_DisableLayer(5);
        InternalCall.m_DisableLayer(6);

        //InternalCall.m_UnHideEntityandchildren(EntityID);

        // InternalCall.m_HideEntityandchildren(EntityID);
        InternalCall.m_InternalCallStopAllAudio();
        CoroutineManager.Instance.StopAllCoroutines();
        InternalCall.m_InternalCallSetTimeScale(1);
        InternalCall.m_UnloadAllScene();
        InternalCall.m_InternalCallLoadScene("LevelSelect");
    }

    private void PauseToSoundMenu()
    {
        InternalCall.m_EnableLayer(16);
        InternalCall.m_DisableLayer(7);
    }
    private void SoundToPauseMenu()
    {
        InternalCall.m_DisableLayer(16); //insert x with layer
        InternalCall.m_EnableLayer(7);

    }

    private void AddSFXBGM(float f, string type)
    {
        switch (type)
        {
            case "SFX":
                SoundManager.sfx += f;
                if (SoundManager.sfx > 1f) SoundManager.sfx = 1f;
                if (SoundManager.sfx < 0f) SoundManager.sfx = 0f;
                break;
            case "BGM":
                SoundManager.bgm += f;
                if (SoundManager.bgm > 1f) SoundManager.bgm = 1f;
                if (SoundManager.bgm < 0f) SoundManager.bgm = 0f;
                break;

            default:
                break;
        }

        //Console.WriteLine(SoundManager.sfx);
        //Console.WriteLine(SoundManager.bgm);

    }
}