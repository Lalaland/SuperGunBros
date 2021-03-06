#include "pierce.h"
#include "piercebullet.h"

double Pierce::gun_rotation_x() const {
    return 0;
}

double Pierce::gun_rotation_y() const {
    return -20;
}

double Pierce::gun_offset_x() const {
    return 2;
}

double Pierce::gun_offset_y() const {
    return -8;
}

double Pierce::barrel_offset_x() const {
    return 0;
}

double Pierce::barrel_offset_y() const {
    return 7;
}

const char* Pierce::gun_image_name() const {
    return "piercer";
}

bool Pierce::in_front() const {
    return true;
}

double Pierce::grip1_dx() const {
    return gun_offset_x() + 11;
}

double Pierce::grip1_dy() const {
    return gun_offset_y() + 9;
}

double Pierce::grip2_dx() const {
    return gun_offset_x() + 23;
}

double Pierce::grip2_dy() const {
    return gun_offset_y() + 11;
}

std::unique_ptr<Bullet> Pierce::create_initial_bullet() const {
    return std::make_unique<PierceBullet>();
}