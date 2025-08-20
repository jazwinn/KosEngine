using System;
using System.Collections;
using System.Collections.Generic;
using System.ComponentModel;
using System.Diagnostics;
using System.IO;
using System.Linq;
using System.Runtime.CompilerServices;
using System.Text.RegularExpressions;

public class GameControllerLevel6 : ScriptBase
{
    #region ID Variables
    private uint EntityID; //Entity ID of the object, do not touch!
    #endregion

    private string blockDoorPrefab;
    public static bool isActivated = false;
    public static bool isBossDead = false;
    private string bossBGM = "aud_bossLevelLoop.wav";

    public static bool gameIsPaused; //For all scripts to check if game is paused

    public static bool gameAudioUp;

    public static bool runOnce; //Prevents PauseGame from running more than once

    private bool isShowingFps;


    public override void Awake(uint id)
    {
        EntityID = id; //Sets ID for object, DO NOT TOUCH
        blockDoorPrefab = "door_block"; 
    }
    public override void Start()
    {
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 doorPosition);

        runOnce = false;
        gameIsPaused = false;
        isShowingFps = false;

        InternalCall.m_DisableLayer(8); //Disables Loadout Menu UI
        InternalCall.m_DisableLayer(7); //Disables Pause Menu UI
        InternalCall.m_DisableLayer(6); //Disable How To Play UI
        InternalCall.m_DisableLayer(9); //Disable Death Screen UI
        InternalCall.m_DisableLayer(16); //Disable sound UI
    }

    public override void Update()
    {
        InputChecker();


        #region Collision Handling
        if (InternalCall.m_InternalCallIsCollided(EntityID) != 0.0f)
        {
            int[] collidedEntities = InternalCall.m_InternalCallGetCollidedEntities(EntityID);

            foreach (int collidedEntitiesID in collidedEntities)
            {
                switch (InternalCall.m_InternalCallGetTag((uint)collidedEntitiesID))
                {
                    case "Player": // If it hits the player
                        if(isActivated == false)
                        {
                            isActivated = true;
                            SpawnDoor();
                            InternalCall.m_InternalCallPlayAudio(EntityID, bossBGM);

                        }
                        break;

                    default:
                        break;
                }
            }
        }
        #endregion
    }

    public void SpawnDoor()
    {
        InternalCall.m_InternalGetTranslate(EntityID, out Vector2 doorPosition);
        InternalCall.m_InternalCallAddPrefab(blockDoorPrefab, doorPosition.X, doorPosition.Y, 0);
    }

    private void PauseGame()
    {
        if (!gameIsPaused && !runOnce)
        {
            gameIsPaused = true;
            gameAudioUp = false;
            runOnce = true;

            //Pauses all coroutines
            CoroutineManager.Instance.PauseAllCoroutines();

            //Sets delta timescale to 0
            InternalCall.m_InternalCallSetTimeScale(0f);

            //Shows Pause Menu Layer
            InternalCall.m_EnableLayer(7);


            InternalCall.m_InternalCallPauseAllAudio();
        }

        else if (gameIsPaused)
        {
            //Resumes all coroutines
            CoroutineManager.Instance.ResumeAllCoroutines();

            //Sets delta timescale to 1
            InternalCall.m_InternalCallSetTimeScale(1f);

            //Hide Pause Menu Layer
            InternalCall.m_DisableLayer(7);

            InternalCall.m_DisableLayer(6);

            //hide sound menu
            InternalCall.m_DisableLayer(16);

            InternalCall.m_InternalCallUnPauseAllAudio();

            gameIsPaused = false;
            runOnce = false;
        }

    }


    private void RestartGame()
    {
        if (LevelSelection.SceneName != null)
        {
            InternalCall.m_DisableLayer(8); //Disables Loadout Menu UI
            InternalCall.m_DisableLayer(7); //Disables Pause Menu UI
            InternalCall.m_DisableLayer(6); //Disable How To Play UI
            InternalCall.m_DisableLayer(9); //Disable Death Screen UI
            InternalCall.m_DisableLayer(15); //Disable FPS counter UI
            InternalCall.m_DisableLayer(16); // disable sound option

            InternalCall.m_InternalCallStopAllAudio();
            CoroutineManager.Instance.StopAllCoroutines();
            InternalCall.m_InternalCallSetTimeScale(1);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene(LevelSelection.SceneName);
        }

        else
        {
            //Console.WriteLine("Level Selection Script Has No SceneName!");

            InternalCall.m_InternalCallStopAllAudio();

            CoroutineManager.Instance.StopAllCoroutines();
            InternalCall.m_InternalCallSetTimeScale(1);
            InternalCall.m_UnloadAllScene();
            InternalCall.m_InternalCallLoadScene("Level6");
        }
    }

    private void Sortie()
    {
        if (!PlayerLoadoutManager.isSortieing)
        {
            PlayerLoadoutManager.isSortieing = true;
            InternalCall.m_InternalCallSetTimeScale(0.2f);
            InternalCall.m_EnableLayer(8);
        }

        else
        {
            PlayerLoadoutManager.isSortieing = false;
            InternalCall.m_InternalCallSetTimeScale(1f);
            InternalCall.m_DisableLayer(8);
        }
    }

    #region Check Inputs

    private void InputChecker()
    {
        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.R) && PlayerController.isDead)
        {
            RestartGame();
        }

        if (PlayerController.isDead) { return; }

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.ESC))
        {
            PauseGame();
        }

        if (gameIsPaused) { return; } //Ensures nothing but pause menu can be activated when game is paused

        if (InternalCall.m_InternalCallIsKeyTriggered(keyCode.TAB))
        {
            Sortie();
        }

    }
    #endregion
}