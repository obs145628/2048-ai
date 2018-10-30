#include <cassert>
#include <iostream>
#include <fstream>
#include <unistd.h>
#include <ai/misc/timer.hh>
#include "world.hh"
#include "agent.hh"
#include "expectimax-ai.hh"
#include "grid.hh"
#include "heur.hh"
#include "human-agent.hh"
#include "min-max-ai.hh"
#include "replay-agent.hh"

int task_gridtest(int argc, char** argv)
{
    if (argc < 4)
    {
	std::cerr << "gridtest: missing arguments\n";
	return 1;
    }
    
    std::string type = argv[2];
    std::string moves_file = argv[3];

    std::ifstream is(moves_file);

    if (type == "world")
    {
	to48::World world(4);
	world.play_actions(to48::World::unserialize_actions(is));
	world.write_full(std::cout);
	return 0;
    }

    else
    {
	std::cerr << "Unknow type: " << type << std::endl; 
	return 1;
    }
}

void play_gui(to48::Agent* agent, bool cli)
{
    to48::World world(4);
    long delta = 0;
    Timer timer;
    agent->init(world);
    if (cli)
      world.render_cli();
    else
      world.render_gui();
    
    while (!world.is_finished())
    {
	timer.reset();
	to48::World::action_t action = agent->action_get(world);
	delta = timer.get();
	world.take_action(action);
	world.delta_set(delta);
	agent->after(world);
	if (cli)
	  world.render_cli();
	else
	  world.render_gui();
    }

    std::cout << "\n";
    agent->stats();

    std::ofstream moves_os("moves.out");
    world.write_actions(moves_os);
    std::ofstream full_os("full.out");
    world.write_full(full_os);
}

template <class T>
void play_stats(to48::heur_f heur)
{
    to48::score_t sum_score = 0;
    to48::score_t min_score = 1e6;
    to48::score_t max_score = 0;
    long min_tilde = 1e6;
    long max_tilde = 0;
    int n = 1;

    to48::World world(4);

    while (true)
    {
	to48::heur_nb_evals = 0;
	to48::grid_t grid = 0;
	to48::score_t score = 0;
	T agent{heur};
	agent.init(world);

	while (!to48::grid_is_finished(grid))
        {
	    to48::World::action_t action = agent.action_get(world);
	    grid = to48::grid_move(grid, score, action);
	    agent.after(world);
        }

	sum_score += score;
	if (score > max_score)
	    max_score = score;
	if (score < min_score)
	    min_score = score;

	long tilde = std::pow(2, to48::grid_get_max(grid));
	if (tilde > max_tilde)
	    max_tilde = tilde;
	if (tilde < min_tilde)
	    min_tilde = tilde;

	agent.stats();

	std::cout << "----------------------\n";
	std::cout << "Run #" << n << ":\n";
	std::cout << "Score: " << score << "( " << tilde << ")\n"; 
	std::cout << "Min: " << min_score << " (" << min_tilde << ")\n";
	std::cout << "Max: " << max_score << " (" << max_tilde << ")\n";

	double av = double(sum_score) / n;
	std::cout << "Average: " << av << "\n";
	std::cout << "----------------------\n\n";

	++n;
    }

}

int main(int argc, char** argv)
{
    if (argc < 3)
	return 1;

    if (argv[1] == std::string("gridtest"))
	return task_gridtest(argc, argv);
  
    to48::grid_init();
    to48::heur_init();
    std::string mode = argv[1];
    std::string agent = argv[2];

    if (mode == "gui" || mode == "cli")
    {
	if (agent == "minmax")
	  play_gui(new to48::MinMaxAI(to48::heur_eval1), mode == "cli");
	else if (agent == "expectimax")
	  play_gui(new to48::ExpectimaxAI(to48::heur_eval1), mode == "cli");
	else if (agent == "human")
	  play_gui(new to48::HumanAgent(), mode == "cli");

	else if (agent == "replay")
	{
	    std::ifstream is(argv[3]);
	    play_gui(new to48::ReplayAgent(to48::World::unserialize_actions(is)), mode == "cli");
	}
	
	else
	    return 1;
    }

    else if (mode == "stats")
    {
	if (agent == "minmax")
	    play_stats<to48::MinMaxAI>(to48::heur_eval1);
	else if (agent == "expectimax")
	    play_stats<to48::ExpectimaxAI>(to48::heur_eval1);
	else
	    return 1;
    }

    else
	return 1;

  

    return 0;
}
