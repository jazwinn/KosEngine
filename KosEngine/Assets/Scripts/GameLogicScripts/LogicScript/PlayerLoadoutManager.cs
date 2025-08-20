using System;
using System.Collections.Generic;

public class PlayerLoadoutManager : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        isSortieing = false;

        totalLeftRightLimbs = 4;
        totalBackLimbs = 2;

        uiLeftLimbSpriteID = (uint)InternalCall.m_InternalCallGetTagID("UILoadoutLeftLimb");
        uiRightLimbSpriteID = (uint)InternalCall.m_InternalCallGetTagID("UILoadoutRightLimb");
        uiBackLimbSpriteID = (uint)InternalCall.m_InternalCallGetTagID("UILoadoutBackLimb");

        uiLeftLimbAnimComp = Component.Get<AnimationComponent>(uiLeftLimbSpriteID);
        uiRightLimbAnimComp = Component.Get<AnimationComponent>(uiRightLimbSpriteID);
        uiBackLimbAnimComp = Component.Get<AnimationComponent>(uiBackLimbSpriteID);

        cameraID = (uint)InternalCall.m_InternalCallGetTagID("Camera");
        cameraTransformComp = GetComponent.GetTransformComponent(cameraID);
    }
    #endregion

    public static int totalLeftRightLimbs;
    public static int totalBackLimbs;

    public static int leftLimbEquippedNo = 0;
    public static int rightLimbEquippedNo = 0;
    public static int backLimbEquippedNo = 0;

    private uint uiLeftLimbSpriteID;
    private uint uiRightLimbSpriteID;
    private uint uiBackLimbSpriteID;

    private AnimationComponent uiLeftLimbAnimComp;
    private AnimationComponent uiRightLimbAnimComp;
    private AnimationComponent uiBackLimbAnimComp;

    private TransformComponent transformComp;
    private TransformComponent cameraTransformComp;

    private uint cameraID;

    public static bool isSortieing;

    public override void Start()
    {
        transformComp = Component.Get<TransformComponent>(EntityID);
    }

    public override void Update()
    {
        if (isSortieing)
        {
            CheckSprites();
        }   
    }

    public void LateUpdate()
    {
        UpdatePosition();
    }

    private void UpdatePosition()
    {
        transformComp.m_position = MoveTowards(transformComp.m_position, Component.Get<TransformComponent>(cameraID).m_position, 20f * InternalCall.m_InternalCallGetDeltaTime());
        if (float.IsNaN(transformComp.m_position.X) || float.IsNaN(transformComp.m_position.Y))
        {
            transformComp.m_position = new Vector2(0, 0);
        }
        Component.Set<TransformComponent>(EntityID, transformComp);
    }

    private void CheckSprites()
    {
        uiLeftLimbAnimComp.m_frameNumber = leftLimbEquippedNo;
        uiRightLimbAnimComp.m_frameNumber = rightLimbEquippedNo;
        uiBackLimbAnimComp.m_frameNumber = backLimbEquippedNo;

        Component.Set<AnimationComponent>(uiLeftLimbSpriteID, uiLeftLimbAnimComp);
        Component.Set<AnimationComponent>(uiRightLimbSpriteID , uiRightLimbAnimComp);
        Component.Set<AnimationComponent>(uiBackLimbSpriteID, uiBackLimbAnimComp);
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