#pragma once
#include <memory>
class TicTacToeBoard;

class Game
{
public:
	Game();
	~Game();

	void play();

private:
	void game_loop();

	void restart();

	// word prompt things
	void intro();
	void show_catscii();
	void instructions();
	void ingame_front_reload_screen();
	void ingame_prompt();
	void ingame_header();
	void clear_screen();

	void wait_for_seconds(const long long seconds);
	
	void human_turn();
	void cpu_turn();

	const int get_minimax_score_result(const int result, const int depth) const;
	int minimax(bool is_maximizing, int depth);

	void quit();

	const int get_winner() const;
	void declare_human_winner();
	void declare_cpu_winner();
	void declare_tie();

private:
	std::unique_ptr<TicTacToeBoard> game_board;
	const int HUMAN_ID = 1, CPU_ID = -1, TIE_ID = 0, NONE_ID = 9001;
	int current_turn;
	bool is_game_active;
	int number_of_turns;
	int human_score, cpu_score, tie_score;
};

