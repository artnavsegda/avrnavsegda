#include <windows.h>
#include <stdio.h>
#include "usb.h"
#include "menu.h"

#define DEVICE_VENDOR_VID 0x03eb
#define DEVICE_VENDOR_PID 0x2423

// the device's endpoints
static unsigned char udi_vendor_ep_interrupt_in = 0;
static unsigned char udi_vendor_ep_interrupt_out = 0;

char statustext[100] = "Start";
char string_manufacturer[100] = "";
char string_product[100] = "";
char string_serial[100] = "";

struct usb_bus *bus;
struct usb_device *device;
usb_dev_handle *device_handle = NULL; // the device handle

void openinterface(void)
{
	// Open interface vendor
	if(usb_set_configuration(device_handle, 1) < 0) {
		sprintf(statustext,"%s","error: setting config 1 failed\n");
		usb_close(device_handle);
		return;
	}
	if(usb_claim_interface(device_handle, 0) < 0) {
		sprintf(statustext,"%s","error: claiming interface 0 failed\n");
		usb_close(device_handle);
		return;
	}
	if (1!=device->config->interface->num_altsetting) {

		if(usb_set_altinterface(device_handle, 1) < 0) {
			sprintf(statustext,"%s","error: set alternate 1 interface 0 failed\n");
			usb_close(device_handle);
			return;
		}
	}
	sprintf(statustext,"%s","Device ready");
}

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
	{
    sprintf(statustext,"%s","Device open");
		if (0!=device->descriptor.iManufacturer)
			usb_get_string_simple(device_handle, device->descriptor.iManufacturer, string_manufacturer, sizeof(string_manufacturer));
		if (0!=device->descriptor.iProduct)
			usb_get_string_simple(device_handle, device->descriptor.iProduct, string_product, sizeof(string_product));
		if (0!=device->descriptor.iSerialNumber)
			usb_get_string_simple(device_handle, device->descriptor.iSerialNumber, string_serial, sizeof(string_serial));
	}
}

void findendpoint(void)
{
	unsigned char nb_ep = 0;
	struct usb_endpoint_descriptor *endpoints;

	if (1==device->config->interface->num_altsetting) {
		// Old firmwares have no alternate setting
		nb_ep = device->config->interface->altsetting[0].bNumEndpoints;
		endpoints = device->config->interface->altsetting[0].endpoint;
	} else {
		// The alternate setting has been added to be USB compliance:
		// 1.2.40 An Isochronous endpoint present in alternate interface 0x00 must have a MaxPacketSize of 0x00
		// Reference document: Universal Serial Bus Specification, Revision 2.0, Section 5.6.3.
		nb_ep = device->config->interface->altsetting[1].bNumEndpoints;
		endpoints = device->config->interface->altsetting[1].endpoint;
	}
	while (nb_ep) {
		nb_ep--;

		unsigned char ep_type = endpoints[nb_ep].bmAttributes	& USB_ENDPOINT_TYPE_MASK;
		unsigned char ep_add = endpoints[nb_ep].bEndpointAddress;
		unsigned char dir_in = (ep_add & USB_ENDPOINT_DIR_MASK) == USB_ENDPOINT_IN;
		unsigned short ep_size = endpoints[nb_ep].wMaxPacketSize;

		switch (ep_type) {
			case USB_ENDPOINT_TYPE_INTERRUPT:
				if (dir_in) {
					udi_vendor_ep_interrupt_in = ep_add;
				} else {
					udi_vendor_ep_interrupt_out= ep_add;
				}
				break;
		}
	}
	sprintf(statustext,"Endpoint in: %02X, out: %02X", udi_vendor_ep_interrupt_in, udi_vendor_ep_interrupt_out);
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
            TextOut(hdc, 100, 100, statustext, 100);
						TextOut(hdc, 100, 120, string_manufacturer, 100);
						TextOut(hdc, 100, 140, string_product, 100);
						TextOut(hdc, 100, 160, string_serial, 100);
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
						case ID_EPOINT:
								if (device_handle != NULL)
								{
									sprintf(statustext,"%s","Searching endpoints");
									findendpoint();
									InvalidateRect(hwnd, NULL, TRUE);
								}
								return 0;
						case ID_IFACE:
								sprintf(statustext,"%s","Initialization device");
								openinterface();
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
