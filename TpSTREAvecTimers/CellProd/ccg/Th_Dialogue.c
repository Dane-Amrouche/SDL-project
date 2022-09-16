/* ***************************************************************** */
/* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Include.c begins         */
/* PragmaDev RTDS posix integration                                  */
#include <stdlib.h>
#include <stdio.h>
/* Include unistd when using a unix based system or cygwin */
#include <unistd.h>

/* POSIX includes */
#include <semaphore.h>
#include <pthread.h>
#include <signal.h>
#include <time.h>

#include "RTDS_Error.h"
#include "RTDS_MACRO.h"
#include "RTDS_OS.h"

#if defined( RTDS_FORMAT_TRACE )
#include "RTDS_FormatTrace.h"
#endif

/* $(RTDS_HOME)/share/ccg/posic/bricks/RTDS_Include.c ends           */
/* ***************************************************************** */
#include "Th_Dialogue.h"
#include "RTDS_messages.h"
#include "RTDS_gen.h"

#define RTDS_PROCESS_NUMBER RTDS_process_Th_Dialogue
#define RTDS_PROCESS_NAME Th_Dialogue

/*
** PROCESS Th_Dialogue:
** --------------------
*/

RTDS_TASK_ENTRY_POINT(Th_Dialogue)
  {
  short RTDS_transitionExecuted;
  int RTDS_savedSdlState = 0;

  int index = 0;
  machinesArray machines = {NULL};
  RTDS_MSG_DATA_DECL

  void * RTDS_myLocals[2];
  void ** RTDS_localsStack[1];


  #include "Th_Dialogue_tempVars.h"

  /* *************************************************************** */
  /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Process_begin.c begins */
  /* PragmaDev RTDS posix integration                                */

  int RTDS_sdlStatePrev = 0;
  RTDS_SdlInstanceId *RTDS_senderId = NULL;

  RTDS_currentContext->readSaveQueue = NULL;
  RTDS_currentContext->writeSaveQueue = NULL;

  /* To synchronize processes at startup time */
  RTDS_START_SYNCHRO_WAIT;

  /*    $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Process_begin.c ends  */
  /* *************************************************************** */

  RTDS_myLocals[0] = (void*)&index;
  RTDS_myLocals[1] = (void*)machines;
  RTDS_localsStack[0] = RTDS_myLocals;


  /* Initial transition */
  do	/* Dummy do/while(0) to be able to do 'break's */
    {
    Iter:
    if ( (RTDS_BOOLEAN)(index < NBRMACHINES) )
      {
      RTDS_PROCESS_CREATE("Th_Machine", RTDS_process_Th_Machine, Th_Machine, RTDS_DEFAULT_PROCESS_PRIORITY);
      index = index + 1;

      machines[index] = OFFSPRING;

      goto Iter;
      }
    else
      {
      RTDS_PROCESS_CREATE("Th_Piece", RTDS_process_Th_Piece, Th_Piece, RTDS_DEFAULT_PROCESS_PRIORITY);
      RTDS_MSG_SEND_pieceArv_TO_ID(OFFSPRING,       machines[index]);
      RTDS_PROCESS_CREATE("Th_Piece", RTDS_process_Th_Piece, Th_Piece, RTDS_DEFAULT_PROCESS_PRIORITY);
      RTDS_MSG_SEND_pieceArv_TO_ID(OFFSPRING,       machines[index - 1]);
      RTDS_PROCESS_CREATE("Th_Piece", RTDS_process_Th_Piece, Th_Piece, RTDS_DEFAULT_PROCESS_PRIORITY);
      RTDS_MSG_SEND_pieceArv_TO_ID(OFFSPRING,       machines[index]);
      RTDS_PROCESS_CREATE("Th_Piece", RTDS_process_Th_Piece, Th_Piece, RTDS_DEFAULT_PROCESS_PRIORITY);
      RTDS_MSG_SEND_pieceArv_TO_ID(OFFSPRING,       machines[index - 2]);
      RTDS_PROCESS_CREATE("Th_Piece", RTDS_process_Th_Piece, Th_Piece, RTDS_DEFAULT_PROCESS_PRIORITY);
      RTDS_MSG_SEND_pieceArv_TO_ID(OFFSPRING,       machines[index - 1]);
      RTDS_SDL_STATE_SET(RTDS_state_Idle);
      break;
      }
    } while (0);

  /* ***************************************************************** */
  /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Proc_loopStart.c begins  */
  /* PragmaDev RTDS posix integration */

  /* Event loop */
  for ( ; ; )
    {
    /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Proc_loopStart.c ends */
    /* *************************************************************** */

    /* *************************************************************** */
    /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Proc_middle.c begins   */
    /* PragmaDev RTDS posix integration */

    /* Check saved messages */
    if ( RTDS_currentContext->readSaveQueue != NULL )
      {
      RTDS_currentContext->currentMessage = RTDS_currentContext->readSaveQueue;
      RTDS_currentContext->readSaveQueue = RTDS_currentContext->readSaveQueue->next;
      }
    /* If no saved messages left, get message from process's message queue */
    else
      {
      RTDS_currentContext->currentMessage = ( RTDS_MessageHeader * )RTDS_MALLOC( sizeof( RTDS_MessageHeader ) );
      RTDS_MSG_QUEUE_READ( RTDS_currentContext->currentMessage );
      RTDS_currentContext->currentMessage->next = NULL;
      }

    /* Discard cancelled timers */
    if ( RTDS_currentContext->currentMessage->timerUniqueId != 0 )
      {
      RTDS_TIMER_CLEAN_UP(RTDS_currentContext);
      }

    /* Let's process the message ! */
    if ( RTDS_currentContext->currentMessage != NULL )
      {
      RTDS_SIMULATOR_TRACE( RTDS_messageReceived , RTDS_currentContext->currentMessage , SELF , RTDS_currentContext );
      RTDS_senderId = RTDS_currentContext->currentMessage->sender;
      RTDS_sdlStatePrev = RTDS_currentContext->sdlState;
      /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Proc_middle.c ends */
      /* ************************************************************* */

      /* Double switch state / signal */
      RTDS_transitionExecuted = 1;
      switch(RTDS_currentContext->sdlState)
        {
        /* Transitions for state Idle */
        case RTDS_state_Idle:
          switch(RTDS_currentContext->currentMessage->messageNumber)
            {
            /* Transition for state Idle - message piece */
            case RTDS_message_piece:
              RTDS_MSG_RECEIVE_piece(          index);
              RTDS_PROCESS_CREATE("Th_Piece", RTDS_process_Th_Piece, Th_Piece, RTDS_DEFAULT_PROCESS_PRIORITY);
              RTDS_MSG_SEND_pieceArv_TO_ID(OFFSPRING,               machines[index]);
              RTDS_SDL_STATE_SET(RTDS_state_Idle);
              break;
            default:
              RTDS_transitionExecuted = 0;
              break;
            } /* End of switch on message */
          break;
        default:
          RTDS_transitionExecuted = 0;
          break;
        } /* End of switch(RTDS_currentContext->sdlState) */
      /* ************************************************************ */
      /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Proc_end.c begins   */
      /* PragmaDev RTDS posix integration                             */
      RTDS_TransitionCleanUp( RTDS_currentContext , RTDS_sdlStatePrev );
      } /* End of if ( RTDS_currentContext->currentMessage != NULL )  */
    } /* End of for ( ; ; ) */

  /* $(RTDS_HOME)/share/ccg/posix/bricks/RTDS_Proc_end.c ends   */
  /* ********************************************************** */
  }
