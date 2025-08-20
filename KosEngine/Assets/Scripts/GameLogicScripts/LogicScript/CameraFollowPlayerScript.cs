using System;
using System.Collections.Generic;
using System.Linq;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class CameraFollowPlayerScript : ScriptBase
{
    private uint EntityID;
    private uint playerID;

    private Vector2 previousPos;

    //Tracks point in between player and mousePos
    private Vector2 trackingPos;
    //Mouse position
    private Vector2 mousePos;
    //Player transform component
    private TransformComponent playerTransformComp;
    //Camera transform component
    private TransformComponent camTransformComp;
    //Tracking scale, multiplier for distance for trackingPos
    private float trackScale;
    //Tracking speed, how fast the camera moves to the new position
    private float trackSpeed;

    private Vector2 shakeOffset = new Vector2(0, 0);
    public static float shakeIntensity = 0f;
    public static float shakeDuration = 0f;


    public override void Awake(uint id)
    {
        EntityID = id;
        playerID = (uint)InternalCall.m_InternalCallGetTagID("Player");
        playerTransformComp = GetComponent.GetTransformComponent(playerID);
        camTransformComp = GetComponent.GetTransformComponent(EntityID);

        trackScale = 0.2f;
        trackSpeed = 30f;
    }

    public override void Start()
    {
        UpdateMousePosition();
        trackingPos = Lerp(playerTransformComp.m_position, mousePos, trackScale);
        previousPos = playerTransformComp.m_position;
    }

    public override void Update()
    {
    }

    public void LateUpdate()
    {
        if (shakeDuration > 0)
        {
            float angle = SimpleRandom(0, 360) * (3.14159f / 180f); // Convert to radians
            float offsetX = (float)Math.Cos(angle) * shakeIntensity;
            float offsetY = (float)Math.Sin(angle) * shakeIntensity;

            shakeOffset = new Vector2(offsetX, offsetY);

            // Reduce shake intensity over time (damping effect)
            shakeDuration -= InternalCall.m_InternalCallGetDeltaTime();
            shakeIntensity *= 0.9f;
        }
        else
        {
            shakeOffset = new Vector2(0, 0);
        }

        if (PlayerLoadoutManager.isSortieing || GameControllerLevel1.gameIsPaused || PlayerController.isDead == true || PlayerController.isControllerLastUsed)
        {
            UpdatePlayerPosition();
            trackingPos = playerTransformComp.m_position;
            
        }

        else
        {
            
            trackingPos = GetTrackingPos();
        }
        

        // Final camera position = tracked position + shake offset
        trackingPos = new Vector2(trackingPos.X + shakeOffset.X, trackingPos.Y + shakeOffset.Y);

        if (float.IsNaN(trackingPos.X) || float.IsNaN(trackingPos.Y) || float.IsNaN(camTransformComp.m_position.X) || float.IsNaN(camTransformComp.m_position.Y) ||
            float.IsInfinity(trackingPos.X) || float.IsInfinity(trackingPos.Y) || float.IsInfinity(camTransformComp.m_position.X) || float.IsInfinity(camTransformComp.m_position.Y))
        {
            trackingPos = previousPos;
            camTransformComp.m_position = previousPos;
        }
        else
        {
            //set previous as current
            previousPos = trackingPos;
            //Console.WriteLine(trackingPos.X);
        }

        camTransformComp.m_position = MoveTowards(camTransformComp.m_position, trackingPos, trackSpeed * InternalCall.m_InternalCallGetDeltaTime());
        Component.Set<TransformComponent>(EntityID, camTransformComp);



    }

    public static void Shake(float intensity, float duration)
    {
        shakeIntensity = intensity * 0.6f;
        shakeDuration = duration;
    }

    Vector2 GetTrackingPos()
    {
        UpdateMousePosition();
        UpdatePlayerPosition();

        return Lerp(playerTransformComp.m_position, mousePos, trackScale);
    }

    void UpdateMousePosition()
    {
        InternalCall.m_InternalGetWorldMousePosition(out mousePos);

    }

    void UpdatePlayerPosition()
    {
        playerTransformComp = GetComponent.GetTransformComponent(playerID);
    }

    void UpdateCameraPosition()
    {
        camTransformComp = GetComponent.GetTransformComponent(EntityID);
    }

    public Vector2 Lerp(Vector2 a, Vector2 b, float t)
    {
        t = Clamp(t, 0f, 1f); // Ensures t stays within [0,1]
        return new Vector2(
            a.X + (b.X - a.X) * t,
            a.Y + (b.Y - a.Y) * t
        );
    }

    public float Clamp(float value, float min, float max)
    {
        if (value < min) return min;
        if (value > max) return max;
        return value;
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

    public float SimpleRandom(float min, float max)
    {
        // Using a simple time-based random seed (not truly random but works for effects)
        int seed = (int)(DateTime.Now.Ticks % int.MaxValue);
        seed = (seed * 1103515245 + 12345) & 0x7FFFFFFF; // LCG (Linear Congruential Generator)
        float normalized = seed / (float)int.MaxValue; // Convert to range 0-1
        return min + (max - min) * normalized;
    }

}