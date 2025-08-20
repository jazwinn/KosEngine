using System;

public class LightFlashing : ScriptBase
{
    public float interval = 1.0f; // Time between full pulses
    public float fadeSpeed = 0.5f; // How quickly the light fades
    public float maxIntensity = 0.5f; // Maximum light intensity

    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    private float timeSinceLastPulse = 0;
    private float currentIntensity;

    public override void Start()
    {
        // Initialize with full intensity
        currentIntensity = maxIntensity;
        UpdateLightIntensity();
    }

    public override void Update()
    {
        float deltaTime = InternalCall.m_GetUnfixedDeltaTime();

        // Count time until next pulse
        timeSinceLastPulse += deltaTime;

        // Fade the light over time
        currentIntensity -= fadeSpeed * deltaTime;

        // If it's time for a new pulse or intensity went below 0
        if (timeSinceLastPulse >= interval || currentIntensity <= 0)
        {
            timeSinceLastPulse = 0;
            currentIntensity = maxIntensity;
        }

        // Clamp intensity to valid range (0 to maxIntensity)
        currentIntensity = Math.Max(0, Math.Min(maxIntensity, currentIntensity));

        UpdateLightIntensity();
    }

    private void UpdateLightIntensity()
    {
        LightComponent lc = Component.Get<LightComponent>(EntityID);
        lc.m_intensity = currentIntensity;
        Component.Set<LightComponent>(EntityID, lc);
    }
}