#include "Entity.h"

Entity::Entity()
{

}

Entity::~Entity()
{
}

void Entity::activate()
{
	active = true;
}

void Entity::deactivate()
{
	active = false;
}
