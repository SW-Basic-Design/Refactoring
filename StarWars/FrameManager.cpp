#include "FrameManager.hpp"

void FrameManager::InitFrame()
{
	this->frame.consoleHandler = GetStdHandle(STD_OUTPUT_HANDLE);
	this->frame.currentBufferIndex = 0;

	CONSOLE_CURSOR_INFO consoleCursor{ 1, FALSE };
	CONSOLE_SCREEN_BUFFER_INFO consoleInfo{ 0, };
	GetConsoleScreenBufferInfo(this->frame.consoleHandler, &consoleInfo);
	consoleInfo.dwSize.X = 40;
	consoleInfo.dwSize.Y = 40;

	this->frame.consoleInfo.nWidth = consoleInfo.srWindow.Right - consoleInfo.srWindow.Left;
	this->frame.consoleInfo.nHeight = consoleInfo.srWindow.Bottom - consoleInfo.srWindow.Top;

	this->frame.bufferHandler[0] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(this->frame.bufferHandler[0], consoleInfo.dwSize);
	SetConsoleWindowInfo(this->frame.bufferHandler[0], TRUE, &consoleInfo.srWindow);
	SetConsoleCursorInfo(this->frame.bufferHandler[0], &consoleCursor);

	this->frame.bufferHandler[1] = CreateConsoleScreenBuffer(GENERIC_READ | GENERIC_WRITE, 0, NULL, CONSOLE_TEXTMODE_BUFFER, NULL);
	SetConsoleScreenBufferSize(this->frame.bufferHandler[1], consoleInfo.dwSize);
	SetConsoleWindowInfo(this->frame.bufferHandler[1], TRUE, &consoleInfo.srWindow);
	SetConsoleCursorInfo(this->frame.bufferHandler[1], &consoleCursor);
}

void FrameManager::DestroyFrame()
{
	if (this->frame.bufferHandler[0] != nullptr)
	{
		CloseHandle(this->frame.bufferHandler[0]);
	}

	if (this->frame.bufferHandler[1] != nullptr)
	{
		CloseHandle(this->frame.bufferHandler[1]);
	}
}

void FrameManager::UpdateFrame()
{
	this->ClearScreen();
	this->SwapBuffer();
	this->ClearBuffer();
}

void FrameManager::ClearScreen()
{
	COORD pos = { 0, 0 };
	DWORD dwWritten = 0;
	unsigned int size = this->frame.consoleInfo.nWidth * this->frame.consoleInfo.nHeight;

	FillConsoleOutputCharacter(this->frame.consoleHandler, ' ', size, pos, &dwWritten);
}

void FrameManager::SwapBuffer()
{
	SetConsoleActiveScreenBuffer(this->frame.bufferHandler[this->frame.currentBufferIndex]);
	this->frame.currentBufferIndex = this->frame.currentBufferIndex ? 0 : 1;
}

void FrameManager::Print(const char* str)
{
	COORD coord = GetCursorPosition();

	PrintWithPosition(str, coord);
}

void FrameManager::PrintWithPosition(const char* str, COORD coord)
{
	int len = strlen(str);
	this->SetCursorPosition(coord);
	WriteFile(this->frame.bufferHandler[this->frame.currentBufferIndex], str, len, nullptr, NULL);
}

void FrameManager::ClearBuffer()
{
	const int WIDTH = this->frame.consoleInfo.nWidth;
	const int HEIGHT = this->frame.consoleInfo.nHeight;

	char* line = (char*)malloc(sizeof(char) * (WIDTH + 1));
	for (int i = 0; i < WIDTH; ++i)
		line[i] = ' ';

	for (short i = 0; i < HEIGHT + 10; ++i)
	{
		this->SetCursorPosition({ 0, i });
		WriteFile(this->frame.bufferHandler[this->frame.currentBufferIndex], line, WIDTH, nullptr, NULL);
	}
}

void FrameManager::SetCursorPosition(COORD coord)
{
	SetConsoleCursorPosition(this->frame.bufferHandler[this->frame.currentBufferIndex], coord);
}

COORD FrameManager::GetCursorPosition()
{
	CONSOLE_SCREEN_BUFFER_INFO consoleBufferInfo;
	GetConsoleScreenBufferInfo(this->frame.bufferHandler[this->frame.currentBufferIndex], &consoleBufferInfo);

	return { consoleBufferInfo.dwCursorPosition.X, consoleBufferInfo.dwCursorPosition.Y };
}

void FrameManager::MakeFrame(std::vector<Object*>& objects)
{
	SetCursorPosition({ (short)((objects[0])->GetCoord().getX() * 2), (short)(20 - (objects[0])->GetCoord().getY()) });
	if (((PlayerCharacter*)objects[0])->isFreeze == true && ((PlayerCharacter*)objects[0])->is_attacked == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
	else if (((PlayerCharacter*)objects[0])->isFreeze == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
	else if (((PlayerCharacter*)objects[0])->is_attacked == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
	else
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 10);
	Print("□");

	SetCursorPosition({ (short)((objects[1])->GetCoord().getX() * 2), (short)(20 - (objects[1])->GetCoord().getY()) });

	if (((PlayerCharacter*)objects[1])->isFreeze == true && ((PlayerCharacter*)objects[1])->is_attacked == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
	else if (((PlayerCharacter*)objects[1])->isFreeze == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
	else if (((PlayerCharacter*)objects[1])->is_attacked == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
	else
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 14);

	Print("□");

	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);

	for (std::vector<Object*>::iterator it = objects.begin() + 2; it != objects.end(); ++it)
	{
		SetCursorPosition({ (short)((*it)->GetCoord().getX() * 2), (short)(20 - (*it)->GetCoord().getY()) });

		switch ((*it)->GetObjectType())
		{
		case ObjectType::WALL:
			Print("○");
			break;
		case ObjectType::PARTICLE:
			if (((Particle*)*it)->isMelee)
			{
				if (((Particle*)*it)->getDamage() <= 5)//fist
				{
					if ((*it)->GetVelocity().getX() >= 0)
						Print(" @");
					else
						Print("@ ");
				}
				else
				{
					if ((*it)->GetVelocity().getX() >= 0)
						Print("＼");
					else
						Print("／");
				}
				break;
			}
			if (((Particle*)*it)->isShotgun)
			{
				Print("∴");
				break;
			}

			if (((Particle*)*it)->isBombing)
			{
				Print("⊙");
				break;
			}

			if (((Particle*)*it)->isHatoken)
			{
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() >= 0)
					Print("∩");
				else if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() < 0)
				{
					Print("∪");
				}
				else if ((*it)->GetVelocity().getX() >= 0)
					Print("⊃");
				else
					Print("⊂");
				break;
			}

			if (((Particle*)*it)->getDamage() <= 5)
			{
				Print("·");
			}
			else if (((Particle*)*it)->getDamage() <= 10)
			{
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() != 0)
					Print("ㅣ");
				else
					Print("－");
			}
			else
			{
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() != 0)
					Print("|");
				else
					Print("─");
			}
			break;

		case ObjectType::DROPPED_SPECIAL_ITEM: case ObjectType::DROPPED_WEAPON:
			Print("▣");
			break;

		case ObjectType::FRIENDLY_NPC:
			Print("A");
			break;

		case ObjectType::ENEMY_NPC:
			COORD c = GetCursorPosition();

			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					SetCursorPosition(COORD{ (short)(c.X + i * 2), (short)(c.Y + j) });
					Print("E");
				}
			}

			if (objects[0] == (*it)->getTarget())
				PrintBossHealth(*it, 0);

			else
				PrintBossHealth(*it, 1);
		}


	}

	drawStatus((Character*)objects[0], (Character*)objects[1]);
}

void FrameManager::drawStatus(Character* player1, Character* player2)
{
	SetCursorPosition({ 1, 22 });
	Print("플레이어 1");
	SetCursorPosition({ 1, 23 });
	Print("남은 목숨 : ");
	Print(std::to_string(player1->life).c_str());
	SetCursorPosition({ 1, 24 });
	Print("체력 : ");
	for (int i = 0; i <= 100; i += 10)
	{
		if (player1->getHealth() >= i && player1->getHealth() != 0)
		{
			Print("■");
		}
		else
		{
			Print("□");
		}
	}
	SetCursorPosition({ 1, 25 });
	Print("무기 : ");
	Print(player1->getWeaponName().c_str());
	SetCursorPosition({ 1, 26 });
	Print("탄약 : ");
	Print(std::to_string(player1->bullet_count).c_str());
	SetCursorPosition({ 1, 27 });
	Print("상태 : ");
	Print(player1->getBuffName().c_str());


	SetCursorPosition({ 52, 22 });
	Print("플레이어 2");
	SetCursorPosition({ 52, 23 });
	Print("남은 목숨 : ");
	Print(std::to_string(player2->life).c_str());
	SetCursorPosition({ 52, 24 });
	Print("체력 : ");
	for (int i = 0; i <= 100; i += 10)
	{
		if (player2->getHealth() >= i && player2->getHealth() != 0)
		{
			Print("■");
		}
		else
		{
			Print("□");
		}
	}
	SetCursorPosition({ 52, 25 });
	Print("무기 : ");
	Print(player2->getWeaponName().c_str());
	SetCursorPosition({ 52, 26 });
	Print("탄약 : ");
	Print(std::to_string(player2->bullet_count).c_str());
	SetCursorPosition({ 52, 27 });
	Print("상태 : ");
	Print(player2->getBuffName().c_str());


	SetCursorPosition({ 52, 28 });
	Print(std::to_string(i++).c_str());
}


void FrameManager::MakeStageOverFrame(std::vector<Object*>& objects, Object* dead_player, int flag)
{
	SetCursorPosition({ (short)((objects[0])->GetCoord().getX() * 2), (short)(20 - (objects[0])->GetCoord().getY()) });
	if (((PlayerCharacter*)objects[0])->isFreeze == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
	else if (objects[0] == dead_player && flag == 1)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 13);
	else
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);

	if (objects[0] == dead_player)
	{
		if (flag == 0)
			Print("□");
		else
			Print("■");
	}
	else
		Print("□");



	SetCursorPosition({ (short)((objects[1])->GetCoord().getX() * 2), (short)(20 - (objects[1])->GetCoord().getY()) });

	if (((PlayerCharacter*)objects[1])->isFreeze == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
	else if (objects[1] == dead_player && flag == 1)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 13);
	else
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 14);

	if (objects[1] == dead_player)
	{
		if (flag == 0)
			Print("□");
		else
			Print("■");
	}
	else
		Print("□");

	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);

	for (std::vector<Object*>::iterator it = objects.begin() + 2; it != objects.end(); ++it)
	{
		SetCursorPosition({ (short)((*it)->GetCoord().getX() * 2), (short)(20 - (*it)->GetCoord().getY()) });

		switch ((*it)->GetObjectType())
		{
		case ObjectType::WALL:
			Print("○");
			break;
		case ObjectType::PARTICLE:
			if (((Particle*)*it)->isMelee)
			{
				if (((Particle*)*it)->getDamage() <= 5)//fist
				{
					if ((*it)->GetVelocity().getX() >= 0)
						Print(" @");
					else
						Print("@ ");
				}
				else
				{
					if ((*it)->GetVelocity().getX() >= 0)
						Print("＼");
					else
						Print("／");
				}
				break;
			}
			if (((Particle*)*it)->isShotgun)
			{
				Print("∴");
				break;
			}

			if (((Particle*)*it)->isBombing)
			{
				Print("⊙");
				break;
			}

			if (((Particle*)*it)->isHatoken)
			{
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() >= 0)
					Print("∩");
				else if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() < 0)
				{
					Print("∪");
				}
				else if ((*it)->GetVelocity().getX() >= 0)
					Print("⊃");
				else
					Print("⊂");
				break;
			}

			if (((Particle*)*it)->getDamage() <= 5)
			{
				Print("·");
			}
			else if (((Particle*)*it)->getDamage() <= 10)
			{
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() != 0)
					Print("ㅣ");
				else
					Print("－");
			}
			else
			{
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() != 0)
					Print("|");
				else
					Print("─");
			}
			break;

		case ObjectType::DROPPED_SPECIAL_ITEM: case ObjectType::DROPPED_WEAPON:
			Print("▣");
			break;

		case ObjectType::FRIENDLY_NPC:
			Print("A");
			break;
		}

	}

	drawStatus((Character*)objects[0], (Character*)objects[1]);
}

void FrameManager::PrintOutSideWalls()
{
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			SetCursorPosition({ (short)(x * 2), (short)(y + 1) });
			if (y != 0 && y != 19 && x != 0 && x != 40)
				Print(" ");
			else
				Print("○");
		}
	}
}

void FrameManager::printDeadPlayerMove(Character* player)
{
	int pos_x = player->GetCoord().getX();
	int pos_y = player->GetCoord().getY();

	int dx, dy;
	if (pos_x < 20)
		dx = 1;
	else if (pos_x > 20)
		dx = -1;
	else
		dx = 0;

	if (pos_y < 10)
		dy = 1;
	else if (pos_y > 10)
		dy = -1;
	else
		dy = 0;


	while (pos_x != 20)
	{
		Sleep(25);
		for (int y = 0; y < 20; y++)
		{
			for (int x = 0; x < 41; x++)
			{
				SetCursorPosition({ (short)(x * 2), (short)(y + 1) });
				if (y != 0 && y != 19 && x != 0 && x != 40)
					Print(" ");
				else
					Print("○");
			}
		}

		SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });

		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
		Print("□");
		UpdateFrame();

		pos_x += dx;
	}

	while (pos_y != 10)
	{
		Sleep(25);
		PrintOutSideWalls();
		SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });

		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
		Print("□");
		UpdateFrame();

		pos_y += dy;
	}

	Sleep(100);
	PrintOutSideWalls();
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("■");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("□");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("■");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("□");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("■");
	UpdateFrame();

	Sleep(300);
	PrintOutSideWalls();
	SetCursorPosition({ (short)((pos_x) * 2), (short)(19 - (pos_y - 1)) });
	Print("★");
	UpdateFrame();

	Sleep(500);
	PrintOutSideWalls();
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			SetCursorPosition({ (short)((pos_x + j) * 2), (short)(19 - (pos_y - 1 + i)) });
			if ((i != 0 && j != 0) || (i == 0 && j == 0))
			{
				Print("┼");
			}


		}
	}
	UpdateFrame();

	Sleep(600);
	PrintOutSideWalls();
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			SetCursorPosition({ (short)((pos_x + j) * 2), (short)(19 - (pos_y - 1 + i)) });
			if ((i != 0 && j != 0) || (i == 0 && j == 0))
			{
				Print("·");
			}


		}
	}
	UpdateFrame();

	Sleep(300);
	PrintOutSideWalls();
	UpdateFrame();

	Sleep(300);

}

int FrameManager::gameOverMassage[20][41] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,0,0,0,0,0,1,1,1,0,0,1,1,0,0,0,0,1,1,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,1,1,1,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,0,0,0,1,1,0,0,1,1,0,1,0,0,1,1,1,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,1,0,0,1,0,1,0,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,1,1,1,0,1,0,1,1,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,0,1,0,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,1,1,0,0,0,1,1,1,0,1,0,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
};

void FrameManager::PrintStageOverMassage(int flag)
{

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			SetCursorPosition({ (short)(x * 2), (short)(y + 1) });
			if (gameOverMassage[y][x] == 1)
			{
				if ((x > 0 && x < 40) && (y > 0 && y < 19))
				{
					if (flag == 1)
					{
						SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
					}
					else
					{

					}
					Print("●");
				}
			}
		}
	}


}

int FrameManager::count[5][20][41] = {
			{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,1,1,0,0,0,1,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,1,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
			{1,0,0,0,0,0,1,1,0,0,0,1,1,1,1,0,1,0,0,1,0,1,0,0,1,0,1,0,0,1,0,0,0,1,1,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,1,0,0,1,0,0,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,1,0,0,1,0,0,1,1,1,0,0,1,1,0,1,0,1,1,1,0,0,0,1,1,0,0,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			},
			{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			},
			{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			},
			{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			},
			{
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,1,1,1,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,1,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
			}
};

void FrameManager::PrintCountDown(int flag)
{

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			SetCursorPosition({ (short)(x * 2), (short)(y + 1) });
			if (count[flag][y][x] == 1)
			{
				if ((x > 0 && x < 40) && (y > 0 && y < 19))
				{
					SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
					Print("●");
				}
			}
		}
	}

}

void FrameManager::PrintBossHealth(Object* obj, int index)
{
	EnemyNPC* boss = (EnemyNPC*)obj;

	SetCursorPosition({ (short)(51 * index + 1), 29 });
	Print("BOSS : ");
	for (int i = 0; i <= 100; i += 10)
	{
		if (boss->getHealth() >= i && boss->getHealth() != 0)
		{
			Print("■");
		}
		else
		{
			Print("□");
		}
	}
}