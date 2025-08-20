using System;
using System.Collections;
using System.Collections.Generic;

public class StartMenuEasterEgg : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    public float jumpHeight = 0.3f; // Adjust height as needed
    public float jumpDuration = 0.2f; // Total time for jump
    private Vector2 originalY;
    private bool isJumping;
    private TransformComponent transformComp;
    public static bool easterEggEnabled;
    private int easterEggCounter;

    public override void Start()
    {
        easterEggCounter = 0;
        easterEggEnabled = false;
        isJumping = false;
        transformComp = Component.Get<TransformComponent>(EntityID);
        originalY = transformComp.m_position;
    }

    public override void Update()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.LMB))
        {
            InternalCall.m_InternalGetButtonComponent(EntityID, out Vector2 position, out Vector2 scale, out bool isClick);
            if (isClick)
            {
                if (isJumping)
                {
                    CoroutineManager.Instance.StopCoroutinesByTag("EasterEgg");
                    transformComp = Component.Get<TransformComponent>(EntityID);
                    transformComp.m_position = originalY;
                    Component.Set<TransformComponent>(EntityID, transformComp);
                }

                isJumping = true;
                CoroutineManager.Instance.StartCoroutine(Jump(), "EasterEgg");
            }
            
        }
    }

    private IEnumerator Jump()
    {
        easterEggCounter++;
        if(easterEggCounter >= 10)
        {
            easterEggEnabled = true;
            SpriteComponent temp = Component.Get<SpriteComponent>((uint)InternalCall.m_InternalCallGetTagID("StartMenuButton"));
            temp.m_color.R = 1.0f;
            temp.m_color.G = 0.0f;
            temp.m_color.B = 0.0f;
            Component.Set<SpriteComponent>((uint)InternalCall.m_InternalCallGetTagID("StartMenuButton"), temp);
        }

        float elapsedTime = 0f;
        while (elapsedTime < jumpDuration)
        {
            //Console.WriteLine(elapsedTime + " " + InternalCall.m_InternalCallGetDeltaTime());
            elapsedTime += InternalCall.m_InternalCallGetDeltaTime();
            float progress = elapsedTime / jumpDuration;
            float smoothStep = (float)(Math.Sin(progress * Math.PI * 0.5f)); // Easing function
            transformComp = Component.Get<TransformComponent>(EntityID);
            transformComp.m_position = new Vector2(transformComp.m_position.X, originalY.Y + jumpHeight * smoothStep);
            Component.Set<TransformComponent>(EntityID, transformComp);
            yield return null;
        }

        // Falling back down
        elapsedTime = 0f;
        while (elapsedTime < jumpDuration)
        {
            elapsedTime += InternalCall.m_InternalCallGetDeltaTime();
            float progress = elapsedTime / jumpDuration;
            float smoothStep = (float)(Math.Sin(progress * Math.PI * 0.5f));
            transformComp = Component.Get<TransformComponent>(EntityID);
            transformComp.m_position = new Vector2(transformComp.m_position.X, originalY.Y + jumpHeight * (1 - smoothStep));
            Component.Set<TransformComponent>(EntityID, transformComp);
            yield return null;
        }

        transformComp = Component.Get<TransformComponent>(EntityID);
        transformComp.m_position = originalY;
        Component.Set<TransformComponent>(EntityID, transformComp);

        isJumping = false;

    }
}