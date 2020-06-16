#pragma once
#include <windows.h>

#define KEYDOWN(a)	GetAsyncKeyState(a) & 0x8000

enum MOVE
{
	NONE,
	LEFT,
	RIGHT,
	UP
};

class Character
{
private:
	int x , y;
	int ani;
	int hp;
	int jump;


public:
	Character();
	~Character();

	void SetPosX(int x);
	void SetPosY(int y);
	void SetAni(int ani);
	void SetHp(int hp);
	void SetJump(int m);

	int GetPosX() const;
	int GetPosY() const;
	int GetAni() const;
	int GetHp() const;
	int GetJump() const;

	void Input();
};

