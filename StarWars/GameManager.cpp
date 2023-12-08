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

	this->game->MakePlayer();
	this->game->MakeMap(this->game->current_stage);
	
	this->game->difficulty = 0;

	while (PrecedeGame())
	{
		this->GetPlayerKeyInput();
	}

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
			if (game->current_stage % 2 == 0)
			{
				PlayerCharacter* loser = (PlayerCharacter*)this->game->getGameOverPlayer();
				loser->life -= 1;
			}

			this->showStageOverScene();

			this->resetStage();

			this->gotoNextStage();

			if (((Character*)this->game->GetObjects()[0])->life <= 0 || ((Character*)this->game->GetObjects()[1])->life <= 0)
			{
				this->game->SetGameOver(true);
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
	Object* dead_player = this->game->getGameOverPlayer();

	this->frameManager.printDeadPlayerMove((PlayerCharacter *)dead_player);

	for (int i = 0; i < 6; i++)
	{
		//this->frameManager.MakeStageOverFrame(this->game->GetObjects(), dead_player, i%2);
		this->frameManager.PrintOutSideWalls();
		this->frameManager.PrintStageOverMassage(i % 2);
		this->frameManager.UpdateFrame();
		Sleep(600);
	}
	
	//this->frameManager.MakeStageOverFrame(this->game->GetObjects(), dead_player, 2);
	//this->frameManager.UpdateFrame();
	//Sleep(600);

	//여기에 ChangeMap 함수 삽입 필요

	for (int i = 0; i < 5; i++)
	{
		this->frameManager.MakeFrame(this->game->GetObjects());
		this->frameManager.PrintCountDown(i);
		this->frameManager.UpdateFrame();
		Sleep(1000);
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
	this->game->MakeMap(1);
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