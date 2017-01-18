#ifndef READY_SCREEN_H_INCLUDED
#define READY_SCREEN_H_INCLUDED

#include "screen.h"
#include "player.h"

class ReadyScreen : public Screen {
public:
	ReadyScreen();

	void render(RenderList& list, double mouseX, double mouseY);
	std::unique_ptr<Screen> update(GLFWwindow* window);
	std::unique_ptr<Screen> on_click(int button, int action, double mouseX, double mouseY);
	std::unique_ptr<Screen> on_key(int key, int action);

private:
	bool contains_player_for_joystick(int index);
	std::shared_ptr<std::vector<Player>> players;
};

#endif