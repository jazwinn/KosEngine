using System;

public class UI_FPSCounter : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
        if (layerEnabled == true)
        {
            InternalCall.m_EnableLayer(15);
        }

        else
        {
            InternalCall.m_DisableLayer(15);
        }
    }
    #endregion

    private TextComponent textComp;

    private float timer = 0f;
    private float interval = 0.2f;
    public static bool layerEnabled;

    public override void Start()
    {
        if (layerEnabled == true)
        {
            InternalCall.m_EnableLayer(15);
        }

        else
        {
            InternalCall.m_DisableLayer(15);
        }

        textComp = Component.Get<TextComponent>((uint)InternalCall.m_InternalCallGetChildrenID(EntityID)[0]);
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.P))
        {
            if (!layerEnabled)
            {
                layerEnabled = true;
                InternalCall.m_EnableLayer(15);
            }

            else if (layerEnabled)
            {
                layerEnabled = false;
                InternalCall.m_DisableLayer(15);
            }

        }

        timer += InternalCall.m_InternalCallGetDeltaTime();

        if (timer >= interval)
        {
            ShowFPS();
            timer = 0f;
        }

    }

    private void ShowFPS()
    {
        //Uncomment and replace with the internal fps get counter thingamajig
        textComp.m_text = "FPS : " + ((int)InternalCall.m_getFPS()).ToString();
        Component.Set<TextComponent>((uint)InternalCall.m_InternalCallGetChildrenID(EntityID)[0], textComp);
    }

}