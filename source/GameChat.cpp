#include <UnigineConsole.h>
#include <UnigineJson.h>

#include "GameChat.h"
#include "AppSystemLogic.h"

REGISTER_COMPONENT(GameChat);

using namespace Unigine;
using namespace ExitGames;
using namespace Chat;

void GameChat::init()
{
	chat_listener = new ChatListener();

	auto application_params = Json::create();
	application_params->load("application_params.json");

	application_ID = application_params->getChild("chat_application_id")->getString();
	application_version = application_params->getChild("chat_application_version")->getString();

	if (String::equal(application_ID, "no-app-id"))
	{
		Log::error("No chat appID in application_params.json, unable to connect to the server!\n");
	}

	chat_listener->addGetMessagesCallback(MakeCallback(this, &GameChat::get_public_message));
	chat_listener->addPrivateMessageCallback(MakeCallback(this, &GameChat::get_private_message));
	chat_listener->addOnConnectedCallback(MakeCallback(this, &GameChat::subscribe_on_connected));

	chat_client = new ExitGames::Chat::Client(*chat_listener, application_ID.get(), application_version.get());
	chat_client->setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));
	Common::Base::setListener(chat_listener);
	Common::Base::setDebugOutputLevel(DEBUG_RELEASE(ExitGames::Common::DebugLevel::INFO, ExitGames::Common::DebugLevel::WARNINGS));


	auto username = static_cast<AppSystemLogic *>(Engine::get()->getSystemLogic(0))->getPlayerName();
	Console::write("Connecting to nameserver as user ");
	Console::write(username.get());
	Console::writeLine("");

	chat_client->connect(Chat::AuthenticationValues().setUserID(username.get()));

	auto gui = WindowManager::getMainWindow()->getGui();

	chat_window = WidgetWindow::create(gui);
	chat_window->setWidth(chat_width);
	chat_window->setHeight(chat_height);
	chat_window->setMoveable(true);
	chat_window->setSizeable(false);
	chat_window->setMaxWidth(chat_width);
	chat_window->setMaxHeight(chat_height);

	WindowManager::getMainWindow()->addChild(chat_window, Gui::ALIGN_OVERLAP);

	auto info_label = WidgetLabel::create("This sample is a simplified example of Photon integration with UNIGINE.<br><br>"
			"Use <b>WASD</b> buttons to move, <b>QE</b> to rotate, <b>LMB</b> to shoot other players.<br><br>"
			"As the life progress bar is empty, the Leave button is displayed on the screen.Click it to return to Lobby.<br><br>"
			"In the message box, you can type in messages to send them to all users, or send messages starting with <b>@username</b> to exchange private messages with that user.<br> ");

	info_label->setFontWrap(true);
	info_label->setFontRich(true);

	chat_window->addChild(info_label, Gui::ALIGN_EXPAND);

	scrollbox = WidgetScrollBox::create(gui, 0, 10);
	scrollbox->setHScrollEnabled(false);
	chat_window->addChild(scrollbox, Gui::ALIGN_TOP | Gui::ALIGN_EXPAND);

	auto hbox = WidgetHBox::create(gui);
	hbox->setWidth(chat_width);

	editline = WidgetEditLine::create(gui);
	hbox->addChild(editline, Gui::ALIGN_LEFT | Gui::ALIGN_EXPAND);

	send_button = WidgetButton::create(gui, "Send");
	send_button->getEventClicked().connect(this, &GameChat::send_message);
	hbox->addChild(send_button, Gui::ALIGN_LEFT);

	chat_window->addChild(hbox, Gui::ALIGN_BOTTOM);
}

void GameChat::update()
{
	chat_client->service();

	if (Input::isKeyDown(Input::KEY_ENTER) && is_subscribed)
	{
		send_message();
	}
}

void GameChat::shutdown()
{
	auto roomname = static_cast<AppSystemLogic *>(Engine::get()->getSystemLogic(0))->getRoomName();
	Common::JVector<Common::JString> chs(1);
	chs.addElement(roomname);
	chat_client->opUnsubscribe(chs);
	chat_client->disconnect();

	chat_window.deleteLater();

	if (chat_client)
	{
		delete chat_client;
	}

	if (chat_listener)
	{
		delete chat_listener;
	}
}

void GameChat::get_public_message(const String &channel, const Vector<String> &senders, const Vector<String> &messages)
{
	for (int i = 0; i < senders.size(); i++)
	{
		add_message("[" + senders[i] + "] >>> ", messages[i]);
	}
}

void GameChat::get_private_message(const String &sender, const String &message, const String &channel)
{
	add_message("[Private " + sender + " to " + channel + "] >>> ", message);
}

void GameChat::add_message(const String &sender, const String &message)
{
	auto hbox = WidgetHBox::create();
	hbox->setWidth(scrollbox->getWidth() - 20);

	auto label_sender = WidgetLabel::create(sender);
	label_sender->arrange();

	hbox->addChild(label_sender, Gui::ALIGN_LEFT | Gui::ALIGN_TOP);

	auto message_vbox = WidgetVBox::create();

	auto label_message = WidgetLabel::create(message);
	label_message->setFontWrap(true);
	label_message->setWidth(hbox->getWidth() - label_sender->getWidth());
	label_message->arrange();
	while (label_message->getWidth() > hbox->getWidth() - label_sender->getWidth())
	{
		int count = label_message->getWidth() / (hbox->getWidth() - label_sender->getWidth()) + 1;
		Unigine::String str = label_message->getText();
		label_message->setText(str.substr(0, str.size() / count - 1));

		message_vbox->addChild(label_message, Gui::ALIGN_LEFT);

		label_message = WidgetLabel::create(str.substr(str.size() / count - 1));
		label_message->setFontWrap(true);
		label_message->setWidth(hbox->getWidth() - label_sender->getWidth());
		label_message->arrange();
	}

	message_vbox->addChild(label_message, Gui::ALIGN_LEFT);

	hbox->addChild(message_vbox, Gui::ALIGN_LEFT | Gui::ALIGN_TOP);

	scrollbox->addChild(hbox, Gui::ALIGN_LEFT);

	scrollbox->arrange();
}

void GameChat::send_message()
{
	Unigine::String buffer = editline->getText();
	editline->setText("");

	if (buffer.size() == 0)
	{
		return;
	}

	if (buffer[0] == '@')
	{
		int sep = buffer.find(' ');
		chat_client->opSendPrivateMessage(buffer.substr(1, sep - 1).get(), buffer.substr(sep + 1).get());
	} else
	{
		if (chat_client->getPublicChannels().getSize() > 0)
		{
			chat_client->opPublishMessage(chat_client->getPublicChannels()[0]->getName(), buffer.get());
		}
	}
}

void GameChat::subscribe_on_connected()
{
	auto roomname = static_cast<AppSystemLogic *>(Engine::get()->getSystemLogic(0))->getRoomName();
	Common::JVector<Common::JString> chs(1);
	chs.addElement(roomname);

	if (chat_client->opSubscribe(chs))
	{
		Log::message("Subscribing...\n");
		is_subscribed = true;
	}
}
