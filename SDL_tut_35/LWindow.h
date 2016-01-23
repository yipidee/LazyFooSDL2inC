#include <SDL.h>
#include <stdbool.h>

typedef struct LWindow
{
    // ref to SDL window
    SDL_Window* mWindow;

    //Window dimensions
    int mWidth;
    int mHeight;

    //Window focus
    bool mMouseFocus;
    bool mKeyboardFocus;
    bool mFullScreen;
    bool mMinimized;
} LWindow;

//initialiser
LWindow* LWindow_create();

//Creates window
bool LWindow_init(LWindow* win, int screenW, int screenH);

//Creates renderer from internal window
SDL_Renderer* LWindow_createRenderer(LWindow* win);

//Handles window events
void LWindow_handleEvent( LWindow* win, SDL_Renderer* renderer, SDL_Event* event );

//Deallocates internals
void LWindow_free(LWindow* win);

//Window dimensions
int LWindow_getWidth(LWindow* win);
int LWindow_getHeight(LWindow* win);

//Window focii
bool LWindow_hasMouseFocus(LWindow* win);
bool LWindow_hasKeyboardFocus(LWindow* win);
bool LWindow_isMinimized(LWindow* win);
