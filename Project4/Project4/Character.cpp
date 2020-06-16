#include "Character.h"



Character::Character()
{
	x = 383, y = 425;
}


Character::~Character()
{
}

void Character::SetPosX(int x)
{
	Character::x = x;
}
void Character::SetPosY(int y)
{
	Character::y = y;
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
		}
	}

	//else if (GetKeyState(VK_UP) & 0x8000)
	//{
	//	ani = UP;
	//	jump = UP;
	//}

}