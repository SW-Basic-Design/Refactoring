#include "Game.hpp"

Game::Game(bool gameOver) :gameOver(gameOver)
{
	this->objects = std::vector<Object*>();
}

void Game::MakeMap()
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if (map[HEIGHT - 1 - y][x] == 1)
			{
				Wall* wall = new Wall();

				wall->SetCoord({ x, y });
				wall->SetNextCoord(wall->GetCoord());

				GetObjects().push_back(wall);
				Curmap[y][x] = wall;
			}

			else
				Curmap[y][x] = nullptr;
		}
	}
}

void Game::MakePlayer()
{
	PlayerCharacter* player1 = new PlayerCharacter();
	PlayerCharacter* player2 = new PlayerCharacter();

	objects.push_back(player1);
	objects.push_back(player2);

	player1->SetCoord({ 10, 1 });
	player2->SetCoord({ 25, 1 });

	player1->SetNextCoord({ 10, 1 });
	player2->SetNextCoord({ 25, 1 });

	player1->SetVelocity({ 0, 0 });
	player2->SetVelocity({ 0, 0 });

	player1->SetSpeed(10);
	player2->SetSpeed(10);

	player1->setWeapon(6);
	player2->setWeapon(5);
}

void Game::MakeItem()
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
			objects.push_back(((Object*)weapon));
			Curmap[weapon->GetCoord().getY()][weapon->GetCoord().getX()] = weapon;
		}

		else			// Item == 1 이면 특수 아이템
		{
			DroppedSpecialItem* item = new DroppedSpecialItem(SetSpecialItem(generate));
			((Object*)item)->SetCoord(SetItemCoord());
			((Object*)item)->SetNextCoord(((Object*)item)->GetCoord());
			objects.push_back(((Object*)item));
			Curmap[item->GetCoord().getY()][item->GetCoord().getX()] = item;
		}
	}
}

Vec2 Game::SetItemCoord()
{
	random_device rd_variable;
	mt19937 generate(rd_variable());
	uniform_int_distribution<> XCoord(1, 39), YCoord(1, 18), SetWay(1, 4);
	int Way = SetWay(generate);

	Vec2 coord(XCoord(generate), YCoord(generate));

	if (Curmap[coord.getY()][coord.getX()] == nullptr)
	{
		return coord;
	}

	else
	{
		while (Curmap[coord.getY()][coord.getX()] != 0)
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
		return coord;
	}
}

bool Game::IsGameOver()
{
	return this->gameOver;
}

void Game::SetGameOver(bool gameOver)
{
	this->gameOver = gameOver;
}

std::vector<Object*>& Game::GetObjects()
{
	return this->objects;
}

void Game::UpdateObjectNextPosition()
{
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
		(*it)->SetNextCoord((*it)->GetCoord() + (*it)->GetVelocity());
}

void Game::UpdateSingleObjectNextPosition(Object* object)
{
	object->SetNextCoord(object->GetCoord() + object->GetVelocity());
}

void Game::UpdateObjects()
{
	auto milli = GetTickCount64();
	
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it)->last_updated + (1000.0 / (*it)->GetSpeed()) > milli)
			continue;

		(*it)->last_updated = milli;

		UpdateSingleObjectNextPosition(*it);

		if ((*it)->GetObjectType() == ObjectType::WALL)
			continue;

		if ((*it)->IsPlayer())
		{
			PlayerCharacter* player = static_cast<PlayerCharacter*>(*it);
			auto milli = GetTickCount64();

			if (player->current_buff != 0 && player->buff_start + player->getBuffTimer() < milli)
			{
				player->current_buff = 0;
				player->SetSpeed(10);
				player->isFreeze = false;
				player->setBuffTimer(0);
			}

			if (player->isFreeze)
			{
				player->SetNextCoord(player->GetCoord());
				continue;
			}

			int next_x = player->GetNextCoord().getX();
			int next_y = player->GetNextCoord().getY();

			if (Curmap[next_y][next_x] != nullptr && Curmap[next_y][next_x]->GetObjectType() == ObjectType::WALL)
			{
				player->SetVelocity({ 0, 0 });

				player->SetNextCoord(player->GetCoord());
				continue;
			}
		}

		if ((*it)->IsItem())
		{
			DroppedItem* item = static_cast<DroppedItem*>(*it);

			PlayerCharacter* p1 = (PlayerCharacter*)(objects[0]);
			PlayerCharacter* p2 = (PlayerCharacter*)(objects[1]);

			if (item->GetCoord() == objects[0]->GetCoord())
				item->useItem(objects[0], objects[1], objects);

			else if (item->GetCoord() == objects[1]->GetCoord())
				item->useItem(objects[1], objects[0], objects);

			else
				continue;
			
			it = objects.erase(it);

			if (it == objects.end())
				break;

		}

		if ((*it)->GetObjectType() == ObjectType::PARTICLE)
		{
			Particle* bullet = static_cast<Particle*>(*it);

			bullet->current_range++;

			int next_x = bullet->GetNextCoord().getX();
			int next_y = bullet->GetNextCoord().getY();

			Object* obj = Curmap[next_y][next_x];

			if (obj == nullptr)
				continue;

			if (obj->GetObjectType() == ObjectType::WALL)
			{
				it = objects.erase(it);

				if (it == objects.end())
					break;

				continue;
			}	

			if (obj->IsCollisionWith(*it) && obj->IsPlayer() && bullet->shooter != obj)
			{
				PlayerCharacter* target = static_cast<PlayerCharacter*>(obj);

				target->giveDamage(bullet->getDamage());
				it = objects.erase(it);

				if (it == objects.end())
					break;

				continue;
			}

			if (bullet->current_range >= bullet->max_range)
			{
				it = objects.erase(it);

				if (it == objects.end())
					break;
			}
		}
	}

	PlayerCharacter* p1 = (PlayerCharacter*)objects[0];
	PlayerCharacter* p2 = (PlayerCharacter*)objects[1];

	int p1_x = p1->GetNextCoord().getX();
	int p1_y = p1->GetNextCoord().getY();

	int p2_x = p2->GetNextCoord().getX();
	int p2_y = p2->GetNextCoord().getY();

	if (p1->IsCollisionWith(p2))
	{
		p1->SetNextCoord(p1->GetCoord());
		p2->SetNextCoord(p2->GetCoord());
	}
}

void Game::UpdateObjectPosition()
{
	for (std::vector<Object*>::iterator it = objects.begin(); it < objects.end(); ++it)
		(*it)->SetCoord((*it)->GetNextCoord());
}

void Game::UpdateMap()
{
	for (int i = 0; i < Game::HEIGHT; ++i)
		for (int j = 0; j < Game::WIDTH; ++j)
			Curmap[i][j] = nullptr;

	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Vec2 coord = (*it)->GetCoord();

		int x = coord.getX();
		int y = coord.getY();

		if (!(0 <= x && x <= Game::WIDTH) || !(0 <= y && y <= Game::HEIGHT))
			continue;

		Curmap[y][x] = *it;
	}
}

bool Game::isOutOfMap(Object* obj)
{
	if (obj->GetNextCoord().getY() >= Game::HEIGHT)
	{
		return true;
	}
	else if (obj->GetNextCoord().getY() < 0)
	{
		return true;
	}
	else if (obj->GetNextCoord().getX() < 0)
	{
		return true;
	}
	else if (obj->GetNextCoord().getX() >= Game::WIDTH)
	{
		return true;
	}
	else if (obj->IsPlayer() && obj->GetNextCoord().getY() + 1 >= Game::HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}


void Game::PlayerShoot(PlayerCharacter* player)
{
	auto milli = GetTickCount64();

	if (player->last_shot + (1000.0 / player->getWeaponRPM()) > milli)
		return;

	if (player->bullet_count != 0)
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
			p->SetCoord(player->GetCoord());
			p->SetNextCoord(player->GetCoord());
			p->SetVelocity(Vec2{ 1, 0 });
		}

		else if (player->direction.getX() < 0 && player->direction.getY() == 0)
		{
			p->SetCoord(player->GetCoord());
			p->SetNextCoord(player->GetCoord());
			p->SetVelocity(Vec2{ -1, 0 });
		}

		else if (player->direction.getY() >= 0)
		{
			p->SetCoord(player->GetCoord());
			p->SetNextCoord(player->GetCoord());
			p->SetVelocity(Vec2{ 0, 1 });
		}

		else
		{
			p->SetCoord(player->GetCoord());
			p->SetNextCoord(player->GetCoord());
			p->SetVelocity(Vec2{ 0, -1 });
		}

		objects.push_back(p);
	}

	if (player->bullet_count == 0)
	{
		player->setWeapon(0);
	}
}