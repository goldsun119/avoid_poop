#include "resource.h"
#include "Character.h"
#include <windows.h>
#include <tchar.h>
#include <map>
#include <list>
#pragma comment(lib, "msimg32.lib") 
/*
재희가 받으면 해야할것
2016000000 유재희 라고 쓴게 프로그램 켰을때 이름인데 그거 학번 수정하기

쓴거
더블버퍼링말곤 뭐 모르겠네..?
*/

HINSTANCE g_hInst; 
LPCTSTR lpszClass = L"Window Class Name";
LPCTSTR lpszWindowName = L"2016000000 유재희";	// 여기 수정하기
LRESULT CALLBACK WndProc(HWND hWnd, UINT iMessage, WPARAM wParam, LPARAM lParam);

int width = 800, heigh = 600;
int scene = 0;
int playerTimer = 0;

std::list<RECT> poo;
int velocity = 1;
int pooSize = 50;
int createSpeed = 700;

int mouse = 0;

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
void PooRender(HDC hdc);
void HpRender(HDC hdc);
void GameoverRender(HDC hdc);
void TitleRender(HDC hdc);
void ButtonRender(HDC hdc);
void ButtonRender2(HDC hdc);

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

		return 0;

	case WM_PAINT: 

	hDC = BeginPaint(hWnd, &ps);
	memDC = CreateCompatibleDC(hDC);

	hBit = CreateCompatibleBitmap(hDC, width, heigh);
	oldBit = (HBITMAP)SelectObject(memDC, hBit);

	Render(memDC);

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

	case WM_MOUSEMOVE:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (x > 200 && x < 600 && y>420 && y < 500)
		{
			mouse = 1;
		}
		else mouse = 0;
		InvalidateRect(hWnd, NULL, false);
	}
		break;

	case WM_LBUTTONDOWN:
	{
		int x = LOWORD(lParam);
		int y = HIWORD(lParam);
		if (x > 200 && x < 600 && y>420 && y < 500)
		{
			scene = SCENE::INGAME;
			SetTimer(hWnd, 1, 60, NULL);
			SetTimer(hWnd, 4, 20, NULL);
			SetTimer(hWnd, 5, createSpeed, NULL);
			SetTimer(hWnd, 6, 10000, NULL);
		}
	}
	break;

	case WM_TIMER:
		switch (wParam)
		{
			case 1:
			{
				playerTimer += 1;
				if (playerTimer > 4)
					playerTimer = 0; 
			}
			break;

			case 2:
			{	
				player.SetPosY(player.GetPosY() - 10);
				if (player.GetPosY() < 325)
				{
					player.SetPosY(325);
					SetTimer(hWnd, 3, 30, NULL);
					KillTimer(hWnd, 2);
				}
			}
			break;

			case 3:
			{
				player.SetPosY(player.GetPosY() + 10);
				if (player.GetPosY() > 425)
				{
					player.SetPosY(425);
					player.SetJump(NONE);
					KillTimer(hWnd, 3);
				}
			}
			break;

			case 4:
			{
				iter = poo.begin();
				while (iter != poo.end())
				{
					iter->top += velocity;
					iter->bottom += velocity;
					if (iter->bottom > 500) 
						iter = poo.erase(iter);
					else iter++;
				}
				InvalidateRect(hWnd, NULL, false);
			}
			break;

			case 5:
			{
				//사각형 생성
				pt = { rand() % 800, 50 };
				rect = { pt.x - pooSize/2, pt.y - pooSize/2, pt.x + pooSize/2, pt.y + pooSize/2 };
				poo.push_back(rect);
			}
			break;

			case 6:
			{
				if (velocity < 20) velocity += 1;
				if (createSpeed > 300) createSpeed -= 50;
				//생성속도 갱신을 위해 Timer 재생성.
				KillTimer(hWnd, 5);
				(HANDLE)SetTimer(hWnd, 5, createSpeed, NULL);
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
		KillTimer(hWnd, 4);
		KillTimer(hWnd, 5);
		KillTimer(hWnd, 6);
		return 0;

	}
	return (DefWindowProc(hWnd, iMessage, wParam, lParam));
}

void Render(HDC hdc)
{
	if (scene == TITLE)
	{
		TitleRender(hdc);
		if(mouse == 0)
			ButtonRender(hdc);
		else
			ButtonRender2(hdc);
	}
	else if (scene == INGAME)
	{
		BackgroundRender(hdc);
		PooRender(hdc);
		HpRender(hdc);
		if(player.GetJump()==UP)
			CharacterRender(hdc, player.GetPosX(), player.GetPosY(), player.GetJump(), playerTimer);
		else
			CharacterRender(hdc, player.GetPosX(), player.GetPosY(), player.GetAni(), playerTimer);
		player.Input();

	}
	else if (scene == GAMEOVER)
	{
		GameoverRender(hdc);
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

void PooRender(HDC hdc)
{
	HBITMAP obstacle;
	HDC memDC;
	std::list<RECT>::iterator iter;

	RECT tempRc, character, rc;
	BOOL collision;
	
	character = player.GetBox();
	obstacle = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP3));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, obstacle);

	iter = poo.begin();
	while (iter != poo.end())
	{
		rc = *iter;
		collision = IntersectRect(&tempRc, &character, &rc);
		//Rectangle(hdc, iter->left, iter->top, iter->right, iter->bottom);	//충돌체크 박스 임시 그리기
		//Rectangle(hdc, character.left, character.top, character.right, character.bottom); //충돌체크 박스 임시 그리기
		if (collision == TRUE)
		{
			player.SetHp(player.GetHp() - 1);
			if (player.GetHp() < 1)	scene = GAMEOVER;
			iter = poo.erase(iter);
			continue;
		}
		TransparentBlt(hdc, iter->left, iter->top, pooSize/*원하는크기*/, pooSize, memDC, 0, 0, 40, 35, RGB(255, 0, 255));
		//Rectangle(hdc, iter->left, iter->top, iter->right, iter->bottom);
		iter++;
	}
	DeleteDC(memDC);
}

void HpRender(HDC hdc)
{
	HBITMAP heart;
	HDC memDC;

	heart = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP4));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, heart);
	for (int i = player.GetHp(); i > 0; --i)
	{
		TransparentBlt(hdc, i*40-40, 0, 40/*원하는크기*/, 40, memDC, 0, 0, 50, 50, RGB(255, 0, 255));
	}
	DeleteDC(memDC);

}

void GameoverRender(HDC hdc)
{
	HBITMAP bg;
	HDC memDC;

	bg = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP5));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, bg);
	StretchBlt(hdc, 0, 0, width, heigh, memDC, 0, 0, 590, 332, SRCCOPY);
	DeleteDC(memDC);
}


void TitleRender(HDC hdc)
{
	HBITMAP bg;
	HDC memDC;

	bg = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP6));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, bg);
	BitBlt(hdc, 0, 0, width, heigh, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);
}

void ButtonRender(HDC hdc)
{
	HBITMAP bg;
	HDC memDC;

	bg = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP7));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, bg);
	BitBlt(hdc, 200, 350, 400, 150, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);
}

void ButtonRender2(HDC hdc)
{
	HBITMAP bg;
	HDC memDC;

	bg = (HBITMAP)LoadBitmap(g_hInst, MAKEINTRESOURCE(IDB_BITMAP8));
	memDC = CreateCompatibleDC(hdc);
	SelectObject(memDC, bg);
	BitBlt(hdc, 200, 350, 400, 150, memDC, 0, 0, SRCCOPY);
	DeleteDC(memDC);
}