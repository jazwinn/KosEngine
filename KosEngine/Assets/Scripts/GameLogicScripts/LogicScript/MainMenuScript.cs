using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class MainMenuScript : ScriptBase
{
    private uint EntityID;
    private string commandPromptPrefab;

   // public bool animPlay = false;
    public override void Awake(uint id)
    {
        EntityID = id;
        
        commandPromptPrefab = "Command Prompt";
    }


    public override void Start()
    {
        InternalCall.m_DisableLayer(9);

        InternalCall.m_EnableLayer(1);
        InternalCall.m_EnableLayer(2);
        InternalCall.m_EnableLayer(3);
        InternalCall.m_EnableLayer(4);
        InternalCall.m_EnableLayer(5);

        CoroutineManager.Instance.StartCoroutine(Wait(), "Wait");
        InternalCall.m_InternalCallOverideMouseWithCursor(true);
    }

    private IEnumerator Wait()
    {
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStart01");
        yield return new CoroutineManager.WaitForSeconds(0.3f);
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceLoop01");
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_A))
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");

            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            
            if (isClick == true || InternalCall.m_InternalIsButtonHovered(EntityID))
            {
                CoroutineManager.Instance.StopAllCoroutines();
                InternalCall.m_InternalCallStopAllAudio();
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceQuit01");
                StartMenuButtons.isReading = false;
                InternalCall.m_DisableLayer(9);
                isClick = false;
                InternalCall.m_InternalCallAddPrefab(commandPromptPrefab, 0, 0, 0);
            }
        }
    }
}