using System;
using System.Runtime.CompilerServices;

public class Prop_GlassWall : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    //Collision
    private int[] collidedEntities;

    private AnimationComponent animComp;
    private ColliderComponent colComp;
    private ColliderComponent rCWallColComp;
    private uint rCWallID;
    private bool isAnimating;
    private bool isBroken;

    public override void Start()
    {
        isBroken = false;
        isAnimating = false;

        rCWallID = (uint)InternalCall.m_InternalCallGetTagID("PropGlassWall");
        rCWallColComp = Component.Get<ColliderComponent>(rCWallID);
        animComp = Component.Get<AnimationComponent>(EntityID);
        colComp = Component.Get<ColliderComponent>(EntityID);
    }

    public override void Update()
    {
        if (isBroken) { return; }

        if (isAnimating)
        {
            animComp = Component.Get<AnimationComponent>(EntityID);

            if (animComp.m_frameNumber == animComp.m_stripCount - 1)
            {
                isAnimating = false;
                animComp = Component.Get<AnimationComponent>(EntityID);
                animComp.m_frameNumber = 3;
                animComp.m_isAnimating = false;

                Component.Set<AnimationComponent>(EntityID, animComp);

                isBroken = true;
            }
        }

        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                    if (!isAnimating && !isBroken)
                    {
                        InternalCall.m_InternalCallPlayAudio(EntityID, "aud_glassBreak01");

                        isAnimating = true;

                        //InternalCall.m_InternalCallDeleteEntity(rCWallID);

                        animComp = Component.Get<AnimationComponent>(EntityID);
                        animComp.m_frameNumber = 0;
                        animComp.m_isAnimating = isAnimating;

                        Component.Set<AnimationComponent>(EntityID, animComp);
                    }
                
            }
        }


    }
}