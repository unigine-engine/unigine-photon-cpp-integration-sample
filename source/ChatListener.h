#pragma once

#include <UnigineString.h>
#include <UnigineSignal.h>
#include <Chat-cpp/inc/Client.h>

class ChatListener: public ExitGames::Chat::Listener
{
public:
	void debugReturn(int debugLevel, const ExitGames::Common::JString &string) override;

	void onStateChange(int state) override;

	void connectionErrorReturn(int errorCode) override;
	void clientErrorReturn(int errorCode) override;
	void warningReturn(int warningCode) override;
	void serverErrorReturn(int errorCode) override;

	void connectReturn(int errorCode, const ExitGames::Common::JString &errorString) override;
	void disconnectReturn() override;
	void subscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString> &channels, const ExitGames::Common::JVector<bool> &results) override;
	void unsubscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString> &channels) override;

	void onGetMessages(const ExitGames::Common::JString &channelName, const ExitGames::Common::JVector<ExitGames::Common::JString> &senders, const ExitGames::Common::JVector<ExitGames::Common::Object> &messages) override;
	void onPrivateMessage(const ExitGames::Common::JString &sender, const ExitGames::Common::Object &message, const ExitGames::Common::JString &channelName) override;
	void onStatusUpdate(const ExitGames::Common::JString &user, int status, bool gotMessage, const ExitGames::Common::Object &message) override;

	void onReceiveBroadcastMessage(const ExitGames::Common::JString &channelName, nByte *inBuff, int inBuffBodyLength) override;

	UNIGINE_CALLBACK_METODS(GetMessages, on_get_message);
	UNIGINE_CALLBACK_METODS(PrivateMessage, on_private_message);
	UNIGINE_CALLBACK_METODS(StatusUpdate, on_status_update);
	UNIGINE_CALLBACK_METODS(RecieveBroadcastMessage, on_recieve_broadcast_message);
	UNIGINE_CALLBACK_METODS(OnConnected, on_connected);

private:
	Unigine::Signal on_get_message;
	Unigine::Signal on_private_message;
	Unigine::Signal on_status_update;
	Unigine::Signal on_recieve_broadcast_message;
	Unigine::Signal on_connected;
};
