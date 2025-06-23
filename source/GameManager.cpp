#include <UnigineGame.h>
#include <UnigineVisualizer.h>

#include "GameManager.h"

REGISTER_COMPONENT(GameManager);

using namespace Unigine;
using namespace Math;

void GameManager::init()
{
	Visualizer::setEnabled(true);

	network_logic = static_cast<AppSystemLogic *>(Engine::get()->getSystemLogic(0));

	player_joined_room_callback_id = network_logic->addJoinRoomEventCallback(MakeCallback(this, &GameManager::on_player_joined_room));
	player_leaved_room_callback_id = network_logic->addLeaveRoomEventCallback(MakeCallback(this, &GameManager::on_player_leaved_room));

	event_transfrom_callback_id = network_logic->addEventTransformCallback(MakeCallback(this, &GameManager::on_event_transform));
	event_shot_callback_id = network_logic->addEventShotCallback(MakeCallback(this, &GameManager::on_event_shot));
	event_hp_callback_id = network_logic->addEventHPCallback(MakeCallback(this, &GameManager::on_event_hp));

	auto room_players = network_logic->getRoomPlayerNumbers();
	for (int i = 0; i < room_players.size(); i++)
	{
		GamePlayer *game_player = new GamePlayer();
		players.insert(room_players[i], game_player);

		if (room_players[i] == network_logic->getPlayerNumber())
		{
			auto player = checked_ptr_cast<PlayerPersecutor>(persecutor.get());

			Game::setPlayer(player);
			player->setFixed(1);
			player->setTarget(game_player->getRootNodeDummy());
			player->setMinDistance(8.0f);
			player->setMaxDistance(12.0f);
			player->setPosition(Vec3(0.0f, -10.0f, 6.0f));

			game_player->setWorldTransform(translate(Vec3(randomizer.getFloat(-15.0f, 15.0f),
				randomizer.getFloat(-15.0f, 15.0f),
				0.0f)));

			main_player = game_player;

			main_player->addOnDeadCallback(MakeCallback(this, &GameManager::on_main_player_dead));
		}
	}

	leave_button = WidgetButton::create("Leave");
	leave_button->setFontSize(32);

	leave_button->getEventClicked().connect(this, &GameManager::on_leave_room);

	leaved = false;
}

void GameManager::update()
{
	if (leaved)
	{
		return;
	}

	// update input
	Vec3 movement(0.0f, 0.0f, 0.0f);

	if ((Input::isKeyPressed(Input::KEY_W) || Input::isKeyPressed(Input::KEY_UP)) && Input::isMouseGrab())
	{
		movement += Vec3(0.0f, 1.0f, 0.0f);
	}

	if ((Input::isKeyPressed(Input::KEY_S) || Input::isKeyPressed(Input::KEY_DOWN)) && Input::isMouseGrab())
	{
		movement += Vec3(0.0f, -1.0f, 0.0f);
	}

	if ((Input::isKeyPressed(Input::KEY_A) || Input::isKeyPressed(Input::KEY_LEFT)) && Input::isMouseGrab())
	{
		movement += Vec3(-1.0f, 0.0f, 0.0f);
	}

	if ((Input::isKeyPressed(Input::KEY_D) || Input::isKeyPressed(Input::KEY_RIGHT)) && Input::isMouseGrab())
	{
		movement += Vec3(1.0f, 0.0f, 0.0f);
	}

	if (movement.length2() > FLT_EPSILON)
	{
		movement.normalize();
	}


	int rotation = 0;

	if (Input::isKeyPressed(Input::KEY_Q) && Input::isMouseGrab())
	{
		rotation += 1;
	}

	if (Input::isKeyPressed(Input::KEY_E) && Input::isMouseGrab())
	{
		rotation += -1;
	}

	bool shot = false;
	if (Input::isMouseButtonPressed(Input::MOUSE_BUTTON_LEFT) && Game::getTime() - last_shot_time > reload_time && Input::isMouseGrab())
	{
		shot = true;
		last_shot_time = Game::getTime();
	}

	// serve input
	main_player->move(movement);
	main_player->rotate(toFloat(rotation));

	if (shot)
	{
		Visualizer::renderLine3D(main_player->getShotPivot(),
			main_player->getShotPivot() + Vec3(main_player->getWorldDirection()) * 1000, vec4_red, 0.1);
	}

	// send transform
	network_logic->sendEventTransform(main_player->getWorldTransform());
	if (shot)
	{
		network_logic->sendEventShot(main_player->getShotPivot(),
			main_player->getWorldDirection());
	}
}

void GameManager::shutdown()
{
	for (auto &player : players)
	{
		delete player.data;
	}

	players.clear();
}

void GameManager::on_player_joined_room(int playerNumber)
{
	if (playerNumber == network_logic->getPlayerNumber())
	{
		return;
	}

	GamePlayer *game_player = new GamePlayer();
	players.insert(playerNumber, game_player);
}

void GameManager::on_player_leaved_room(int playerNumber)
{
	auto player = players.get(playerNumber);
	players.erase(playerNumber);
	delete player;
}

void GameManager::on_event_transform(int playerNumber, const Unigine::Math::Mat4 &transform)
{
	if (players.contains(playerNumber))
	{
		players.get(playerNumber)->setWorldTransform(transform);
	}
}

void GameManager::on_event_shot(int playerNumber, const Unigine::Math::Vec3 &start, const Unigine::Math::Vec3 &direction)
{
	if (!players.contains(playerNumber))
	{
		return;
	}

	Visualizer::renderLine3D(start,
		start + direction * 1000,
		vec4_red, 0.1);

	Vector<NodePtr> excludes;
	auto root_node = players.get(playerNumber)->getRootNodeDummy();
	for (int i = 0; i < root_node->getNumChildren(); i++)
		excludes.push_back(root_node->getChild(i));

	auto obj = World::getIntersection(start, start + direction * 1000, 1, excludes);
	NodePtr intersected_root_node = (obj != nullptr) ? obj->getParent() : nullptr;

	if (intersected_root_node == main_player->getRootNodeDummy())
	{
		network_logic->sendEventHP(main_player->damage());
	}
}

void GameManager::on_event_hp(int playerNumber, int hp)
{
	if (players.contains(playerNumber))
	{
		players.get(playerNumber)->setHP(hp);
	}
}

void GameManager::on_main_player_dead()
{
	network_logic->removeEventTransformCallback(event_transfrom_callback_id);
	network_logic->removeEventShotCallback(event_shot_callback_id);
	network_logic->removeEventHPCallback(event_hp_callback_id);

	auto player_spectator = PlayerSpectator::create();
	auto player_persecutor = checked_ptr_cast<PlayerPersecutor>(persecutor.get());

	player_persecutor->setTarget(nullptr);

	player_spectator->setWorldTransform(player_persecutor->getWorldTransform());

	Game::setPlayer(player_spectator);

	WindowManager::getMainWindow()->addChild(leave_button, Gui::ALIGN_CENTER);
}

void GameManager::on_leave_room()
{
	for (auto &player : players)
	{
		players.erase(player.key);
		delete player.data;
	}

	leaved = true;

	network_logic->removeJoinRoomEventCallback(player_joined_room_callback_id);
	network_logic->removeLeaveRoomEventCallback(player_leaved_room_callback_id);

	WindowManager::getMainWindow()->removeChild(leave_button);

	network_logic->leaveRoom();
}
