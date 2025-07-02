#pragma once

#include <UnigineNode.h>
#include <UnigineObjects.h>
#include <UnigineSignal.h>

class GamePlayer
{
public:
	GamePlayer();
	~GamePlayer();

	void move(const Unigine::Math::Vec3 &translation);
	void rotate(float direction);
	void setWorldTransform(const Unigine::Math::Mat4 &transform);

	Unigine::Math::Mat4 getWorldTransform() const;
	Unigine::Math::vec3 getWorldDirection() const;

	Unigine::Math::Vec3 getShotPivot() const;

	const Unigine::NodePtr &getRootNodeDummy() const;

	void setHP(int value);

	int damage();

	bool isAlive() const;

	UNIGINE_CALLBACK_METHODS(OnDead, on_dead);

private:
	Unigine::Signal on_dead;

	Unigine::NodePtr root_node = nullptr;
	Unigine::ObjectBillboardsPtr hp_billboard = nullptr;

	const int max_hp = 5;
	int current_hp = 0;
	float movement_speed = 3.0f;
	float rotation_speed = 30.0f;

	bool alive = true;
};
