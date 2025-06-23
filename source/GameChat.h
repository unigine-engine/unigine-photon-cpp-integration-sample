#pragma once

#include "ChatListener.h"

#include <UnigineComponentSystem.h>
#include <UnigineVector.h>
#include <UnigineWidgets.h>

#include <Chat-cpp/inc/Client.h>

class GameChat: public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(GameChat, Unigine::ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_UPDATE(update);
	COMPONENT_SHUTDOWN(shutdown);

private:
	void init();
	void update();
	void shutdown();

	void get_public_message(const Unigine::String &channel, const Unigine::Vector<Unigine::String> &senders, const Unigine::Vector<Unigine::String> &messages);
	void get_private_message(const Unigine::String &sender, const Unigine::String &message, const Unigine::String &channel);

	void add_message(const Unigine::String &sender, const Unigine::String &message);

	void send_message();

	void subscribe_on_connected();

	// System
	ChatListener *chat_listener = nullptr;
	ExitGames::Chat::Client *chat_client = nullptr;

	// UI
	Unigine::WidgetWindowPtr chat_window;
	Unigine::WidgetScrollBoxPtr scrollbox;
	Unigine::WidgetEditLinePtr editline;
	Unigine::WidgetButtonPtr send_button;

	Unigine::String application_ID = "no-app-id";
	Unigine::String application_version = "1.0";

	int chat_width = 300;
	int chat_height = 500;

	bool is_subscribed = false;
};
