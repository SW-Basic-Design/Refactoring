#include "Game.h"

Game::Game(bool gameOver) :gameOver(gameOver)
{
	this->objects = std::vector<Object*>();
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
	{
		(*it)->SetVelocity((*it)->GetVelocity()); // + object->GetAcceleration()
		(*it)->SetNextCoord((*it)->GetCoord() + (*it)->GetVelocity());
	}
}

void Game::UpdateObjects()
{
	for (std::vector<Object*>::iterator it = objects.begin(); it != objects.end(); ++it)
	{
		if ((*it)->GetObjectType() == ObjectType::WALL)
			continue;

		
	}
}

void Game::UpdateObjects()
{
	auto milli = GetTickCount64();

	for (auto& it : objects)
	{
		if (it->last_updated + (1000.0 / it->GetSpeed()) > milli)
			continue;
		if (it->GetObjectType() == ObjectType::PARTICLE)
		{
			it->last_updated = milli;
		}

		if (it->GetObjectType() == ObjectType::PLAYER_CHARACTER)
		{
			PlayerCharacter* player = (PlayerCharacter*)(it);

			if (player->getBuffTimer() > 0)
			{
				player->setBuffTimer(player->getBuffTimer() - 1);
			}
			else
			{
				player->isFreeze = false;
				player->SetSpeed(20);
				player->current_buff = 0;
			}

			if (player->isFreeze == true)
			{
				continue;
			}

			if (player->is_mid_air && player->getJumpTimer() < player->getJumpLimit())
				player->GetVelocity().setY(1);

			else if (player->is_mid_air && player->getJumpTimer() >= player->getJumpLimit())
				player->GetVelocity().setY(-1);

			it->last_updated = milli;

		}

		UpdateObjectNextPosition(it);

		if (it == objects[0] || it == objects[1])
		{
			PlayerCharacter* player = (PlayerCharacter*)it;

			player->is_mid_air = true;
		}

		for (int i = 0; i < 2; ++i)
		{
			for (auto& it2 : objects)
			{
				PlayerCharacter* player = (PlayerCharacter*)objects[i];

				if (	player->GetVelocity().getY() <= 0  
						&&	
						( 
							(it2->GetObjectType() == ObjectType::WALL) && (player->GetCoord() + Vec2(0, -1) == it2->GetCoord())
							|| 
							(it2->IsCharacter()) && (player->GetCoord() + Vec2(0, -2) == it2->GetCoord()) 
						) 
					)
				{
					player->GetVelocity().setY(0);
					player->is_mid_air = false;

					break;
				}
			}
		}
		

		for (auto& it2 : objects)
		{
			if (it != it2 && it->IsCollisionWith(it2))
			{
				if (it->IsCharacter() && (it2->GetObjectType() == ObjectType::WALL || it2->IsCharacter()))
				{
					if (it->GetNextCoord() == it2->GetCoord() || it->GetNextCoord() + Vec2(0,1) == it2->GetCoord() || it->GetNextCoord() + Vec2(0, -1) == it2->GetCoord())
						it->GetVelocity().setX(0);

					if (it->GetCoord().getY() + 2 == it2->GetCoord().getY())
					{
						PlayerCharacter* player = (PlayerCharacter*)it;

						player->setJumpTimer(player->getJumpLimit());
						it->SetNextCoord(it->GetCoord());
					
						break;
					}

				UpdateObjectNextPosition(it);
				}

				if (it->IsCharacter() && it2->IsItem())
				{
					if (it == objects[0])
					{
						((DroppedItem*)it2)->useItem(objects[0], objects[1], Game::GetObjects());
					}
					else
					{
						((DroppedItem*)it2)->useItem(objects[1], objects[0], Game::GetObjects());
					}
					Curmap[it2->GetCoord().getY()][it2->GetCoord().getX()] = 0;
					it2->SetDeleteObject(true);
					should_delete = true;

					break;
				}

				if (it->GetObjectType() == ObjectType::PARTICLE)
				{
					if (it2->GetObjectType() == ObjectType::WALL)
					{
						it->SetDeleteObject(true);
						should_delete = true;

						((Wall*)it2)->giveDamage(((Particle*)it)->getDamage());
						if (((Wall*)it2)->getHealth() <= 0)
						{
							Curmap[it2->GetCoord().getY()][it2->GetCoord().getX()] = 0;
							it2->SetDeleteObject(true);
							should_delete = true;
						}

						break;
					}

					if (it2->IsCharacter() && ((Particle *)it)->shooter != it2)
					{
						(dynamic_cast<Character*>(it2))->giveDamage((dynamic_cast<Particle*>(it))->getDamage());

						it->SetDeleteObject(true);
						should_delete = true;

						break;
					}
				}
			}
		}

		if (it->GetObjectType() == ObjectType::PARTICLE)
		{
			if (((Particle*)it)->cur_range >= ((Particle*)it)->max_range)
			{
				it->SetDeleteObject(true);
				should_delete = true;
			}
			else
			{
				((Particle*)it)->cur_range++;
			}
		}

		if (isOutOfMap(it))
		{
			if (it->IsCharacter())
			{
				((Character*)it)->setHealth(0);
			}
			else
			{
				it->SetDeleteObject(true);
				should_delete = true;
			}
		}


		if (it->GetObjectType() == ObjectType::PLAYER_CHARACTER && ((Character *)it)->getHealth() <= 0)
		{
			this->SetGameOver(true);
		}

		UpdateObjectPosition();
	}

	if (!should_delete)
		return;

	for (int i = 2; i < objects.size(); ++i)
	{
		if (objects[i]->GetDeleteObject())
		{
			delete (objects[i]);
			objects.erase(objects.begin() + i);

			--i;
		}
	}
}

void Game::UpdateObjectPosition()
{
	PlayerCharacter* player1 = (PlayerCharacter*)objects[0];
	PlayerCharacter* player2 = (PlayerCharacter*)objects[1];

	if (player1->GetCoord() != player1->GetNextCoord())
		player1->setJumpTimer(player1->getJumpTimer() + 1);
	
	if (player2->GetCoord() != player2->GetNextCoord())
		player2->setJumpTimer(player2->getJumpTimer() + 1);

	Curmap[player1->GetCoordAsInt().getY()][player1->GetCoord().getX()] = 0;
	Curmap[player2->GetCoord().getY()][player2->GetCoord().getX()] = 0;
	Curmap[player1->GetCoord().getY() + 1][player1->GetCoord().getX()] = 0;
	Curmap[player2->GetCoord().getY() + 1][player2->GetCoord().getX()] = 0;

	for (std::vector<Object*>::iterator it = objects.begin(); it < objects.end(); ++it)
		(*it)->SetCoord((*it)->GetNextCoord());

	Curmap[player1->GetCoord().getY()][player1->GetCoord().getX()] = 2;
	Curmap[player2->GetCoord().getY()][player2->GetCoord().getX()] = 2;
	Curmap[player1->GetCoord().getY() + 1][player1->GetCoord().getX()] = 2;
	Curmap[player2->GetCoord().getY() + 1][player2->GetCoord().getX()] = 2;
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
	else if (obj->IsCharacter() && obj->GetNextCoord().getY() + 1 >= Game::HEIGHT)
	{
		return true;
	}
	else
	{
		return false;
	}
}