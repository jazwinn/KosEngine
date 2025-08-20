using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading;
using System.Threading.Tasks;
using System.Xml;

public class testscript : ScriptBase
{

    private uint EntityID;

    public int test;

    public float test2;

    public bool test3;

    public string test4;

    public override void Awake(uint id)
    {
        EntityID = id;

    }


    public override void Start()
    {
        //TextComponent cc = Component.Get<TextComponent>(EntityID);
        //Component.Set<TextComponent>(EntityID, cc);

        //unsafe
        //{
        //    Console.WriteLine("testt");
        //    int* ptr = null;
        //    *ptr = 10;  // This will cause a crash due to dereferencing a null pointer
           
        //}
    }

    public override void Update()
    {
       
    }

    public void LateUpdate()
    {

    }

}