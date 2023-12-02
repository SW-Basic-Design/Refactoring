#include "GameManager.h"

GameManager::GameManager()
{
	this->game = new Game(false);
	this->last_item_SpawnTime = 0;
}

void GameManager::MakeMap()
{
	for (int y = 0; y < game->HEIGHT; ++y)
	{
		for (int x = 0; x < game->WIDTH; ++x)
		{
			if (Game::map[game->HEIGHT - 1 - y][x] == 1)
			{
				Wall* wall = new Wall();

				wall->SetCoord({ x, y });
				wall->SetNextCoord(wall->GetCoord());

				game->GetObjects().push_back(wall);
				Game::Curmap[y][x] = 1;
			}
		}
	}
}

void GameManager::MakePlayer()
{
	PlayerCharacter* player1 = new PlayerCharacter();
	PlayerCharacter* player2 = new PlayerCharacter();

	this->game->GetObjects().push_back(player1);
	this->game->GetObjects().push_back(player2);

	player1->SetCoord({ 10, 1 });
	player2->SetCoord({ 25, 1 });
 
	player1->SetNextCoord({ 10, 1 });
	player2->SetNextCoord({ 25, 1 });

	player1->SetVelocity({ -1, 0 });
	player2->SetVelocity({ 1, 0 });

	player1->SetSpeed(20);
	player2->SetSpeed(20);

	player1->setWeapon(6);
	player2->setWeapon(5);

	Game::Curmap[player1->GetCoord().getY()][player1->GetCoord().getX()] = 2;
	Game::Curmap[player1->GetCoord().getY() + 1][player1->GetCoord().getX()] = 2;
	Game::Curmap[player2->GetCoord().getY()][player2->GetCoord().getX()] = 2;
	Game::Curmap[player2->GetCoord().getY() + 1][player2->GetCoord().getX()] = 2;
}

void GameManager::MakeItem()
{
	random_device rd_variable;
	mt19937 generate(rd_variable());
	uniform_int_distribution<> IsItem(0, 1), SetWeapon(1, 6), SetSpecialItem(0, 3);
	int Item = IsItem(generate);

	for (int i = 0; i < 3; i++)
	{
		if (!Item)		// Item == 0 이면 무기
		{
			DroppedWeapon* weapon = new DroppedWeapon(SetWeapon(generate));
			((Object*)weapon)->SetCoord(SetItemCoord());
			((Object*)weapon)->SetNextCoord(((Object*)weapon)->GetCoord());
			this->game->GetObjects().push_back(((Object*)weapon));
		}

		else			// Item == 1 이면 특수 아이템
		{
			DroppedSpecialItem* item = new DroppedSpecialItem(SetSpecialItem(generate));
			((Object*)item)->SetCoord(SetItemCoord());
			((Object*)item)->SetNextCoord(((Object*)item)->GetCoord());
			this->game->GetObjects().push_back(((Object*)item));
		}
	}
}


void GameManager::StartGame()
{
	this->frameManager.InitFrame();

	MakePlayer();
	MakeMap();
	while (PrecedeGame()) 
	{
		this->GetPlayerKeyInput();
		
		this->frameManager.MakeFrame(this->game->GetObjects());
		this->frameManager.UpdateFrame();
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
			MakeItem();
		}

		this->game->UpdateObjectNextPosition();
		this->game->UpdateObjects();
		this->game->UpdateObjectPosition();
		this->game->UpdateMap();
		return true;
	}

	return false;
}

void GameManager::GetPlayerKeyInput()
{
	PlayerCharacter* player1 = dynamic_cast<PlayerCharacter*>(game->GetObjects()[0]);
	PlayerCharacter* player2 = dynamic_cast<PlayerCharacter*>(game->GetObjects()[1]);

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
			PlayerShoot(player1);
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
			PlayerShoot(player2);
	}

	if (GetAsyncKeyState(0x57))
	{
		if (player1->GetVelocity().getY() == 0)
		{
			player1->setJumpTimer(0);
			player1->GetVelocity().setY(1);
			player1->direction.setY(1);
		}
	}

	if (GetAsyncKeyState(VK_UP))
	{
		if (player2->GetVelocity().getY() == 0)
		{
			player2->setJumpTimer(0);
			player2->GetVelocity().setY(1);
			player2->direction.setY(1);
		}
	}

	if (GetAsyncKeyState(0x53))
	{
		player1->direction.setY(-1);
	}

	if (GetAsyncKeyState(VK_DOWN))
	{
		player2->direction.setY(-1);
	}
}

void GameManager::PlayerShoot(PlayerCharacter* player)
{
	auto milli = GetTickCount64();

	if (player->last_shot + (1000.0 / player->getWeaponRPM()) > milli)
		return;

	if(player->bullet_count != 0)
		player->bullet_count -= 1;
	player->last_shot = milli;

	for (int i = 0; i < (player->isWeaponShotgun() ? 2 : 1); i++)
	{
		Particle* p = new Particle();

		p->SetSpeed(player->getWeaponSpeed());
		p->setDamage(player->getWeaponDamage());
		p->shooter = player;
		p->max_range = player->getWeaponMaxRange();
		p->isMelee = player->isWeaponMelee();
		p->isShotgun = player->isWeaponShotgun();
		p->isHatoken = player->isWeaponHatoken();

		if (player->direction.getX() >= 0 && player->direction.getY() == 0)
		{
			p->SetCoord(player->GetCoord() + Vec2{ 1, i });
			p->SetNextCoord(player->GetCoord() + Vec2{ 1, i });
			p->SetVelocity(Vec2{ 1, 0 });
		}
		else if (player->direction.getX() < 0 && player->direction.getY() == 0)
		{
			p->SetCoord(player->GetCoord() + Vec2{ -1, i });
			p->SetNextCoord(player->GetCoord() + Vec2{ -1, i });
			p->SetVelocity(Vec2{ -1, 0 });
		}
		else if (player->direction.getY() >= 0)
		{
			p->SetCoord(player->GetCoord() + Vec2{ i, 2 });
			p->SetNextCoord(player->GetCoord() + Vec2{ i, 2 });
			p->SetVelocity(Vec2{ 0, 1 });
		}
		else
		{
			p->SetCoord(player->GetCoord() + Vec2{ i, -1 });
			p->SetNextCoord(player->GetCoord() + Vec2{ i, -1 });
			p->SetVelocity(Vec2{ 0, -1 });
		}

		game->GetObjects().push_back(p);
	}

	if (player->bullet_count == 0)
	{
		player->setWeapon(0);
	}

	
}

Vec2 GameManager::SetItemCoord()
{
	random_device rd_variable;
	mt19937 generate(rd_variable());
	uniform_int_distribution<> XCoord(1, 39), YCoord(1, 18), SetWay(1, 4);
	int Way = SetWay(generate);

	Vec2 coord(XCoord(generate), YCoord(generate));

	if (Game::Curmap[coord.getY()][coord.getX()] == 0)
	{
		Game::Curmap[coord.getY()][coord.getX()] = 3;
		return coord;
	}

	else
	{
		while (Game::Curmap[coord.getY()][coord.getX()] != 0)
		{
			if (Way == 1)		// 상
			{
				coord.setY(coord.getY() + 1);
				if (coord.getY() >= 19)
				{
					coord.setY(18);
					Way = SetWay(generate);
				}
			}
			if (Way == 2)		// 하
			{
				coord.setY(coord.getY() - 1);
				if (coord.getY() <= 0)
				{
					coord.setY(1);
					Way = SetWay(generate);
				}
			}
			if (Way == 3)		// 좌
			{
				coord.setX(coord.getX() - 1);
				if (coord.getX() <= 0)
				{
					coord.setX(1);
					Way = SetWay(generate);
				}
			}
			if (Way == 4)		// 우
			{
				coord.setX(coord.getX() + 1);
				if (coord.getX() >= 40)
				{
					coord.setX(39);
					Way = SetWay(generate);
				}
			}
		}
		Game::Curmap[coord.getY()][coord.getX()] = 3;
		return coord;
	}
	
}