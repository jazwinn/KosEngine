using System;
using System.Data.SqlClient;

public class LazerForwardBackward : ScriptBase
{
    #region Entity ID
    private uint EntityID;

    public bool startOpposite;

    const int HORIZONTAL = 0;
    const int VERTICAL = 1;

    private int movement;

    private int Waypoint1;
    private int Waypoint2;

    private Vector2 wayPoint1_tc;
    private Vector2 wayPoint2_tc;

    private float speed;
    private RigidBodyComponent lazerRigidBody;

    private waypoint ToporLeft;
    private waypoint BottomorRight;

    private int lastwaypointhit;

    private struct waypoint
    {
        public float coordinate;
        public int waypointID;
    }

    public override void Awake(uint id)
    {
        EntityID = id;

        Waypoint1 = -1;
        Waypoint2 = -1;
        lastwaypointhit = -1;

        int[] waypoints = InternalCall.m_InternalCallGetChildrenID(EntityID);

        if (waypoints.Length != 2)
        {
            return;
        }

        Waypoint1 = waypoints[0];
        Waypoint2 = waypoints[1];

        InternalCall.m_InternalGetTranslate((uint)Waypoint1, out wayPoint1_tc);
        InternalCall.m_InternalGetTranslate((uint)Waypoint2, out wayPoint2_tc);

        if(Math.Abs(wayPoint1_tc.X - wayPoint2_tc.X) > Math.Abs(wayPoint1_tc.Y - wayPoint2_tc.Y))
        {
            movement = HORIZONTAL;
        }
        else
        {
            movement = VERTICAL;
        }

        
        
        speed = 5f;
        if (startOpposite) speed *= -1;
    }
    #endregion



    public override void Start()
    {
        // see which waypoint is the top
        if (movement == HORIZONTAL)
        {
            if (wayPoint1_tc.X > wayPoint2_tc.X)
            {
                BottomorRight.coordinate = wayPoint1_tc.X;
                BottomorRight.waypointID = Waypoint1;

                ToporLeft.coordinate = wayPoint2_tc.X;
                ToporLeft.waypointID = Waypoint2;
            }
            else
            {
                BottomorRight.coordinate = wayPoint2_tc.X;
                BottomorRight.waypointID = Waypoint2;

                ToporLeft.coordinate = wayPoint1_tc.X;
                ToporLeft.waypointID = Waypoint1;
            }
        }
        if (movement == VERTICAL)
        {
            if (wayPoint1_tc.Y > wayPoint2_tc.Y)
            {
                ToporLeft.coordinate = wayPoint1_tc.Y;
                ToporLeft.waypointID = Waypoint1;

                BottomorRight.coordinate = wayPoint2_tc.Y;
                BottomorRight.waypointID= Waypoint2;
            }
            else
            {
                ToporLeft.coordinate = wayPoint2_tc.Y;
                ToporLeft.waypointID = Waypoint2;

                BottomorRight.coordinate = wayPoint1_tc.Y;
                BottomorRight.waypointID = Waypoint1;
            }
        }
        lazerRigidBody = Component.Get<RigidBodyComponent>(EntityID);
        if (movement == VERTICAL)
        {
            lazerRigidBody.m_Velocity.Y = speed;
        }
        else
        {
            lazerRigidBody.m_Velocity.X = speed;
        }
        Component.Set<RigidBodyComponent>(EntityID, lazerRigidBody);
    }

    public override void Update()
    {
        if(Waypoint1 < 0 || Waypoint2 < 0)
        {
            //Console.WriteLine("No Waypoints, add 2 waypoint in child");
            return;
        }


        lazerRigidBody = Component.Get<RigidBodyComponent>(EntityID);
        Vector2 currentTranslate;
        InternalCall.m_InternalGetTranslate(EntityID, out currentTranslate);

        if (movement == VERTICAL)
        {
            // verticle

            if (currentTranslate.Y > ToporLeft.coordinate && (lastwaypointhit != ToporLeft.waypointID))
            {
                lazerRigidBody.m_Velocity.Y *= -1;
                lastwaypointhit = ToporLeft.waypointID;
            }
            if (currentTranslate.Y < BottomorRight.coordinate && (lastwaypointhit != BottomorRight.waypointID))
            {
                lazerRigidBody.m_Velocity.Y *= -1;
                lastwaypointhit = BottomorRight.waypointID;
            }
        }
        else
        {
            //horizontal
            if(currentTranslate.X > BottomorRight.coordinate && (lastwaypointhit != BottomorRight.waypointID))
            {
                lazerRigidBody.m_Velocity.X *= -1;
                lastwaypointhit = BottomorRight.waypointID;
            }
            if (currentTranslate.X < ToporLeft.coordinate && (lastwaypointhit != ToporLeft.waypointID))
            {
                lazerRigidBody.m_Velocity.X *= -1;
                lastwaypointhit = ToporLeft.waypointID;
            }



        }




        Component.Set<RigidBodyComponent>(EntityID, lazerRigidBody);


    }
}