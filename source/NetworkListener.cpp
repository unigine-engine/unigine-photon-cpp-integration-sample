#include <UnigineConsole.h>

#include "Utils.h"
#include "NetworkListener.h"

using namespace Unigine;
using namespace Math;
using namespace ExitGames;

void NetworkListener::debugReturn(int debugLevel, const Common::JString &string)
{
	Log::message("debugReturn : %s\n", String(string.cstr()).get());
}

void NetworkListener::connectionErrorReturn(int errorCode)
{
	Log::error("connectionErrorReturn : %d\n", errorCode);
}

void NetworkListener::clientErrorReturn(int errorCode)
{
	Log::error("clientErrorReturn : %d\n", errorCode);
}

void NetworkListener::warningReturn(int warningCode)
{
	Log::warning("warningReturn : %d\n", warningCode);
}

void NetworkListener::serverErrorReturn(int errorCode)
{
	Log::error("serverErrorReturn : %d\n", errorCode);
}

void NetworkListener::joinRoomEventAction(int playerNr, const Common::JVector<int> &playernrs, const LoadBalancing::Player &player)
{
	Log::message("player#%d - name:%s joined the room\n", playerNr, String(player.getName().cstr()).get());

	on_join_room_event.invoke(playerNr);
}

void NetworkListener::leaveRoomEventAction(int playerNr, bool isInactive)
{
	Log::message("player#%d left the room\n", playerNr);

	on_leave_room_event.invoke(playerNr);
}

void NetworkListener::customEventAction(int playerNr, nByte eventCode, const Common::Object &eventContent)
{
	switch (eventCode)
	{
		case EVENT_TYPE::TRANSFORM:
			on_event_transform.invoke(playerNr, Common::ValueObject<PhotonMat4>(eventContent).getDataCopy().getData());
			break;
		case EVENT_TYPE::SHOT:
		{
			auto values = Common::ValueObject<Common::Hashtable>(eventContent).getDataCopy();
			Vec3 start = Common::ValueObject<PhotonVec3>(values.getValue("start")).getDataCopy();
			Vec3 direction = Common::ValueObject<PhotonVec3>(values.getValue("direction")).getDataCopy();
			on_event_shot.invoke(playerNr, start, direction);
		}
		break;
		case EVENT_TYPE::HP:
			on_event_hp.invoke(playerNr, Common::ValueObject<int>(eventContent).getDataCopy());
			break;
	}
}

void NetworkListener::connectReturn(int errorCode, const Common::JString &errorString, const Common::JString &region, const Common::JString &cluster)
{
	if (errorCode == LoadBalancing::ErrorCode::OK)
	{
		Log::message("connected to cluster %s of region %s\n", String(cluster.cstr()).get(), String(region.cstr()).get());

		on_connected.invoke();
	} else
	{
		Log::error("connect failed %d %s\n", errorCode, String(errorString.cstr()).get());
	}
}

void NetworkListener::disconnectReturn(void)
{
	Log::message("disconnected\n");
}

void NetworkListener::createRoomReturn(int localPlayerNr, const Common::Hashtable &gameProperties, const Common::Hashtable &playerProperties, int errorCode, const Common::JString &errorString)
{
	if (errorCode == LoadBalancing::ErrorCode::OK)
	{
		Log::message("room created\n");

		on_room_created.invoke();
	} else
	{
		Log::error("opCreateRoom() failed: %s\n", String(errorString.cstr()).get());
	}
}

void NetworkListener::joinOrCreateRoomReturn(int localPlayerNr, const Common::Hashtable &gameProperties, const Common::Hashtable &playerProperties, int errorCode, const Common::JString &errorString)
{
	if (errorCode == LoadBalancing::ErrorCode::OK)
	{
		Log::message("room has been entered\n");

		on_join_room.invoke();
	} else
	{
		Log::error("opJoinOrCreateRoom() failed: %s\n", String(errorString.cstr()).get());
	}
}

void NetworkListener::joinRoomReturn(int localPlayerNr, const Common::Hashtable &gameProperties, const Common::Hashtable &playerProperties, int errorCode, const Common::JString &errorString)
{
	if (errorCode == LoadBalancing::ErrorCode::OK)
	{
		Log::message("room joined\n");

		on_join_room.invoke();
	} else
	{
		Log::error("opJoinRoom() failed: %s\n", String(errorString.cstr()).get());
	}
}

void NetworkListener::joinRandomRoomReturn(int localPlayerNr, const Common::Hashtable &gameProperties, const Common::Hashtable &playerProperties, int errorCode, const Common::JString &errorString)
{
	if (errorCode == LoadBalancing::ErrorCode::OK)
	{
		Log::message("random room joined\n");

		on_join_room.invoke();
	} else
	{
		Log::error("opJoinRandomRoom() failed: %s\n", String(errorString.cstr()).get());
	}
}

void NetworkListener::leaveRoomReturn(int errorCode, const Common::JString &errorString)
{
	if (errorCode == LoadBalancing::ErrorCode::OK)
	{
		Log::message("room left\n");
	} else
	{
		Log::error("opLeaveRoom() failed: %s\n", String(errorString.cstr()).get());
	}
}

void NetworkListener::joinLobbyReturn()
{
	Log::message("lobby joined\n");
}

void NetworkListener::leaveLobbyReturn()
{
	Log::message("lobby left\n");
}

void NetworkListener::onRoomListUpdate()
{
	on_room_list_updated.invoke();
}
