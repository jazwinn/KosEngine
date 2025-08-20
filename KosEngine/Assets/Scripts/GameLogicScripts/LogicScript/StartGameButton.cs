using System;
using System.Collections.Generic;
using System.Linq;
using System.Runtime.Versioning;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class StartGameButton : ScriptBase
{
    //public static string yolo;// see moving object script
    //public float speed;
    //private Queue<int> queue = new Queue<int>();

    private uint EntityID;
    private string commandPromptPrefab;

    public override void Awake(uint id)
    {
        EntityID = id;
        commandPromptPrefab = "Command Prompt";
    }

    public override void Start()
    {
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceStart01");
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceLoop01");
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB))
        {
            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            if (isClick == true)
            {
                InternalCall.m_InternalCallStopAllAudio();
                isClick = false;
                StartMenuButtons.isReading = false;
                InternalCall.m_DisableLayer(9);
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                InternalCall.m_InternalCallPlayAudio(EntityID, "aud_startMenuAmbienceEnd01");
                InternalCall.m_InternalCallAddPrefab(commandPromptPrefab, 0, 0, 0);
            }
        }   
    }
}