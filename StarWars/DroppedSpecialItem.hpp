#pragma once

#include "DroppedItem.hpp"
#include "Character.hpp"
#include "GameManager.hpp"

#ifndef DROPPED_SPECIAL_ITEM_HPP
#define DROPPED_SPECIAL_ITEM_HPP

class DroppedSpecialItem : public DroppedItem {
public:
	DroppedSpecialItem();
	DroppedSpecialItem(int i_id);
	~DroppedSpecialItem() = default;
	void useItem(Object* itemuser, Object * elseuser, vector< Object*>& objects);
	int getSpecialItemId();
private:
	int special_item_id; // item id , 0 : heal, 1 : speed up, 3 : freeze;
};

#endif