#pragma once

#include <UnigineLogic.h>

#include "NetworkListener.h"

class AppSystemLogic: public Unigine::SystemLogic
{
public:
	AppSystemLogic();
	~AppSystemLogic();

	int init() override;
	int update() override;
	int shutdown() override;

	void connect(Unigine::String username);
	void createRoom(Unigine::String gameID);
	void joinRoom(Unigine::String gameID);
	void leaveRoom();
	void disconnect();

	Unigine::Vector<int> getRoomPlayerNumbers() const;
	int getPlayerNumber() const;
	const char *getRoomName() const;
	Unigine::String getPlayerName() const;

	void sendEventTransform(const Unigine::Math::Mat4 &transform);
	void sendEventShot(const Unigine::Math::Vec3 &start, const Unigine::Math::vec3 &direction);
	void sendEventHP(int hp);

	bool isConnected() const;

	UNIGINE_CALLBACK_METODS(RoomListUpdated, on_room_list_updated);
	UNIGINE_CALLBACK_METODS(JoinRoomEvent, on_joint_room_event);
	UNIGINE_CALLBACK_METODS(LeaveRoomEvent, on_leave_room_event);
	UNIGINE_CALLBACK_METODS(OnConnected, on_connected);

	UNIGINE_CALLBACK_METODS(EventTransform, on_event_transform);
	UNIGINE_CALLBACK_METODS(EventShot, on_event_shot);
	UNIGINE_CALLBACK_METODS(EventHP, on_event_hp);

private:
	void join_room_event(int playerNumber);
	void leave_room_event(int playerNumber);
	void load_room_world();
	void update_room_list();
	void connected_event();

	void recieve_event_transform(int playerNumber, const Unigine::Math::Mat4 &transform);
	void recieve_event_shot(int playerNumber, const Unigine::Math::Vec3 &start, const Unigine::Math::Vec3 &direction);
	void recieve_event_hp(int playerNumber, int hp);

	Unigine::Signal on_room_list_updated;
	Unigine::Signal on_joint_room_event;
	Unigine::Signal on_leave_room_event;
	Unigine::Signal on_connected;

	Unigine::Signal on_event_transform;
	Unigine::Signal on_event_shot;
	Unigine::Signal on_event_hp;

	NetworkListener *network_listener = nullptr;
	ExitGames::LoadBalancing::Client *network_client = nullptr;

	Unigine::String application_ID = "no-app-id";
	Unigine::String application_version = "1.0";

	Unigine::String menu_world = "menu_world";
	Unigine::String room_world = "room_world";

	Unigine::Vector<Unigine::String> room_list;

	bool connected = false;
};
