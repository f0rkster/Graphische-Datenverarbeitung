
#include <assert.h>
#include <iostream>
#include <windows.h>

// -----------------------------------------------------------------------------
// Global constants.
// -----------------------------------------------------------------------------
const wchar_t* g_pWindowClassName = L"ApplicationWindow";

// -----------------------------------------------------------------------------
// Global fields.
// -----------------------------------------------------------------------------
HINSTANCE g_pNativeApplication   = nullptr; ///< Operating system handle to our application.
HWND      g_pNativeWindow        = nullptr; ///< A handle of the operating system to the window.
HDC       g_pNativeDeviceContext = nullptr; ///< A handle to the device context of the window.

// -----------------------------------------------------------------------------
// Extern method declaration.
// -----------------------------------------------------------------------------
extern void OnDraw();

// -----------------------------------------------------------------------------
// Method forward declaration.
// -----------------------------------------------------------------------------
long __stdcall ProcessMessages(HWND _pNativeWindowHandle, unsigned int _MessageType, unsigned int _Arg1, long _Arg2);

// -----------------------------------------------------------------------------
// Prints a message to the console.
// -----------------------------------------------------------------------------
void LogErrorMessage(const char* _pMessage)
{
    assert(_pMessage != nullptr);

    std::cout << _pMessage << std::endl;
}

// -----------------------------------------------------------------------------
// Creates a window.
// -----------------------------------------------------------------------------
bool OpenWindow(int _Width, int _Height, const char* _pTitle)
{
    HWND      pNativeWindow;
    HINSTANCE pNativeApplication;
    HDC       pNativeDeviceContext;
    wchar_t   Title[4096];

    assert((_pTitle != nullptr) && (::strlen(_pTitle) < (sizeof(Title) / sizeof(Title[0]))));

    pNativeApplication = ::GetModuleHandle(nullptr);

    if (pNativeApplication == nullptr)
    {
        return false;
    }

    // -----------------------------------------------------------------------------
    // Define the look and feel of the window.
    // -----------------------------------------------------------------------------
    WNDCLASSEX WindowClass;

    WindowClass.cbSize        = sizeof(WindowClass);                        // The number of bytes of the 'WindowClass' descriptor.
    WindowClass.style         = CS_HREDRAW | CS_VREDRAW | CS_DBLCLKS;       // The style of our window, which for example defines the behavior when our window is resized.
    WindowClass.lpfnWndProc   = ProcessMessages;                            // The method where the messages of the operating system are processed.
    WindowClass.cbClsExtra    = 0;                                          // The number of extra bytes we want to append to our 'WindowClass' descriptor.
    WindowClass.cbWndExtra    = 0;                                          // The number of extra bytes we want to append to our 'HWND' handle.
    WindowClass.hInstance     = pNativeApplication;                         // A handle to the instance, which owns the 'ProcessMessages' method, i.e. our application.
    WindowClass.hIcon         = nullptr;                                    // An icon which we can associate with out window via the 'LoadIcon' method.
    WindowClass.hCursor       = LoadCursor(nullptr, IDC_ARROW);             // Defines the look of our cursor.
    WindowClass.hbrBackground = reinterpret_cast<HBRUSH>(COLOR_WINDOW + 3); // Allows us define our background behavior.
    WindowClass.lpszMenuName  = nullptr;                                    // Name of our menu definitions in the resources.
    WindowClass.lpszClassName = g_pWindowClassName;                         // Name of our window class, which is not the title, but an ID to get a pointer to this 'WindowClass' descriptor.
    WindowClass.hIconSm       = nullptr;                                    // A small icon which we can associate with out window.

    // -----------------------------------------------------------------------------
    // Registers our window class in the operating system.
    // -----------------------------------------------------------------------------
    if (!::RegisterClassEx(&WindowClass))
    {
        LogErrorMessage("Failed to register window class.");

        return false;
    }

    // -----------------------------------------------------------------------------
    // Defines the size of the window in pixels.
    // -----------------------------------------------------------------------------
    RECT Dimensions = { 0, 0, _Width, _Height };

    // -----------------------------------------------------------------------------
    // Adjust the size of the window, because we do not want, that the total window
    // including border, menu, and title has the dimensions defined by '_Width' and
    // '_Height', but the inner area where we render our scene.
    // -----------------------------------------------------------------------------
    ::AdjustWindowRect(&Dimensions, WS_OVERLAPPEDWINDOW, FALSE);

    int Width  = Dimensions.right  - Dimensions.left;
    int Height = Dimensions.bottom - Dimensions.top;

    // -----------------------------------------------------------------------------
    // Create the window in the operating system.
    // -----------------------------------------------------------------------------
    ::MultiByteToWideChar(CP_ACP, 0, _pTitle, -1, Title, sizeof(Title) / sizeof(Title[0]));

    pNativeWindow = CreateWindow(g_pWindowClassName, Title, WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, Width, Height, nullptr, nullptr, pNativeApplication, nullptr);

    if (pNativeWindow == nullptr)
    {
        ::UnregisterClass(g_pWindowClassName, pNativeApplication);

        LogErrorMessage("Failed to create window.");

        return false;
    }

    // -----------------------------------------------------------------------------
    // Get the device context of the window.
    // -----------------------------------------------------------------------------
    pNativeDeviceContext = GetDC(pNativeWindow);

    if (pNativeDeviceContext == nullptr)
    {
        ::UnregisterClass(g_pWindowClassName, pNativeApplication);

        LogErrorMessage("Failed to create window.");

        return false;
    }

    // -----------------------------------------------------------------------------
    // Show up the window.
    // -----------------------------------------------------------------------------
    ::ShowWindow  (pNativeWindow, SW_SHOWDEFAULT);
    ::UpdateWindow(pNativeWindow);

    // -----------------------------------------------------------------------------
    // Memorize the handles.
    // -----------------------------------------------------------------------------
    g_pNativeApplication   = pNativeApplication;
    g_pNativeWindow        = pNativeWindow;
    g_pNativeDeviceContext = pNativeDeviceContext;

    return true;
}

// -----------------------------------------------------------------------------
// Closes the window.
// -----------------------------------------------------------------------------
void CloseWindow()
{
    assert(g_pNativeApplication   != nullptr);
    assert(g_pNativeWindow        != nullptr);
    assert(g_pNativeDeviceContext != nullptr);

    ::ReleaseDC(g_pNativeWindow, g_pNativeDeviceContext);

    ::UnregisterClass(g_pWindowClassName, g_pNativeApplication);
}

// -----------------------------------------------------------------------------
// Callback to handle all incoming events of the window.
// -----------------------------------------------------------------------------
long __stdcall ProcessMessages(HWND _pNativeWindowHandle, unsigned int _MessageType, unsigned int _Arg1, long _Arg2)
{
    switch (_MessageType)
    {
        case WM_DESTROY:
        {
            // -----------------------------------------------------------------------------
            // Tell the operating system to leave the application.
            // -----------------------------------------------------------------------------
            ::PostQuitMessage(0);

            break;
        }

        default:
        {
            // -----------------------------------------------------------------------------
            // Let the operating system handle all the messages we do not want to handle.
            // -----------------------------------------------------------------------------
            return ::DefWindowProc(_pNativeWindowHandle, _MessageType, _Arg1, _Arg2);
        }
    }

    return 0;
}

// -----------------------------------------------------------------------------
// Main loop to keep the window alive.
// -----------------------------------------------------------------------------
void Run()
{
    // -----------------------------------------------------------------------------
    // The main loop of our application, which dispatches all the event messages
    // created by out window, e.h. resizing, update, ...
    // -----------------------------------------------------------------------------
    MSG Message;

    ::memset(&Message, 0, sizeof(Message));

    while (Message.message != WM_QUIT)
    {
        if (::PeekMessage(&Message, 0, 0U, 0U, PM_REMOVE))
        {
            ::TranslateMessage(&Message);
            ::DispatchMessage (&Message);
        }
        else
        {
            // -----------------------------------------------------------------------------
            // Render our scene.
            // -----------------------------------------------------------------------------
            OnDraw();
        }
    }
}

// -----------------------------------------------------------------------------
// Draws a pixel into the window.
// -----------------------------------------------------------------------------
void DrawPixel(int _X, int _Y)
{
    ::SetPixel(g_pNativeDeviceContext, _X, _Y, RGB(255, 0, 0));
}
