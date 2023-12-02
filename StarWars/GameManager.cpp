#include "GameManager.hpp"

GameManager::GameManager()
{
	this->game = new Game(false);
	this->last_item_SpawnTime = 0;
}

void GameManager::StartGame()
{
	this->frameManager.InitFrame();

	this->game->MakePlayer();
	this->game->MakeMap();

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
		auto milli = GetTickCount64();

		if (last_item_SpawnTime + spawn_term < milli)
		{
			last_item_SpawnTime = milli;
			game->MakeItem();
		}

		game->last_updated = milli;

		this->game->UpdateObjects();
		this->game->UpdateObjectPosition();
		this->game->UpdateMap();

		this->frameManager.MakeFrame(this->game->GetObjects());
		this->frameManager.UpdateFrame();


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
		player1->GetVelocity().setX(-1);
		player1->direction.setX(-1);
		player1->direction.setY(0);
	}

	if (GetAsyncKeyState(0x44))
	{
		player1->GetVelocity().setX(1);
		player1->direction.setX(1);
		player1->direction.setY(0);
	}

	else if (!GetAsyncKeyState(0x41) && !GetAsyncKeyState(0x44))
		player1->GetVelocity().setX(0);

	if (GetAsyncKeyState(0x47))
	{
		if (!player1->isFreeze)
			game->PlayerShoot(player1);
	}

	if (GetAsyncKeyState(VK_LEFT))
	{
		player2->GetVelocity().setX(-1);
		player2->direction.setX(-1);
		player2->direction.setY(0);
	}

	if (GetAsyncKeyState(VK_RIGHT))
	{
		player2->GetVelocity().setX(1);
		player2->direction.setX(1);
		player2->direction.setY(0);
	}

	else if (!GetAsyncKeyState(VK_LEFT) && !GetAsyncKeyState(VK_RIGHT))
		player2->GetVelocity().setX(0);

	if (GetAsyncKeyState(VK_NUMPAD5))
	{
		if (!player2->isFreeze)
			game->PlayerShoot(player2);
	}

	if (GetAsyncKeyState(0x57))
	{
		player1->GetVelocity().setY(1);
		player1->direction.setX(0);
		player1->direction.setY(1);
	}

	if (GetAsyncKeyState(VK_UP))
	{
		player2->GetVelocity().setY(1);
		player2->direction.setX(0);
		player2->direction.setY(1);
	}

	else if (!GetAsyncKeyState(0x57) && !GetAsyncKeyState(0x53))
		player1->GetVelocity().setY(0);

	if (GetAsyncKeyState(0x53))
	{
		player1->GetVelocity().setY(-1);
		player1->direction.setX(0);
		player1->direction.setY(-1);
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		player2->GetVelocity().setY(-1);
		player2->direction.setX(0);
		player2->direction.setY(-1);
	}

	else if (!GetAsyncKeyState(VK_UP) && !GetAsyncKeyState(VK_DOWN))
		player2->GetVelocity().setY(0);
}