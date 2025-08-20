using System;

public class UI_ControllerSwap : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public int functionNo;

    public override void Start()
    {
    }

    public override void Update()
    {
        if (PlayerController.isControllerLastUsed)
        {
            SwapToControllerUI();
        }

        else
        {
            SwapToKeyboardUI();
        }
    }

    private void SwapToControllerUI()
    {

    }

    private void SwapToKeyboardUI()
    {

    }
}