using System;
using System.ComponentModel;

public class UI_AmmoHUD : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        cameraID = (uint)InternalCall.m_InternalCallGetTagID("Camera");
        cameraTransformComp = GetComponent.GetTransformComponent(cameraID);

        uiLeftLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameLeftLimbCounter");
        uiRightLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameRightLimbCounter");
        uiBackLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameBackLimbCounter");

        uiLeftLimbIconID = (uint)InternalCall.m_InternalCallGetTagID("UIGameLeftLimbCounterIcon");
        uiRightLimbIconID = (uint)InternalCall.m_InternalCallGetTagID("UIGameRightLimbCounterIcon");
        uiBackLimbIconID = (uint)InternalCall.m_InternalCallGetTagID("UIGameBackLimbCounterIcon");

        uiLeftLimbIconSC = Component.Get<SpriteComponent>(uiLeftLimbIconID);
        uiRightLimbIconSC = Component.Get<SpriteComponent>(uiRightLimbIconID);
        uiBackLimbIconSC = Component.Get<SpriteComponent>(uiBackLimbIconID);

        uiLeftLimbCounterSC = Component.Get<SpriteComponent>(uiLeftLimbCounterID);
        uiRightLimbCounterSC = Component.Get<SpriteComponent>(uiRightLimbCounterID);
        uiBackLimbCounterSC = Component.Get<SpriteComponent>(uiBackLimbCounterID);

        uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
        uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
        uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiBackLimbCounterID);

    }
    #endregion

    private string gunIconTexture = "img_hudGunIcon.png";
    private string katanaIconTexture = "img_hudKatanaIcon.png";
    private string boosterIconTexture = "img_hudBoosterIcon.png";
    private string shotgunIconTexture = "img_hudShotgunIcon.png";
    private string railgunIconTexture = "img_hudRailgunIcon.png";

    private string fullBarTexture = "ani_hudFullBar_strip7.png";
    private string segmentedBarTexture = "ani_hudSegmentBar_strip7.png";



    private TransformComponent transformComp;
    private TransformComponent cameraTransformComp;

    private uint cameraID;

    private uint uiLeftLimbCounterID;
    private uint uiRightLimbCounterID;
    private uint uiBackLimbCounterID;

    private uint uiLeftLimbIconID;
    private uint uiRightLimbIconID;
    private uint uiBackLimbIconID;

    private SpriteComponent uiLeftLimbIconSC;
    private SpriteComponent uiRightLimbIconSC;
    private SpriteComponent uiBackLimbIconSC;

    private SpriteComponent uiLeftLimbCounterSC;
    private SpriteComponent uiRightLimbCounterSC;
    private SpriteComponent uiBackLimbCounterSC;

    private AnimationComponent uiLeftLimbCounterAC;
    private AnimationComponent uiRightLimbCounterAC;
    private AnimationComponent uiBackLimbCounterAC;


    public override void Start()
    {
        transformComp = Component.Get<TransformComponent>(EntityID);

        UpdateUIIcons();
    }

    public override void Update()
    {
        if (PlayerLoadoutManager.isSortieing)
        {
            UpdateUIIcons();
        }
        
    }

    public void LateUpdate()
    {
        UpdatePosition();
    }

    private void UpdateUIIcons()
    {
        uiLeftLimbIconSC.m_imageFile = CheckFrontWeaponIcon(PlayerLoadoutManager.leftLimbEquippedNo);
        uiRightLimbIconSC.m_imageFile = CheckFrontWeaponIcon(PlayerLoadoutManager.rightLimbEquippedNo);
        uiBackLimbIconSC.m_imageFile = CheckBackWeaponIcon(PlayerLoadoutManager.backLimbEquippedNo);

        Component.Set<SpriteComponent>(uiLeftLimbIconID, uiLeftLimbIconSC);
        Component.Set<SpriteComponent>(uiRightLimbIconID, uiRightLimbIconSC);
        Component.Set<SpriteComponent>(uiBackLimbIconID, uiBackLimbIconSC);

        uiLeftLimbCounterSC.m_imageFile = CheckFrontWeaponCounter(PlayerLoadoutManager.leftLimbEquippedNo);
        uiRightLimbCounterSC.m_imageFile = CheckFrontWeaponCounter(PlayerLoadoutManager.rightLimbEquippedNo);
        uiBackLimbCounterSC.m_imageFile = CheckBackWeaponCounter(PlayerLoadoutManager.backLimbEquippedNo);

        Component.Set<SpriteComponent>(uiLeftLimbCounterID, uiLeftLimbCounterSC);
        Component.Set<SpriteComponent>(uiRightLimbCounterID, uiRightLimbCounterSC);
        Component.Set<SpriteComponent>(uiBackLimbCounterID, uiBackLimbCounterSC);
    }

    private string CheckFrontWeaponIcon(int wepID)
    {
        switch (wepID)
        {
            case 0:
                return gunIconTexture;

            case 1:
                return katanaIconTexture;

            case 2:
                return shotgunIconTexture;

            case 3:
                return railgunIconTexture;

            default:
                return null;
        }
    }

    private string CheckBackWeaponIcon(int wepID)
    {
        switch (wepID)
        {
            //Katana
            case 0:
                return gunIconTexture;

            case 1:
                return boosterIconTexture;

            default:
                return null;
        }
    }

    private string CheckFrontWeaponCounter(int wepID)
    {
        switch (wepID)
        {
            case 0:
                return segmentedBarTexture;

            case 1:
                return fullBarTexture;

            case 2:
                return segmentedBarTexture;

            case 3:
                return fullBarTexture;

            default:
                return null;
        }
    }

    private string CheckBackWeaponCounter(int wepID)
    {
        switch (wepID)
        {
            //Katana
            case 0:
                return segmentedBarTexture;

            case 1:
                return fullBarTexture;

            default:
                return fullBarTexture;
        }
    }

    private void UpdatePosition()
    {
        transformComp.m_position = MoveTowards(transformComp.m_position, Component.Get<TransformComponent>(cameraID).m_position, 30f * InternalCall.m_InternalCallGetDeltaTime());
        if (float.IsNaN(transformComp.m_position.X) || float.IsNaN(transformComp.m_position.Y))
        {
            transformComp.m_position = new Vector2(0, 0);
        }

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
}