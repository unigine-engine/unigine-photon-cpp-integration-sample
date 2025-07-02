#pragma once
#include <UnigineSignal.h>
#include <UnigineString.h>

#include <LoadBalancing-cpp/inc/Client.h>

class NetworkListener: public ExitGames::LoadBalancing::Listener
{
public:
	void debugReturn(int debugLevel, const ExitGames::Common::JString &string) override;

	void connectionErrorReturn(int errorCode) override;
	void clientErrorReturn(int errorCode) override;
	void warningReturn(int warningCode) override;
	void serverErrorReturn(int errorCode) override;

	void joinRoomEventAction(int playerNr, const ExitGames::Common::JVector<int> &playernrs, const ExitGames::LoadBalancing::Player &player) override;
	void leaveRoomEventAction(int playerNr, bool isInactive) override;
	void customEventAction(int playerNr, nByte eventCode, const ExitGames::Common::Object &eventContent) override;

	void connectReturn(int errorCode, const ExitGames::Common::JString &errorString, const ExitGames::Common::JString &region, const ExitGames::Common::JString &cluster) override;
	void disconnectReturn(void) override;
	void createRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable &gameProperties, const ExitGames::Common::Hashtable &playerProperties, int errorCode, const ExitGames::Common::JString &errorString) override;
	void joinOrCreateRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable &gameProperties, const ExitGames::Common::Hashtable &playerProperties, int errorCode, const ExitGames::Common::JString &errorString) override;
	void joinRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable &gameProperties, const ExitGames::Common::Hashtable &playerProperties, int errorCode, const ExitGames::Common::JString &errorString) override;
	void joinRandomRoomReturn(int localPlayerNr, const ExitGames::Common::Hashtable &gameProperties, const ExitGames::Common::Hashtable &playerProperties, int errorCode, const ExitGames::Common::JString &errorString) override;
	void leaveRoomReturn(int errorCode, const ExitGames::Common::JString &errorString) override;
	void joinLobbyReturn() override;
	void leaveLobbyReturn() override;
	void onRoomListUpdate() override;

	UNIGINE_CALLBACK_METHODS(RoomJoined, on_join_room);
	UNIGINE_CALLBACK_METHODS(RoomCreated, on_room_created);
	UNIGINE_CALLBACK_METHODS(RoomListUpdated, on_room_list_updated);
	UNIGINE_CALLBACK_METHODS(JoinRoomEvent, on_join_room_event);
	UNIGINE_CALLBACK_METHODS(LeaveRoomEvent, on_leave_room_event);
	UNIGINE_CALLBACK_METHODS(OnConnected, on_connected);

	UNIGINE_CALLBACK_METHODS(EventTransform, on_event_transform);
	UNIGINE_CALLBACK_METHODS(EventShot, on_event_shot);
	UNIGINE_CALLBACK_METHODS(EventHP, on_event_hp);

private:
	Unigine::Signal on_join_room;
	Unigine::Signal on_room_created;
	Unigine::Signal on_room_list_updated;
	Unigine::Signal on_join_room_event;
	Unigine::Signal on_leave_room_event;
	Unigine::Signal on_connected;

	Unigine::Signal on_event_transform;
	Unigine::Signal on_event_shot;
	Unigine::Signal on_event_hp;
};
