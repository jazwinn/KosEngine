using System;

public class BlankScript : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public override void Start()
    {
    }

    public override void Update()
    {
    }
}