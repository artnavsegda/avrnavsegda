#include <windows.h>
#include <stdio.h>
#include "usb.h"
#include "menu.h"

#define DEVICE_VENDOR_VID 0x03eb
#define DEVICE_VENDOR_PID 0x2423

char statustext[20] = "Start";

struct usb_bus *bus;
struct usb_device *device;
usb_dev_handle *device_handle = NULL; // the device handle

void opendevice(void)
{
  // Search and open device
	for(bus = usb_get_busses(); bus; bus = bus->next) {
		for(device = bus->devices; device; device = device->next) {
			if(device->descriptor.idVendor == DEVICE_VENDOR_VID
					&& device->descriptor.idProduct == DEVICE_VENDOR_PID) {
				device_handle = usb_open(device);
				break;
			}
		}
	}
	if(device_handle == NULL)
    sprintf(statustext,"%s","Device not found");
  else
    sprintf(statustext,"%s","Device open");
}

LRESULT CALLBACK WindowProc(HWND hwnd, UINT uMsg, WPARAM wParam, LPARAM lParam)
{
    switch (uMsg)
    {
    case WM_DESTROY:
        PostQuitMessage(0);
        return 0;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hwnd, &ps);
            FillRect(hdc, &ps.rcPaint, (HBRUSH) (COLOR_WINDOW+1));
            TextOut(hdc, 100, 100, statustext, 20);
            EndPaint(hwnd, &ps);
        }
        return 0;

    case WM_COMMAND:
        switch (LOWORD(wParam))
        {
            case ID_EXIT:
                DestroyWindow(hwnd);
                return 0;
            case ID_OPEN:
                sprintf(statustext,"%s","Opening");
                opendevice();
                InvalidateRect(hwnd, NULL, TRUE);
                return 0;
        }
        return 0;
    }
    return DefWindowProc(hwnd, uMsg, wParam, lParam);
}

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR pCmdLine, int nCmdShow)
{
    // Libusb initialization
  	usb_init();         // initialize the library
  	usb_find_busses();  // find all busses
  	usb_find_devices(); // find all connected devices

    // Register the window class.
    const char CLASS_NAME[]  = "Sample Window Class";

    WNDCLASS wc = { };

    wc.lpfnWndProc   = WindowProc;
    wc.hInstance     = hInstance;
    wc.lpszClassName = CLASS_NAME;
    wc.lpszMenuName      = "Menu";

    RegisterClass(&wc);

    // Create the window.

    HWND hwnd = CreateWindowEx(
        0,                              // Optional window styles.
        CLASS_NAME,                     // Window class
        "Learn to Program Windows",    // Window text
        WS_OVERLAPPEDWINDOW,            // Window style

        // Size and position
        CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT,

        NULL,       // Parent window
        NULL,       // Menu
        hInstance,  // Instance handle
        NULL        // Additional application data
        );

    if (hwnd == NULL)
    {
        return 0;
    }

    ShowWindow(hwnd, nCmdShow);

    // Run the message loop.

    MSG msg = { };
    while (GetMessage(&msg, NULL, 0, 0))
    {
        TranslateMessage(&msg);
        DispatchMessage(&msg);
    }

    return 0;
}
