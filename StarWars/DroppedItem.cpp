#include "DroppedItem.h"

DroppedItem::DroppedItem() : Object() 
{
	this->object_type = ObjectType::DROPPED_ITEM;
	this->size = { 1, 1 };
}