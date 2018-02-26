#pragma once

#include "human-agent.hh"
#include <ai/sfml/sf-app.hh>
#include <ai/shell/date.hh>

namespace to48
{

    inline World::action_t HumanAgent::action_get(const World& world)
    {
	World::action_t res = World::ACTION_NO;
        auto& win = SfApp::instance();
        sf::Event evt;

        while (res == World::ACTION_NO)
	{
            Date::sleep(10);

            while (win.pollEvent(evt))
	    {
                if (evt.type == sf::Event::EventType::KeyPressed)
		{
                    auto key = evt.key.code;
                    if (key == sf::Keyboard::Left)
                        res = World::ACTION_LEFT;
                    else if (key == sf::Keyboard::Right)
                        res = World::ACTION_RIGHT;
                    else if (key == sf::Keyboard::Up)
                        res = World::ACTION_UP;
                    else if (key == sf::Keyboard::Down)
                        res = World::ACTION_DOWN;
                }
            }

            if (res != World::ACTION_NO && !world.is_action_valid(res))            
		res = World::ACTION_NO;
        }


        return res;
    }

}
