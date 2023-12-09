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
	mt19937 generate(rd());
	uniform_int_distribution<> XCoord(1, 39), YCoord(1, 18), SetWay(1, 4);

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
		for (int i = 0; i < 20; i++)
		{
			int Way = SetWay(generate);
			Particle* laser = new Particle();
			laser->isLaser = true;
			laser->SetSpeed(15);
			laser->setDamage(10);
			laser->max_range = 50;

			if (Way == 1) 
			{
				laser->SetCoord({ XCoord(generate), 19 });
				laser->SetNextCoord(laser->GetCoord() + Vec2{ 0, -1 });
				laser->SetVelocity(Vec2{ 0, -1 });
			}
			if (Way == 2)
			{
				laser->SetCoord({ XCoord(generate), 1 });
				laser->SetNextCoord(laser->GetCoord() + Vec2{ 0, 1 });
				laser->SetVelocity(Vec2{ 0, 1 });
			}
			if (Way == 3)
			{
				laser->SetCoord({ 1, YCoord(generate) });
				laser->SetNextCoord(laser->GetCoord() + Vec2{ 1, 0 });
				laser->SetVelocity(Vec2{ 1, 0 });
			}
			if (Way == 4)
			{
				laser->SetCoord({ 39, YCoord(generate) });
				laser->SetNextCoord(laser->GetCoord() + Vec2{ -1, 0 });
				laser->SetVelocity(Vec2{ -1, 0 });
			}
			objects.push_back(laser);
		}
		break;

	case 911:
		FriendlyNPC * fnpc = new FriendlyNPC();

		objects.push_back(fnpc);

		Vec2 coord = user_1->GetCoord();

		fnpc->SetCoord(coord + Vec2{ 0, 1 });

		fnpc->SetNextCoord(fnpc->GetCoord());

		fnpc->setHealth(30);

		fnpc->SetVelocity({ 0, 0 });
		fnpc->SetSpeed(5);
		fnpc->setOriginalSpeed(5);

		fnpc->setWeapon(99);
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