#include <UnigineWorld.h>
#include <UniginePrimitives.h>
#include <UnigineMaterials.h>
#include <UnigineGame.h>
#include <UnigineNodes.h>

#include "GamePlayer.h"


using namespace Unigine;
using namespace Math;

GamePlayer::GamePlayer()
{
	root_node = NodeDummy::create();

	auto ball = World::loadNode("material_ball/material_ball.node");

	root_node->addChild(ball);

	hp_billboard = ObjectBillboards::create();
	hp_billboard->addBillboard(2.0f, 0.1f);

	root_node->addChild(hp_billboard);
	hp_billboard->setPosition(Vec3(0.0f, 0.0f, 1.2f));

	auto hp_material = Materials::findMaterialByPath("hp_material.mat");

	hp_billboard->setMaterial(hp_material, "*");

	current_hp = max_hp;

	alive = true;
}

GamePlayer::~GamePlayer()
{
	root_node.deleteLater();

	alive = false;
}

void GamePlayer::move(const Vec3 &direction)
{
	if (alive)
	{
		root_node->translate(direction * movement_speed * Game::getIFps());
	}
}

void GamePlayer::rotate(float direction)
{
	if (alive)
	{
		root_node->rotate(quat(vec3_up, rotation_speed * Game::getIFps() * direction));
	}
}

void GamePlayer::setWorldTransform(const Unigine::Math::Mat4 &transform)
{
	if (alive)
	{
		root_node->setWorldTransform(transform);
	}
}

Mat4 GamePlayer::getWorldTransform() const
{
	return alive ? root_node->getWorldTransform() : Mat4_identity;
}

vec3 GamePlayer::getWorldDirection() const
{
	return alive ? root_node->getWorldDirection(AXIS_Y) : vec3_zero;
}

Vec3 GamePlayer::getShotPivot() const
{
	return alive ? root_node->getWorldPosition() + Vec3(0.0f, 0.0f, 0.3f) : Vec3_zero;
}

const NodePtr &GamePlayer::getRootNodeDummy() const
{
	return root_node;
}

void GamePlayer::setHP(int value)
{
	current_hp = value;

	if (current_hp > 0)
	{
		hp_billboard->setWidth(0, 2 * current_hp * 1.0f / max_hp);
	} else
	{
		root_node->setEnabled(false);

		alive = false;

		on_dead.invoke();
	}
}

int GamePlayer::damage()
{
	setHP(current_hp - 1);

	return current_hp;
}

bool GamePlayer::isAlive() const
{
	return alive;
}
