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
	PrintAttackedEffect(objects[0], 10);

	SetCursorPosition({ (short)((objects[1])->GetCoord().getX() * 2), (short)(20 - (objects[1])->GetCoord().getY()) });
	PrintAttackedEffect(objects[1], 14);

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

			if (((Particle*)*it)->isLaser)
			{
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() != 0)
					Print("|");
				else
					Print("─");
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
			}
			else
			{
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
			}

			break;

		case ObjectType::DROPPED_SPECIAL_ITEM: case ObjectType::DROPPED_WEAPON:
			Print("▣");
			break;

		case ObjectType::FRIENDLY_NPC:
			PrintAttackedEffect(*it, 15);
			break;

		case ObjectType::ENEMY_NPC:
			PrintAttackedEffect(*it, 15);

			if (objects[0] == (*it)->getTarget())
				PrintBossHealth(*it, 0);

			else
				PrintBossHealth(*it, 1);

			break;
		}
	}
	drawStatus((PlayerCharacter*)objects[0], (PlayerCharacter*)objects[1]);
}

void FrameManager::drawStatus(PlayerCharacter* player1, PlayerCharacter* player2)
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
	SetCursorPosition({ 1, 28 });
	Print((std::to_string(player1->missed_bullet) + " / " + std::to_string(player1->shot_bullet)).c_str());


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
	Print((std::to_string(player2->missed_bullet) + " / " + std::to_string(player2->shot_bullet)).c_str());
}


void FrameManager::MakeStageOverFrame(std::vector<Object*>& objects, Object* dead_player, int flag)
{
	SetCursorPosition({ (short)((objects[0])->GetCoord().getX() * 2), (short)(20 - (objects[0])->GetCoord().getY()) });
	if (((PlayerCharacter*)objects[0])->isFreeze == true)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
	else if (objects[0] == dead_player && flag == 1)
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 13);
	else
		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 10);

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

			if (((Particle*)*it)->isLaser)
			{
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
				if ((*it)->GetVelocity().getX() == 0 && (*it)->GetVelocity().getY() != 0)
					Print("|");
				else
					Print("─");
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
			}
			else
			{
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

	drawStatus((PlayerCharacter*)objects[0], (PlayerCharacter*)objects[1]);
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

void FrameManager::printDeadPlayerMove(Character* player, int number)
{
	int pos_x = player->GetCoord().getX();
	int pos_y = player->GetCoord().getY();

	int dx, dy;

	int player_color;
	if (number == 0)
		player_color = 12;
	else
		player_color = 14;


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

		this->PrintOutSideWalls();



		SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });

		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
		Print("□");
		UpdateFrame();

		pos_x += dx;
	}

	while (pos_y != 10)
	{
		Sleep(25);
		PrintOutSideWalls();
		SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });

		SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
		Print("□");
		UpdateFrame();

		pos_y += dy;
	}

	Sleep(100);
	PrintOutSideWalls();
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("■");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("□");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("■");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("□");
	UpdateFrame();

	Sleep(100);
	PrintOutSideWalls();
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
	SetCursorPosition({ (short)(pos_x * 2), (short)(19 - (pos_y - 1)) });
	Print("■");
	UpdateFrame();

	Sleep(300);
	PrintOutSideWalls();
	SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
	SetCursorPosition({ (short)((pos_x) * 2), (short)(19 - (pos_y - 1)) });
	Print("★");
	UpdateFrame();

	Sleep(500);
	PrintOutSideWalls();
	for (int i = -1; i <= 1; i++)
	{
		for (int j = -1; j <= 1; j++)
		{
			SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], player_color);
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
		{1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,1,0,1,0,1},
		{1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,1,0,0,1},
		{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,0,0,0,0,0,1,0,1,0,1},
		{1,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,1},
		{1,0,0,0,0,0,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1},
		{1,0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,1,0,0,0,0,0,1},
		{1,0,0,1,0,1,1,0,1,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,1,0,0,0,0,1},
		{1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,1,1,0,0,1},
		{1,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
		{0,0,0,0,0,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1}
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
			{1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,0,1,1,1,0,0,1,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,1},
			{1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,1,0,0,1},
			{1,0,0,1,1,1,1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,0,0,1},
			{1,0,0,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
			{1,0,0,1,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
			{1,0,0,1,0,0,1,0,0,1,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
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
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,0,1,0,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,1,0,1},
			{1,0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,0,1,0,0,0,1,1,0,1,0,1,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0,0,1},
			{1,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1},
			{1,0,0,1,0,0,0,0,0,1,1,1,1,1,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,1,0,0,0,1,1,0,0,0,0,0,1},
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
	for (int i = 0; i <= 300; i += 30)
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

void FrameManager::drawGameStart(int flag)
{

	int data[20][41] = {
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,1,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,1,0,1,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,0,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0,1,1,1,0},
		{0,1,0,0,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0},
		{0,1,1,1,0,0,1,0,0,1,1,1,0,1,1,0,0,1,0,1,0,0,1,0,0,1,1,1,0,0,1,0,0,1,0,1,0,1,1,0,0},
		{0,0,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,0,1,0,0,1,0,0,1,0,1,0,0,1,0,0,1,0,1,0,1,0,1,0},
		{0,1,1,1,0,0,1,0,0,1,0,1,0,1,0,1,0,1,1,0,0,1,1,1,0,1,0,1,0,0,1,0,0,1,1,1,0,1,0,1,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
		{1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,1,0}
	};
	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			SetCursorPosition({ (short)(x * 2), (short)(y + 1) });
			if (data[y][x] == 1)
			{
				Print("●");
			}
		}
	}

	if (flag == 1)
	{

		string str = "Press Any Key To Start Game";
		SetCursorPosition({ (short)(40 - (str.length() / 2)), 18 });
		Print(str.c_str());
	}
}

void FrameManager::drawGameStory()
{
	string story[20] = {
		"플레이어들은 우주 여행을 즐기는 우주 모험가로,",
		"여러 은하를 누비며 모험을 하던중",
		"강력한 중력을 지닌 블랙홀을 만나",
		"순식간에 길을 잃고 외계 행성에 불시착합니다.",
		"그리고 그들은 그곳의 알 수 없는 생명체들에게 납치되어",
		"거대하고도 복잡한 투기장으로 끌려갑니다.",
		" ",
		"미지의 생명체들은. ",
		"\"이 투기장에서 빠져나갈 수 있는 사람은 단 1명이며,",
		"패배자들은 이 행성의 노예로서",
		"남은 인생을 살아가야 한다\" 라고 이야기 합니다.",
		" ",
		"투기장은 다양한 환경과 독특한 지형을 가진",
		"다섯 개의 지역으로 나뉘어져 있으며,",
		"플레이어들은 투기장 곳곳에 있는 각종 무기를 습득해",
		"서로 싸우게 됩니다.",
		" ",
		"지금까지 플레이어들은 서로 동료였지만,",
		"이제는 각자 자기자신의 생존을 위해 투기장에서 싸워야만 합니다.",
		"그들의 처절한 이야기가 지금 바로 시작됩니다." };


	int story_length = 20;

	string cur_story[10];

	int cnt = 0;

	char c = _getch();

	auto milli = GetTickCount64();
	auto last_milli = 0;

	while (story_length + 10 > cnt)
	{
		if (last_milli == 0 || last_milli + 1000 < milli)
		{
			last_milli = milli;

			this->PrintOutSideWalls();

			for (int i = 0; i < 9; i++)
			{
				cur_story[9 - i] = cur_story[9 - i - 1];
				SetCursorPosition({ (short)(40 - (cur_story[9 - i].length() / 2)), (short)(5 + i + 1) });
				Print(cur_story[9 - i].c_str());
			}
			if (cnt < story_length)
				cur_story[0] = story[cnt];
			else
				cur_story[0] = "";

			SetCursorPosition({ (short)(40 - (cur_story[0].length() / 2)), (short)(15) });
			Print(cur_story[0].c_str());

			cnt++;

			string str = "Press ENTER Key To Skip";
			SetCursorPosition({ (short)(40 - (str.length() / 2)), 18 });
			Print(str.c_str());
			this->UpdateFrame();
		}

		if (_kbhit())
		{
			c = _getch();

			if (c == VK_RETURN)
			{
				return;
			}

		}

		milli = GetTickCount64();
	}
}

void FrameManager::drawGameModeSelectScreen(bool isPVP)
{
	this->PrintOutSideWalls();

	string str = "게임 모드를 선택하세요!";

	SetCursorPosition({ (short)(40 - (str.length() / 2)), 5 });
	Print(str.c_str());


	if (!isPVP)
	{
		SetCursorPosition({ (short)(16), 10 });
		Print(">>");
	}
	SetCursorPosition({ (short)(20), 10 });
	Print("PVE 모드");

	if (isPVP)
	{
		SetCursorPosition({ (short)(50), 10 });
		Print(">>");
	}
	SetCursorPosition({ (short)(54), 10 });
	Print("PVP 모드");

}

void FrameManager::drawHowToControl(bool isPVP)
{
	this->PrintOutSideWalls();
	string str = "플레이어 1 조작법";
	SetCursorPosition({ (short)(40 - (str.length() / 2)), 3 });
	Print(str.c_str());

	str = "W, A, S, D : 좌, 우, 아래, 위 방향 이동";
	SetCursorPosition({ (short)(40 - (str.length() / 2)), 4 });
	Print(str.c_str());

	str = "G : 무기 발사";
	SetCursorPosition({ (short)(40 - (str.length() / 2)), 5 });
	Print(str.c_str());

	if (isPVP)
	{
		str = "플레이어 2 조작법";
		SetCursorPosition({ (short)(40 - (str.length() / 2)), 7 });
		Print(str.c_str());

		str = "각 방향키 : 좌, 우, 아래, 위 방향 이동";
		SetCursorPosition({ (short)(40 - (str.length() / 2)), 8 });
		Print(str.c_str());

		str = "NUM 5 : 무기 발사";
		SetCursorPosition({ (short)(40 - (str.length() / 2)), 9 });
		Print(str.c_str());
	}

	str = "플레이 방법";
	SetCursorPosition({ (short)(40 - (str.length() / 2)), 11 });
	Print(str.c_str());

	str = "1. 일반 스테이지에서 상대 플레이어를 공격하세요.";
	SetCursorPosition({ (short)(14), 12 });
	Print(str.c_str());

	str = "- 맵 밖으로 밀려나면 즉시 라운드를 패배하니 주의하세요!";
	SetCursorPosition({ (short)(16), 13 });
	Print(str.c_str());

	str = "2. 아이템 상자를 획득하여 새로운 무기나 능력을 얻으세요.";
	SetCursorPosition({ (short)(14), 14 });
	Print(str.c_str());

	str = "3. 보스를 먼저 처치하여 특수 아이템을 획득하세요.";
	SetCursorPosition({ (short)(14), 15 });
	Print(str.c_str());

	str = "4. 상대 플레이어의 목숨을 먼저 소진시켜 승리를 쟁취하세요.";
	SetCursorPosition({ (short)(14), 16 });
	Print(str.c_str());




	str = "Press Any Key To Skip";
	SetCursorPosition({ (short)(40 - (str.length() / 2)), 18 });
	Print(str.c_str());
}

void FrameManager::MakeBossDeadFrame(std::vector<Object*>& objects, Object* dead_boss, int flag)
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

		case ObjectType::ENEMY_NPC:

			if ((*it) == dead_boss)
			{
				if (flag == 1)
				{
					SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
				}
				if (flag == 2)
				{
					continue;
				}
			}


			COORD c = GetCursorPosition();

			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					SetCursorPosition(COORD{ (short)(c.X + i * 2), (short)(c.Y + j) });
					Print("E");
				}
			}

			SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
		}


	}
}

void FrameManager::drawGameOverScene(int winner_id, int flag)
{
	int massage[2][20][41] = {
	{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,0,0,0,1,0,1,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,0,1,1,1,0,0,0,1,0,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	},
	{
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,0,0,0,1,0,1,0,0,1,1,0,1,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,1,1,1,0,1,0,0,0,1,0,1,0,1,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,0,0,0,1,0,1,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,1,0,0,0,1,0,1,1,1,1,1,0,1,1,1,1,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,1,0,0,0,1,0,0,1,0,1,0,0,1,0,0,0,0,0,1,0,0,1,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,1,0,0,0,1,1,1,1,1,0,1,0,0,0,1,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,1,1,1,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,0,0,1,0,0,0,1,0,1,0,1,0,1,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,1,0,0,0,1,0,0,0,1,1,0,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,1,1,1,0,0,0,1,1,1,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,1,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,1,0,0,0,0,0,1,0,1,0,1,0,0,0,1,0,0,0,1,0,0,1,1,0,0,0,0,0,0},
	{0,0,0,0,0,0,1,0,0,0,0,0,1,1,1,1,1,0,0,1,0,1,0,0,0,1,1,1,0,0,1,0,0,0,1,0,0,1,0,0,0},
	{0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0,0}
	} };

	for (int y = 0; y < 20; y++)
	{
		for (int x = 0; x < 41; x++)
		{
			SetCursorPosition({ (short)(x * 2), (short)(y + 1) });
			if (massage[winner_id][y][x] == 1)
			{
				if (y < 13 && flag == 1)
				{
					SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
				}

				else
				{
					SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
				}

				Print("●");
			}
		}
	}

	void FrameManager::PrintAttackedEffect(Object * obj, int color_code)
	{
		switch (obj->GetObjectType())
		{
		case ObjectType::PLAYER_CHARACTER:
			if (((PlayerCharacter*)obj)->isFreeze == true && ((PlayerCharacter*)obj)->is_attacked == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
			else if (((PlayerCharacter*)obj)->isFreeze == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
			else if (((PlayerCharacter*)obj)->is_attacked == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
			else
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], color_code);

			Print("□");
			break;
		case ObjectType::ENEMY_NPC:
			COORD c = GetCursorPosition();
			if (((EnemyNPC*)obj)->isFreeze == true && ((EnemyNPC*)obj)->is_attacked == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
			else if (((EnemyNPC*)obj)->isFreeze == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
			else if (((EnemyNPC*)obj)->is_attacked == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
			else
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);

			for (int i = -1; i <= 1; ++i)
			{
				for (int j = -1; j <= 1; ++j)
				{
					SetCursorPosition(COORD{ (short)(c.X + i * 2), (short)(c.Y + j) });
					if (i == -1 && j == -1)
						Print("┌");
					if (i == -1 && j == 0)
						Print("├");
					if (i == -1 && j == 1)
						Print("└");
					if (i == 0 && j == -1)
						Print("┬");
					if (i == 0 && j == 0)
						Print("⊙");
					if (i == 0 && j == 1)
						Print("┴");
					if (i == 1 && j == -1)
						Print("┐");
					if (i == 1 && j == 0)
						Print("┤");
					if (i == 1 && j == 1)
						Print("┘");
				}
			}
			SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
			break;
		case ObjectType::FRIENDLY_NPC:
			if (((FriendlyNPC*)obj)->isFreeze == true && ((FriendlyNPC*)obj)->is_attacked == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
			else if (((FriendlyNPC*)obj)->isFreeze == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 9);
			else if (((FriendlyNPC*)obj)->is_attacked == true)
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 12);
			else
				SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], color_code);

			Print("◈");
			SetConsoleTextAttribute(this->frame.bufferHandler[this->frame.currentBufferIndex], 15);
			break;
		default:
			break;
		}
	}
}