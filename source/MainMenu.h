#pragma once
#include "AppSystemLogic.h"

#include <UnigineComponentSystem.h>

class MainMenu: public Unigine::ComponentBase
{
public:
	COMPONENT_DEFINE(MainMenu, Unigine::ComponentBase);
	COMPONENT_INIT(init);
	COMPONENT_SHUTDOWN(shutdown);

private:
	void init();
	void shutdown();

	void connected();
	void join_lobby_clicked();
	void create_room_clicked();
	void join_room_clicked(const Unigine::WidgetLabelPtr &room_label);
	void leave_lobby_clicked();

	void update_room_list(const Unigine::Vector<Unigine::String> &room_names);

	void *update_room_list_callback_index = nullptr;

	Unigine::WidgetWindowPtr authentication_window = nullptr;
	Unigine::WidgetWindowPtr lobby_window = nullptr;
	Unigine::WidgetEditLinePtr nickname_editline = nullptr;
	Unigine::WidgetScrollBoxPtr rooms_scrollbox = nullptr;

	AppSystemLogic *network_logic = nullptr;

	static int created_rooms_count;
};
