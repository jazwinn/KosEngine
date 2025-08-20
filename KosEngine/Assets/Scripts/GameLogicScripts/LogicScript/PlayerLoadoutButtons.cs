using System;

public class PlayerLoadoutButtons : ScriptBase
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
        if (PlayerLoadoutManager.isSortieing)
        {
            if (!GameControllerLevel1.gameIsPaused)
            {
                CheckForCollisions();
                CheckForHover();
                CheckForClicks();
            }
            else
            {
                isHovering = false;
                CheckForHover();
            }
        }
        
    }

    private void CheckForCollisions()
    {
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "PlayerLoadoutCursor":
                        if (isHovering)
                        {
                            break;
                        }
                        isHovering = true;
                        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonHover01");
                        break;

                    default:
                        break;
                }
            }
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
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.leftLimbEquippedNo--;
                    if (PlayerLoadoutManager.leftLimbEquippedNo < 0) { PlayerLoadoutManager.leftLimbEquippedNo = PlayerLoadoutManager.totalLeftRightLimbs - 1; }
                    break;
                case 11:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.leftLimbEquippedNo++;
                    if (PlayerLoadoutManager.leftLimbEquippedNo > PlayerLoadoutManager.totalLeftRightLimbs - 1) { PlayerLoadoutManager.leftLimbEquippedNo = 0; }
                    break;
                case 20:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.rightLimbEquippedNo--;
                    if (PlayerLoadoutManager.rightLimbEquippedNo < 0) { PlayerLoadoutManager.rightLimbEquippedNo = PlayerLoadoutManager.totalLeftRightLimbs - 1; }
                    break;
                case 21:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.rightLimbEquippedNo++;
                    if (PlayerLoadoutManager.rightLimbEquippedNo > PlayerLoadoutManager.totalLeftRightLimbs - 1) { PlayerLoadoutManager.rightLimbEquippedNo = 0; }
                    break;
                case 30:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.backLimbEquippedNo--;
                    if (PlayerLoadoutManager.backLimbEquippedNo < 0) { PlayerLoadoutManager.backLimbEquippedNo = PlayerLoadoutManager.totalBackLimbs - 1; }
                    break;
                case 31:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.backLimbEquippedNo++;
                    if (PlayerLoadoutManager.backLimbEquippedNo > PlayerLoadoutManager.totalBackLimbs - 1) { PlayerLoadoutManager.backLimbEquippedNo = 0; }
                    break;
                case -1:
                    InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
                    PlayerLoadoutManager.isSortieing = false;
                    InternalCall.m_InternalCallSetTimeScale(1f);
                    InternalCall.m_DisableLayer(8);
                    break;
                default:
                    break;
            }
        }


        if (InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_B))
        {
            InternalCall.m_InternalCallPlayAudio(EntityID, "aud_buttonClick01");
            PlayerLoadoutManager.isSortieing = false;
            InternalCall.m_InternalCallSetTimeScale(1f);
            InternalCall.m_DisableLayer(8);

        }
    }
}