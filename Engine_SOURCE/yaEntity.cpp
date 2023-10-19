#include "yaEntity.h"

namespace ya
{
	UINT32 ya::Entity::gNextID = UINT32_MAX;

	Entity::Entity()
		: mID(++gNextID)
		, mName(L"")
	{
	}
	Entity::Entity(const Entity& other)
		: mID(++gNextID)
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}

namespace gui
{
	UINT32 gui::Entity::gNextID = UINT32_MAX;

	Entity::Entity()
		: mID(++gNextID)
		, mName("")
	{
	}

	Entity::Entity(const char* name)
		: mID(++gNextID)
		, mName(name)
	{
	}

	Entity::Entity(const Entity& other)
		: mID(++gNextID)
		, mName(other.mName)
	{
	}
	Entity::~Entity()
	{
	}
}