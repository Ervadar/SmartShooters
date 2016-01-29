#include "Entity.h"

Entity::Entity()
{

}

void Entity::activate()
{
	active = true;
}

void Entity::deactivate()
{
	CCLOG("DI AKTI WEEE");
	active = false;
	removeFromParentAndCleanup(true);
}
