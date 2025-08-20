using System;
using System.Collections.Generic;
using System.Linq;
using System.Security.Policy;
using System.Text;
using System.Threading.Tasks;
using System.Xml;

public class MainMenuTransition : ScriptBase
{
    private uint EntityID;

    //Animation Variables
    private int frameNumber;
    private int framesPerSecond;
    private float frameTimer;
    private bool isAnimating;
    private int stripCount;

    // public bool animPlay = false;
    public override void Awake(uint id)
    {
        EntityID = id;
        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);
    }


    public override void Start()
    {
        InternalCall.m_InternalSetAnimationComponent(EntityID, frameNumber, 10, 1, true, stripCount);
    }

    public override void Update()
    {
        InternalCall.m_InternalGetAnimationComponent(EntityID, out frameNumber, out framesPerSecond, out frameTimer, out isAnimating, out stripCount);

        if (frameNumber == 4)
        {
            InternalCall.m_UnloadAllScene();

            if (StartMenuEasterEgg.easterEggEnabled)
            {
                InternalCall.m_InternalCallLoadScene("LevelChallenge");
            }
            else
            {
                InternalCall.m_InternalCallLoadScene("LevelSelect");
            }
            
        }
    }
}
