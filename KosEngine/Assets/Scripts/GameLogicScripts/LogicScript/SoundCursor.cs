using System;

public class SoundCursor : ScriptBase
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
        if(InternalCall.m_IsLayerVisable(16))
        {
            transComp = Component.Get<TransformComponent>(EntityID);
            InternalCall.m_InternalGetWorldMousePosition(out transComp.m_position);
            Component.Set<TransformComponent>(EntityID, transComp);
        }


    }
}