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

	switch (special_item_id) 
	{
	case 0:
		player->addHealth(PLAYER_MAX_HEALTH * 30 / 100);
		break;

	case 1:
		player->setBuffTimer(2000);
		player->SetSpeed(30);
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
		/*random_device rd;
		mt19937 gen(rd());
		uniform_int_distribution<> dist(1, 20);

		for (int i = 0; i < 10; i++)
		{
			Particle* bullet = new Particle();
			bullet->isBombing = true;

			bullet->SetSpeed(1);
			bullet->setDamage(5);
			bullet->max_range = 50;
			bullet->SetCoord({ dist(gen), 19});
			bullet->SetNextCoord(bullet->GetCoord() + Vec2<int>{ 1, -1 });
			bullet->SetVelocity(Vec2<int>{ 1.0, -1.0 });

			objects.push_back(bullet);
		}

		for (int i = 0; i < 10; i++)
		{
			Particle* p = new Particle();
			p->isBombing = true;

			p->SetSpeed(1);
			p->setDamage(5);
			p->max_range = 50;
			p->SetCoord({ dist(gen) + 20, 19 });
			p->SetNextCoord(p->GetCoord() + Vec2{ -1, -1 });
			p->SetVelocity(Vec2{ -1, -1 });

			objects.push_back(p);
		}*/
		break;
	}
}