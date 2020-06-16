#include "resource.h"
#include "Character.h"
#include <windows.h>
#include <tchar.h>
#include <map>
#include <list>

/*
재희가 받으면 해야할것
2016000000 유재희 라고 쓴게 프로그램 켰을때 이름인데 그거 학번 수정하기

쓴거
더블버퍼링
*/

HINSTANCE g_hInst; 
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2016000000 유재희";	// 여기 수정하기
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int width = 800, heigh = 600;
int scene = 1;
int playerTimer = 0;

std::list<RECT> poo;
int velocity = 1;
int pooSize = 50;
int createSpeed = 700;

enum SCENE
{
	TITLE,
	INGAME,
	GAMEOVER
};
Character player;

void Render(HDC hdc);
void BackgroundRender(HDC hdc);
void CharacterRender(HDC hdc, int x, int y, int ani, int timer);

int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdParam, int nCmdShow)
{
	HWND hWnd;
	MSG Message;
	WNDCLASSEX WndClass;
	g_hInst = hInstance;

	WndClass.cbSize = sizeof(WndClass);
	WndClass.style = CS_HREDRAW | CS_VREDRAW;
	WndClass.lpfnWndProc = (WNDPROC)WndProc;
	WndClass.cbClsExtra = 0;
	WndClass.cbWndExtra = 0;
	WndClass.hInstance = hInstance;
	WndClass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	WndClass.hCursor = LoadCursor(NULL, IDC_ARROW);
	WndClass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	WndClass.lpszMenuName = NULL;
	WndClass.lpszClassName = lpszClass;
	WndClass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);
	RegisterClassEx(&WndClass);

	hWnd = CreateWindow ( lpszClass, lpszWindowName, WS_OVERLAPPEDWINDOW, 0, 0, width, heigh, NULL ,(HMENU)NULL, hInstance, NULL);

	ShowWindow(hWnd, nCmdShow);
	UpdateWindow(hWnd);

	while (GetMessage(&Message, 0, 0, 0))
	{
		TranslateMessage(&Message);
		DispatchMessage(&Message);
	}
	return Message.wParam;
}

LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam)
{
	PAINTSTRUCT ps;
	HDC hDC, memDC;
	HBITMAP hBit, oldBit;

	RECT rect;
	POINT pt;
	std::list<RECT>::iterator iter;

	switch (iMessage) 
	{ 
	case WM_CREATE:
		SetTimer(hWnd, 1, 60, NULL);
		return 0;

	case WM_PAINT: 

	hDC = BeginPaint(hWnd, &ps);
	memDC = CreateCompatibleDC(hDC);

	hBit = CreateCompatibleBitmap(hDC, width, heigh);
	oldBit = (HBITMAP)SelectObject(memDC, hBit);

	Render(memDC);
	//CharacterRender(memDC, 50, 50, 1, 1);

	BitBlt(hDC, 0, 0, width, heigh, memDC, 0, 0, SRCCOPY);
	SelectObject(memDC, oldBit);
	DeleteDC(memDC);
	DeleteDC(hDC);
	EndPaint(hWnd, &ps);
	break;
	
	case WM_KEYDOWN:
		if (wParam == VK_UP)
			if (player.GetJump() != UP)
			{
				player.SetJump(UP);
				SetTimer(hWnd, 2, 30, NULL);
			}
	break;

	case WM_TIMER:
		switch (wParam)
		{
			case 1:
				playerTimer += 1;
				if (playerTimer > 4)
					playerTimer = 0;
			break;

			case 2:
				player.SetPosY(player.GetPosY() - 10);
				if (player.GetPosY() < 325)
				{ 
					player.SetPosY(325);
					SetTimer(hWnd, 3, 30, NULL);
					KillTimer(hWnd, 2);
				}
			break;

			case 3:
				player.SetPosY(player.GetPosY() + 10);
				if (player.GetPosY() > 425)
				{
					player.SetPosY(425);
					player.SetJump(NONE);
					KillTimer(hWnd, 3);
				}
			break;
		}
		InvalidateRect(hWnd, NULL, false);
	break;

	case WM_DESTROY:
		PostQuitMessage(0);
		KillTimer(hWnd, 1);
		KillTimer(hWnd, 2);
		KillTimer(hWnd, 3);
		return 0;

	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Render(HDC hdc)
{
	if (scene == TITLE)
	{

	}
	else if (scene == INGAME)
	{
		BackgroundRender(hdc);
		if(player.GetJump()==UP)
			CharacterRender(hdc, player.GetPosX(), player.GetPosY(), player.GetJump(), playerTimer);
		else
			CharacterRender(hdc, player.GetPosX(), player.GetPosY(), player.GetAni(), playerTimer);
		player.Input();
	}
	else if (scene == GAMEOVER)
	{

	}
}

void BackgroundRender(HDC hdc)
{
	HBITMAP bg;
	HDC memDC;

	bg = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP2));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, bg);
	BitBlt(hdc, 0, 0, width, heigh, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);

}

void CharacterRender(HDC hdc, int x, int y, int ani, int timer)
{
	HBITMAP character;
	HDC memDC;
	
	character = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP1));
	memDC = CreateCompatibleDC(hdc);

	SelectObject(memDC, character);
	TransparentBlt(hdc, x, y, 50/*원하는크기*/, 80, memDC, timer*27, ani*33, 27, 33, RGB(255, 0, 255));
	DeleteDC(memDC);
}