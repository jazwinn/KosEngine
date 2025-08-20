using System;

public class PlayerLoadoutText : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public int textID;

    private TextComponent textComp;

    public override void Start()
    {
        textComp = Component.Get<TextComponent>(EntityID);
    }

    public override void Update()
    {
        UpdateText();
    }

    private void UpdateText()
    {
        switch (textID)
        {
            case 10:
                textComp = Component.Get<TextComponent>(EntityID);
                textComp.m_text = GetWeaponName(PlayerLoadoutManager.leftLimbEquippedNo);
                Component.Set<TextComponent>(EntityID, textComp);
                break;
            case 20:
                textComp = Component.Get<TextComponent>(EntityID);
                textComp.m_text = GetWeaponName(PlayerLoadoutManager.rightLimbEquippedNo);
                Component.Set<TextComponent>(EntityID, textComp);
                break;
            case 30:
                textComp = Component.Get<TextComponent>(EntityID);
                textComp.m_text = GetBackWeaponName(PlayerLoadoutManager.backLimbEquippedNo);
                Component.Set<TextComponent>(EntityID, textComp);
                break;
            default:
                break;
        }
    }

    private string GetWeaponName(int weaponNameID)
    {
        switch (weaponNameID)
        {
            case 0:
                return "Gun";

            case 1:
                return "Katana";

            case 2:
                return "Shotgun";

            case 3:
                return "Railgun";

            default:
                return "Placeholder_Weapon";
        }
    }

    private string GetBackWeaponName(int weaponNameID)
    {
        switch (weaponNameID)
        {
            case 0:
                return "Gun";

            case 1:
                return "Booster";

            default:
                return null;
        }
    }
}
