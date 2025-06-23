#pragma once
#include <UnigineComponentSystem.h>
#include <Common-cpp/inc/ValueObject.h>

#include "AppSystemLogic.h"
#include "GamePlayer.h"

class GameManager: public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(GameManager, Unigine::ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);

	PROP_PARAM(Node, persecutor);

private:
	void init();
	void update();
	void shutdown();

	void on_player_joined_room(int playerNumber);
	void on_player_leaved_room(int playerNumber);

	void on_event_transform(int playerNumber, const Unigine::Math::Mat4 &transform);
	void on_event_shot(int playerNumber, const Unigine::Math::Vec3 &start, const Unigine::Math::Vec3 &direction);
	void on_event_hp(int playerNumber, int hp);

	void on_main_player_dead();

	void on_leave_room();

	void *player_joined_room_callback_id;
	void *player_leaved_room_callback_id;
	void *event_transfrom_callback_id;
	void *event_shot_callback_id;
	void *event_hp_callback_id;

	GamePlayer *main_player = nullptr;
	Unigine::HashMap<int, GamePlayer *> players;

	AppSystemLogic *network_logic = nullptr;

	Unigine::Math::Random randomizer;

	Unigine::WidgetButtonPtr leave_button;

	float last_shot_time = 0;
	float reload_time = 1;

	bool leaved = false;
};
