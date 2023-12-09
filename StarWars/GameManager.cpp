#include "GameManager.hpp"

GameManager::GameManager()
{
	this->game = new Game(false);
	this->last_item_SpawnTime = 0;
}

void GameManager::StartGame()
{
	this->frameManager.InitFrame();

	this->waitForStart();

	this->showGameStory();

	this->gameModeSelecter();

	this->showHowToControl();

	this->game->MakePlayer();
	this->game->MakeMap(this->game->current_stage);
	
	this->game->difficulty = 0;

	this->showCountDown();

	while (PrecedeGame())
	{
		this->GetPlayerKeyInput();
	}

	this->showGameOver();

	Sleep(100);
}

bool GameManager::PrecedeGame()
{
	if (!this->game->IsGameOver())
	{
		if (!this->game->IsStageOver())
		{
			auto milli = GetTickCount64();

			if (this->game->current_stage % 2 == 0)
			{
				if (last_item_SpawnTime + spawn_term < milli)
				{
					last_item_SpawnTime = milli;
					game->MakeItem();
				}
			}

			else
			{
				if (last_item_SpawnTime + spawn_term / 2 < milli)
				{
					last_item_SpawnTime = milli;
					game->MakeItem();
				}
			}

			this->game->UpdateObjects();
			this->game->UpdateObjectPosition();
			this->game->UpdateMap();

			this->frameManager.MakeFrame(this->game->GetObjects());
			this->frameManager.UpdateFrame();
		}

		else
		{
			Character* loser = (Character*)this->game->getGameOverPlayer();

			if (game->current_stage % 2 == 0)
			{
				loser->life -= 1;
			}

			if (((Character*)this->game->GetObjects()[0])->life <= 0 || ((Character*)this->game->GetObjects()[1])->life <= 0)
			{
				this->game->SetGameOver(true);
			}

			else
			{

				if (game->current_stage % 2 == 0)
				{
					this->showStageOverScene();
				}

				else
				{
					if (loser == nullptr)
					{
						this->showBossEliminated();
					}

					else
					{
						this->showStageOverScene();
					}
				}

				this->resetStage();
				this->gotoNextStage();
				this->showCountDown();
			}

			this->game->SetStageOver(false);
		}

		return true;
	}

	return false;
}

void GameManager::GetPlayerKeyInput()
{
	PlayerCharacter* player1 = (PlayerCharacter*)(game->GetObjects()[0]);
	PlayerCharacter* player2 = (PlayerCharacter*)(game->GetObjects()[1]);

	if (GetAsyncKeyState(0x41))
	{
		player1->SetVelocity({ -1, 0 });
		player1->direction = { -1, 0 };
	}

	if (GetAsyncKeyState(0x44))
	{
		player1->direction = { 1, 0 };
		player1->SetVelocity({ 1, 0 });
	}

	else if (!GetAsyncKeyState(0x41) && !GetAsyncKeyState(0x44))
		player1->GetVelocity().setX(0);

	if (GetAsyncKeyState(0x47))
		game->CharacterShoot(player1);

	if (GetAsyncKeyState(VK_LEFT))
	{
		player2->SetVelocity({ -1, 0 });
		player2->direction = { -1, 0 };
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		player2->direction = { 1, 0 };
		player2->SetVelocity({ 1, 0 });
	}

	else if (!GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT))
		player2->GetVelocity().setX(0);

	if (GetAsyncKeyState(VK_NUMPAD5))
		game->CharacterShoot(player2);

	if (GetAsyncKeyState(0x57))
	{
		player1->direction = { 0, 1 };
		player1->SetVelocity({ 0, 1 });
	}

	if (GetAsyncKeyState(VK_UP))
	{
		player2->direction = { 0, 1 };
		player2->SetVelocity({ 0, 1 });
	}

	else if (!GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x53))
		player1->GetVelocity().setY(0);

	if (GetAsyncKeyState(0x53))
	{
		player1->direction = { 0, -1 };
		player1->SetVelocity({ 0, -1 });
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		player2->direction = { 0, -1 };
		player2->SetVelocity({ 0, -1 });
	}

	else if (!GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
		player2->GetVelocity().setY(0);
}

void GameManager::showStageOverScene()
{
	int player_number;

	Object* dead_player = this->game->getGameOverPlayer();

	if (dead_player == this->game->GetObjects()[0])
		player_number = 0;
	else
		player_number = 1;

	this->frameManager.printDeadPlayerMove((PlayerCharacter *)dead_player, player_number);

	for (int i = 0; i < 5; i++)
	{
		this->frameManager.PrintOutSideWalls();
		this->frameManager.PrintStageOverMassage(i % 2);
		this->frameManager.UpdateFrame();
		Sleep(600);
	}
	
}

void GameManager::resetStage()
{
	GetAsyncKeyState(0x41);
	GetAsyncKeyState(0x44);
	GetAsyncKeyState(0x47);
	GetAsyncKeyState(VK_LEFT);
	GetAsyncKeyState(VK_RIGHT);
	GetAsyncKeyState(VK_NUMPAD5);
	GetAsyncKeyState(0x57);
	GetAsyncKeyState(VK_UP);
	GetAsyncKeyState(0x53);
	GetAsyncKeyState(VK_DOWN);

	this->removeAllComponents();

	this->game->replacePlayer();
}

void GameManager::removeAllComponents()
{
	for (std::vector<Object*>::iterator it = this->game->GetObjects().begin() + 2; it != this->game->GetObjects().end(); ++it)
	{
		it = this->game->GetObjects().erase(it);

		--it;
	}
}

void GameManager::gotoNextStage()
{
	bool is_boss_stage = this->game->current_stage % 2 == 0;

	if (is_boss_stage)
		this->makeBossStage();

	else 
		this->makeNormalStage();

	this->game->current_stage += 1;
}

void GameManager::makeNormalStage()
{
	this->game->MakeMap(this->game->current_stage >= 3 ? 8 : this->game->current_stage * 3 + 1);
}

void GameManager::makeBossStage()
{
	this->game->MakeBossMap();

	this->game->SummonBoss();
}

void GameManager::waitForStart()
{
	auto last_milli = GetTickCount64();
	auto milli = GetTickCount64();
	int flag = 1;

	while (1)
	{
		milli = GetTickCount64();

		this->frameManager.drawGameStart(flag % 2);
		this->frameManager.UpdateFrame();

		if (_kbhit())
		{
			break;
		}

		if (last_milli + 500 < milli)
		{
			flag++;
			last_milli = milli;
		}
	}
}

void GameManager::showCountDown()
{
	for (int i = 0; i < 5; i++)
	{
		this->frameManager.MakeFrame(this->game->GetObjects());
		this->frameManager.PrintCountDown(i);
		this->frameManager.UpdateFrame();
		Sleep(1000);
	}
}

void GameManager::showGameStory()
{
	this->frameManager.drawGameStory();
}

void GameManager::gameModeSelecter()
{
	char c = _getch();
	while (1)
	{
		this->frameManager.drawGameModeSelectScreen(this->game->isPvP);
		this->frameManager.UpdateFrame();

		if (_kbhit())
		{
			c = _getch();
			if (c == -32)
			{
				c = _getch();
				if (c == 75 || c == 77 )
				{
					this->game->isPvP = this->game->isPvP == true ? false : true;
				}
			}
			

			if (c == VK_RETURN)
			{
				break;
			}
		}
	}
}

void GameManager::showHowToControl()
{
	this->frameManager.drawHowToControl(this->game->isPvP);
	this->frameManager.UpdateFrame();

	while (1)
	{
		if (_kbhit())
		{
			break;
		}
	}
}

void GameManager::showBossEliminated()
{
	Object* dead_boss = this->game->getGameOverBoss();

	for (int i = 0; i < 5; i++)
	{
		this->frameManager.MakeBossDeadFrame(this->game->GetObjects(), dead_boss, i % 2);
		this->frameManager.UpdateFrame();

		Sleep(700);
	}

	this->frameManager.MakeBossDeadFrame(this->game->GetObjects(), dead_boss, 2);
	this->frameManager.UpdateFrame();
	Sleep(700);

	for (int i = 0; i < 5; i++)
	{
		//this->frameManager.MakeStageOverFrame(this->game->GetObjects(), dead_player, i%2);
		this->frameManager.PrintOutSideWalls();
		this->frameManager.PrintStageOverMassage(i % 2);
		this->frameManager.UpdateFrame();
		Sleep(600);
	}


}

void GameManager::showGameOver()
{
	int player_number;

	Object* dead_player = this->game->getGameOverPlayer();

	if (dead_player == this->game->GetObjects()[0])
		player_number = 0;
	else
		player_number = 1;

	this->frameManager.printDeadPlayerMove((PlayerCharacter*)dead_player, player_number);

	for (int i = 0; i < 5; i++)
	{
		this->frameManager.drawGameOverScene(1 - player_number, i % 2);
		this->frameManager.UpdateFrame();
		Sleep(500);
	}


}