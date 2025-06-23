#include "ChatListener.h"

using namespace Unigine;

void ChatListener::debugReturn(int debugLevel, const ExitGames::Common::JString &string)
{
	Log::message("%s\n", String(string.cstr()).get());
}

void ChatListener::onStateChange(int state)
{
	Log::message("onStateChange %d\n", state);
}

void ChatListener::connectionErrorReturn(int errorCode)
{
	Log::error("connectionErrorReturn: %d\n", errorCode);
}

void ChatListener::clientErrorReturn(int errorCode)
{
	Log::error("clientErrorReturn: %d\n", errorCode);
}

void ChatListener::warningReturn(int warningCode)
{
	Log::warning("warningReturn: %d\n", warningCode);
}

void ChatListener::serverErrorReturn(int errorCode)
{
	Log::error("serverErrorReturn: %d\n", errorCode);
}

void ChatListener::connectReturn(int errorCode, const ExitGames::Common::JString &errorString)
{
	Log::message("connectReturn: %d\n %s\n", errorCode, String(errorString.cstr()).get());

	on_connected.invoke();
}

void ChatListener::disconnectReturn()
{
	Log::message("disconnectReturn\n");
}

void ChatListener::subscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString> &channels, const ExitGames::Common::JVector<bool> &results)
{
	Log::message("subscribeReturn: %s, %s\n", String(channels.toString().cstr()).get(), String(results.toString().cstr()).get());
}

void ChatListener::unsubscribeReturn(const ExitGames::Common::JVector<ExitGames::Common::JString> &channels)
{
	Log::message("subscribeReturn: %s\n", String(channels.toString().cstr()).get());
}

void ChatListener::onGetMessages(const ExitGames::Common::JString &channelName, const ExitGames::Common::JVector<ExitGames::Common::JString> &senders, const ExitGames::Common::JVector<ExitGames::Common::Object> &messages)
{
	Vector<String> unigine_senders(senders.getSize());
	for (int i = 0; i < (int)senders.getSize(); i++)
	{
		unigine_senders[i] = String(senders[i].cstr());
	}

	Vector<String> unigine_messages(messages.getSize());
	for (int i = 0; i < (int)messages.getSize(); i++)
	{
		unigine_messages[i] = String(messages[i].toString().cstr());
	}

	on_get_message.invoke(String(channelName.cstr()), unigine_senders, unigine_messages);
}

void ChatListener::onPrivateMessage(const ExitGames::Common::JString &sender, const ExitGames::Common::Object &message, const ExitGames::Common::JString &channelName)
{
	on_private_message.invoke(String(sender.cstr()), String(message.toString().cstr()), String(channelName));
}

void ChatListener::onStatusUpdate(const ExitGames::Common::JString &user, int status, bool gotMessage, const ExitGames::Common::Object &message)
{
	on_status_update.invoke(String(user.cstr()), status, gotMessage, String(message.toString().cstr()));
}

void ChatListener::onReceiveBroadcastMessage(const ExitGames::Common::JString &channelName, nByte *inBuff, int inBuffBodyLength)
{
	on_recieve_broadcast_message.invoke(String(channelName), inBuff, inBuffBodyLength);
}
