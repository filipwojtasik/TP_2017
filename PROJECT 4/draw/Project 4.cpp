// draw.cpp : Defines the entry point for the application.
//

#include "stdafx.h"
#include "Project 4.h"
#include <cstdio>
#include <fstream>
#include <vector>
#include <sstream>

// Global Variables:
HINSTANCE hInst;								// current instance
TCHAR szTitle[MAX_LOADSTRING];					// The title bar text
TCHAR szWindowClass[MAX_LOADSTRING];			// the main window class name

// buttons
HWND hwndButton;
std::vector <double> logs;

const double one_degree = 0.017453;
const double pi = 3.141592653589793;
const int length[] = { 200,180 };
Point XOY(800, 400);
RECT robot = { 
	XOY.X - length[1] - length[0] - radius * 2 ,
	XOY.Y - length[1] - length[0] - radius * 2 ,
	XOY.X + length[1] + length[0] + radius * 2 ,
	XOY.Y + length[1] + length[0] + radius * 2 };
double alpha =  pi/ 4.;
double beta = -pi/6.;
int mode = 0;
int anim = 0;
Point end_pos;
std::vector <object> ball;
//object ball;
// Forward declarations of functions included in this code module:
ATOM				MyRegisterClass(HINSTANCE hInstance);
BOOL				InitInstance(HINSTANCE, int);
LRESULT CALLBACK	WndProc(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	About(HWND, UINT, WPARAM, LPARAM);
INT_PTR CALLBACK	Buttons(HWND, UINT, WPARAM, LPARAM);

Point get_end_position(Point last_end, double angle ,int arm_number)
{
	Point ret;
		ret.X = last_end.X + length[arm_number]*cos(angle);
		ret.Y = last_end.Y - length[arm_number]*sin(angle);
	return ret;
}

std::wstring string_wstring(const std::string& s)
{
    int len;
    int slength = (int)s.length() + 1;
    len = MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, 0, 0); 
    wchar_t* buf = new wchar_t[len];
    MultiByteToWideChar(CP_ACP, 0, s.c_str(), slength, buf, len);
    std::wstring r(buf);
    delete[] buf;
    return r;
}
std::string conv(int val)
{
	std::string buff;
	int h1;
	int h2=0;
	if (val > 9) {
		h2 = 1;
		h1 = val % 10;
		buff.push_back(48 + h1);
		buff.push_back(48 + h2);
	}
	else
	{
		h1 = val;
		buff.push_back(48 + h1);
	}
	return buff;

}
void Show_mass(HDC hdc, int text,int i)
{
	std::wstring stemp = string_wstring(conv(text));
	TextOut(hdc, ball[i].get_ball_loc().X + radius-1, ball[i].get_ball_loc().Y+1, stemp.c_str(), 2);
}
Point draw_arm(HDC hdc, double alpha, double beta)
{
	Graphics graphics(hdc);

	Pen pen(Color(255, 0, 0, 255), 5);
	Pen red(Color(255, 255, 0, 0), 10);

	Point second;
	Point end;

	graphics.DrawLine(&pen, XOY, second = get_end_position(XOY, alpha, 0));

	graphics.DrawLine(&pen, second, end = get_end_position(second, beta, 1));
	graphics.DrawEllipse(&red, second.X - 1, second.Y - 1, 4, 4);
	for (int i = 0; i < ball.size(); i++)
		if (ball[i].get_egs())
			ball[i].draw_object(hdc);
	return end;
}
bool animate(HDC hdc, double alpha_d, double beta_d)
{

	if ((abs(alpha - alpha_d) > one_degree + 0.01) || (abs(beta - beta_d) > one_degree + 0.01))
	{
		if (alpha < alpha_d)alpha = alpha + one_degree;
		if (beta < beta_d)beta = beta + one_degree;
		if (alpha > alpha_d)alpha = alpha - one_degree;
		if (beta > beta_d)beta = beta - one_degree;
		end_pos = draw_arm(hdc, alpha, beta);
		return true;
	}
	else return false;

}
// main function (exe hInstance)
int APIENTRY _tWinMain(HINSTANCE hInstance,
	HINSTANCE hPrevInstance,
	LPTSTR    lpCmdLine,
	int       nCmdShow)
{
	UNREFERENCED_PARAMETER(hPrevInstance);
	UNREFERENCED_PARAMETER(lpCmdLine);

	// TODO: Place code here.
	MSG msg;
	HACCEL hAccelTable;

	GdiplusStartupInput gdiplusStartupInput;
	ULONG_PTR           gdiplusToken;
	GdiplusStartup(&gdiplusToken, &gdiplusStartupInput, NULL);

	// Initialize global strings
	LoadString(hInstance, IDS_APP_TITLE, szTitle, MAX_LOADSTRING);
	LoadString(hInstance, IDC_DRAW, szWindowClass, MAX_LOADSTRING);
	MyRegisterClass(hInstance);



	// Perform application initialization:
	if (!InitInstance(hInstance, nCmdShow))
	{
		return FALSE;
	}

	hAccelTable = LoadAccelerators(hInstance, MAKEINTRESOURCE(IDC_DRAW));

	// Main message loop:
	while (GetMessage(&msg, NULL, 0, 0))
	{
		if (!TranslateAccelerator(msg.hwnd, hAccelTable, &msg))
		{
			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}
	}

	GdiplusShutdown(gdiplusToken);

	return (int)msg.wParam;
}

//
//  FUNCTION: MyRegisterClass()
//
//  PURPOSE: Registers the window class.
//
//  COMMENTS:
//
//    This function and its usage are only necessary if you want this code
//    to be compatible with Win32 systems prior to the 'RegisterClassEx'
//    function that was added to Windows 95. It is important to call this function
//    so that the application will get 'well formed' small icons associated
//    with it.
//
ATOM MyRegisterClass(HINSTANCE hInstance)
{
	WNDCLASSEX wcex;

	wcex.cbSize = sizeof(WNDCLASSEX);

	wcex.style = CS_HREDRAW | CS_VREDRAW;
	wcex.lpfnWndProc = WndProc;
	wcex.cbClsExtra = 0;
	wcex.cbWndExtra = 0;
	wcex.hInstance = hInstance;
	wcex.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(IDI_DRAW));
	wcex.hCursor = LoadCursor(NULL, IDC_ARROW);
	wcex.hbrBackground = (HBRUSH)(COLOR_WINDOW + 1);
	wcex.lpszMenuName = MAKEINTRESOURCE(IDC_DRAW);
	wcex.lpszClassName = szWindowClass;
	wcex.hIconSm = LoadIcon(wcex.hInstance, MAKEINTRESOURCE(IDI_SMALL));

	return RegisterClassEx(&wcex);
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
	HWND hWnd;


	hInst = hInstance; // Store instance handle (of exe) in our global variable

					   // main window
	hWnd = CreateWindow(szWindowClass,
		szTitle,
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT, 0,
		CW_USEDEFAULT, 0,
		NULL, 
		NULL, 
		hInstance,
		NULL);

	// create button and store the handle                                                       
	

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("SAVE"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 00,
		100, 30,
		hWnd,
		(HMENU)ID_BUTTON_SAVE,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("MODE 1"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		100, 110,
		100, 30,
		hWnd,
		(HMENU)ID_BUTTON2,
		GetModuleHandle(NULL),
		NULL);

	hwndButton = CreateWindow(TEXT("button"), TEXT("MODE 2"),
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		100, 155,
		100, 30,
		hWnd,
		(HMENU)ID_BUTTON3,
		GetModuleHandle(NULL),
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("MODE 3"),                  
		WS_CHILD | WS_VISIBLE | BS_AUTORADIOBUTTON,
		100, 200,
		100, 30,
		hWnd,
		(HMENU)ID_BUTTON4,
		hInstance,
		NULL);
	hwndButton = CreateWindow(TEXT("button"),
		TEXT("ANIMATE"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		220, 0,
		100, 30,
		hWnd,
		(HMENU)ID_BUTTON_ANIMATE,
		hInstance,
		NULL);

	hwndButton = CreateWindow(TEXT("button"),
		TEXT("BALL"),
		WS_CHILD | WS_VISIBLE | BS_PUSHBUTTON,
		100, 245,
		100, 30,
		hWnd,
		(HMENU)ID_CREATE_BALLS,
		hInstance,
		NULL);
	
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
//  WM_COMMAND	- process the application menu
//  WM_PAINT	- Paint the main window (low priority)
//  WM_DESTROY	- post a quit message and return
//
//
LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam)
{
	int wmId, wmEvent;
	PAINTSTRUCT ps;
	HDC hdc;

	switch (message)
	{
	case WM_COMMAND:
		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);

		// MENU & BUTTON messages
		// Parse the menu selections:
		switch (wmId)
		{
		case IDM_ABOUT:
			DialogBox(hInst, MAKEINTRESOURCE(IDD_ABOUTBOX), hWnd, About);
			break;
		case IDM_EXIT:
			DestroyWindow(hWnd);
			break;
		case ID_BUTTON_SAVE:
			logs.push_back(alpha);
			logs.push_back(beta);
			break;
		case ID_BUTTON2:
			mode = 0;
			break;
		case ID_BUTTON3:
			mode = 1;
			break;
		case ID_BUTTON4:
			mode = 2;		
			break;
		case ID_BUTTON_ANIMATE:
			if (logs.size() != 0)
				SetTimer(hWnd, TMR_ANIMATION, 20, 0);
			break;
		case ID_CREATE_BALLS:
		{
			SetTimer(hWnd, TMR_BALLS, 20, 0);
			object b(700, 300, rand()%10 , FALSE);
			ball.push_back(b);
			break;
		}
		default:
			return DefWindowProc(hWnd, message, wParam, lParam);
		}
		SetFocus(hWnd);
		SetActiveWindow(hWnd);
		break;
	case WM_KEYDOWN:

		wmId = LOWORD(wParam);
		wmEvent = HIWORD(wParam);
		switch (wmId)
		{
		case VK_LEFT:
			if (mode == 0)alpha = alpha + one_degree;
			else if (mode == 1)
			{
				alpha = alpha + one_degree;
				beta = beta + one_degree;
			}
			else if (mode == 2);
			break;
		case VK_RIGHT:
			if (mode == 0)alpha = alpha - one_degree;
			else if (mode == 1)
			{
				alpha = alpha - one_degree;
				beta = beta - one_degree;
			}
			else if (mode == 2);
			break;
		case VK_UP:
			beta = beta + one_degree;
			break;
		case VK_DOWN:
			beta = beta - one_degree;			
			break;
		case VK_SPACE:
			for (int i = 0; i < ball.size(); i++) 
			{
				if (!ball[i].get_egs())
				{
					if (ball[i].catch_ball(end_pos))
					//KillTimer(hWnd, TMR_BALLS);
					ball[i].set_egs(true);
					
				}
				else 
				{
					ball[i].set_egs(false);
					SetTimer(hWnd, TMR_BALLS, 20, 0);
					ball[i].stop();
				}
			}
			
			
			break;
		case VK_RETURN:
			logs.push_back(alpha);
			logs.push_back(beta);
		}

		if  (alpha > 2 * pi)alpha = alpha - 2 * pi;
		if (beta > 2 * pi)beta = beta - 2 * pi;
		if (alpha < -2*pi)alpha = alpha + 2 * pi;
		if (beta < -2*pi)beta = beta + 2 * pi;

		InvalidateRect(hWnd, &robot, TRUE);

		break;
	case WM_PAINT:
		hdc = BeginPaint(hWnd, &ps);
		MoveToEx(hdc, robot.left, robot.top, 0);
		LineTo(hdc, robot.left, 524);
		LineTo(hdc, robot.right, 524);
		LineTo(hdc, robot.right, robot.top);
		LineTo(hdc, robot.left, robot.top);
		end_pos = draw_arm(hdc, alpha, beta);
		for (int i = 0; i < ball.size(); i++)
		{
			Show_mass(hdc, ball[i].get_mass(),i);
			ball[i].draw_object(hdc);
			
			if (ball[i].get_egs())ball[i].catch_ball(end_pos);
		}
		EndPaint(hWnd, &ps);
		break;
	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	case WM_TIMER:
		hdc = BeginPaint(hWnd, &ps);
		switch (wParam)
		{
		case TMR_ANIMATION:
			if (!animate(hdc, logs[anim], logs[anim + 1]))
				anim = anim + 2;
			InvalidateRect(hWnd, &robot, TRUE);
			
			if (anim >= logs.size())
			{
				KillTimer(hWnd, TMR_ANIMATION);
				anim = 0;
			}
			break;
		case TMR_BALLS:
			//ball.draw_object(hdc);
			InvalidateRect(hWnd, &robot, TRUE);

			for(int i=0;i<ball.size();i++)ball[i].freefall();
			break;
		}
		EndPaint(hWnd, &ps);
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
