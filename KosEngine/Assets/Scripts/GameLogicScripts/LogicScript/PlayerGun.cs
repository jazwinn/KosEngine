using System;
using System.Collections;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Text.RegularExpressions;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class PlayerGun : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;

        gunshotSound = "aud_gunshot01.wav";
        cleaverSound = "aud_cleaver.wav";
        katanaSound = "aud_katana01.wav";
        emptyGunSound = "aud_emptyGun01.wav";
        cockingSound = "aud_shotgunCocking01.wav";
        shotGunShotSound = "aud_shotgunShot01.wav";
        railGunChargeSound = "aud_railgunChargeUp01.wav";
        railGunUnchargeSound = "aud_railgunChargeDown01.wav";
        railGunReleaseSound = "aud_railgunShot01.wav";
        boosterBoostSound = "aud_boosters01.wav";
        boosterMovementSound = "aud_vacuumStart01.wav";

        leftCleaverTexture = "ani_cleaverLeftAnim_strip6.png";
        rightCleaverTexture = "ani_cleaverRightAnim_strip6.png";

        leftKatanaTexture = "ani_katanaLeftAnim_strip19.png";
        rightKatanaTexture = "ani_katanaRightAnim_strip19.png";

        backBoosterTexture = "ani_boostersTailAnim_strip18.png";

        leftGunTexture = "ani_gunLeftAnim_strip8.png";
        rightGunTexture = "ani_gunRightAnim_strip8.png";
        backGunTexture = "ani_gunTailAnim_strip8.png";

        leftShotGunTexture = "ani_shotGunLeftAnim_strip8.png";
        rightShotGunTexture = "ani_shotGunRightAnim_strip8.png";

        leftRailGunTexture = "ani_railgunLeftAnim_strip10.png";
        rightRailGunTexture = "ani_railgunRightAnim_strip10.png";

        bulletPrefab = "prefab_playerBullet";
        shotgunBulletPrefab = "prefab_playerShotgunBullet";
        bulletPrefabType2 = "PlayerBulletType2";

        leftLimbGunAmmo = 6;
        rightLimbGunAmmo = 6;
        backLimbGunAmmo = 6;

        leftLimbShotGunAmmo = 6;
        rightLimbShotGunAmmo = 6;

        leftWeaponCount = 1; // for cool down / firerate
        rightWeaponCount = 1;
        backWeaponcount = 1;

        leftLimbRailGunHold = 0;
        rightLimbRailGunHold = 0;
        chargeDurationRailGun = 1.5f; //duration to charge up railgun
        railGunLightMaxIntensity = 1f;

        playerBoost = false;
        boostAcceleration = 40f;
        boostDuration = 0.60f;
        boostCoolDown = 1.4f;

        limbTag = InternalCall.m_InternalCallGetTag(EntityID);
        leftLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("LeftLimbPos");
        rightLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("RightLimbPos");
        backLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("BackLimbPos");
        leftShotgunLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("LeftShotgunLimbPos");
        rightShotgunLimbPosID = (uint)InternalCall.m_InternalCallGetTagID("RightShotgunLimbPos");

        InternalCall.m_InternalGetTranslate(EntityID, out limbPos);
        InternalCall.m_InternalGetTranslate(EntityID, out shotgunLimbPos);
        GetComponentValues();

    }
    #endregion

    #region Variables
    public string limbTag;
    private uint leftLimbPosID;
    private uint rightLimbPosID;
    private uint backLimbPosID;
    private uint leftShotgunLimbPosID;
    private uint rightShotgunLimbPosID;
    private Vector2 limbPos;
    private Vector2 shotgunLimbPos;

    private int weaponEquipped;

    //Audio
    private string gunshotSound;
    private string cleaverSound;
    private string katanaSound;
    private string emptyGunSound;
    private string shotGunShotSound;
    private string cockingSound;
    private string railGunChargeSound;
    private string railGunUnchargeSound;
    private string railGunReleaseSound;
    private string boosterBoostSound;
    private string boosterMovementSound;

    //Texture
    private string leftCleaverTexture;
    private string rightCleaverTexture;
    private string leftKatanaTexture;
    private string rightKatanaTexture;
    private string backBoosterTexture;
    private string leftGunTexture;
    private string rightGunTexture;
    private string backGunTexture;
    private string bulletPrefab;
    private string bulletPrefabType2;
    private string shotgunBulletPrefab;
    private string leftShotGunTexture;
    private string rightShotGunTexture;
    private string leftRailGunTexture;
    private string rightRailGunTexture;

    private SpriteComponent limbSpriteComp;
    private AnimationComponent limbAnimComp;
    private TransformComponent playerTransformComp;

    private uint playerID;

    public static int leftLimbGunAmmo;
    public static int rightLimbGunAmmo;
    public static int backLimbGunAmmo;

    public static int leftLimbShotGunAmmo;
    public static int rightLimbShotGunAmmo;

    public static int leftWeaponCount;
    public static int rightWeaponCount;
    public static int backWeaponcount;

    public float leftLimbRailGunHold;
    public float rightLimbRailGunHold;
    public float chargeDurationRailGun;

    public float railGunLightMaxIntensity;

    private bool isRailGunCharging;

    private bool isBoosting;

    //UI
    private uint uiLeftLimbCounterID;
    private uint uiRightLimbCounterID;
    private uint uiBackLimbCounterID;

    public static bool playerBoost;
    public float boostAcceleration;
    public float boostDuration;
    public float boostCoolDown;

    private SpriteComponent uiLeftLimbCounterSC;
    private SpriteComponent uiRightLimbCounterSC;
    private SpriteComponent uiBackLimbCounterSC;

    private AnimationComponent uiLeftLimbCounterAC;
    private AnimationComponent uiRightLimbCounterAC;
    private AnimationComponent uiBackLimbCounterAC;

    //Animation
    private bool uiLeftLimbCounterAnimating;
    private bool uiRightLimbCounterAnimating;
    private bool uiBackLimbCounterAnimating;

    private bool isAnimating;
    #endregion

    public override void Start()
    {
        CheckWeapons();

        uiLeftLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameLeftLimbCounter");
        uiRightLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameRightLimbCounter");
        uiBackLimbCounterID = (uint)InternalCall.m_InternalCallGetTagID("UIGameBackLimbCounter");

        uiLeftLimbCounterSC = Component.Get<SpriteComponent>(uiLeftLimbCounterID);
        uiRightLimbCounterSC = Component.Get<SpriteComponent>(uiRightLimbCounterID);
        uiBackLimbCounterSC = Component.Get<SpriteComponent>(uiBackLimbCounterID);

        uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
        uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
        uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiBackLimbCounterID);

        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player");
    }

    public override void Update()
    {
        if (GameControllerLevel1.gameIsPaused || PlayerController.isDead)
        {
            if (PlayerController.isDead)
            {
                StopAnimation();
                return;
            }

            if (isAnimating == true)
            {
                CoroutineManager.Instance.StartCoroutine(PauseAnimation(), "PauseAnimation");
                return;
            }

            return;
        }

        if (PlayerLoadoutManager.isSortieing)
        {
            CheckWeapons();
        }

        switch (weaponEquipped)
        {
            case 0:
                CheckAmmo();
                break;

            case 1:
                if (limbTag == "LeftLimbSprite" || limbTag == "RightLimbSprite")
                {
                    CheckMeleeCounter();
                    break;
                }
                else if (limbTag == "BackLimbSprite")
                {
                    CheckBoosterCounter();
                    break;
                }

                break;

            case 2:
                CheckAmmo();
                break;

            case 3:
                CheckRailgunCounter();
                break;
            default:
                break;
        }

        if (!GameControllerLevel1.gameIsPaused || isBoosting)
        {
            CheckAndSetBoost();

            if (!isBoosting)
            {
                CheckInputs();
            }
        }



        #region Animation

        if (isAnimating)
        {
            GetComponentValues();
            if (limbAnimComp.m_frameNumber == limbAnimComp.m_stripCount - 1)
            {
                StopAnimation();
            }
        }

        if (uiLeftLimbCounterAnimating)
        {
            uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);

            if (uiLeftLimbCounterAC.m_frameNumber == uiLeftLimbCounterAC.m_stripCount - 1)
            {
                uiLeftLimbCounterAnimating = false;
                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = 6;
                uiLeftLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
            }
        }

        if (uiRightLimbCounterAnimating)
        {
            uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);

            if (uiRightLimbCounterAC.m_frameNumber == uiRightLimbCounterAC.m_stripCount - 1)
            {
                uiRightLimbCounterAnimating = false;
                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = 6;
                uiRightLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
            }
        }
        if (uiBackLimbCounterAnimating)
        {
            uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiBackLimbCounterID);

            if (uiBackLimbCounterAC.m_frameNumber == uiBackLimbCounterAC.m_stripCount - 1)
            {
                uiBackLimbCounterAnimating = false;
                uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiBackLimbCounterAC.m_frameNumber = 6;
                uiBackLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiBackLimbCounterID, uiBackLimbCounterAC);
            }
        }
        #endregion

    }
    private void AttemptShootGun()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                if (leftLimbGunAmmo != 0) { leftLimbGunAmmo--; }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    return;
                }
                break;

            case "RightLimbSprite":
                if (rightLimbGunAmmo != 0) { rightLimbGunAmmo--; }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    return;
                }
                break;

            case "BackLimbSprite":
                if (backLimbGunAmmo != 0) { backLimbGunAmmo--; }
                else
                {
                    InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    return;
                }
                break;

            default:
                return;
        }

        CoroutineManager.Instance.StartCoroutine(Shoot(), "Shooting");
    }
    private void AttemptShootShotGun()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                if (leftLimbShotGunAmmo != 0 && leftWeaponCount != 0) { 
                    leftLimbShotGunAmmo--;
                    leftWeaponCount--;
                }
                else
                {   
                    if(leftLimbShotGunAmmo <= 0)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                        
                    }
                    
                    return;
                }
                break;

            case "RightLimbSprite":
                if (rightLimbShotGunAmmo != 0 && rightWeaponCount != 0) { 
                    rightLimbShotGunAmmo--; 
                    rightWeaponCount--;
                }
                else
                {
                    if(rightLimbShotGunAmmo <= 0)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, emptyGunSound);
                    }
                    
                    return;
                }
                break;

            default:
                return;
        }

        CoroutineManager.Instance.StartCoroutine(Shoot(), "Shooting");
    }
    private void SetRailGunLightIntensity(float intensity, string playergunlimb)
    {
        int lightid = InternalCall.m_InternalCallGetTagID(playergunlimb);
        if (lightid > 0)
        {
            LightComponent lc = Component.Get<LightComponent>((uint)lightid);
            lc.m_intensity = intensity;
            Component.Set<LightComponent>((uint)lightid, lc);

        }
    }
    private void AttemptShootRailGunHold()
    {
        if (!isRailGunCharging) { InternalCall.m_InternalCallPlayAudio(EntityID, railGunChargeSound); }

        isRailGunCharging = true;

        switch (limbTag)
        {
            case "LeftLimbSprite":
                {
                    leftLimbRailGunHold += InternalCall.m_GetUnfixedDeltaTime();
                    int strip = GetStripCount(leftRailGunTexture);
                    if (strip == 0) break;//in case strip returns 0

                    int railGunstrip = (int)Math.Floor(leftLimbRailGunHold / (chargeDurationRailGun / (float)strip));
                    float lightintensity = leftLimbRailGunHold / (chargeDurationRailGun / (float)railGunLightMaxIntensity);
                    float glowFactor = (float)Math.Cos((double)InternalCall.m_InternalCallGetGameTime()*20);
                    glowFactor = glowFactor/2.0f + 1.0f;

                    if (railGunstrip >= strip)
                    {
                        railGunstrip = (strip - 1);
                        lightintensity = railGunLightMaxIntensity * glowFactor;
                    }

                    SetRailGunLightIntensity(lightintensity, "PlayerGunLightLeft");

                    GetComponentValues();
                    
                    limbAnimComp.m_frameNumber = (railGunstrip);// -1 cause start from 0

                    Component.Set<AnimationComponent>(EntityID, limbAnimComp);

                }

                //Console.WriteLine(leftLimbRailGunHold);
                break;

            case "RightLimbSprite":
                {
                    rightLimbRailGunHold += InternalCall.m_GetUnfixedDeltaTime();
                    int strip = GetStripCount(leftRailGunTexture);
                    if (strip == 0) break;//in case strip returns 0

                    int railGunstrip = (int)Math.Floor(rightLimbRailGunHold / (chargeDurationRailGun / (float)strip));
                    float lightintensity = rightLimbRailGunHold / (chargeDurationRailGun / (float)railGunLightMaxIntensity);
                    float glowFactor = (float)Math.Cos((double)InternalCall.m_InternalCallGetGameTime() * 20);
                    glowFactor = glowFactor / 2.0f + 1.0f;

                    if (railGunstrip >= strip)
                    {
                        railGunstrip = (strip - 1);
                        lightintensity = railGunLightMaxIntensity * glowFactor;
                    }

                    SetRailGunLightIntensity(lightintensity, "PlayerGunLightRight");

                    GetComponentValues();
                    limbAnimComp.m_frameNumber = (railGunstrip);// -1 cause start from 0
                    Component.Set<AnimationComponent>(EntityID, limbAnimComp);
                }
               
                break;

            default:
                return;
        }

    }
    private void AttemptShootRailGunRelease()
    {
        InternalCall.m_InternalCallStopAudio(EntityID, railGunChargeSound);

        isRailGunCharging = false;

        switch (limbTag)
        {
            case "LeftLimbSprite":

                if (leftLimbRailGunHold >= chargeDurationRailGun)
                {
                    CoroutineManager.Instance.StartCoroutine(Shoot(), "Shooting");

                    //reset gun
                    leftLimbRailGunHold = 0f;
                    SetRailGunLightIntensity(0f, "PlayerGunLightLeft"); //set light intensity to 0 to reset
                    GetComponentValues();
                    limbAnimComp.m_frameNumber = (0);
                    Component.Set<AnimationComponent>(EntityID, limbAnimComp);

                    CameraFollowPlayerScript.Shake(0.6f, 1f);

                }

                else if(leftLimbRailGunHold > 0f)
                {
                    //reset gun
                    leftLimbRailGunHold = 0f;
                    SetRailGunLightIntensity(0f, "PlayerGunLightLeft"); //set light intensity to 0 to reset
                    GetComponentValues();
                    limbAnimComp.m_frameNumber = (0);
                    Component.Set<AnimationComponent>(EntityID, limbAnimComp);
                    InternalCall.m_InternalCallPlayAudio(EntityID, railGunUnchargeSound);
                }


                break;

            case "RightLimbSprite":

                if (rightLimbRailGunHold >= chargeDurationRailGun)
                {
                    CoroutineManager.Instance.StartCoroutine(Shoot(), "Shooting");

                    rightLimbRailGunHold = 0f;
                    SetRailGunLightIntensity(0f, "PlayerGunLightRight"); //set light intensity to 0 to reset
                    GetComponentValues();
                    limbAnimComp.m_frameNumber = (0);
                    Component.Set<AnimationComponent>(EntityID, limbAnimComp);

                    CameraFollowPlayerScript.Shake(0.6f, 1f);

                }


                else if(rightLimbRailGunHold > 0f)
                {
                    rightLimbRailGunHold = 0f;
                    SetRailGunLightIntensity(0f, "PlayerGunLightRight"); //set light intensity to 0 to reset
                    GetComponentValues();
                    limbAnimComp.m_frameNumber = (0);
                    Component.Set<AnimationComponent>(EntityID, limbAnimComp);
                    InternalCall.m_InternalCallPlayAudio(EntityID, railGunUnchargeSound);
                }


                break;

            default:
                return;
        }

    }
    private void AttemptMelee()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                if (leftWeaponCount != 0)
                {
                    leftWeaponCount--;
                    CoroutineManager.Instance.StartCoroutine(Melee(), "Melee");
                }
                else { return; }
                break;

            case "RightLimbSprite":
                if (rightWeaponCount != 0)
                {
                    rightWeaponCount--;
                    CoroutineManager.Instance.StartCoroutine(Melee(), "Melee");
                }
                else { return; }
                break;

            default:
                return;
        }
    }
    private void AttemptBoost()
    {
        switch (limbTag)
        {
            case "BackLimbSprite":
                if (backWeaponcount != 0)
                {
                    backWeaponcount--;
                    CoroutineManager.Instance.StartCoroutine(Boost(), "Boost");
                }
                else { return; }
                break;
        }
    }

    #region Coroutines
    private IEnumerator Melee()
    {
        CoroutineManager.Instance.StartCoroutine(StartWeaponCooldown(1f), "MeleeCooldown");

        if (isAnimating)
        {
            StopAnimation();
            yield return new CoroutineManager.WaitForSeconds(0.001f);
            StartAnimation();
        }

        else 
        {
            StartAnimation();
        }

        switch (limbTag)
        {
            case "LeftLimbSprite":
                uiLeftLimbCounterAnimating = true;

                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = 0;
                uiLeftLimbCounterAC.m_isAnimating = true;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
                break;

            case "RightLimbSprite":
                uiRightLimbCounterAnimating = true;

                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = 0;
                uiRightLimbCounterAC.m_isAnimating = true;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                break;

            default:
                break;
        }
        InternalCall.m_InternalCallPlayAudio(EntityID, cleaverSound);
        yield return new CoroutineManager.WaitForSeconds(0.1f);
        InternalCall.m_InternalCallPlayAudio(EntityID, katanaSound);

        uint killZoneID = (uint)InternalCall.m_InternalCallGetTagID("MeleeKillZoneSpawn");
        ColliderComponent killZoneCollComp = Component.Get<ColliderComponent>(killZoneID);
        killZoneCollComp.m_collisionCheck = true;
        killZoneCollComp.m_drawDebug = true;
        Component.Set<ColliderComponent>(killZoneID, killZoneCollComp);

        yield return new CoroutineManager.WaitForSeconds(0.01f);

        killZoneCollComp.m_drawDebug = false;
        killZoneCollComp.m_collisionCheck = false;
        Component.Set<ColliderComponent>(killZoneID, killZoneCollComp);

    }
    private IEnumerator StartWeaponCooldown(float duration)
    {
        yield return new CoroutineManager.WaitForSeconds(duration);

        switch (limbTag)
        {
            case "LeftLimbSprite":
                leftWeaponCount++;
                break;

            case "RightLimbSprite":
                rightWeaponCount++;
                break;

            case "BackLimbSprite":
                backWeaponcount++;
                break;

            default:
                break;
        }
    }
    private IEnumerator Boost()
    {
        InternalCall.m_InternalCallPlayAudio(EntityID, boosterMovementSound);
        InternalCall.m_InternalCallPlayAudio(EntityID, boosterBoostSound);

        CoroutineManager.Instance.StartCoroutine(StartWeaponCooldown(boostCoolDown), "BoostCooldown");

        if (isAnimating)
        {
            StopAnimation();

            yield return new CoroutineManager.WaitForSeconds(0.001f);
            StartAnimation();
        }

        else
        {
            StartAnimation();

            //create boost light
            LightComponent lc = Component.Get<LightComponent>(EntityID);
            lc.m_intensity = 1f;
            Component.Set(EntityID, lc);

            int frontlightID = InternalCall.m_InternalCallGetTagID("PlayerGunLightBack");

            LightComponent lc2 = Component.Get<LightComponent>((uint)frontlightID);
            lc2.m_intensity = 0.8f;
            Component.Set((uint)frontlightID, lc2);



        }


        switch (limbTag)
        {
            case "BackLimbSprite":

                CoroutineManager.Instance.StartCoroutine(BoosterCooldownUI(), "BoostCooldownUI");

                break;

            default:
                break;
        }

        if (weaponEquipped == 1) // booster
        {
            // take control away in playercontroller.cs
            playerBoost = true;
            CheckAndSetBoost();

            Console.WriteLine("loop");

            yield return new CoroutineManager.WaitForSeconds(boostDuration);

            //reset control
            RigidBodyComponent rigidBodyComponent = Component.Get<RigidBodyComponent>(playerID);
            rigidBodyComponent = Component.Get<RigidBodyComponent>(playerID);
            rigidBodyComponent.m_Acceleration.X = 0;
            rigidBodyComponent.m_Acceleration.Y = 0;
 

            Component.Set<RigidBodyComponent>(playerID, rigidBodyComponent);

            playerBoost = false;

            LightComponent lc = Component.Get<LightComponent>(EntityID);
            lc.m_intensity = 0f;
            Component.Set(EntityID, lc);

            int frontlightID = InternalCall.m_InternalCallGetTagID("PlayerGunLightBack");

            LightComponent lc2 = Component.Get<LightComponent>((uint)frontlightID);
            lc2.m_intensity = 0f;
            Component.Set((uint)frontlightID, lc2);

        }

        //RigidBodyComponent rigidBodyComponent = Component.Get<RigidBodyComponent>(EntityID);
        ////rigidBodyComponent.m_Force.Y = -3f;
        //rigidBodyComponent.m_Acceleration
        //    Component.Set<RigidBodyComponent>(EntityID, rigidBodyComponent);

    }

    private IEnumerator BoosterCooldownUI()
    {
        int temp = 0;

        uiBackLimbCounterAC.m_frameNumber = temp;
        Component.Set<AnimationComponent>(uiBackLimbCounterID, uiBackLimbCounterAC);

        while (temp < uiBackLimbCounterAC.m_stripCount)
        {
            uiBackLimbCounterAC.m_frameNumber = temp;
            temp++;
            Component.Set<AnimationComponent>(uiBackLimbCounterID, uiBackLimbCounterAC);

            yield return new CoroutineManager.WaitForSeconds(boostCoolDown/6);
        }

        yield return null;
    }

    private IEnumerator Shoot()
    {
        //Plays Audio
        switch (weaponEquipped)
        {
            case 0:
                InternalCall.m_InternalCallPlayAudio(EntityID, gunshotSound);
                break;
            case 2:
                InternalCall.m_InternalCallPlayAudio(EntityID, shotGunShotSound);
                break;
            case 3:
                InternalCall.m_InternalCallPlayAudio(EntityID, railGunReleaseSound);
                break;
            default :
                break;

        }

        //Update Component Values, mainly for roomba rotation
        GetComponentValues();

        //only animate gun -> shotgun, except railgun
        switch (weaponEquipped)
        {
            case 0:
            case 1:
            case 2:
                if (isAnimating)
                {
                    StopAnimation();
                    yield return new CoroutineManager.WaitForSeconds(0.001f);
                    StartAnimation();
                }

                else
                {
                    StartAnimation();
                }
                break;
            //Railgun
            case 3:
                break;

            default:
                break;
        }

        switch (limbTag)
        {
            case "LeftLimbSprite":
                //Get limbPos
                InternalCall.m_InternalGetTranslate(leftLimbPosID, out limbPos);
                InternalCall.m_InternalGetTranslate(leftShotgunLimbPosID, out shotgunLimbPos);
                break;

            case "RightLimbSprite":
                //Get limbPos
                InternalCall.m_InternalGetTranslate(rightLimbPosID, out limbPos);
                InternalCall.m_InternalGetTranslate(rightShotgunLimbPosID, out shotgunLimbPos);
                break;

            case "BackLimbSprite":
                //Get limbPos
                InternalCall.m_InternalGetTranslate(backLimbPosID, out limbPos);
                break;

            default:
                break;
        }

        //Pistol
        if(weaponEquipped == 0)
        {
            //Spawn bullet at limb
            InternalCall.m_InternalCallAddPrefab(bulletPrefab, limbPos.X, limbPos.Y, playerTransformComp.m_rotation);

            //Shake Camera
            CameraFollowPlayerScript.Shake(0.3f, 1f);
        }

        //Shotgun
        if(weaponEquipped == 2)
        {
            CoroutineManager.Instance.StartCoroutine(StartWeaponCooldown(0.7f), "GunCooldown");

            int numberofpallets = 4;
            float spread = 45.0f; // in degree

            double interval = Math.Floor(spread / (float)numberofpallets);

            float startangle = playerTransformComp.m_rotation - spread / 2.0f;

            for (int n = 0; n < numberofpallets; n++)
            {
                InternalCall.m_InternalCallAddPrefab(shotgunBulletPrefab, shotgunLimbPos.X, shotgunLimbPos.Y, startangle);
                startangle += (float)interval; 
            }

            CameraFollowPlayerScript.Shake(0.5f, 1f);

            yield return new CoroutineManager.WaitForSeconds(0.5f);

            InternalCall.m_InternalCallPlayAudio(EntityID, cockingSound);

        }

        if(weaponEquipped == 3)
        {
            //Spawn bullet at limb
            InternalCall.m_InternalCallAddPrefab(bulletPrefabType2, shotgunLimbPos.X, shotgunLimbPos.Y, playerTransformComp.m_rotation);
        }



    }
    private IEnumerator PauseAnimation()
    {
        isAnimating = false;
        GetComponentValues();
        limbAnimComp.m_isAnimating = false;
        int tempCurrentFrame = limbAnimComp.m_frameNumber;
        Component.Set<AnimationComponent>(EntityID, limbAnimComp);

        yield return new CoroutineManager.WaitForCondition(() =>
        {
            return !GameControllerLevel1.gameIsPaused;
        });

        //Console.WriteLine("Condition Met " + !GameControllerLevel1.gameIsPaused);

        isAnimating = true;
        limbAnimComp.m_isAnimating = true;
        limbAnimComp.m_frameNumber = tempCurrentFrame;
        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }
    #endregion

    private void CheckAndSetBoost()
    {
        if (playerBoost)
        {
            RigidBodyComponent rigidBodyComponent = Component.Get<RigidBodyComponent>(playerID);
            TransformComponent transformComponent = Component.Get<TransformComponent>(playerID);
            //get boost direction based on rotation
            float degree = transformComponent.m_rotation;
            float radian = degree * ((float)Math.PI / 180f);
            Vector2 direction = new Vector2((float)Math.Sin(radian), (float)Math.Cos(radian));

            rigidBodyComponent.m_Acceleration.X = direction.X * boostAcceleration;
            rigidBodyComponent.m_Acceleration.Y = direction.Y * boostAcceleration;

            Component.Set<RigidBodyComponent>(playerID, rigidBodyComponent);
        }
    }
    private void CheckAmmo()
    {
        int leftWeaponAmmo = 0;
        int rightWeaponAmmo = 0;
        int backWeaponAmmo = 0;

        switch (weaponEquipped)
        {
            case 0:
                leftWeaponAmmo = leftLimbGunAmmo;
                rightWeaponAmmo = rightLimbGunAmmo;
                backWeaponAmmo = backLimbGunAmmo;
                break;

            case 2:
                leftWeaponAmmo = leftLimbShotGunAmmo;
                rightWeaponAmmo = rightLimbShotGunAmmo;
                break;

            case 3:
                break;

            default:
                break;
        }


        switch (limbTag)
        {
            case "LeftLimbSprite":
                //Update UI
                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = leftWeaponAmmo;
                uiLeftLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
                break;

            case "RightLimbSprite":
                //Update UI
                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = rightWeaponAmmo;
                uiRightLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                break;

            case "BackLimbSprite":
                //Update UI
                uiBackLimbCounterAC = Component.Get<AnimationComponent>(uiBackLimbCounterID);
                uiBackLimbCounterAC.m_frameNumber = backWeaponAmmo;
                uiBackLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiBackLimbCounterID, uiBackLimbCounterAC);
                break;

            default:
                break;
        }
    }
    private void CheckMeleeCounter()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                //Update UI
                if (leftWeaponCount == 1)
                {
                    uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                    uiLeftLimbCounterAC.m_frameNumber = 6;
                    uiLeftLimbCounterAC.m_isAnimating = false;

                    Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
                }

                break;

            case "RightLimbSprite":
                //Update UI
                if (rightWeaponCount == 1)
                {
                    uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                    uiRightLimbCounterAC.m_frameNumber = 6;
                    uiRightLimbCounterAC.m_isAnimating = false;

                    Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                }

                break;
            case "BackLimbSprite":
                if (backWeaponcount == 1)
                {
                    uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                    uiRightLimbCounterAC.m_frameNumber = 6;
                    uiRightLimbCounterAC.m_isAnimating = false;

                    Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                }
                break;
            default:
                break;
        }
    }
    private void CheckRailgunCounter()
    {
        int strip = GetStripCount("ani_hudFullBar_strip7.png");

        switch (limbTag)
        {
            case "LeftLimbSprite":
                int tempLeft = (int)Math.Floor((leftLimbRailGunHold / chargeDurationRailGun) * (strip - 1));

                if (tempLeft >= strip) tempLeft = strip - 1;

                uiLeftLimbCounterAC = Component.Get<AnimationComponent>(uiLeftLimbCounterID);
                uiLeftLimbCounterAC.m_frameNumber = tempLeft;
                uiLeftLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiLeftLimbCounterID, uiLeftLimbCounterAC);
                break;

            case "RightLimbSprite":
                int tempRight = (int)Math.Floor(rightLimbRailGunHold / (chargeDurationRailGun) * (strip - 1));

                if (tempRight >= strip)
                {
                    tempRight = strip - 1;
                }


                uiRightLimbCounterAC = Component.Get<AnimationComponent>(uiRightLimbCounterID);
                uiRightLimbCounterAC.m_frameNumber = tempRight;
                uiRightLimbCounterAC.m_isAnimating = false;

                Component.Set<AnimationComponent>(uiRightLimbCounterID, uiRightLimbCounterAC);
                break;
        }

    }
    private void CheckBoosterCounter()
    {

    }

    private string GetWeaponSprite(string weaponName)
    {
        string limbTag = InternalCall.m_InternalCallGetTag(EntityID);

        switch (limbTag)
        {
            case "LeftLimbSprite":
                return this.GetType().GetField("left" + weaponName + "Texture",
                                               System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance)
                                 ?.GetValue(this) as string;

            case "RightLimbSprite":
                return this.GetType().GetField("right" + weaponName + "Texture",
                                               System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance)
                                 ?.GetValue(this) as string;

            case "BackLimbSprite":
                return this.GetType().GetField("back" + weaponName + "Texture",
                                               System.Reflection.BindingFlags.NonPublic | System.Reflection.BindingFlags.Instance)
                                 ?.GetValue(this) as string;

            default:
                return null;
        }
    }
    private int GetStripCount(string fileName)
    {
        // Find the last non-digit position before the number
        string numberStr = "";

        // Iterate through the string to find the last sequence of digits
        for (int i = fileName.Length - 1; i >= 0; i--)
        {
            if (char.IsDigit(fileName[i]))
            {
                numberStr = fileName[i] + numberStr; // Prepend to keep the correct order
            }
            else if (numberStr.Length > 0)
            {
                break; // Stop when we encounter a non-digit after collecting digits
            }
        }

        // Convert to integer
        return int.TryParse(numberStr, out int result) ? result : -1;
    }
    private void GetComponentValues()
    {
        limbSpriteComp = GetComponent.GetSpriteComponent(EntityID);
        limbAnimComp = Component.Get<AnimationComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>(playerID);
    }
    private void SwitchWeapon(string weaponName)
    {
        string fileName = GetWeaponSprite(weaponName);

        limbSpriteComp.m_imageFile = fileName;
        SetComponent.SetSpriteComponent(EntityID, limbSpriteComp);

        limbAnimComp.m_frameNumber = 0;
        limbAnimComp.m_isAnimating = false;
        limbAnimComp.m_stripCount = GetStripCount(fileName);
        Component.Set<AnimationComponent>(EntityID, limbAnimComp);

        //Reset Railgun Values
        AttemptShootRailGunRelease();
    }
    private void StartAnimation()
    {
        isAnimating = true;

        GetComponentValues();
        limbAnimComp.m_frameNumber = 0;
        limbAnimComp.m_isAnimating = isAnimating;

        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }
    private void StopAnimation()
    {
        isAnimating = false;

        GetComponentValues();
        limbAnimComp.m_frameNumber = 0;
        limbAnimComp.m_isAnimating = isAnimating;

        Component.Set<AnimationComponent>(EntityID, limbAnimComp);
    }
    private void CheckWeapons()
    {
        switch (limbTag)
        {
            case "LeftLimbSprite":
                weaponEquipped = PlayerLoadoutManager.leftLimbEquippedNo;
                break;

            case "RightLimbSprite":
                weaponEquipped = PlayerLoadoutManager.rightLimbEquippedNo;
                break;

            case "BackLimbSprite":
                weaponEquipped = PlayerLoadoutManager.backLimbEquippedNo;
                break;

            default:
                break;
        }

        ChangeWeaponSprite();
    }
    private void ChangeWeaponSprite()
    {
        if (limbTag == "LeftLimbSprite" || limbTag == "RightLimbSprite")
        {
            switch (weaponEquipped)
            {
                case 0:
                    SwitchWeapon("Gun");
                    break;

                case 1:
                    SwitchWeapon("Katana");
                    break;

                case 2:
                    SwitchWeapon("ShotGun");
                    break;

                case 3:
                    SwitchWeapon("RailGun");
                    break;

                default:
                    break;
            }
        }
        
        else if (limbTag == "BackLimbSprite")
        {
            switch (weaponEquipped)
            {
                case 0:
                    SwitchWeapon("Gun");
                    break;

                case 1:
                    SwitchWeapon("Booster");
                    break;

                default:
                    break;
            }
        }
    }
    private void CheckInputs()
    {
        #region Checks

        if ((InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_LEFT_TRIGGER))&& limbTag == "LeftLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 0:
                    AttemptShootGun();
                    break;

                case 1:
                    AttemptMelee();
                    break;
                case 2:
                    AttemptShootShotGun();
                    break;

                default:
                    break;
            }
        }

        if ((InternalCall.m_InternalCallIsKeyTriggered(keyCode.RMB) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_RIGHT_TRIGGER)) && limbTag == "RightLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 0:
                    AttemptShootGun();
                    break;

                case 1:
                    AttemptMelee();
                    break;
                case 2:
                    AttemptShootShotGun();
                    break;

                default:
                    break;
            }
        }

        //Railgun
        if ((InternalCall.m_InternalCallIsKeyPressed(keyCode.LMB) || InternalCall.m_InternalCallIsControllerPress(keyCode.CONTROLLER_LEFT_TRIGGER)) && limbTag == "LeftLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 3:
                    AttemptShootRailGunHold();
                    break;

                default:
                    break;
            }
        }

        if ((InternalCall.m_InternalCallIsKeyPressed(keyCode.RMB) || InternalCall.m_InternalCallIsControllerPress(keyCode.CONTROLLER_RIGHT_TRIGGER)) && limbTag == "RightLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 3:
                    AttemptShootRailGunHold();
                    break;

                default:
                    break;
            }
        }

        if ((InternalCall.m_InternalCallIsKeyReleased(keyCode.LMB) || InternalCall.m_InternalCallIsControllerReleased(keyCode.CONTROLLER_LEFT_TRIGGER)) && limbTag == "LeftLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 3:
                    AttemptShootRailGunRelease();
                    break;

                default:
                    break;
            }
        }

        if ((InternalCall.m_InternalCallIsKeyReleased(keyCode.RMB) || InternalCall.m_InternalCallIsControllerReleased(keyCode.CONTROLLER_RIGHT_TRIGGER)) && limbTag == "RightLimbSprite" && PlayerLoadoutManager.isSortieing == false)
        {
            switch (weaponEquipped)
            {
                case 3:
                    AttemptShootRailGunRelease();
                    break;

                default:
                    break;
            }
        }

        //Back limb
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.SPACE) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.MMB) || InternalCall.m_InternalCallIsKeyTriggered(keyCode.LeftShift) || InternalCall.m_InternalCallIsControllerTriggered(keyCode.CONTROLLER_RBUMP))
        {
            if (limbTag == "BackLimbSprite" && PlayerLoadoutManager.isSortieing == false)
            {
                switch (weaponEquipped)
                {
                    case 0:
                        AttemptShootGun();
                        break;

                    case 1:
                        AttemptBoost();
                        break;

                    default:
                        break;
                }
            }
        }

        #endregion
    }


}