// Final CoroutineManager.cs
using System;
using System.Collections;
using System.Collections.Generic;

public class CoroutineManager : ScriptBase
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

    // Singleton instance
    private static CoroutineManager instance;
    public static CoroutineManager Instance
    {
        get
        {
            if (instance == null)
            {
                instance = new CoroutineManager();
            }
            return instance;
        }
    }

    private CoroutineManager() { }

    // Internal coroutine representation
    private class Coroutine
    {
        public IEnumerator Routine { get; }
        public float NextExecutionTime { get; set; }
        public bool Paused { get; set; }
        public string Tag { get; }
        public float PausedTime { get; set; } //Stores time when paused

        public Coroutine(IEnumerator routine, float startTime, string tag)
        {
            Routine = routine;
            NextExecutionTime = startTime;
            Paused = false;
            Tag = tag;
            PausedTime = 0;
        }
    }

    private List<Coroutine> activeCoroutines = new List<Coroutine>();

    // Starts a coroutine
    public void StartCoroutine(IEnumerator routine, string tag = null)
    {
        activeCoroutines.Add(new Coroutine(routine, GetTime(), tag));
        //Log($"Starting coroutine with tag: {tag ?? "None"}");
    }

    // Stops a specific coroutine
    public void StopCoroutine(IEnumerator routine)
    {
        activeCoroutines.RemoveAll(c => c.Routine == routine);
        //Log($"Stopping coroutine: {routine}");
    }

    // Stops all coroutines
    public void StopAllCoroutines()
    {
        activeCoroutines.Clear();
        //Log("Stopping all coroutines.");
    }

    // Stops coroutines by tag
    public void StopCoroutinesByTag(string tag)
    {
        activeCoroutines.RemoveAll(c => c.Tag == tag);
        //Log($"Stopping coroutines with tag: {tag}");
    }

    // Pauses all coroutines
    public void PauseAllCoroutines()
    {
        float currentTime = GetTime();
        foreach (var coroutine in activeCoroutines)
        {
            coroutine.Paused = true;
            coroutine.PausedTime = coroutine.NextExecutionTime - currentTime; // Store remaining time
            //Log($"Paused coroutine with tag '{coroutine.Tag}', remaining time: {coroutine.PausedTime}");
        }
        //Log("Pausing all coroutines.");
    }

    // Resumes all coroutines
    public void ResumeAllCoroutines()
    {
        float currentTime = GetTime();
        foreach (var coroutine in activeCoroutines)
        {
            coroutine.Paused = false;
            coroutine.NextExecutionTime = currentTime + coroutine.PausedTime; // Restore timing
            //Log($"Resumed coroutine with tag '{coroutine.Tag}', next execution time: {coroutine.NextExecutionTime}");
        }
        //Log("Resuming all coroutines.");
    }

    // Updates all active coroutines
    public override void Update()
    {
        float currentTime = GetTime();
        for (int i = activeCoroutines.Count - 1; i >= 0; i--)
        {
            Coroutine coroutine = activeCoroutines[i];
            if (!coroutine.Paused && currentTime >= coroutine.NextExecutionTime)
            {
                try
                {
                    //Log($"Processing coroutine with tag '{coroutine.Tag}'. CurrentTime: {currentTime}");

                    if (coroutine.Routine.Current is WaitForCondition condition)
                    {
                        if (condition.Condition())
                        {
                            coroutine.NextExecutionTime = currentTime;
                            //Log($"Condition evaluated as TRUE for coroutine with tag '{coroutine.Tag}'.");
                        }
                        else
                        {
                            coroutine.NextExecutionTime = currentTime + 0.1f;
                            //Log($"Condition evaluated as FALSE for coroutine with tag '{coroutine.Tag}'. Retrying... NextExecutionTime: {coroutine.NextExecutionTime}");
                            continue; // Skip MoveNext to prevent progression
                        }
                    }

                    if (!coroutine.Routine.MoveNext())
                    {
                        // Coroutine finished
                        activeCoroutines.RemoveAt(i);
                        //Log($"Coroutine with tag '{coroutine.Tag}' finished.");
                    }
                    else
                    {
                        // Handle other yield instructions
                        if (coroutine.Routine.Current is WaitForSeconds wait)
                        {
                            coroutine.NextExecutionTime = currentTime + wait.Seconds;
                            //Log($"Coroutine with tag '{coroutine.Tag}' waiting for {wait.Seconds} seconds.");
                        }
                        else
                        {
                            coroutine.NextExecutionTime = currentTime;
                            //Log($"Coroutine with tag '{coroutine.Tag}' proceeding without wait.");
                        }
                    }
                }
                catch (Exception ex)
                {
                    //Log($"Error in coroutine with tag '{coroutine.Tag}': {ex.Message}");
                    activeCoroutines.RemoveAt(i);
                }
            }
        }
    }

    // Time source
    public float GetTime()
    {
        return (float)InternalCall.m_InternalCallGetGameTime(); // Replace with engine's time API
    }
   
    // Debugging
    private bool EnableDebugLogging { get; set; } = true;
    private void Log(string message)
    {
        if (EnableDebugLogging) Console.WriteLine($"[CoroutineManager] {message}");
    }

    // Yield instructions
    public class WaitForSeconds
    {
        public float Seconds { get; }
        public WaitForSeconds(float seconds)
        {
            Seconds = seconds;
        }
    }

    public class WaitForCondition
    {
        public Func<bool> Condition { get; }
        public WaitForCondition(Func<bool> condition)
        {
            Condition = condition;
        }
    }
}