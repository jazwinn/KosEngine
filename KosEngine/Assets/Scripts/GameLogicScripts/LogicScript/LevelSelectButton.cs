using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelSelectButton : ScriptBase
{
    private uint EntityID;

    public int buttonFunctionNo;
    //private string SceneName;
    private bool LevelSelected = false;
    private bool isSelected;
    private bool isClick;
    public static bool otherIsClick;

    private ButtomComponent buttonComp;

    public static List<LevelSelectButton> allButtonScripts = new List<LevelSelectButton>();

    private AnimationComponent animComp;

    private bool isHovering;

    

    public override void Awake(uint id)
    {
        EntityID = id;
        HideAllBlueprints();
    }


    public override void Start()
    {
        allButtonScripts.Add(this);
        HideAllBlueprints();
        isClick = false;
        otherIsClick = false;
        isHovering = false;
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_A))
        {
            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            if ((isClick == true || InternalCall.m_InternalIsButtonHovered(EntityID))&& isSelected == false)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                UnclickOtherButtons();
                ClickButton();   
            }

            else if (isClick == true && isSelected == true)
            {
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                UnclickButton();
            }
        }

        if(InternalCall.m_InternalIsButtonHovered(EntityID))
        {
            if (isHovering) { return; }
            animComp = Component.Get<AnimationComponent>(EntityID);
            animComp.m_frameNumber = 1;
            Component.Set<AnimationComponent>(EntityID, animComp);
            isHovering = true;
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonHover01");
        }

        else if (!isSelected)
        {
            isHovering = false;
            animComp = Component.Get<AnimationComponent>(EntityID);
            animComp.m_frameNumber = 0;
            Component.Set<AnimationComponent>(EntityID, animComp);
        }


    }

    private void ClickButton()
    {
        AnimationComponent temp = Component.Get<AnimationComponent>(EntityID);
        temp.m_frameNumber = 1;
        Component.Set<AnimationComponent>(EntityID, temp);
        isSelected = true;
        isClick = false;
        
        switch (buttonFunctionNo)
        {
            case 10:
                LevelSelection.SceneName = "TutorialCutscene01";
                InternalCall.m_EnableLayer(1);
                break;
            case 20:
                LevelSelection.SceneName = "CutsceneLevel2";
                InternalCall.m_EnableLayer(2);
                break;
            case 30:
                LevelSelection.SceneName = "CutsceneLevel3";
                InternalCall.m_EnableLayer(3);
                break;
            case 40:
                LevelSelection.SceneName = "CutsceneLevel4";
                InternalCall.m_EnableLayer(4);
                break;
            case 50:
                LevelSelection.SceneName = "CutsceneLevel5";
                InternalCall.m_EnableLayer(5);
                break;
            case 60:
                LevelSelection.SceneName = "CutsceneLevel6";
                InternalCall.m_EnableLayer(6);
                break;
            case -1:
                InternalCall.m_InternalCallStopAllAudio();
                CoroutineManager.Instance.StopAllCoroutines();
                InternalCall.m_UnloadAllScene();
                InternalCall.m_InternalCallLoadScene("MainMenu");
                InternalCall.m_DisableLayer(1);
                InternalCall.m_DisableLayer(2);
                InternalCall.m_DisableLayer(3);
                InternalCall.m_DisableLayer(4);
                InternalCall.m_DisableLayer(5);
                InternalCall.m_DisableLayer(6);
                break;
            default:
                break;
        }
        LevelSelection.LevelSelected = true;
       
    }

    public void UnclickButton()
    {
        HideAllBlueprints();
        AnimationComponent temp = Component.Get<AnimationComponent>(EntityID);
        temp.m_frameNumber = 0;
        Component.Set<AnimationComponent>(EntityID, temp);
        isSelected = false;
        isClick = false;
        LevelSelection.LevelSelected = false;
        LevelSelection.SceneName = null;
    }

    private void UnclickOtherButtons()
    {
        allButtonScripts.Remove(this);
        foreach(LevelSelectButton lsbScript in allButtonScripts)
        {
            if (lsbScript.isSelected)
            {
                lsbScript.UnclickButton();
            }
        }
        allButtonScripts.Add(this);
    }

    private void HideAllBlueprints()
    {
        InternalCall.m_DisableLayer(1);
        InternalCall.m_DisableLayer(2);
        InternalCall.m_DisableLayer(3);
        InternalCall.m_DisableLayer(4);
        InternalCall.m_DisableLayer(5);
        InternalCall.m_DisableLayer(6);
    }
}
