using System;

public class UI_TabPrompt : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    private AnimationComponent animComp;

    public override void Start()
    {
        animComp = Component.Get<AnimationComponent>(EntityID);
    }

    public override void Update()
    {
    }
}