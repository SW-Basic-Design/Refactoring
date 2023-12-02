#include "DroppedItem.hpp"

DroppedItem::DroppedItem() : Object() 
{
	this->object_type = ObjectType::DROPPED_ITEM;

	this->speed = 1000;
}