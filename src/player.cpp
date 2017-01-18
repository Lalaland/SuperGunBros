
#include "player.h"

const double arm_length = 13;
const double arm_radius = 4;

const double leg_length = 21;
const double leg_radius = 5;

const double gun_rotation_x = 1;
const double gun_rotation_y = -30;

const double gun_offset_x = 3;
const double gun_offset_y = -2;

const double barrel_offset_x = gun_rotation_x + gun_offset_x;
const double barrel_offset_y = gun_rotation_y + gun_offset_y + 4;

const double arm_y_offset = -25;

double dist_sq(double x1, double y1, double x2, double y2) {
    return (x1 - x2) * (x1 - x2) + (y1 - y2) * (y1 - y2);
}

double calculate_angle(double target_x, double target_y, double center_x, double center_y, double b1_x, double b1_y) {

    double theta = atan2(center_y - b1_y, b1_x - center_x);

    double b1_offset_sq = dist_sq(b1_x, b1_y, center_x, center_y);
    double target_offset_sq = dist_sq(target_x, target_y, center_x, center_y);

    double far_angle = asin(sqrt(b1_offset_sq) * sin(theta) / sqrt(target_offset_sq));

    double starting_angle = atan2(target_y - center_y, target_x - center_x);

    return starting_angle + far_angle;
}

Player::Player(double start_x, double start_y, PlayerInfo a_info) : info(a_info){
    state.x = start_x;
    state.y = start_y;
    current_time = 0;
    AnimationState frames_init[] = {
            {
                    {30, -30},
                    {20, 0},
            },
            {
                    {20, -20},
                    {30, 30},
            },
            {
                    {0, 0},
                    {90, 0},
            },
            {
                    {-20, 20},
                    {60, 0},
            },


            {
                    {-30, 30},
                    {0, 20},
            },
            {
                    {-20, 20},
                    {30, 30},
            },
            {
                    {0, 0},
                    {0, 90},
            },
            {
                    {20, -20},
                    {0, 60},
            },
    };

    frames = std::vector<AnimationState>(std::begin(frames_init), std::end(frames_init));

}

AnimationState Player::get_interpolated_frame() const {
    AnimationState current_frame = frames[(int) (current_time) % frames.size()];
    AnimationState next_frame = frames[(int) (current_time + 1) % frames.size()];

    double ratio = current_time - floor(current_time);

    AnimationState interpolated = {
            {current_frame.hip_angle[0] * (1 - ratio) + next_frame.hip_angle[0] * ratio, current_frame.hip_angle[1] * (1 - ratio) + next_frame.hip_angle[1] * ratio},
            {current_frame.knee_angle[0] * (1 - ratio) + next_frame.knee_angle[0] * ratio, current_frame.knee_angle[1] * (1 - ratio) + next_frame.knee_angle[1] * ratio},
    };

    return interpolated;
}

void Player::update(GLFWwindow* window) {
    current_time += state.dx * (is_facing_right() ? 1 : -1) * .75;
    prev_state = state;
    // Deal with player input.
    if (info.type == PlayerType::KEYBOARD) {

        state.input.ls.x = 0;
        state.input.ls.x -= (glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_LEFT) == GLFW_PRESS);
        state.input.ls.x += (glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS || glfwGetKey(window, GLFW_KEY_RIGHT) == GLFW_PRESS);

        state.input.ls.y = 0;
        state.input.ls.y -= glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS;
        state.input.ls.y += glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS;

        //Directional Aiming
        double cursorX;
        double cursorY;
        glfwGetCursorPos(window, &cursorX, &cursorY);

        state.input.rs.x = (float)(cursorX - (prev_state.x * 60));
        state.input.rs.y = (float)(cursorY - (prev_state.y * 60));



                //target_point(cursorX, cursorY);

        state.input.buttons[ButtonName::A] = 0;
        state.input.buttons[ButtonName::B] = 0;
        state.input.buttons[ButtonName::X] = 0;
        state.input.buttons[ButtonName::Y] = 0;

        state.input.buttons[ButtonName::LB] = (button_val)(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_RIGHT) == GLFW_PRESS);
        state.input.buttons[ButtonName::RB] = (button_val) (glfwGetKey(window, GLFW_KEY_LEFT_SHIFT) == GLFW_PRESS);

        state.input.buttons[ButtonName::LT] = (button_val) (glfwGetKey(window, GLFW_KEY_SPACE) == GLFW_PRESS);
        state.input.buttons[ButtonName::RT] = (button_val)(glfwGetMouseButton(window, GLFW_MOUSE_BUTTON_LEFT) == GLFW_PRESS);

        state.input.buttons[ButtonName::BACK] = 0;
        state.input.buttons[ButtonName::START] = 0;
        state.input.buttons[ButtonName::L3] = 0;
        state.input.buttons[ButtonName::R3] = 0;

        state.input.buttons[ButtonName::UD] = 0;
        state.input.buttons[ButtonName::DD] = 0;
        state.input.buttons[ButtonName::LD] = 0;
        state.input.buttons[ButtonName::RD] = 0;
    } else {
        state.input = info.gamePad->getInputs();
    }
}

bool Player::is_facing_right() const {
    return fabs(state.gun_angle) < M_PI / 2;
}

Bullet Player::spawn_bullet() const {
    Bullet next_bullet;

    double scaling_factor = is_facing_right() ? 1 : -1;

    double scaled_gun_angle = is_facing_right() ? state.gun_angle : M_PI -state.gun_angle;

    next_bullet.x = state.x + scaling_factor * (gun_rotation_x + cos(scaled_gun_angle) * (gun_offset_x + 20) - sin(scaled_gun_angle) * (gun_offset_y + 4))/60.0;
    next_bullet.y = state.y + (gun_rotation_y + sin(scaled_gun_angle) * (gun_offset_x + 20) + cos(scaled_gun_angle) * (gun_offset_y + 4))/60.0;

    next_bullet.x_vel = BULLET_VEL * cos(state.gun_angle);
    next_bullet.y_vel = BULLET_VEL * sin(state.gun_angle);

    return next_bullet;
}

void Player::target_point(double mouseX, double mouseY) {

    if (is_facing_right()) {
        state.gun_angle = calculate_angle(
                mouseX, mouseY,
                state.x * 60 + gun_rotation_x,
                state.y * 60 + gun_rotation_y,
                state.x * 60 + barrel_offset_x,
                state.y * 60 + barrel_offset_y
        );
    } else {
        state.gun_angle = calculate_angle(
                mouseX, mouseY,
                state.x * 60 - gun_rotation_x,
                state.y * 60 + gun_rotation_y,
                state.x * 60 - barrel_offset_x,
                state.y * 60 + barrel_offset_y
        );
    }

}

void Player::render(RenderList& list) const {
    AnimationState interpolated = get_interpolated_frame();

    double scaled_gun_angle = is_facing_right() ? state.gun_angle : (M_PI - state.gun_angle);

    double posX = state.x * 60;
    double posY = state.y * 60;

    double grip1_dx = gun_offset_x + 2;
    double grip1_dy = gun_offset_y + 10;

    double grip1_x = posX + gun_rotation_x + cos(scaled_gun_angle) * grip1_dx - sin(scaled_gun_angle) * grip1_dy;
    double grip1_y = posY + gun_rotation_y + sin(scaled_gun_angle) * grip1_dx + cos(scaled_gun_angle) * grip1_dy;

    double dist_to_grip1 = sqrt(dist_sq(grip1_x, grip1_y, posX, posY + arm_y_offset));

    double extra_angle1 = acos(dist_to_grip1 / (arm_length * 2));

    double needed_angle1 = atan2(grip1_y - (posY + arm_y_offset), grip1_x - (posX)) - M_PI/2;

    double grip2_dx = gun_offset_x + 15;
    double grip2_dy = gun_offset_y + 10;

    double grip2_x = posX + gun_rotation_x + cos(scaled_gun_angle) * grip2_dx - sin(scaled_gun_angle) * grip2_dy;
    double grip2_y = posY + gun_rotation_y + sin(scaled_gun_angle) * grip2_dx + cos(scaled_gun_angle) * grip2_dy;

    double dist_to_grip2 = sqrt(dist_sq(grip2_x, grip2_y, posX, posY + arm_y_offset));

    double extra_angle2 = acos(dist_to_grip2 / (arm_length * 2));

    double needed_angle2 = atan2(grip2_y - (posY + arm_y_offset), grip2_x - (posX)) - M_PI/2;

    list.translate(posX, posY);

    if (!is_facing_right()) {
        list.scale(-1, 1);
    }

    list.add_image("black", -20, -66, 40, 8);
    list.add_image("red", -18, -64, 36, 4);
    list.add_image("green", -18, -64, 36 * state.health / 100.0, 4);

    {
        list.rotate(interpolated.hip_angle[0] * M_PI/180);


        list.translate(0, leg_length);

        list.rotate(interpolated.knee_angle[0] * M_PI/180);

        list.add_image("legPiece", -leg_radius, -leg_radius);
        list.add_image("boot", -leg_radius, leg_length);

        list.rotate(-interpolated.knee_angle[0] * M_PI/180);
        list.translate(0, -leg_length);

        list.add_image("legPiece", -leg_radius, -leg_radius);

        list.rotate(-interpolated.hip_angle[0] * M_PI/180);
    }

    {
        list.translate(0, arm_y_offset);

        list.rotate(needed_angle2 + extra_angle2);
        list.add_image("armPiece", -arm_radius, -arm_radius);

        list.translate(0, arm_length);

        list.rotate(-2 * extra_angle2);

        list.add_image("armPiece", -arm_radius, -arm_radius);
        list.rotate(2 * extra_angle2);

        list.translate(0, -arm_length);

        list.rotate(-(needed_angle2 + extra_angle2));

        list.translate(0, -arm_y_offset);
    }

    const char* body_color = nullptr;

    switch (info.color) {
        case PlayerColor::RED:
            body_color = "redBody";
            break;

        case PlayerColor::YELLOW:
            body_color = "yellowBody";
            break;

        case PlayerColor::BLUE:
            body_color = "yellowBody";
            break;

        case PlayerColor::GREEN:
            body_color = "yellowBody";
            break;
    }

    list.add_image(body_color, -11, -36);
    list.add_image("head", -11, -55);

    {
        list.translate(0, arm_y_offset);

        list.rotate(needed_angle1 + extra_angle1);
        list.add_image("armPiece", -arm_radius, -arm_radius);

        list.translate(0, arm_length);

        list.rotate(-2 * extra_angle1);

        list.add_image("armPiece", -arm_radius, -arm_radius);
        list.rotate(2 * extra_angle1);

        list.translate(0, -arm_length);

        list.rotate(-(needed_angle1 + extra_angle1));

        list.translate(0, -arm_y_offset);
    }

    {

        list.rotate(interpolated.hip_angle[1] * M_PI/180);
        list.add_image("legPiece", -leg_radius, -leg_radius);

        list.translate(0, leg_length);

        list.rotate(interpolated.knee_angle[1] * M_PI/180);

        list.add_image("legPiece", -leg_radius, -leg_radius);

        list.add_image("boot", -leg_radius, leg_length);

        list.rotate(-interpolated.knee_angle[1] * M_PI/180);


        list.translate(0, -leg_length);

        list.rotate(-interpolated.hip_angle[1] * M_PI/180);
    }

    {
        list.translate(gun_rotation_x, gun_rotation_y);

        list.rotate(scaled_gun_angle);

        list.add_image("rifle", gun_offset_x, gun_offset_y);

        list.rotate(-scaled_gun_angle);

        list.translate(-gun_rotation_x, -gun_rotation_y);
    }

    if (!is_facing_right()) {
        list.scale(-1, 1);
    }

    list.translate(-posX, -posY);
}
