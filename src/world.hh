#pragma once

#include <iostream>
#include <vector>
#include <SFML/Graphics.hpp>
#include <ai/shell/shell-canvas.hh>
#include <ai/misc/random-mt.hh>

namespace to48
{

    class World
    {
    public:

	using action_t = int;

	static constexpr action_t ACTION_LEFT = 0;
	static constexpr action_t ACTION_RIGHT = 1;
	static constexpr action_t ACTION_UP = 2;
	static constexpr action_t ACTION_DOWN = 3;
	static constexpr action_t ACTION_NO = 4;

	static void serialize_actions(const std::vector<int>& actions,
				      std::ostream& os);
	static std::vector<int> unserialize_actions(std::istream& is);
	
	World(std::size_t size);
	std::size_t size_get() const;
	std::size_t score_get() const;
	long delta_get() const;

	/**
	 * Returns vector of all actions since start of the game
	 * [random_pos, random_val, random_pos, random_val, ]
	 * [action, random_pos, random_val, ...]
	 */
	const std::vector<int>& actions_get() const;
	
	void score_set(std::size_t score);
	void delta_set(long delta);
	int cell_get(std::size_t row, std::size_t col) const;
	int cell_get(std::size_t pos) const;
	void cell_set(std::size_t row, std::size_t col, int val);
	void cell_set(std::size_t pos, int val);

	void reset();
	void add_rand();
	void add_num(std::size_t pos, int val);
	void render_gui();
	void render_cli();

	/**
	 * Move methods: Update the grid and the score
	 * Doesn't add the random tile
	 */
	std::size_t move_left();
	std::size_t move_right();
	std::size_t move_up();
	std::size_t move_down();
	std::size_t take_action(action_t action, bool check = true);

	void play_actions(const std::vector<int>& actions);

	bool is_action_valid(action_t action) const;
	bool is_finished() const;
	

	
	void serialize(std::ostream& os) const;
	void write_actions(std::ostream& os) const;
	void read_actions(std::istream& is);

	void write_full(std::ostream& os) const;

    private:
	std::size_t size_;
	std::size_t score_;
	std::vector<int> actions_;
	long delta_;
	std::vector<int> data_;
	RandomMT rand_;
	sf::Font font_;

	void flip_();
	void transpose_();
	

    };

}
