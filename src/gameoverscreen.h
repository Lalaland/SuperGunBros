#ifndef GAME_OVER_SCREEN_H_INCLUDED
#define GAME_OVER_SCREEN_H_INCLUDED

#include "screen.h"
#include "player.h"

class GameOverScreen : public Screen {
public:
	GameOverScreen(const PlayerAttributes& winning_player);

	void render(RenderList& list, double mouseX, double mouseY);
	std::unique_ptr<Screen> update(GLFWwindow* window);
	std::unique_ptr<Screen> on_click(int button, int action, double mouseX, double mouseY);
	std::unique_ptr<Screen> on_key(int key, int action);

private:
	PlayerAttributes winning_player;
};

#endif