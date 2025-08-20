using System;

public class EnemyCorpseHalf : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public override void Awake(uint id)
    {
        EntityID = id;
    }
    #endregion

    TransformComponent transformComp;
    TransformComponent playerTransformComp;

    public override void Start()
    {
        Random random = new Random();

        Vector2 movement;

        Vector2 direction;

        transformComp = Component.Get<TransformComponent>(EntityID);
        playerTransformComp = Component.Get<TransformComponent>((uint)InternalCall.m_InternalCallGetTagID("Player"));

        direction.X = (transformComp.m_position.X - playerTransformComp.m_position.X); //Gets Vector.X away from player
        direction.Y = (transformComp.m_position.Y - playerTransformComp.m_position.Y); //Gets Vector.Y away from player

        float rotationFloat = (float)(Math.Atan2(direction.X, direction.Y) * (180 / Math.PI)); //Gets rotation away player

        float rotationRandom = (float)(random.NextDouble() * 30 + 10);

        if (random.Next(0, 2) == 0)
        {
            rotationRandom = -rotationRandom; // Make it negative 30 to -75
        }

        rotationFloat += rotationRandom;

        //Convert into radians
        float rotationInRadians = (float)((rotationFloat) * Math.PI / 180.0);


        transformComp.m_rotation = rotationFloat;

        Component.Set<TransformComponent>(EntityID, transformComp);

        //Get forward vector X
        float forwardX = (float)(Math.Sin(rotationInRadians));

        //Get forward vector Y
        float forwardY = (float)(Math.Cos(rotationInRadians));

        movement.X = forwardX * 0.8f;
        movement.Y = forwardY * 0.8f;

        InternalCall.m_InternalSetVelocity(EntityID, in movement);
    }

    public override void Update()
    {

    }
}