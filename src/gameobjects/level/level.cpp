#include "level.h"

#include <fstream>

#include <rapidjson/document.h>
#include <rapidjson/istreamwrapper.h>
#include <rapidjson/error/en.h>

#include <iostream>

#include <exception>

const bool level_build_debug = true;

const char* const level_names[] = {
        "../assets/level/testlevel.json",
        "../assets/level/platforms.json",
        "../assets/level/more_platforms.json",
        "../assets/level/shafts.json",
        "../assets/level/close_quarters.json",
        //"../assets/level/complex.json"
        "../assets/level/the_gap.json"
};

const double line_width = 4;

std::vector<Level> Level::load_all_levels() {
    std::vector<Level> loaded_levels;
    for (const auto& level: level_names) {
        int i = loaded_levels.size();
        try {
            Level loaded_level = Level::load_from_file(level, i);
            loaded_levels.push_back(std::move(loaded_level));
        } catch (const std::exception& ex) {
            std::cout<<"Got error while reloading " << level << ":" << ex.what() <<std::endl;
        };

    }

    return loaded_levels;
}

Level Level::load_from_file(const char* filename, unsigned int index) {
    std::ifstream file(filename);

    rapidjson::BasicIStreamWrapper<std::ifstream> wrapper(file);

    rapidjson::Document level_data;

    level_data.ParseStream<rapidjson::kParseCommentsFlag | rapidjson::kParseTrailingCommasFlag>(wrapper);

    if (level_data.HasParseError()) {
        throw std::runtime_error(std::string("Could not parse ") + filename);
    }

    double l_width = level_data["width"].GetDouble();
    double l_height = level_data["height"].GetDouble();

    bool mirrored = false;

    if (level_data.HasMember("mirrored")){
        mirrored = level_data["mirrored"].GetBool();
    }

    std::vector<Rectangle> obstacles;

    for (const auto& obstacle: level_data["obstacles"].GetArray()) {
        double width = obstacle["xRight"].GetDouble() - obstacle["xLeft"].GetDouble();
        double height = obstacle["yBottom"].GetDouble() - obstacle["yTop"].GetDouble();

        double x = obstacle["xRight"].GetDouble() - width/2;
        double y = obstacle["yBottom"].GetDouble() - height/2;

        Rectangle rect(
            x,
            y,
            width,
            height);

        obstacles.push_back(rect);

        bool is_mirrored = obstacle.HasMember("mirrored") ? obstacle["mirrored"].GetBool() : mirrored;

        if (is_mirrored) {
            Rectangle rect2(
                    l_width - x,
                    y,
                    width,
                    height);

            obstacles.push_back(rect2);
        }
    }


    std::vector<BoxSpawn> box_spawn_locations;
    for (const auto& spawn_location: level_data["boxSpawnLocations"].GetArray()) {

        double x = spawn_location["x"].GetDouble();
        double y = spawn_location["y"].GetDouble();

        Point pos{
            x,
            y};

        std::vector<std::string> weapons;

        for (const auto& weapon : spawn_location["weapons"].GetArray()) {
            weapons.push_back(weapon.GetString());
        }

        int initial_spawn_delay = 100;
        int respawn_delay = 100;

        if (spawn_location.HasMember("initialSpawnDelay")) {
            initial_spawn_delay = spawn_location["initialSpawnDelay"].GetInt();
        }

        if (spawn_location.HasMember("respawnDelay")) {
            respawn_delay = spawn_location["respawnDelay"].GetInt();
        }

        box_spawn_locations.push_back(BoxSpawn(pos, weapons, initial_spawn_delay, respawn_delay, box_spawn_locations.size()));

        bool is_mirrored = spawn_location.HasMember("mirrored") ? spawn_location["mirrored"].GetBool() : mirrored;

        if (is_mirrored)  {
            Point pos2{
                    l_width - x,
                    y};

            box_spawn_locations.push_back(BoxSpawn(pos2,weapons, initial_spawn_delay, respawn_delay, box_spawn_locations.size()));
        }

    }

    std::vector<Point> player_spawn_locations;
    for (const auto& spawn_location: level_data["spawnLocations"].GetArray()) {

        double x = spawn_location["x"].GetDouble();
        double y = spawn_location["y"].GetDouble();
        Point p = {x, y};
        player_spawn_locations.push_back(p);

        bool is_mirrored = spawn_location.HasMember("mirrored") ? spawn_location["mirrored"].GetBool() : mirrored;

        if (is_mirrored)  {
            Point p2 = {l_width-x, y};
            player_spawn_locations.push_back(p2);
        }
    }

    std::vector<Rectangle> killboxes;
    for (const auto& kb: level_data["killboxes"].GetArray()) {
        double width = kb["xRight"].GetDouble() - kb["xLeft"].GetDouble();
        double height = kb["yBottom"].GetDouble() - kb["yTop"].GetDouble();

        double x = kb["xRight"].GetDouble() - width/2;
        double y = kb["yBottom"].GetDouble() - height/2;

        Rectangle rect(
                x,
                y,
                width,
                height);

        killboxes.push_back(rect);

        bool is_mirrored = kb.HasMember("mirrored") ? kb["mirrored"].GetBool() : mirrored;

        if (is_mirrored) {
            Rectangle rect2(
                    l_width - x,
                    y,
                    width,
                    height);

            killboxes.push_back(rect2);
        }
    }

    std::string title = "untitled";
    if (level_data.HasMember("title")) {
        title = level_data["title"].GetString();
    }

    return Level(obstacles, box_spawn_locations, player_spawn_locations, killboxes, l_width, l_height, index, title);
}

Level::Level(
    const std::vector<Rectangle>& a_obstacles,
    const std::vector<BoxSpawn>& a_box_spawns,
    const std::vector<Point>& a_player_spawns,
    const std::vector<Rectangle>& a_killboxes,
    double a_width,
    double a_height,
    unsigned int a_index,
    const std::string& a_title) :
        Level(
            a_obstacles,
            a_box_spawns,
            a_player_spawns,
            a_killboxes,
            a_width,
            a_height,
            a_index,
            a_title,
            TileBackground("tile", Rectangle(a_width/2, a_height/2, 2*a_width, 2*a_height))){}
            //TileBackground("tile", Rectangle(-a_width, -a_height, 2*a_width, 2*a_height))){}

Level::Level(
        const std::vector<Rectangle>& a_obstacles,
        const std::vector<BoxSpawn>& a_box_spawns,
        const std::vector<Point>& a_player_spawns,
        const std::vector<Rectangle>& a_killboxes,
        double a_width,
        double a_height,
        unsigned int a_index,
        const std::string& a_title,
        const TileBackground& bg):
        obstacles(a_obstacles),
        box_spawn_locations(a_box_spawns),
        player_spawn_locations(a_player_spawns),
        killboxes(a_killboxes),
        background(bg),
        width(a_width),
        height(a_height),
        index(a_index),
        title(a_title){}

void Level::render(RenderList& list) const {
    try {
        background.render(list);
    } catch(std::exception e) {
        printf(e.what());
    }
    render_obstacles(list);

    if (level_build_debug) {

        for (auto &kb : killboxes) {
            list.add_outline("red", kb, 3);
        }

        for (auto &spawn : player_spawn_locations) {
            list.add_rect("blue", Rectangle(spawn.x, spawn.y, 10, 10));
        }
    }
}

void Level::render_thumbnail(RenderList &list) const {
    list.add_image("grey", 0, 0, width, height);
    render_obstacles(list, false);
}

void Level::render_obstacles(RenderList &list, bool show_border) const {
    if (show_border) {
        for (const Rectangle& rect : obstacles) {
            list.add_outline("black", rect, line_width);
        }

        for (const Rectangle& rect : obstacles) {
            list.add_image("darkGrey", rect.x - rect.width/2 + line_width, rect.y - rect.height/2 + line_width, rect.width - line_width * 2, rect.height - line_width * 2);
        }
    } else {
        for (const Rectangle& rect : obstacles) {
            double x1 = std::max(0.0, rect.x - rect.width/2);
            double x2 = std::min(width, rect.x + rect.width/2);


            double y1 = std::max(0.0, rect.y - rect.height/2);
            double y2 = std::min(height, rect.y + rect.height/2);

            list.add_image("darkGrey", x1, y1, (x2 - x1), (y2 - y1));
        }
    }
}

bool Level::colliding_with(const Rectangle& other) const {
    for (const Rectangle& rect : obstacles) {
        if (rect.colliding_with(other)) {
            return true;
        }
    }

    return false;
}

bool Level::intersects_line(double x_1, double y_1, double x_2, double y_2) const {
    for (const Rectangle& rect : obstacles) {
        if (rect.intersects_line(x_1, y_1, x_2, y_2)) {
            return true;
        }
    }

    return false;
}

std::vector<Point> Level::get_player_spawn_locations() const {
    return player_spawn_locations;
}

const std::vector<BoxSpawn>& Level::get_box_spawns() const {
    return box_spawn_locations;
}

bool Level::in_killbox(double x, double y) const{
    for (auto& k : killboxes){
        if (k.contains_point(x, y)) return true;
    }
    return false;
}