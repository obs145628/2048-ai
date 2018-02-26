#include "world.hh"
#include <algorithm>
#include <cassert>
#include <ai/shell/date.hh>
#include <ai/sfml/sf-app.hh>
#include <ai/shell/grid.hh>
#include <ai/shell/shell-sprite.hh>
#include <ai/shell/shell.hh>

namespace
{

    constexpr int GRID_SIZE = 600;
    constexpr int MARGIN = 20;
    constexpr int FONT_SIZE = 150;

    const sf::Color BORDER_COLOR(187, 173, 160);


    const sf::Color BG_COLORS[] =
    {
	sf::Color(205, 192, 180), //empty
	sf::Color(238, 228, 218), //2
	sf::Color(237, 224, 200), //4
	sf::Color(242, 177, 121), //8
	sf::Color(242, 177, 121), //16
	sf::Color(246, 124, 95), //32
	sf::Color(246, 94, 59), //64
	sf::Color(237, 207, 114), //128
	sf::Color(237, 204, 97), //256
	sf::Color(237, 200, 80), //512
	sf::Color(237, 197, 63), //1024
	sf::Color(237, 194, 46), //2048
	sf::Color(0, 0, 0), //4096
	sf::Color(0, 0, 0), //8192
	sf::Color(0, 0, 0), //16384
	sf::Color(0, 0, 0), //32768
	sf::Color(0, 0, 0), //65536
	sf::Color(0, 0, 0), //131072
    };


    const sf::Color FG_COLORS[] =
    {
	sf::Color(205, 192, 180), //empty
	sf::Color(119, 110, 101), //2
	sf::Color(119, 110, 101), //4
	sf::Color(255, 255, 255), //8
	sf::Color(255, 255, 255), //16
	sf::Color(255, 255, 255), //32
	sf::Color(255, 255, 255), //64
	sf::Color(255, 255, 255), //128
	sf::Color(255, 255, 255), //256
	sf::Color(255, 255, 255), //512
	sf::Color(255, 255, 255), //1024
	sf::Color(255, 255, 255), //2048
	sf::Color(255, 255, 255), //4096
	sf::Color(255, 255, 255), //8192
	sf::Color(255, 255, 255), //16384
	sf::Color(255, 255, 255), //32768
	sf::Color(255, 255, 255), //65536
	sf::Color(255, 255, 255), //131072
    };


    int pow2(int n)
    {
        int res = 1;
        for (int i = 0; i < n; ++i)
            res *= 2;
        return res;
    }

    static const std::string GRID_LABELS[] =
    {
	"     ",
	"  2  ",
	"  4  ",
	"  8  ",
	" 16  ",
	" 32  ",
	" 64  ",
	" 128 ",
	" 256 ",
	" 512 ",
	"1024 ",
	"2048 ",
	"4096 ",
	"8192 ",
	"16384",
	"32768",
    };


    static const char* GRID_FGS[] =
    {
	Shell::FG_DEFAULT,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_BLACK,
	Shell::FG_WHITE,
	Shell::FG_WHITE,
	Shell::FG_WHITE,
	Shell::FG_WHITE,
    };

    static const char* GRID_BGS[] =
    {
	Shell::BG_DEFAULT,
	Shell::BG_LGRAY,
	Shell::BG_DGRAY,
	Shell::BG_LGREEN,
	Shell::BG_GREEN,
	Shell::BG_LYELLOW,
	Shell::BG_YELLOW,
	Shell::BG_LRED,
	Shell::BG_RED,
	Shell::BG_LMAGENTA,
	Shell::BG_MAGENTA,
	Shell::BG_BLUE,
	Shell::BG_BLACK,
	Shell::BG_BLACK,
	Shell::BG_BLACK,
	Shell::BG_BLACK
    };
}

namespace to48 {

    void World::serialize_actions(const std::vector<int>& actions,
				  std::ostream& os)
    {
	os << actions[0] << " " << actions[1] << std::endl;
	os << actions[2] << " " << actions[3] << std::endl;
	for (std::size_t i = 4; i < actions.size(); i += 3)
	    os << actions[i] << " " << actions[i + 1] << " " << actions[i + 2] << std::endl;
    }

    std::vector<int> World::unserialize_actions(std::istream& is)
    {
	std::vector<int> res;

	while (!is.eof())
	{
	    int val;
	    is >> val;
	    res.push_back(val);
	}

	if ((res.size() - 4) % 3 * 3 != res.size() - 4)
	    res.pop_back();
	
	return res;
    }

    World::World(std::size_t size)
	: size_(size), score_(0), data_(size_ * size_, 0)
    {
        assert(size_ >= 2);
        font_.loadFromFile("../misc/courier.ttf");
	reset();
    }

    std::size_t World::size_get() const
    {
        return size_;
    }

    long World::delta_get() const
    {
	return delta_;
    }

    const std::vector<int>& World::actions_get() const
    {
	return actions_;
    }

    int World::cell_get(std::size_t row, std::size_t col) const
    {
        assert(row < size_);
        assert(col < size_);
        return data_[row * size_ + col];
    }

    int World::cell_get(std::size_t pos) const
    {
        assert(pos < size_ * size_);
        return data_[pos];
    }

    void World::cell_set(std::size_t row, std::size_t col, int val)
    {
        assert(row < size_);
        assert(col < size_);
        data_[row * size_ + col] = val;
    }

    void World::cell_set(std::size_t pos, int val)
    {
        assert(pos < size_ * size_);
        data_[pos] = val;
    }

    std::size_t World::score_get() const
    {
	return score_;
    }

    void World::score_set(std::size_t score)
    {
        score_ = score;
    }

    void World::delta_set(long delta)
    {
	delta_ = delta;
    }

    void World::reset()
    {
	std::fill(data_.begin(), data_.end(), 0);
	score_ = 0;
	delta_ = 0;
	actions_.clear();
    }

    void World::add_rand()
    {
	int val = 1 + (rand_.int32_get() % 10) / 9;
	int pos;

	while (true)
	{
	    pos = rand_.int32_get() % (size_ * size_);
	    if (data_[pos])
		continue;
	    data_[pos] = val;
	    break;
	}

	actions_.push_back(pos);
	actions_.push_back(val);
    }

    void World::add_num(std::size_t pos, int val)
    {
	assert(pos < size_ * size_);
        data_[pos] = val;
	actions_.push_back(pos);
	actions_.push_back(val);
    }

    void World::render_gui()
    {
        auto& win = SfApp::instance();
	if (!win.isOpen())
	    win.create(sf::VideoMode(800, 800), "2048");
	
        int n = size_;
        int square_size = GRID_SIZE / n;
        int grid_size = square_size * n;

	win.poll_events();
	win.clear(sf::Color::White);

        for (int i = 0; i < n; ++i)
            for (int j = 0; j < n; ++j) {
                int val = cell_get(i, j);
                int x = MARGIN + j * square_size;
                int y = MARGIN + i * square_size;

                win.fill_rect(x, y, square_size, square_size,
                              BG_COLORS[val]);

                sf::Text text;
                text.setFont(font_);
                text.setString(std::to_string(pow2(val)));
                text.setCharacterSize(FONT_SIZE / n);
                text.setFillColor(FG_COLORS[val]);
                auto size = text.getLocalBounds();
                text.setPosition(x + (square_size - size.width) / 2,
                                 y + (square_size - size.height) / 2);
                win.draw(text);
            }


        for (int i = 0; i <= n; ++i) {
            win.draw_line(MARGIN, MARGIN + square_size * i,
                          MARGIN + grid_size, MARGIN + square_size * i,
                          BORDER_COLOR);
            win.draw_line(MARGIN + square_size * i, MARGIN,
                          MARGIN + square_size * i, MARGIN + grid_size,
                          BORDER_COLOR);
        }

	sf::Text score_text;
	score_text.setFont(font_);
	score_text.setString("score: " + std::to_string(score_));
	score_text.setCharacterSize(FONT_SIZE / 6);
	score_text.setFillColor(sf::Color::Black);
	score_text.setPosition(MARGIN, MARGIN + grid_size + 1 * square_size / 4);
	win.draw(score_text);

	sf::Text delta_text;
	delta_text.setFont(font_);
	delta_text.setString("delta: " + std::to_string(delta_));
	delta_text.setCharacterSize(FONT_SIZE / 6);
	delta_text.setFillColor(sf::Color::Black);
	delta_text.setPosition(MARGIN, MARGIN + grid_size + 1 * square_size / 2);
	win.draw(delta_text);

	win.display();
    }

    void World::render_cli()
    {
	auto& cvs = ShellCanvas::instance();
	cvs.clear();
	cvs.draw_string(0, 0, "Delta: " + std::to_string(delta_));
	cvs.draw_string(0, 1, "Score: " + std::to_string(score_));

	Grid res(size_, size_);
	for (std::size_t i = 0; i < size_; ++i)
	    for (std::size_t j = 0; j < size_; ++j)
	    {
		auto val = cell_get(i, j);
		res.at(i, j) = GRID_LABELS[val];
		res.fg_set(i, j, GRID_FGS[val]);
		res.bg_set(i, j, GRID_BGS[val]);
	    }
	
	cvs.draw_sprite(2, 2, res.to_sprite());
	cvs.render();
    }

    std::size_t World::move_left()
    {
	std::size_t n = size_;
	std::size_t reward = 0;

	for (std::size_t i = 0; i < n; ++i)
	{
	    std::size_t level = 0;
	    bool can_fusion = false;
	    while (level < n)
	    {
		std::size_t j = level;
		while (j < n && cell_get(i, j) == 0)
		    ++j;
		if (j == n)
		    break;

		int val = cell_get(i, j);
		cell_set(i, j, 0);
		if (can_fusion && val == cell_get(i, level - 1))
		{
		    cell_set(i, level - 1, val + 1);
		    reward += pow2(val + 1);
		    can_fusion = false;
		}
		else
		{
		    cell_set(i, level, val);
		    can_fusion = true;
		    ++level;
		}
	    }
	}

	return reward;
    }

    std::size_t World::move_right()
    {
	/*
	flip_();
	auto reward = move_left();
	flip_();
	return reward;
	*/

	std::size_t n = size_;
	std::size_t reward = 0;

	for (std::size_t i = 0; i < n; ++i)
	{
	    std::size_t level = n -1;
	    bool can_fusion = false;
	    while (level < n)
	    {
		std::size_t j = level;
		while (j < n && cell_get(i, j) == 0)
		    --j;
		if (j > n)
		    break;

		int val = cell_get(i, j);
		cell_set(i, j, 0);
		if (can_fusion && val == cell_get(i, level + 1))
		{
		    cell_set(i, level + 1, val + 1);
		    reward += pow2(val + 1);
		    can_fusion = false;
		}
		else
		{
		    cell_set(i, level, val);
		    can_fusion = true;
		    --level;
		}
	    }
	}

	return reward;
    }

    std::size_t World::move_up()
    {
	/*
	transpose_();
	auto reward = move_left();
	transpose_();
	return reward;
	*/

	std::size_t n = size_;
	std::size_t reward = 0;

	for (std::size_t i = 0; i < n; ++i)
	{
	    std::size_t level = 0;
	    bool can_fusion = false;
	    while (level < n)
	    {
		std::size_t j = level;
		while (j < n && cell_get(j, i) == 0)
		    ++j;
		if (j == n)
		    break;

		int val = cell_get(j, i);
		cell_set(j, i, 0);
		if (can_fusion && val == cell_get(level - 1, i))
		{
		    cell_set(level - 1, i, val + 1);
		    reward += pow2(val + 1);
		    can_fusion = false;
		}
		else
		{
		    cell_set(level, i, val);
		    can_fusion = true;
		    ++level;
		}
	    }
	}

	return reward;
    }

    std::size_t World::move_down()
    {
	/*
        transpose_();
	auto reward = move_right();
	transpose_();
	return reward;
	*/

	std::size_t n = size_;
	std::size_t reward = 0;

	for (std::size_t i = 0; i < n; ++i)
	{
	    std::size_t level = n -1;
	    bool can_fusion = false;
	    while (level < n)
	    {
		std::size_t j = level;
		while (j < n && cell_get(j, i) == 0)
		    --j;
		if (j > n)
		    break;

		int val = cell_get(j, i);
		cell_set(j, i, 0);
		if (can_fusion && val == cell_get(level + 1, i))
		{
		    cell_set(level + 1, i, val + 1);
		    reward += pow2(val + 1);
		    can_fusion = false;
		}
		else
		{
		    cell_set(level, i, val);
		    can_fusion = true;
		    --level;
		}
	    }
	}

	return reward;
    }
    
    std::size_t World::take_action(action_t action, bool check)
    {
	
	assert(action < 4);
	if (check)
	    assert(is_action_valid(action));

	std::size_t reward;
	if (action == ACTION_LEFT)
	    reward = move_left();
	else if (action == ACTION_RIGHT)
	    reward = move_right();
	else if (action == ACTION_UP)
	    reward = move_up();
	else
	    reward = move_down();

	score_ += reward;
	actions_.push_back(action);
	return reward;
    }

    void World::play_actions(const std::vector<int>& actions)
    {
	reset();
	add_num(actions[0], actions[1]);
	add_num(actions[2], actions[3]);

	for (std::size_t i = 4; i < actions.size(); i += 3)
	{
	    take_action(actions[i]);
	    add_num(actions[i + 1], actions[i + 2]);
	}
    }

    bool World::is_action_valid(action_t action) const
    {
	World copy(size_);
	std::copy(data_.begin(), data_.end(), copy.data_.begin());
	copy.take_action(action, false);
	return !std::equal(data_.begin(), data_.end(), copy.data_.begin());
    }
    
    bool World::is_finished() const
    {
	for (action_t a = 0; a < 4; ++a)
	    if (is_action_valid(a))
		return false;
	return true;
    }

    void World::serialize(std::ostream& os) const
    {
	for (std::size_t i = 0; i < size_ * size_; ++i)
	    os << cell_get(i) << " ";
	os << score_;
    }

    void World::write_actions(std::ostream& os) const
    {
	serialize_actions(actions_, os);
    }
    
    void World::read_actions(std::istream& is)
    {
	play_actions(unserialize_actions(is));
    }

    void World::write_full(std::ostream& os) const
    {
	World copy(size_);

	copy.add_num(actions_[0], actions_[1]);
	os << actions_[0] << " " << actions_[1] << std::endl;
	copy.serialize(os);
	os << std::endl;
	copy.add_num(actions_[2], actions_[3]);
	os << actions_[2] << " " << actions_[3] << std::endl;
	copy.serialize(os);
	os << std::endl;

	for (std::size_t i = 4; i < actions_.size(); i += 3)
	{
	    copy.take_action(actions_[i]);
	    copy.add_num(actions_[i + 1], actions_[i + 2]);
	    os << actions_[i] << " " << actions_[i + 1] << " "
	       << actions_[i + 2] << std::endl;
	    copy.serialize(os);
	    os << std::endl;
	}
    }


    void World::flip_()
    {
	std::vector<int> data(size_ * size_);

	for (std::size_t i = 0; i < size_; ++i)
	    for (std::size_t j = 0; j < size_; ++j)
		data[i * size_ + j] = data_[i * size_ + size_ - 1 - j];

	data_ = data;
    }

    void World::transpose_()
    {
	std::vector<int> data(size_ * size_);

	for (std::size_t i = 0; i < size_; ++i)
	    for (std::size_t j = 0; j < size_; ++j)
		data[i * size_ + j] = data_[j * size_ + i];

	data_ = data;
    }

}
