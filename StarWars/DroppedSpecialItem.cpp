#include "DroppedSpecialItem.hpp"

DroppedSpecialItem::DroppedSpecialItem() : DroppedSpecialItem(0)
{

}

DroppedSpecialItem::DroppedSpecialItem(int item_id) : DroppedItem() 
{
	this->object_type = ObjectType::DROPPED_SPECIAL_ITEM;

	this->special_item_id = item_id;
}

void DroppedSpecialItem::useItem(Object* user_1, Object* user_2, vector< Object*>& objects)
{
	const int PLAYER_MAX_HEALTH = 100;
	Character* player = static_cast<Character*>(user_1);
	Character* opponent = static_cast<Character*>(user_2);

	auto milli = GetTickCount64();


	random_device rd;
	mt19937 gen(rd());
	uniform_int_distribution<> dist(1, 20);

	switch (special_item_id) 
	{
	case 0:
		player->addHealth(PLAYER_MAX_HEALTH * 30 / 100);
		break;

	case 1:
		player->setBuffTimer(2000);
		player->SetSpeed((double)player->getOriginalSpeed() * 1.5);
		player->current_buff = 1;
		player->buff_start = milli;
		break;
	
	case 2:
		opponent->setBuffTimer(2000);
		opponent->isFreeze = true;
		opponent->current_buff = 2;
		opponent->buff_start = milli;
		break;

	case 3:
		for (int i = 0; i < 10; i++)
		{
			Particle* bullet = new Particle();
			bullet->isBombing = true;
			bullet->isMelee = false;
			bullet->shooter = user_1;
			bullet->SetSpeed(1);
			bullet->setDamage(5);
			bullet->max_range = 50;
			bullet->SetCoord({ dist(gen), 19});
			bullet->SetNextCoord(bullet->GetCoord() + Vec2{ 1, -1 });
			bullet->SetVelocity(Vec2{ 1, -1 });

			objects.push_back(bullet);
		}

		for (int i = 0; i < 10; i++)
		{
			Particle* p = new Particle();
			p->isBombing = true;

			p->shooter = user_1;
			p->SetSpeed(1);
			p->setDamage(5);
			p->max_range = 50;
			p->SetCoord({ dist(gen) + 20, 19 });
			p->SetNextCoord(p->GetCoord() + Vec2{ -1, -1 });
			p->SetVelocity(Vec2{ -1, -1 });

			objects.push_back(p);
		}

		break;

	case 4:
		FriendlyNPC * fnpc = new FriendlyNPC();

		objects.push_back(fnpc);

		fnpc->SetCoord({ 11, 1 });
		fnpc->SetNextCoord({ 11, 1 });

		fnpc->setHealth(30);

		fnpc->SetVelocity({ 0, 0 });
		fnpc->SetSpeed(5);
		fnpc->setOriginalSpeed(5);

		fnpc->setWeapon(6);
		fnpc->setAI(true);
		fnpc->setMaster(player);
		fnpc->setTarget(opponent);
		
		break;
	}
}

int DroppedSpecialItem::getSpecialItemId()
{
	return special_item_id;
}