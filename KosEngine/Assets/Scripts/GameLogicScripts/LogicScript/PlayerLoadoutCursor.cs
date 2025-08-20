using System;

public class PlayerLoadoutCursor : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    private TransformComponent transComp;

    public override void Start()
    {
    }

    public override void Update()
    {
        if(PlayerLoadoutManager.isSortieing)
        {
            transComp = Component.Get<TransformComponent>(EntityID);
            InternalCall.m_InternalGetWorldMousePosition(out transComp.m_position);
            Component.Set<TransformComponent>(EntityID, transComp);
        }
    }
}