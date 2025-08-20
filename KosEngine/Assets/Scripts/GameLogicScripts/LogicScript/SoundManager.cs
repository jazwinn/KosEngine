using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class SoundManager : ScriptBase
{

    private uint EntityID;

    static public float sfx;
    static public float bgm;

    float prevsfx;
    float prevbgm;

    private int BGMtextID;
    private int SFXtextID;

    private int BGMSoundbarID;
    private int SFXSoundbarID;

    private float textValueMultiplier;

    private TransformComponent transformComp;
    private TransformComponent playerTransformComp;
    private uint cameraID;
    public override void Awake(uint id)
    {
        EntityID = id;

        sfx = InternalCall.m_InternalCallGetGlobalSFXVolume();
        bgm = InternalCall.m_InternalCallGetGlobalBGMVolume();


        BGMtextID = InternalCall.m_InternalCallGetTagID("UIPauseTextBGM");
        SFXtextID = InternalCall.m_InternalCallGetTagID("UIPauseTextSFX");

        BGMSoundbarID = InternalCall.m_InternalCallGetTagID("SoundBarBGM");
        SFXSoundbarID = InternalCall.m_InternalCallGetTagID("SoundBarSFX");

        textValueMultiplier = 100;

        prevsfx = sfx;
        prevbgm = bgm;

        cameraID = (uint)InternalCall.m_InternalCallGetTagID("Camera");
        playerTransformComp = GetComponent.GetTransformComponent(cameraID);

    }


    public override void Start()
    {
        transformComp = Component.Get<TransformComponent>(EntityID);
        UpdateSoundbar();
    }

    private void UpdateSoundbar()
    {
        int[] sfxChilds = InternalCall.m_InternalCallGetChildrenID((uint)SFXSoundbarID);
        int[] bgmChilds = InternalCall.m_InternalCallGetChildrenID((uint)BGMSoundbarID);

        int sfxcount = (int)Math.Round(sfx * 10, 0);
        int bgmCount = (int)Math.Round(bgm * 10, 0);
        for(int i = 0; i < sfxChilds.Length - 2; i++)
        {
            int child = sfxChilds[i];
            AnimationComponent AC = Component.Get<AnimationComponent>((uint)child);
            AC.m_frameNumber = (i < sfxcount) ? 1 : 0; 
            Component.Set<AnimationComponent>((uint)child, AC); 
        }

        for (int i = 0; i < bgmChilds.Length - 2; i++)
        {
            int child = bgmChilds[i];
            AnimationComponent AC = Component.Get<AnimationComponent>((uint)child);
            AC.m_frameNumber = (i < bgmCount) ? 1 : 0;
            Component.Set<AnimationComponent>((uint)child, AC);
        }

    }

    private void UpdatePosition()
    {
        transformComp.m_position = MoveTowards(transformComp.m_position, Component.Get<TransformComponent>(cameraID).m_position, 20f * InternalCall.m_InternalCallGetDeltaTime());
        Component.Set<TransformComponent>(EntityID, transformComp);
    }

    public Vector2 MoveTowards(Vector2 current, Vector2 target, float maxDistance)
    {
        float dx = target.X - current.X;
        float dy = target.Y - current.Y;
        float distance = (float)Math.Sqrt(dx * dx + dy * dy);

        // If the distance to the target is less than maxDistance, move directly to the target
        if (distance <= maxDistance || distance == 0f)
        {
            return target;
        }

        // Otherwise, move a fraction towards the target
        float ratio = maxDistance / distance;
        return new Vector2(
            current.X + dx * ratio,
            current.Y + dy * ratio
        );
    }

    public override void Update()
    {
       InternalCall.m_InternalCallSetGlobalBGMVolume(bgm);
       InternalCall.m_InternalCallSetGlobalSFXVolume(sfx);

        //update text for BGM and SFX
        
        if(BGMtextID >= 0)
        {
            TextComponent BGM_tc = Component.Get<TextComponent>((uint)BGMtextID);
            BGM_tc.m_text = ((int)Math.Round(bgm * (float)textValueMultiplier)).ToString();
            Component.Set((uint)BGMtextID, BGM_tc);
        }
        if(SFXtextID >= 0)
        {
            TextComponent SFX_tc = Component.Get<TextComponent>((uint)SFXtextID);
            SFX_tc.m_text = ((int)Math.Round(sfx * (float)textValueMultiplier)).ToString();
            Component.Set((uint)SFXtextID, SFX_tc);
        }

        //check for changes in sfx/bgm
        if((sfx != prevsfx) || (bgm != prevbgm))
        {
            UpdateSoundbar();
            prevsfx = sfx;
            prevbgm = bgm;
        }

        //update postion to follow camera
        UpdatePosition();
    }

    public void LateUpdate()
    {

    }

}