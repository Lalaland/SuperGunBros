#ifndef GAME_SCREEN_H_INCLUDED
#define GAME_SCREEN_H_INCLUDED

#include "screen.h"
#include <vector>
#include "player.h"
#include "level.h"

struct Bullet {
	unsigned int player_owner;

	double x;
	double y;
	double x_vel;
	double y_vel;
};

class GameScreen : public Screen {
public:
	GameScreen(const std::shared_ptr<std::vector<Player>>& players);

	void render(RenderList& list, double mouseX, double mouseY);
	std::unique_ptr<Screen> update(GLFWwindow* window);
	std::unique_ptr<Screen> on_click(int button, int action, double mouseX, double mouseY);
	std::unique_ptr<Screen> on_key(int key, int action);

private:
	std::shared_ptr<std::vector<Player>> players;
	std::vector<Bullet> bullets;
    Level level;

    // Is the rectangle at x,y with width and height colliding with the ground?
	bool is_colliding_with_ground(double x, double y, double width, double height);

    // Is there a collision between the two provided rectangles?
	bool rect_rect_colliding(double x1, double y1, double width1, double height1, double x2, double y2, double width2, double height2);
};

#endif