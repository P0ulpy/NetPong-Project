//
// Created by flo on 11/02/2022.
//

#ifndef NETPONG_PROJECT_PLAYERSETTINGS_HPP
#define NETPONG_PROJECT_PLAYERSETTINGS_HPP

#include <string>
#include <SFML/Network/Packet.hpp>
//#include "../PacketOverload.hpp"

struct PlayerSettings
{
    PlayerSettings(const std::string &name = "Unnamed Player", int color = 0) : name(name), color(color) {}

    std::string name;
    int color;
};

#endif //NETPONG_PROJECT_PLAYERSETTINGS_HPP
