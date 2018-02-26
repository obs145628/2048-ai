#include <iostream>
#include <ai/shell/date.hh>
#include <ai/sfml/sf-app.hh>
#include "world.hh"
#include "grid.hh"
#include "heur.hh"
#include "min-max-ai.hh"
#include "human-agent.hh"
#include "monte-carlo.hh"


void grid_update_world(to48::World& world, to48::grid_t grid)
{
    for (int i = 0; i < 4; ++i)
        for (int j = 0; j < 4; ++j)
            world.cell_set(i, j, to48::grid_get(grid, i, j));
}

void update_window(to48::World& world,
                   to48::grid_t grid, to48::score_t score)
{
    auto& win = SfApp::instance();
    grid_update_world(world, grid);
    world.score_set(score);

    sf::Event evt;
    while (win.pollEvent(evt))
        continue;

    win.clear(sf::Color::White);
    world.render_gui();
    win.display();
}

int main()
{
    SfApp::instance().create(sf::VideoMode(800, 800), "2048");

    to48::heur_init();
    to48::grid_init();
    to48::World world(4);

    //to48::AI* agent = new to48::MinMaxAI(to48::heur_eval1);
    //auto agent = new to48::MinMaxAI(to48::heur_eval2);
    auto agent = new to48::MonteCarlo();

    to48::grid_t grid = 0;
    to48::score_t score = 0;

    agent->init(grid, score);

    long delta = 0;

    update_window(world, grid, score);

    while (!to48::grid_is_finished(grid))
    {
        long start = Date::now();
        to48::grid_t res = to48::grid_move(grid, score, agent->move_get(grid));
        delta = Date::now() - start;

        (void) delta;

        assert(res != grid);
        grid = res;

        agent->after(grid, score);

        update_window(world, grid, score);
    }

    Date::sleep(5000);

    agent->stats();

    return 0;
}
