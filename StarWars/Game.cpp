#include "Game.hpp"

Game::Game(bool gameOver) :gameOver(gameOver)
{
	this->objects = std::vector<Object*>();
	this->isPvP = false;
}

void Game::MakeBossMap()
{
	MakeMap(current_stage >= 3 ? 8 : current_stage * 3 + 1);

	for (int y = 0; y < HEIGHT; ++y)
	{
		if (Curmap[y][WIDTH / 2] != nullptr && Curmap[y][WIDTH / 2]->GetObjectType() == ObjectType::WALL)
		{
			((Wall*)Curmap[y][WIDTH / 2])->setHealth(INT_MAX);
		}

		else
		{
			Wall* wall = new Wall();

			wall->SetCoord({ WIDTH / 2, y });
			wall->SetNextCoord(wall->GetCoord());
			wall->setHealth(INT_MAX);

			objects.push_back(wall);
			Curmap[y][WIDTH / 2] = wall;
		}
	}
}

void Game::MakeMap(int map_index)
{
	for (int y = 0; y < HEIGHT; ++y)
	{
		for (int x = 0; x < WIDTH; ++x)
		{
			if (map[map_index][HEIGHT - 1 - y][x] == 1)
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

	player1->SetCoord({ 9, HEIGHT / 2 });
	player2->SetCoord({ 31, HEIGHT / 2 });

	player1->setHealth(9999);

	player1->SetNextCoord({ 9, HEIGHT / 2 });
	player2->SetNextCoord({ 31, HEIGHT / 2 });

	player1->SetVelocity({ 0, 0 });
	player2->SetVelocity({ 0, 0 });

	player1->SetSpeed(10);
	player2->SetSpeed(5);

	player1->setOriginalSpeed(10);
	player2->setOriginalSpeed(5);

	player1->setWeapon(5);
	player2->setWeapon(1);

	player1->setTarget(player2);
	player2->setTarget(player1);

	player2->setAI(true);
	player2->setTarget(player1);
}

void Game::replacePlayer()
{
	PlayerCharacter* player1 = (PlayerCharacter*)objects[0];
	PlayerCharacter* player2 = (PlayerCharacter*)objects[1];

	player1->SetCoord({ 9, HEIGHT / 2 });
	player2->SetCoord({ 31, HEIGHT / 2 });

	player1->SetNextCoord({ 9, HEIGHT / 2 });
	player2->SetNextCoord({ 31, HEIGHT / 2 });

	player1->SetVelocity({ 0, 0 });
	player2->SetVelocity({ 0, 0 });

	if (player1->getHealth() <= 0)
	{
		player1->setHealth(100);
	}

	if (player2->getHealth() <= 0)
	{
		player2->setHealth(100);
	}

	player1->setTarget(player2);
	player2->setTarget(player1);

	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Object* obj = *it;

		if (obj->GetObjectType() == ObjectType::ENEMY_NPC || obj->GetObjectType() == ObjectType::FRIENDLY_NPC)
		{
			((Character*)*it)->setHealth(0);

			it = objects.erase(it);
			--it;
		}
	}
}

void Game::removeParticles()
{
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		Object* obj = *it;

		if (obj->GetObjectType() == ObjectType::PARTICLE) 
		{
			it = objects.erase(it);
			--it;
		}
	}

	this->UpdateMap();
}

void Game::MakeItem()
{
	random_device rd_variable;
	mt19937 generate(rd_variable());
	uniform_int_distribution<> IsItem(0, 1), SetWeapon(1, Game::WEAPON_COUNT - 1), SetSpecialItem(0, Game::SPECIAL_ITEM_COUNT - 1);
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
	uniform_int_distribution<> XCoord(1, Game::WIDTH - 2), YCoord(1, Game::HEIGHT - 2), SetWay(1, 4);
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
				if (coord.getY() >= Game::HEIGHT - 1)
				{
					coord.setY(Game::HEIGHT - 2);
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
				if (coord.getX() >= Game::WIDTH - 1)
				{
					coord.setX(Game::WIDTH - 2);
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

bool Game::IsStageOver()
{
	return stageOver;
}

void Game::SetStageOver(bool stageOver)
{
	this->stageOver = stageOver;
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
		if ((*it)->GetObjectType() == ObjectType::ENEMY_NPC && ((Character*)*it)->getHealth() <= 0)
		{
			stageOver = true;
			break;
		}

		if ((*it)->GetObjectType() == ObjectType::PLAYER_CHARACTER && ((PlayerCharacter*)*it)->life <= 0)
		{
			gameOver = true;
			break;
		}

		if ((*it)->IsCharacter() && ((Character*)*it)->IsAI())
		{
			Character* ai = static_cast<Character*>(*it);

			if (ai->GetObjectType() == ObjectType::FRIENDLY_NPC && ai->getHealth() <= 0)
			{
				(*it)->should_delete = true;
				continue;
			}

			Character* ai_opponent = (Character*)ai->getTarget();

			int ai_x = ai->GetCoord().getX();
			int ai_y = ai->GetCoord().getY();

			int target_x = ai_opponent->GetCoord().getX();
			int target_y = ai_opponent->GetCoord().getY();

			for (std::vector<Object*>::iterator it2 = objects.begin(); it2 != objects.end(); ++it2)
			{
				if ((*it2)->getTarget() == *it || (ai->getWeapon() == 0 && ((*it2)->GetObjectType() == ObjectType::DROPPED_SPECIAL_ITEM || (*it2)->GetObjectType() == ObjectType::DROPPED_WEAPON)))
				{
					int target_x_2 = (*it2)->GetCoord().getX();
					int target_y_2 = (*it2)->GetCoord().getY();

					double dist = sqrt((ai_x - target_x) * (ai_x - target_x) + (ai_y - target_y) * (ai_y - target_y));

					if (ai_opponent->getHealth() <= 0)
						dist = 10000.0;

					double dist_2 = sqrt((ai_x - target_x_2) * (ai_x - target_x_2) + (ai_y - target_y_2) * (ai_y - target_y_2));

					if (dist > dist_2 && ai->getMaster() != *it2)
						ai->setTarget(*it2);
				}
			}

			getShortestWay((*it), (*it)->getTarget());

			if (shouldShoot(*it) && !(*it)->getTarget()->IsItem())
				CharacterShoot(ai);
		}

		if ((*it)->last_updated + (1000.0 / (*it)->GetSpeed()) > milli)
			continue;

		(*it)->last_updated = milli;

		if ((*it)->GetObjectType() == ObjectType::WALL)
		{
			Wall* wall = (Wall*)*it;

			if (wall->getHealth() <= 0)
				(*it)->should_delete = true;

			continue;
		}

		UpdateSingleObjectNextPosition(*it);

		if ((*it)->IsPlayer())
		{
			PlayerCharacter* player = static_cast<PlayerCharacter*>(*it);
			auto milli = GetTickCount64();

			if (player->current_buff != 0 && player->buff_start + player->getBuffTimer() < milli)
			{
				player->current_buff = 0;
				player->SetSpeed(player->getOriginalSpeed());
				player->isFreeze = false;
				player->setBuffTimer(0);
			}

			if (player->GetHitTimer() > 0)
				player->SetHitTimer(player->GetHitTimer() - 1);

			else
				player->is_attacked = false;

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

			if (Curmap[next_y][next_x] != nullptr && Curmap[next_y][next_x]->GetObjectType() == ObjectType::PARTICLE)
			{
				Particle* bullet = (Particle*)Curmap[next_y][next_x];

				if (bullet->shooter == *it)
					continue;

				PlayerCharacter* target = static_cast<PlayerCharacter*>(*it);

				target->giveDamage(bullet->getDamage());

				if (player->getHealth() <= 0)
				{
					this->stageOver = true;
				}

				for (std::vector<Object*>::iterator it2 = objects.begin(); it2 != objects.end(); ++it2)
					if (*it2 == bullet)
						(*it2)->should_delete = true;
			}

			if (next_y <= 0 || next_y >= 19 || next_x <= 0 || next_x >= 40)
			{
				PlayerCharacter* target = static_cast<PlayerCharacter*>(*it);

				target->setHealth(0);

				this->stageOver = true;

				continue;
			}
		}

		if ((*it)->IsItem())
		{
			DroppedItem* item = static_cast<DroppedItem*>(*it);

			PlayerCharacter* p1 = (PlayerCharacter*)(objects[0]);
			PlayerCharacter* p2 = (PlayerCharacter*)(objects[1]);

			if (item->GetCoord() == objects[0]->GetCoord())
			{
				item->useItem(objects[0], objects[1], objects);
			}

			else if (item->GetCoord() == objects[1]->GetCoord())
			{
				item->useItem(objects[1], objects[0], objects);
			}

			else
				continue;

			if (p2->IsAI())
			{
				int ai_x = p2->GetCoord().getX();
				int ai_y = p2->GetCoord().getY();

				for (std::vector<Object*>::iterator it2 = objects.begin(); it2 != objects.end(); ++it2)
				{
					if (!(*it2)->IsCharacter() || p2 == *it2)
						continue;

					if (p2->getTarget()->IsItem())
					{
						if ((*it2)->getTarget() == p2)
							p2->setTarget(*it2);

						break;
					}

					else
					{
						int target_x = p2->getTarget()->GetCoord().getX();
						int target_y = p2->getTarget()->GetCoord().getY();

						int target_x_2 = (*it2)->GetCoord().getX();
						int target_y_2 = (*it2)->GetCoord().getY();

						double dist = sqrt((ai_x - target_x) * (ai_x - target_x) + (ai_y - target_y) * (ai_y - target_y));

						double dist_2 = sqrt((ai_x - target_x_2) * (ai_x - target_x_2) + (ai_y - target_y_2) * (ai_y - target_y_2));

						if (dist > dist_2)
							p2->setTarget(*it2);
					}
				}
			}

			item->should_delete = true;

			if (item->GetObjectType() == ObjectType::DROPPED_SPECIAL_ITEM && (((DroppedSpecialItem*)item)->getSpecialItemId() == 3 || ((DroppedSpecialItem*)item)->getSpecialItemId() == 4))
				break;

			continue;
		}

		if ((*it)->GetObjectType() == ObjectType::PARTICLE)
		{
			Particle* bullet = static_cast<Particle*>(*it);

			bullet->current_range++;

			int next_x = bullet->GetNextCoord().getX();
			int next_y = bullet->GetNextCoord().getY();

			if (bullet->current_range >= bullet->max_range || !(0 <= next_x && next_x <= 40) || !(0 <= next_y && next_y <= 19))
			{
				(*it)->should_delete = true;
				continue;
			}


			Object* obj = Curmap[next_y][next_x];

			if (obj == nullptr)
				continue;

			if (obj->GetObjectType() == ObjectType::WALL)
			{
				Wall* wall = static_cast<Wall*>(obj);

				wall->giveDamage(bullet->getDamage());
				(*it)->should_delete = true;

				continue;
			}

			if (obj->IsCollisionWith(*it) && bullet->shooter != obj && !obj->IsItem() && obj->GetObjectType() != ObjectType::PARTICLE)
			{
				Character* target = static_cast<Character*>(obj);

				target->giveDamage(bullet->getDamage());
				target->is_attacked = true;
				target->SetHitTimer(1);

				if (obj->IsPlayer() && ((PlayerCharacter*)obj)->getHealth() <= 0)
				{
					this->stageOver = true;
				}

				Vec2 nextCoord = target->GetCoord() + bullet->bullet_direction;

				int count = 0;

				for (int i = -target->size.getX() / 2; i <= target->size.getX() / 2; ++i)
				{
					for (int j = -target->size.getY() / 2; j <= target->size.getY() / 2; ++j)
					{
						int next_y = nextCoord.getY() + j;
						int next_x = nextCoord.getX() + i;

						if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT)
						{
							count += 1;
							continue;
						}

						Object* obj = Curmap[nextCoord.getY() + j][nextCoord.getX() + i];

						if (obj == nullptr || (obj != nullptr && obj->GetObjectType() != ObjectType::WALL))
							count += 1;
					}
				}

				if (count == target->size.getX() * target->size.getY())
					target->SetNextCoord(nextCoord);

				(*it)->should_delete = true;

				continue;
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

	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it)->should_delete)
		{
			it = objects.erase(it);
			--it;
		}
			
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

void Game::SummonBoss()
{
	for (int i = 0; i < 2; ++i)
	{
		EnemyNPC* boss = new EnemyNPC();

		boss->SetCoord({ i * 20 + 11, 11 });
		boss->SetNextCoord(boss->GetCoord());

		boss->setHealth(300);

		boss->SetVelocity({ 0, 0 });
		boss->SetSpeed(3);
		boss->setOriginalSpeed(3);

		boss->setWeapon(99);
		boss->size = { 3, 3 };

		boss->setTarget(objects[i]);
		boss->setAI(true);

		objects.push_back(boss);
;	}
}

void Game::CharacterShoot(Character* character)
{
	auto milli = GetTickCount64();

	if (character->last_shot + (1000.0 / character->getWeaponRPM()) > milli)
		return;

	if (character->isFreeze)
		return;

	if (character->bullet_count != 0)
		character->bullet_count -= 1;

	character->last_shot = milli;

	int start = character->isWeaponShotgun() ? -1 : 0;
	int end = character->isWeaponShotgun() ? 2 : 1;

	for (int i = start; i < end; i++)
	{
		Particle* p = new Particle();

		p->bullet_direction = character->direction;
		p->SetSpeed(character->getWeaponSpeed());
		p->setDamage(character->getWeaponDamage());
		p->shooter = character;
		p->max_range = character->getWeaponMaxRange();
		p->isMelee = character->isWeaponMelee();
		p->isShotgun = character->isWeaponShotgun();
		p->isHatoken = character->isWeaponHatoken();

		if (character->direction.getX() >= 0 && character->direction.getY() == 0)
		{
			p->SetCoord(character->GetCoord() + Vec2(0,i));
			p->SetNextCoord(character->GetCoord() + Vec2(0, i));
			p->SetVelocity(Vec2{ 1, 0 });
		}

		else if (character->direction.getX() < 0 && character->direction.getY() == 0)
		{
			p->SetCoord(character->GetCoord() + Vec2(0, i));
			p->SetNextCoord(character->GetCoord() + Vec2(0, i));
			p->SetVelocity(Vec2{ -1, 0 });
		}

		else if (character->direction.getY() >= 0)
		{
			p->SetCoord(character->GetCoord() + Vec2(i, 0));
			p->SetNextCoord(character->GetCoord() + Vec2(i, 0));
			p->SetVelocity(Vec2{ 0, 1 });
		}

		else
		{
			p->SetCoord(character->GetCoord() + Vec2(i, 0));
			p->SetNextCoord(character->GetCoord() + Vec2(i, 0));
			p->SetVelocity(Vec2{ 0, -1 });
		}

		objects.push_back(p);
	}

	if (character->bullet_count == 0)
	{
		character->setWeapon(0);
	}
}

int Game::shortestPathBinaryMatrix(Object* ai, Object* enemy, Vec2 way) {

	Vec2 start = ai->GetCoord() + way;
	Vec2 target = enemy->GetCoord();

	// To store if visited or not
	bool visited[20][41] = { false };

	// Using BFS
	queue<pair<Vec2, int>> q;
	q.push(make_pair(start, 1));

	while (!q.empty()) {
		int y = q.front().first.getY();
		int x = q.front().first.getX();
		int value = q.front().second;
		q.pop();
		//printf("%d %d->", x, y);
		// If the target node is reached, return current value. As, we use BFS, we will get only shortest path
		if (y == target.getY() && x == target.getX()) return value;

		// If current is out of bounds or is 1 or visited, skip it
		if (y < 0 || y >= 20 || x < 0 || x >= 41 || (Curmap[y][x] != nullptr && Curmap[y][x]->GetObjectType() == ObjectType::WALL) || visited[y][x]) continue;

		bool skip = false;

		for (int i = -ai->size.getX() / 2; i <= ai->size.getX() / 2; ++i)
		{
			for (int j = -ai->size.getY() / 2; j <= ai->size.getY() / 2; ++j)
			{
				int next_y = y + j;
				int next_x = x + i;

				if (next_x < 0 || next_x >= WIDTH || next_y < 0 || next_y >= HEIGHT)
					continue;

				Object* obj = Curmap[next_y][next_x];

				if (obj != nullptr && obj->GetObjectType() == ObjectType::WALL)
				{
					skip = true;
					break;
				}
			}
		}

		if (skip)
			continue;

		random_device rd_variable;
		mt19937 generate(rd_variable());
		uniform_int_distribution<> avoid(0, 3);

		int should_avoid = avoid(generate);

		if (should_avoid <= difficulty && ai->GetObjectType() != ObjectType::FRIENDLY_NPC)
			if (Curmap[y][x] != nullptr && (Curmap[y][x]->GetObjectType() == ObjectType::PARTICLE && ((Particle*)Curmap[y][x])->shooter != ai))
				continue;

		// Mark visited and add cells in all directions
		visited[y][x] = true;
		q.push(make_pair(Vec2(x, y - 1), value + 1));
		q.push(make_pair(Vec2(x - 1, y), value + 1));
		q.push(make_pair(Vec2(x + 1, y), value + 1));
		q.push(make_pair(Vec2(x, y + 1), value + 1));
	}

	return -1;
}

void Game::getShortestWay(Object* ai, Object* target)
{
	Character* ai_character = (PlayerCharacter*)ai;

	Vec2 ways[4] = { Vec2(1,0),Vec2(-1,0) ,Vec2(0,-1) ,Vec2(0,1) };
	Vec2 bestway(0, 0);
	int shortest = INT_MAX;

	for (int i = 0; i < 4; i++)
	{
		int dist = shortestPathBinaryMatrix(ai_character, target, ways[i]);
		if (dist != -1 && dist < shortest)
		{
			shortest = dist;
			bestway.setX(ways[i].getX());
			bestway.setY(ways[i].getY());
		}
	}

	if (ai_character->getWeapon() != 0)
	{
		if (shortest <= ai_character->getWeaponMaxRange() - 1 && (ai_character->GetCoord().getX() == target->GetCoord().getX() || ai_character->GetCoord().getY() == target->GetCoord().getY()))
			ai_character->SetVelocity({ 0,0 });

		else
			ai_character->SetVelocity(bestway);
	}

	else
		ai_character->SetVelocity(bestway);

	if (ai_character->GetVelocity() != Vec2{ 0,0 })
		ai_character->direction = ai_character->GetVelocity();

	if (ai_character->GetCoord().getX() == target->GetCoord().getX())
	{
		if (ai_character->GetCoord().getY() - target->GetCoord().getY() < 0)
			ai_character->direction = Vec2{ 0, 1 };

		else
			ai_character->direction = Vec2{ 0, -1 };
	}

	if (ai_character->GetCoord().getY() == target->GetCoord().getY())
	{
		if (ai_character->GetCoord().getX() - target->GetCoord().getX() < 0)
			ai_character->direction = Vec2{ 1, 0 };

		else
			ai_character->direction = Vec2{ -1, 0 };
	}
}

bool Game::shouldShoot(Object* ai)
{
	Character* ai_character = (Character*)ai;

	if (ai_character->getTarget() == nullptr)
		return false;

	Vec2 ai_coord = ai_character->GetCoord();
	Vec2 target_coord = ai_character->getTarget()->GetCoord();

	int ai_x = ai_coord.getX();
	int ai_y = ai_coord.getY();

	int target_x = target_coord.getX();
	int target_y = target_coord.getY();

	double dist = sqrt((ai_x - target_x) * (ai_x - target_x) + (ai_y - target_y) * (ai_y - target_y));

	if (dist - 3 > (double)ai_character->getWeaponMaxRange())
		return false;

	if (ai_x == target_x)
		return true;

	if (ai_y == target_y)
		return true;

	return false;
}

Object* Game::getGameOverPlayer()
{
	if (((PlayerCharacter*)objects[0])->getHealth() <= 0)
		return(objects[0]);

	else if (((PlayerCharacter*)objects[1])->getHealth() <= 0)
		return(objects[1]);
	else
	{
		for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
		{
			if ((*it)->GetObjectType() == ObjectType::ENEMY_NPC && ((Character*)(*it))->getHealth() <= 0)
			{
				if (((*it)->getTarget() == objects[0]))
				{
					return objects[1];
				}

				else
				{
					return objects[0];
				}
			}
		}
	}	

	return objects[0];
}

Object* Game::getGameOverBoss()
{
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it)->GetObjectType() == ObjectType::ENEMY_NPC && ((Character*)(*it))->getHealth() <= 0)
			return (*it);
	}

	return nullptr;
}

void Game::makeAIforWinner(Character * loser)
{
	FriendlyNPC* npc = new FriendlyNPC();
	objects.push_back(npc);

	if (loser == this->objects[0])
	{
		npc->SetCoord(objects[1]->GetCoord() + Vec2(1,0));
		npc->SetNextCoord(objects[1]->GetCoord() + Vec2(1, 0));
	}

	else
	{
		npc->SetCoord(objects[0]->GetCoord() + Vec2(-1, 0));
		npc->SetNextCoord(objects[0]->GetCoord() + Vec2(-1, 0));
	}

	npc->SetVelocity({ 0, 0 });

	npc->SetSpeed(10);

	npc->setOriginalSpeed(10);

	npc->setWeapon(99);

	npc->setTarget((Object *)loser);

	npc->setAI(true);
}