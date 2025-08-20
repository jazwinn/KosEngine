using System;
using System.Collections;
using System.Collections.Generic;

public abstract class EnemyState
{
    #region Default Functions
    #endregion

    protected EnemyScript enemyScript; //References to EnemyScript
    public EnemyState(EnemyScript enemyScript)
    {
        this.enemyScript = enemyScript;
    }

    public abstract void DoActionUpdate(float dTime);

    public abstract void LostTarget();

    public abstract void PlayerDead();

    public abstract void EnemyDead();



}



public class EnemyStatePatrol : EnemyState
{
    public EnemyStatePatrol(EnemyScript enemyScript) : base(enemyScript)
    {
        if (enemyScript.enemyRoamBehaviour == EnemyScript.EnemyRoamType.Patrolling)
        {
            enemyScript.SetInitialPatrolPaths();

            if (enemyScript.AreWaypointsAvailable()) // Ensure waypoints exist before patrolling
            {
                enemyScript.StartPatrol();
            }
            else
            {
                //Console.WriteLine("[WARNING] No valid waypoints for patrol!");
            }
        }
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.UpdateRayCastToPlayerPosition(); //Update Location of player

        // Only update patrol state if necessary
        if (enemyScript.isPatrolling)
        {
            enemyScript.StartPatrol();
        }

        if (enemyScript.CheckPlayerWithinSight()) // Player spotted, change state
        {
            switch (enemyScript.enemyType)
            {
                case EnemyScript.EnemySelection.Helpless:
                    enemyScript.SetCurrentState(new EnemyStateRun(enemyScript));
                    break;

                case EnemyScript.EnemySelection.Melee:
                case EnemyScript.EnemySelection.AlertMelee:
                    enemyScript.SetCurrentState(new EnemyStateChase(enemyScript));
                    break;

                case EnemyScript.EnemySelection.Ranged:
                case EnemyScript.EnemySelection.AlertRanged:
                    enemyScript.SetCurrentState(new EnemyStateRangedAttack(enemyScript));
                    break;
            }
            enemyScript.isPatrolling = false;
        }
    }

    public override void LostTarget() { }
    public override void PlayerDead() { }
    public override void EnemyDead() { }
}

public class EnemyStateChase : EnemyState
{
    public EnemyStateChase(EnemyScript enemyScript) : base(enemyScript)
    {
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.RunAtPlayer();
    }

    public override void LostTarget()
    {
        // Switch to search state
        enemyScript.SetCurrentState(new EnemyStateMeleeSearch(enemyScript));
    }

    public override void PlayerDead()
    {
    }

    public override void EnemyDead()
    {
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
    }

}

public class EnemyStateMeleeSearch : EnemyState
{
    public EnemyStateMeleeSearch(EnemyScript enemyScript) : base(enemyScript)
    {
        enemyScript.MeleeSearchStart();
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.MeleeSearchUpdate();
    }

    public override void LostTarget()
    {
        // Already searching, nothing to do
    }

    public override void PlayerDead()
    {
        // Return to patrol
        enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
    }

    public override void EnemyDead()
    {
        // Handle enemy death
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));

    }
}

public class EnemyStateReturnToHome : EnemyState
{

    public EnemyStateReturnToHome(EnemyScript enemyScript) : base(enemyScript)
    {
        enemyScript.ReturnHomeStart();
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.ReturnHomeUpdate();
    }

    public override void LostTarget()
    {

    }

    public override void PlayerDead()
    {
    }

    public override void EnemyDead()
    {
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
    }

}

public class EnemyStateRun : EnemyState
{

    public EnemyStateRun(EnemyScript enemyScript) : base(enemyScript)
    {

    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.RunFromPlayer();
    }

    public override void LostTarget()
    {
        enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
    }

    public override void PlayerDead()
    {
    }

    public override void EnemyDead()
    {
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
    }

}
public class EnemyStateEnemyDead : EnemyState
{
    public EnemyStateEnemyDead(EnemyScript enemyScript) : base(enemyScript)
    {

    }

    public override void DoActionUpdate(float dTime)
    {

    }

    public override void LostTarget() { }

    public override void PlayerDead() { }

    public override void EnemyDead() { }

}

public class EnemyStatePanic : EnemyState
{
    public EnemyStatePanic(EnemyScript enemyScript) : base(enemyScript)
    {

    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.UpdateRayCastToPlayerPosition(); //Update Location of player

        switch (enemyScript.enemyRoamBehaviour)
        {
            case EnemyScript.EnemyRoamType.Static:
                break;

            case EnemyScript.EnemyRoamType.Patrolling:
                enemyScript.StartPatrol();
                break;

            default:
                break;
        }

        if (enemyScript.CheckPlayerWithinSight()) //Checks if player is within sight
        {
            switch (enemyScript.enemyType)
            {
                case EnemyScript.EnemySelection.Helpless: //Start Helpless State
                    enemyScript.SetCurrentState(new EnemyStateChase(enemyScript));
                    enemyScript.isPatrolling = false;
                    break;

                case EnemyScript.EnemySelection.Melee: //Start Melee State
                case EnemyScript.EnemySelection.AlertMelee:
                    enemyScript.SetCurrentState(new EnemyStateChase(enemyScript));
                    enemyScript.isPatrolling = false;
                    break;

                default:
                    break;
            }
        }
    }

    public override void LostTarget() { }

    public override void PlayerDead() { }

    public override void EnemyDead() { }

}

public class EnemyStateRangedAttack : EnemyState
{


    public EnemyStateRangedAttack(EnemyScript enemyScript) : base(enemyScript)
    {
        enemyScript.RangedAttackStart();
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.RangedAttackUpdate();

    }

    public override void LostTarget()
    {
        // Switch to search state
        enemyScript.SetCurrentState(new EnemyStateRangedSearch(enemyScript));
    }

    public override void PlayerDead()
    {
        // Return to patrol
        enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
    }

    public override void EnemyDead()
    {
        // Handle enemy death
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));
    }
}


public class EnemyStateRangedSearch : EnemyState
{


    public EnemyStateRangedSearch(EnemyScript enemyScript) : base(enemyScript)
    {
        enemyScript.RangedSearchStart();
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.RangedSearchUpdate();
    }

    public override void LostTarget()
    {
        // Already searching, nothing to do
    }

    public override void PlayerDead()
    {
        // Return to patrol
        enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
    }

    public override void EnemyDead()
    {
        // Handle enemy death
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));

    }
}

public class EnemyStateScan : EnemyState
{
    public EnemyStateScan(EnemyScript enemyScript) : base(enemyScript)
    {
        enemyScript.EnemyScanStart();
    }

    public override void DoActionUpdate(float dTime)
    {
        enemyScript.EnemyScanUpdate();
    }

    public override void LostTarget()
    {
        // Already searching, nothing to do
    }

    public override void PlayerDead()
    {
        // Return to patrol
        enemyScript.SetCurrentState(new EnemyStatePatrol(enemyScript));
    }

    public override void EnemyDead()
    {
        // Handle enemy death
        enemyScript.SetCurrentState(new EnemyStateEnemyDead(enemyScript));

    }
}
