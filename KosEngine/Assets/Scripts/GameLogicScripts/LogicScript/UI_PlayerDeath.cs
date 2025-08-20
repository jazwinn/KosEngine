using System;

public class UI_PlayerDeath : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
        playerTransformComp = GetComponent.GetTransformComponent(playerID);
    }
    #endregion

    private TransformComponent transformComp;
    private TransformComponent playerTransformComp;

    private uint playerID;

    public override void Start()
    {
        transformComp = Component.Get<TransformComponent>(EntityID);
        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player");
    }

    public override void Update()
    {
        UpdatePosition();

        if (PlayerController.isDead)
        {
            InternalCall.m_EnableLayer(9);     
        }
    }

    private void UpdatePosition()
    {
        transformComp.m_position = MoveTowards(transformComp.m_position, Component.Get<TransformComponent>(playerID).m_position, 30f * InternalCall.m_InternalCallGetDeltaTime());
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