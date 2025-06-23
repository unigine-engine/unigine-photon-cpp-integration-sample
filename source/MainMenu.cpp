#include "MainMenu.h"

#include <UnigineCallback.h>
#include <UnigineConsole.h>
#include <UnigineWidgets.h>

REGISTER_COMPONENT(MainMenu);

using namespace Unigine;

int MainMenu::created_rooms_count = 0;

void MainMenu::init()
{
	network_logic = static_cast<AppSystemLogic *>(Engine::get()->getSystemLogic(0));

	network_logic->addOnConnectedCallback(MakeCallback(this, &MainMenu::connected));

	authentication_window = WidgetWindow::create("Authentication");
	authentication_window->setWidth(300);
	authentication_window->setHeight(400);

	auto nickname_label = WidgetLabel::create("Nickname:");
	nickname_label->setFontSize(28);

	nickname_editline = WidgetEditLine::create();
	nickname_editline->setFontSize(28);

	auto vbox = WidgetVBox::create();
	vbox->setWidth(300);
	vbox->addChild(nickname_label, Gui::ALIGN_LEFT);
	vbox->addChild(nickname_editline, Gui::ALIGN_EXPAND);

	authentication_window->addChild(vbox, Gui::ALIGN_TOP);

	auto join_button = WidgetButton::create("Join Lobby");
	join_button->getEventClicked().connect(this, &MainMenu::join_lobby_clicked);
	join_button->setFontSize(30);

	vbox = WidgetVBox::create();
	vbox->addChild(join_button, Gui::ALIGN_CENTER);

	authentication_window->addChild(vbox, Gui::ALIGN_TOP | Gui::ALIGN_EXPAND);

	lobby_window = WidgetWindow::create("Lobby");
	lobby_window->setWidth(500);
	lobby_window->setHeight(300);

	rooms_scrollbox = WidgetScrollBox::create();
	rooms_scrollbox->setWidth(500);
	rooms_scrollbox->setHeight(250);
	rooms_scrollbox->setHScrollEnabled(false);

	update_room_list_callback_index = network_logic->addRoomListUpdatedCallback(MakeCallback(this, &MainMenu::update_room_list));

	lobby_window->addChild(rooms_scrollbox, Gui::ALIGN_TOP);

	auto gridbox = WidgetGridBox::create(2);

	auto leave_lobby_button = WidgetButton::create("Leave");
	leave_lobby_button->getEventClicked().connect(this, &MainMenu::leave_lobby_clicked);
	gridbox->addChild(leave_lobby_button, Gui::ALIGN_LEFT);

	auto create_room_button = WidgetButton::create("Create Room");
	create_room_button->getEventClicked().connect(this, &MainMenu::create_room_clicked);
	gridbox->addChild(create_room_button, Gui::ALIGN_RIGHT);

	lobby_window->addChild(gridbox, Gui::ALIGN_EXPAND);

	if (network_logic->isConnected())
	{
		WindowManager::getMainWindow()->addChild(lobby_window, Gui::ALIGN_CENTER);
	} else
	{
		WindowManager::getMainWindow()->addChild(authentication_window, Gui::ALIGN_CENTER);
	}
}

void MainMenu::shutdown()
{
	authentication_window.deleteLater();
	lobby_window.deleteLater();

	network_logic->removeRoomListUpdatedCallback(update_room_list_callback_index);
}

void MainMenu::connected()
{
	WindowManager::getMainWindow()->removeChild(authentication_window);
	WindowManager::getMainWindow()->addChild(lobby_window, Gui::ALIGN_CENTER);
}

void MainMenu::join_lobby_clicked()
{
	network_logic->connect(nickname_editline->getText());
}

void MainMenu::create_room_clicked()
{
	network_logic->createRoom(network_logic->getPlayerName() + String::itoa(++created_rooms_count));
}

void MainMenu::join_room_clicked(const WidgetLabelPtr &room_label)
{
	network_logic->joinRoom(room_label->getText());
}

void MainMenu::leave_lobby_clicked()
{
	WindowManager::getMainWindow()->removeChild(lobby_window);

	network_logic->disconnect();

	WindowManager::getMainWindow()->addChild(authentication_window, Gui::ALIGN_CENTER);
}

void MainMenu::update_room_list(const Vector<String> &room_names)
{
	int num = rooms_scrollbox->getNumChildren();
	for (int i = 0; i < num; i++)
	{
		auto child = rooms_scrollbox->getChild(0);
		rooms_scrollbox->removeChild(child);
	}

	for (int i = 0; i < room_names.size(); i++)
	{
		auto room_label = WidgetLabel::create(room_names[i]);
		room_label->setFontSize(20);
		room_label->getEventDoubleClicked().connect(*this, [this, room_label]() { join_room_clicked(room_label); });
		rooms_scrollbox->addChild(room_label, Gui::ALIGN_TOP | Gui::ALIGN_EXPAND);
	}
}
