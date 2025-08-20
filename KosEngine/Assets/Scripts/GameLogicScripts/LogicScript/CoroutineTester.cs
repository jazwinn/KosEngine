// Final CoroutineManagerTester.cs 
using System;
using System.Collections;

public class CoroutineTester : ScriptBase
{
    private uint EntityID;
    private static bool tempBool;

    public override void Awake(uint id)
    {
        EntityID = id;
    }

    public override void Start()
    {
        tempBool = false;

        // Test basic coroutine 
        CoroutineManager.Instance.StartCoroutine(SimpleCoroutine(), "BasicTest");

        // Test WaitForCondition with delayed activation 
        CoroutineManager.Instance.StartCoroutine(ConditionCoroutine(), "ConditionTest");
    }

    public override void Update()
    {
    }

    private IEnumerator SimpleCoroutine()
    {
        //Console.WriteLine("SimpleCoroutine Start");
        yield return new CoroutineManager.WaitForSeconds(4);
        //Console.WriteLine("SimpleCoroutine After 4 seconds");
    }

    private IEnumerator ConditionCoroutine()
    {
        //Console.WriteLine("ConditionCoroutine Waiting for condition...");
        yield return new CoroutineManager.WaitForCondition(() =>
        {
            return tempBool;
        });
        //Console.WriteLine("ConditionCoroutine Condition met!");
    }

    private bool CheckCondition()
    {
        return tempBool;
    }
}