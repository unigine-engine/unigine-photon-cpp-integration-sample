#include <UnigineComponentSystem.h>
#include <UnigineConsole.h>
#include <UnigineJson.h>

#include "AppSystemLogic.h"
#include "Utils.h"

using namespace Unigine;
using namespace Math;
using namespace ExitGames;

AppSystemLogic::AppSystemLogic()
{
}

AppSystemLogic::~AppSystemLogic()
{
}

int AppSystemLogic::init()
{
	PhotonVec3::registerType();
	PhotonMat4::registerType();

	ComponentSystem::get()->initialize();
	Engine::get()->setBackgroundUpdate(Engine::BACKGROUND_UPDATE_RENDER_ALWAYS);

	auto application_params = Json::create();
	application_params->load("application_params.json");

	application_ID = application_params->getChild("realtime_application_id")->getString();
	application_version = application_params->getChild("realtime_application_version")->getString();

	if (String::equal(application_ID, "no-app-id"))
	{
		Log::error("No realtime appID in application_params.json, unable to connect to the server!\n");
	}

	connected = false;

	network_listener = new NetworkListener();

	network_client = new LoadBalancing::Client(*network_listener, application_ID.get(), application_version.get());
	network_client->setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	network_client->setSentCountAllowance(2000);

	Common::Base::setListener(network_listener);
	Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));

	network_listener->addRoomJoinedCallback(MakeCallback(this, &AppSystemLogic::load_room_world));
	network_listener->addRoomCreatedCallback(MakeCallback(this, &AppSystemLogic::load_room_world));
	network_listener->addRoomListUpdatedCallback(MakeCallback(this, &AppSystemLogic::update_room_list));
	network_listener->addJoinRoomEventCallback(MakeCallback(this, &AppSystemLogic::join_room_event));
	network_listener->addLeaveRoomEventCallback(MakeCallback(this, &AppSystemLogic::leave_room_event));
	network_listener->addOnConnectedCallback(MakeCallback(this, &AppSystemLogic::connected_event));

	network_listener->addEventTransformCallback(MakeCallback(this, &AppSystemLogic::recieve_event_transform));
	network_listener->addEventShotCallback(MakeCallback(this, &AppSystemLogic::recieve_event_shot));
	network_listener->addEventHPCallback(MakeCallback(this, &AppSystemLogic::recieve_event_hp));

	World::loadWorld(menu_world);

	return 1;
}

int AppSystemLogic::update()
{
	network_client->service();

	return 1;
}

int AppSystemLogic::shutdown()
{
	if (network_client)
	{
		network_client->disconnect();
		delete network_client;
	}


	if (network_listener)
	{
		delete network_listener;
	}

	PhotonVec3::unregisterType();
	PhotonMat4::unregisterType();

	return 1;
}

void AppSystemLogic::connect(Unigine::String username)
{
	Common::JString name(username.get());

	network_client->connect();
	network_client->getLocalPlayer().setName(name);
}

void AppSystemLogic::createRoom(Unigine::String gameID)
{
	network_client->opCreateRoom(Common::JString(gameID.get()),
		LoadBalancing::RoomOptions().setPlayerTtl(INT_MAX).setEmptyRoomTtl(0));
}

void AppSystemLogic::joinRoom(Unigine::String gameID)
{
	network_client->opJoinRoom(Common::JString(gameID.get()));
}

void AppSystemLogic::leaveRoom()
{
	network_client->opLeaveRoom();

	World::loadWorld(menu_world);
}

void AppSystemLogic::disconnect()
{
	network_client->disconnect();
}

Unigine::Vector<int> AppSystemLogic::getRoomPlayerNumbers() const
{
	Common::JVector<LoadBalancing::Player *> room_players = network_client->getCurrentlyJoinedRoom().getPlayers();
	Vector<int> players(room_players.getSize());
	for (int i = 0; i < (int)room_players.getSize(); i++)
	{
		if (room_players[i]->getIsInactive() == false)
			players[i] = room_players[i]->getNumber();
	}

	return players;
}

int AppSystemLogic::getPlayerNumber() const
{
	return network_client->getLocalPlayer().getNumber();
}

const char *AppSystemLogic::getRoomName() const
{
	return String(network_client->getCurrentlyJoinedRoom().getName().cstr()).get();
}

Unigine::String AppSystemLogic::getPlayerName() const
{
	Common::JString name = network_client->getLocalPlayer().getName();
	return String(name.cstr());
}

void AppSystemLogic::sendEventTransform(const Unigine::Math::Mat4 &transform)
{
	network_client->opRaiseEvent(false, PhotonMat4(transform), EVENT_TYPE::TRANSFORM);
}

void AppSystemLogic::sendEventShot(const Unigine::Math::Vec3 &start, const vec3 &direction)
{
	Common::Hashtable values;

	values.put("start", PhotonVec3(start));
	values.put("direction", PhotonVec3(Vec3(direction)));

	network_client->opRaiseEvent(true, values, EVENT_TYPE::SHOT);
}

void AppSystemLogic::sendEventHP(int hp)
{
	network_client->opRaiseEvent(true, hp, EVENT_TYPE::HP);
}

bool AppSystemLogic::isConnected() const
{
	return connected;
}

void AppSystemLogic::join_room_event(int playerNumber)
{
	on_joint_room_event.invoke(playerNumber);
}

void AppSystemLogic::leave_room_event(int playerNumber)
{
	on_leave_room_event.invoke(playerNumber);
}

void AppSystemLogic::load_room_world()
{
	World::loadWorld(room_world);
}

void AppSystemLogic::update_room_list()
{
	const Common::JVector<LoadBalancing::Room *> rooms = network_client->getRoomList();
	room_list.clear();
	for (int i = 0; i < (int)rooms.getSize(); i++)
	{
		room_list.push_back(String(rooms[i]->getName().cstr()));
	}

	on_room_list_updated.invoke(room_list);
}

void AppSystemLogic::connected_event()
{
	connected = true;

	on_connected.invoke();
}

void AppSystemLogic::recieve_event_transform(int playerNumber, const Unigine::Math::Mat4 &transform)
{
	on_event_transform.invoke(playerNumber, transform);
}


void AppSystemLogic::recieve_event_shot(int playerNumber, const Unigine::Math::Vec3 &start, const Unigine::Math::Vec3 &direction)
{
	on_event_shot.invoke(playerNumber, start, direction);
}

void AppSystemLogic::recieve_event_hp(int playerNumber, int hp)
{
	on_event_hp.invoke(playerNumber, hp);
}
