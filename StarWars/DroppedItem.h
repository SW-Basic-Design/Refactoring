#pragma once

#include "Object.h"

#ifndef DROPPED_ITEM_HPP
#define DROPPED_ITEM_HPP

class DroppedItem : public Object {
public:
	DroppedItem();
	~DroppedItem() = default;
	virtual void useItem(Object* itemuser, Object* elseuser, vector< Object*>& objects) = 0;
protected:
};

#endif