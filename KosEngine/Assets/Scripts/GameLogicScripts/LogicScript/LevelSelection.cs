using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class LevelSelection : ScriptBase
{
    private uint EntityID;
    public static string SceneName;
    public static bool LevelSelected;

    public override void Awake(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        LevelSelected = false;
        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_mainMenuLoop");
    }

    public override void Update()
    {
        if((InternalCall.m_InternalCallIsKeyPressed(keyCode.Y) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_Y) )&& LevelSelected == true)
        {
            ShowGameLayers();

            InternalCall.m_InternalCallStopAllAudio();
            CoroutineManager.Instance.StopAllCoroutines();

            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene(SceneName);


        }
        else if (InternalCall.m_InternalCallIsKeyPressed(keyCode.N) || InternalCall.m_InternalCallIsKeyPressed(keyCode.ESC) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_B))
        {
            HideGameLayers();

            InternalCall.m_InternalCallStopAllAudio();
            CoroutineManager.Instance.StopAllCoroutines();
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("MainMenu");
        }
    }
    
    private void ShowGameLayers()
    {
        InternalCall.m_EnableLayer(1);
        InternalCall.m_EnableLayer(2);
        InternalCall.m_EnableLayer(3);
        InternalCall.m_EnableLayer(4);
        InternalCall.m_EnableLayer(5);
        InternalCall.m_DisableLayer(6);
    }

    private void HideGameLayers()
    {
        InternalCall.m_DisableLayer(1);
        InternalCall.m_DisableLayer(2);
        InternalCall.m_DisableLayer(3);
        InternalCall.m_DisableLayer(4);
        InternalCall.m_DisableLayer(5);
        InternalCall.m_DisableLayer(6);
    }
}
