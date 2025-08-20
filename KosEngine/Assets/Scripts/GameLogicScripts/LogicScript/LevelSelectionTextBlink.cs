using System;
using System.Collections;

public class LevelSelectionTextBlink : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    private TextComponent textComp;

    private bool isBlinking;

    public override void Start()
    {
        isBlinking = false;
        textComp = Component.Get<TextComponent>(EntityID);
    }

    public override void Update()
    {
        if (LevelSelection.LevelSelected == true && isBlinking == false)
        {
            isBlinking = true;
            CoroutineManager.Instance.StartCoroutine(BlinkText(), "BlinkText");
        }

        else if (LevelSelection.LevelSelected == false)
        {
            isBlinking = false;

            CoroutineManager.Instance.StopCoroutinesByTag("BlinkText");
            textComp.m_fontLayer = -1;
            Component.Set<TextComponent>(EntityID, textComp);
        }

    }

    private IEnumerator BlinkText()
    {
        while (LevelSelection.LevelSelected == true)
        {
            float bpm = 120f;
            float beatDuration = 60f / bpm; // One beat in seconds (0.5s for 120 BPM)
            float nextBlinkTime = CoroutineManager.Instance.GetTime();

            while (LevelSelection.LevelSelected == true)
            {
                textComp.m_fontLayer = 10;
                Component.Set<TextComponent>(EntityID, textComp);

                nextBlinkTime += beatDuration;
                yield return new CoroutineManager.WaitForSeconds(nextBlinkTime - CoroutineManager.Instance.GetTime());

                textComp.m_fontLayer = -1;
                Component.Set<TextComponent>(EntityID, textComp);

                nextBlinkTime += beatDuration;
                yield return new CoroutineManager.WaitForSeconds(nextBlinkTime - CoroutineManager.Instance.GetTime());

                
            }
        }

    }
}