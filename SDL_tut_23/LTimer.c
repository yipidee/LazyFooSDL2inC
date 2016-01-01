#include <stdlib.h>
#include <SDL.h>
#include "LTimer.h"

//Constructor
LTimer* LTimer_create()
{
    LTimer* timer = malloc(sizeof(LTimer));
    if(timer!=NULL)
    {
        timer->mStarted = false;
        timer->mPaused = false;
        timer->mStartTicks = 0;
        timer->mPausedTicks = 0;
        return timer;
    }
    else return NULL;
}

//Timer actions
void LTimer_start(LTimer* timer)
{
    //Start the timer
    timer->mStarted = true;

    //Unpause the timer
    timer->mPaused = false;

    //Get the current clock time
    timer->mStartTicks = SDL_GetTicks();
	timer->mPausedTicks = 0;
}

void LTimer_stop(LTimer* timer)
{
    //Stop the timer
    timer->mStarted = false;

    //Unpause the timer
    timer->mPaused = false;

	//Clear tick variables
	timer->mStartTicks = 0;
	timer->mPausedTicks = 0;
}

void LTimer_pause(LTimer* timer)
{
    //If the timer is running and isn't already paused
    if( timer->mStarted && !timer->mPaused )
    {
        //Pause the timer
        timer->mPaused = true;

        //Calculate the paused ticks
        timer->mPausedTicks = SDL_GetTicks() - timer->mStartTicks;
		timer->mStartTicks = 0;
    }
}

void LTimer_unpause(LTimer* timer)
{
    //If the timer is running and paused
    if( timer->mStarted && timer->mPaused )
    {
        //Unpause the timer
        timer->mPaused = false;

        //Reset the starting ticks
        timer->mStartTicks = SDL_GetTicks() - timer->mPausedTicks;

        //Reset the paused ticks
        timer->mPausedTicks = 0;
    }
}

//Get time
Uint32 LTimer_getTicks(LTimer* timer)
{
    //The actual timer time
	Uint32 time = 0;

    //If the timer is running
    if( timer->mStarted )
    {
        //If the timer is paused
        if( timer->mPaused )
        {
            //Return the number of ticks when the timer was paused
            time = timer->mPausedTicks;
        }
        else
        {
            //Return the current time minus the start time
            time = SDL_GetTicks() - timer->mStartTicks;
        }
    }

    return time;
}

//Get timerr status
bool LTimer_isStarted(LTimer* timer)
{
    return timer->mStarted;
}

bool LTimer_isPaused(LTimer* timer)
{
    return timer->mPaused;
}

void LTimer_destroy(LTimer* timer)
{
    free(timer);
}
