// game of life.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "game of life.h"

#define MAX_LOADSTRING 100

#define X_SIZE 1500
#define Y_SIZE 950
#define X_RECT 25
#define Y_RECT 25

// Global Variables:
HINSTANCE hInst;                                // current instance
WCHAR szTitle[MAX_LOADSTRING];                  // The title bar text
WCHAR szWindowClass[MAX_LOADSTRING];            // the main window class name

HWND hwnd;

bool playing = true;

static bool squares[X_SIZE / X_RECT][Y_SIZE / Y_RECT];

int time_delay = 2;
int thickness = 3;
int odds = 3;


// Forward declarations of functions included in this code module:
ATOM                MyRegisterClass(HINSTANCE hInstance);
BOOL                InitInstance(HINSTANCE, int);
LRESULT CALLBACK    WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK    About(HWND, UINT, WPARAM, LPARAM);

void updateCells();

int APIENTRY wWinMain(_In_ HINSTANCE hInstance,
                     _In_opt_ HINSTANCE hPrevInstance,
                     _In_ LPWSTR    lpCmdLine,
                     _In_ int       nCmdShow)
{
    UNREFERENCED_PARAMETER(hPrevInstance);
    UNREFERENCED_PARAMETER(lpCmdLine);

    // TODO: Place code here.

	//squares[28][17] = squares[29][17] = squares[30][17] = true;
	squares[28][17] = squares[28][18] = squares[28][19] = squares[29][19] = squares[30][18] = true;
	srand(time(NULL));

    // Initialize global strings
    LoadStringW(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
    LoadStringW(hInstance, IDC_GAMEOFLIFE, szWindowClass, MAX_LOADSTRING);
    MyRegisterClass(hInstance);

    // Perform application initialization:
    if (!InitInstance (hInstance, nCmdShow))
    {
        return FALSE;
    }

    HACCEL hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_GAMEOFLIFE));

	MSG msg = { 0 };

	std::clock_t start;

	start = std::clock();

    // Main message loop:
    while (msg.message != WM_QUIT)
    {
        if (PeekMessage(&msg, nullptr, 0, 0, PM_REMOVE))
        {
            TranslateMessage(&msg);
            DispatchMessage(&msg);
        }

		else if ((((std::clock() - start) / (double)CLOCKS_PER_SEC) > time_delay) && playing)
		{
			updateCells();
			start = std::clock();
		}
		
    }

    return (int) msg.wParam;
}



void updateCells()
{
	//here is where the logic of the game is put

	bool oldSquares[X_SIZE / X_RECT][Y_SIZE / Y_RECT];

	for (int x = 0; x < X_SIZE / X_RECT; x++)
	{
		for (int y = 0; y < Y_SIZE / Y_RECT; y++)
		{
			oldSquares[x][y] = squares[x][y];
		}
	}

	for (int x = 0; x < X_SIZE / X_RECT; x++)
	{
		for (int y = 0; y < Y_SIZE / Y_RECT; y++)
		{
			int neighbours= 0;

			if (y != 0)
				neighbours += oldSquares[x][y - 1];

			if (y != (Y_SIZE / Y_RECT) - 1)
				neighbours += oldSquares[x][y + 1];

			if (x != 0)
				neighbours += oldSquares[x - 1][y];

			if (x != (X_SIZE / X_RECT) - 1)
				neighbours += oldSquares[x + 1][y];

			if (y != 0 && x != 0)
				neighbours += oldSquares[x - 1][y - 1];

			if (y != (Y_SIZE / Y_RECT) - 1 && x != 0)
				neighbours += oldSquares[x - 1][y + 1];

			if (y != 0 && x != (X_SIZE / X_RECT) - 1)
				neighbours += oldSquares[x + 1][y - 1];

			if (y != (Y_SIZE / Y_RECT) - 1 && x != (X_SIZE / X_RECT) - 1)
				neighbours += oldSquares[x + 1][y + 1];

			
			if (neighbours >= 4 && oldSquares[x][y])
				squares[x][y] = false;
			else if (neighbours <= 1 && oldSquares[x][y])
				squares[x][y] = false;
			else if (neighbours == 3 && !(oldSquares[x][y]))
				squares[x][y] = true;

		}
	}

	InvalidateRect(hwnd, NULL, TRUE);
	return;
}



//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
    WNDCLASSEXW wcex;

    wcex.cbSize = sizeof(WNDCLASSEX);

	COLORREF back = RGB(0, 0, 0);
	HBRUSH brush = CreateSolidBrush(back);

    wcex.style          = CS_HREDRAW | CS_VREDRAW;
    wcex.lpfnWndProc    = WndProc;
    wcex.cbClsExtra     = 0;
    wcex.cbWndExtra     = 0;
    wcex.hInstance      = hInstance;
    wcex.hIcon          = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_GAMEOFLIFE));
    wcex.hCursor        = LoadCursor(nullptr, IDC_ARROW);
    wcex.hbrBackground  = brush;
    wcex.lpszMenuName   = MAKEINTRESOURCEW(IDC_GAMEOFLIFE);
    wcex.lpszClassName  = szWindowClass;
    wcex.hIconSm        = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	DeleteObject(brush);

    return RegisterClassExW(&wcex);
}

//
//   FUNCTION: InitInstance(HINSTANCE, int)
//
//   PURPOSE: Saves instance handle and creates main window
//
//   COMMENTS:
//
//        In this function, we save the instance handle in a global variable and
//        create and display the main program window.
//
BOOL InitInstance(HINSTANCE hInstance, int nCmdShow)
{
   hInst = hInstance; // Store instance handle in our global variable

   RECT wr = { 0, 0, X_SIZE, Y_SIZE};    // set the size, but not the position
   AdjustWindowRect(&wr, WS_OVERLAPPEDWINDOW, TRUE);    // adjust the size

   HWND hWnd =  CreateWindowW(szWindowClass, szTitle, WS_OVERLAPPEDWINDOW,
      CW_USEDEFAULT, 0, wr.right - wr.left, wr.bottom - wr.top, nullptr, nullptr, hInstance, nullptr);

   hwnd = hWnd;

   if (!hWnd)
   {
      return FALSE;
   }

   ShowWindow(hWnd, nCmdShow);
   UpdateWindow(hWnd);

   return TRUE;
}

//
//  FUNCTION: WndProc(HWND, UINT, WPARAM, LPARAM)
//
//  PURPOSE:  Processes messages for the main window.
//
//  WM_COMMAND  - process the application menu
//  WM_PAINT    - Paint the main window
//  WM_DESTROY  - post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
    switch (message)
    {
    case WM_COMMAND:
        {
            int wmId = LOWORD(wParam);
            // Parse the menu selections:
            switch (wmId)
            {
            case IDM_ABOUT:
                DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
                break;
            case IDM_EXIT:
                DestroyWindow(hWnd);
                break;
			case IDM_FILL:
				{
					for (int x = 0; x < X_SIZE / X_RECT; x++)
					{
						for (int y = 0; y < Y_SIZE / Y_RECT; y++)
						{
							squares[x][y] = true;
						}
					}

					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			case IDM_EMPTY:
				{
					for (int x = 0; x < X_SIZE / X_RECT; x++)
					{
						for (int y = 0; y < Y_SIZE / Y_RECT; y++)
						{
							squares[x][y] = false;
						}
					}

					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
			case IDM_RANDOM:
				{
					for (int x = 0; x < X_SIZE / X_RECT; x++)
					{
						for (int y = 0; y < Y_SIZE / Y_RECT; y++)
						{
							if (!(rand() % odds))
								squares[x][y] = true;
							else
								squares[x][y] = false;
						}
					}

					InvalidateRect(hWnd, NULL, TRUE);
				}
			break;
			case IDM_INVERT:
				{
					for (int x = 0; x < X_SIZE / X_RECT; x++)
					{
						for (int y = 0; y < Y_SIZE / Y_RECT; y++)
						{
							squares[x][y] = !(squares[x][y]);
						}
					}

					InvalidateRect(hWnd, NULL, TRUE);
				}
				break;
            default:
                return DefWindowProc(hWnd, message, wParam, lParam);
            }
        }
        break;

	case WM_KEYDOWN:
		{
			switch (wParam)
			{
			case VK_SPACE:
				{
					playing = !playing;
				}
				break;

			case VK_UP:
				{
					if(time_delay!=0)
						time_delay -= 1;
				}
				break;

			case VK_DOWN:
				{
					time_delay += 1;
				}
				break;

			case VK_LEFT:
				{
					if(thickness!=0)
						thickness -= 1;
				}
				break;

			case VK_RIGHT:
				{
					thickness += 1;
				}
				break;

			case VK_NUMPAD1:
				odds = 1;
				break;

			case VK_NUMPAD2:
				odds = 2;
				break;

			case VK_NUMPAD3:
				odds = 3;
				break;

			case VK_NUMPAD4:
				odds = 4;
				break;

			case VK_NUMPAD5:
				odds = 5;
				break;

			case VK_NUMPAD6:
				odds = 6;
				break;

			case VK_NUMPAD7:
				odds = 7;
				break;

			case VK_NUMPAD8:
				odds = 8;
				break;

			case VK_NUMPAD9:
				odds = 9;
				break;
			}
		}
		break;

	case WM_LBUTTONDOWN:
		{
			int xCoords = LOWORD(lParam);
			int yCoords = HIWORD(lParam);

			int XcellClicked = xCoords / X_RECT;
			int YcellClicked = yCoords / Y_RECT;

			playing = false;

			squares[XcellClicked][YcellClicked] = !(squares[XcellClicked][YcellClicked]);

			InvalidateRect(hWnd, NULL, TRUE);
		}
		break;

    case WM_PAINT:
        {
            PAINTSTRUCT ps;
            HDC hdc = BeginPaint(hWnd, &ps);
            // TODO: Add any drawing code that uses hdc here...

			HPEN hPenOld;

			HPEN hRectPen;
			COLORREF qRectColor;
			qRectColor = RGB(0, 0, 0);
			hRectPen = CreatePen(PS_SOLID, thickness, qRectColor);
			hPenOld = (HPEN)SelectObject(hdc, hRectPen);

			//Rectangle(hdc, 100, 100, 250, 250);

			for (int x = 0; x < X_SIZE / X_RECT; x++)
			{
				for (int y = 0; y < Y_SIZE / Y_RECT; y++)
				{
					COLORREF qRectFillColorFull;
					COLORREF qRectFillColorEmpty;
					qRectFillColorFull = RGB(255, 0, 0);
					qRectFillColorEmpty = RGB(255, 255, 255);
					COLORREF chosen = squares[x][y] ? qRectFillColorFull : qRectFillColorEmpty;
					HBRUSH brush = CreateSolidBrush(chosen);

					RECT rect = { X_RECT*x, Y_RECT*y, X_RECT*(x + 1), Y_RECT*(y + 1) };
					if (thickness > 0)
						Rectangle(hdc, X_RECT*x, Y_RECT*y, X_RECT*(x + 1), Y_RECT*(y + 1));
					FillRect(hdc, &rect, brush);

					DeleteObject(brush);
				}
			}

			SelectObject(hdc, hPenOld);
			DeleteObject(hRectPen);

            EndPaint(hWnd, &ps);
        }
        break;
    case WM_DESTROY:
        PostQuitMessage(0);
        break;
    default:
        return DefWindowProc(hWnd, message, wParam, lParam);
    }
    return 0;
}

// Message handler for about box.
INT_PTR CALLBACK About(HWND hDlg, UINT message, WPARAM wParam, LPARAM lParam)
{
    UNREFERENCED_PARAMETER(lParam);
    switch (message)
    {
    case WM_INITDIALOG:
        return (INT_PTR)TRUE;

    case WM_COMMAND:
        if (LOWORD(wParam) == IDOK || LOWORD(wParam) == IDCANCEL)
        {
            EndDialog(hDlg, LOWORD(wParam));
            return (INT_PTR)TRUE;
        }
        break;
    }
    return (INT_PTR)FALSE;
}
