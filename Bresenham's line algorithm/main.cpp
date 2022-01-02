#include <math.h>

int g_WindowWidth = 640;
int g_WindowHeight = 480;

extern void Run();
extern bool OpenWindow(int _Width, int _Height, const char* _pTitle);
extern void CloseWindow();
extern void DrawPixel(int _X, int _Y);


void calcError(float _oldError, int _dSlow, float* _error)
{
     *_error = _oldError - _dSlow;
}

// -----------------------------------------------------------------------------
// Draws a line from (X1,Y1) to (X2,Y2).
// -----------------------------------------------------------------------------
void DrawLine(int _X0, int _Y0, int _X1, int _Y1)
{
    int dX = abs(_X1 - _X0);
    int dY = abs(_Y1 - _Y0);
    bool xIsGreater;
    int* dFaster;
    int* dSlower;

    dX > dY ? xIsGreater = true : xIsGreater = false;
    xIsGreater ? dFaster = &dX : dFaster = &dY;
    xIsGreater ? dSlower = &dY : dSlower = &dX;
    float error = *dFaster/2;

    for (int ii = _X0; ii < *dFaster; ii++)
    {
        calcError(error, *dSlower, &error);

        if (error < 0) 
        {
            _Y0++;
            error += *dFaster;
        }
        DrawPixel(ii, _Y0);
    }
}

// -----------------------------------------------------------------------------
// Callback which indicates that the window is drawn.
// -----------------------------------------------------------------------------
void OnDraw()
{
    // Add another DrawLine command here if you want to draw multiple lines.
    DrawLine(20, 100, 620, 100);
    DrawLine(20, 150, 620, 200);
    DrawLine(20, 200, 120, 230);
    DrawLine(20, 100, 620, 400);
    DrawLine(-20, 100, 720, 400);

}

void main()
{
    OpenWindow(g_WindowWidth, g_WindowHeight, "GDV Lesson 2");

    Run();

    CloseWindow();
}