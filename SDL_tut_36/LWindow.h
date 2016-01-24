#ifdef __linux
#include <SDL2/SDL.h>
#elif _WIN32
#include <SDL.h>
#endif
#include <stdbool.h>

typedef struct LWindow
{
    // ref to SDL window
    SDL_Window* mWindow;
    SDL_Renderer* mRenderer;
    int mWindowID;

    //Window dimensions
    int mWidth;
    int mHeight;

    //Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
    bool mShown;
} LWindow;

//initialiser
LWindow* LWindow_create();

//Creates window
bool LWindow_init(LWindow* win, int screenW, int screenH);

//Handles window events
void LWindow_handleEvent( LWindow* win, SDL_Event* event );

//Deallocates internals
void LWindow_free(LWindow* win);

//Focuses on window
void LWindow_focus(LWindow* win);

//Shows windows contents
void LWindow_render(LWindow* win);

//Window dimensions
int LWindow_getWidth(LWindow* win);
int LWindow_getHeight(LWindow* win);

//Window focii
bool LWindow_hasMouseFocus(LWindow* win);
bool LWindow_hasKeyboardFocus(LWindow* win);
bool LWindow_isMinimized(LWindow* win);
bool LWindow_isShown(LWindow* win);
