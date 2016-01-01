//The application time based timer

#include <stdbool.h>

typedef struct LTimer
{
    //The clock time when the timer started
    Uint32 mStartTicks;

    //The ticks stored when the timer was paused
    Uint32 mPausedTicks;

    //The timer status
    bool mPaused;
    bool mStarted;

}LTimer;

//Constructor
LTimer* LTimer_create();
void LTimer_destroy(LTimer* timer);

//Timer actions
void LTimer_start(LTimer* timer);
void LTimer_stop(LTimer* timer);
void LTimer_pause(LTimer* timer);
void LTimer_unpause(LTimer* timer);

//Get time
Uint32 LTimer_getTicks(LTimer* timer);

//Get timer status
bool LTimer_isStarted(LTimer* timer);
bool LTimer_isPaused(LTimer* timer);
