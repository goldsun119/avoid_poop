#include "Character.h"



Character::Character()
{
	x = 383, y = 425;
	hp = 3;
	box.left = x;
	box.top = y;
	box.right = x + 50;
	box.bottom = y + 80;
}


Character::~Character()
{
}

void Character::SetPosX(int x)
{
	Character::x = x;

	box.left = Character::x;
	box.top = Character::y;
	box.right = Character::x + 50;
	box.bottom = Character::y + 80;
}
void Character::SetPosY(int y)
{
	Character::y = y;
	box.left = Character::x;
	box.top = Character::y;
	box.right = Character::x + 50;
	box.bottom = Character::y + 80;
}
void Character::SetAni(int ani)
{
	Character::ani = ani;
}
void Character::SetHp(int hp)
{
	Character::hp = hp;
}
void Character::SetJump(int m)
{
	Character::jump = m;
}

int Character::GetPosX() const
{
	return x;
}

int Character::GetPosY() const
{
	return y;
}
int Character::GetAni() const
{
	return ani;
}

int Character::GetHp() const
{
	return hp;
}
int Character::GetJump() const
{
	return jump;
}

RECT Character::GetBox() const
{
	return box;
}

void Character::Input()
{
	ani = NONE;
	if (GetKeyState(VK_LEFT) & 0x8000)
	{
		if (jump != UP)
		{
			ani = LEFT;
			x -= 15;
			if (x < 0)
				x = 0;
			box.left = Character::x;
			box.right = Character::x + 50;
		}
	}

	else if (GetKeyState(VK_RIGHT) & 0x8000)
	{
		if (jump != UP)
		{
			ani = RIGHT;
			x += 15;
			if (x > 740)
				x = 740;
			box.left = Character::x;
			box.right = Character::x + 50;
		}
	}

	//else if (GetKeyState(VK_UP) & 0x8000)
	//{
	//	ani = UP;
	//	jump = UP;
	//}

}