#include "pch.h"
#include "Game.h"
#include "TicTacToeBoard.h"
#include <iostream>
#include <chrono>
#include <thread>
#include <algorithm>

Game::Game() : human_score(0), cpu_score(0), tie_score(0), number_of_turns(0), is_game_active(true)
{
	current_turn = HUMAN_ID;
	game_board = std::make_unique<TicTacToeBoard>();
}

Game::~Game()
{}

void Game::play()
{
	int input = 1;
	const int RETRY_INPUT = 1;

	intro();
	do
	{
		game_loop();
		std::cout << "\n\n-----------Scores-----------\n";
		std::cout << "Human : " << human_score << " CPU: " << cpu_score << " ties: " << tie_score;
		std::cout << "\nPlay again? (1 - yes, 0 - no) : ";
		std::cin >> input;

		if (input == RETRY_INPUT)
		{
			restart();
		}

	} while (input == RETRY_INPUT);

	quit();
}

void Game::game_loop()
{
	do
	{
		ingame_front_reload_screen();

		if (is_game_active)
		{
			if (number_of_turns == game_board->get_squared_board_size())
			{
				declare_tie();
			}
			else if (current_turn == HUMAN_ID)
			{
				human_turn();
			}
			else if (current_turn == CPU_ID)
			{
				cpu_turn();
			}
		}

		wait_for_seconds(1);
	} while (is_game_active);
}

void Game::restart()
{
	current_turn = HUMAN_ID;
	number_of_turns = 0;
	is_game_active = true;
	game_board->reset_board();
}

void Game::intro()
{
	std::cout << "Welcome to Tic Tac T0e!\n\n";
	wait_for_seconds(1);

	instructions();
	wait_for_seconds(2);

	std::cout << "You'll be fighting against the mighty lvl 200 CPU.\n";
	wait_for_seconds(2);
	std::cout << "It be very smarts, so you must choose wisely!!\n\n";
	wait_for_seconds(2);

	std::cout << "Game will start in 3 seconds...\n";
	wait_for_seconds(2);

	
	show_catscii();
	wait_for_seconds(1);
}

void Game::show_catscii()
{
	std::cout << ".          .\n";
	std::cout << "| \\_-- - _/|\n";
	std::cout << "/   o_o    \\\n";
	std::cout << "|    U     |\n";
	std::cout << "\\  ._I_.   /\n";
	std::cout << " `-_____ -'\n";
}

void Game::instructions()
{
	std::cout << "Each position on the board corresponds to the following numbers.\n\n";
	game_board->instructional_overlay();
	wait_for_seconds(2);
}

void Game::ingame_front_reload_screen()
{
	clear_screen();
	ingame_header();
	game_board->print_board();
	ingame_prompt();
}

void Game::ingame_prompt()
{
	std::cout << "-1 : exit \n0 : instructions\n";
}

void Game::ingame_header()
{
	std::cout << "Tic Tac Toe Game!\n\n";
}

void Game::clear_screen()
{
	system("cls");
}

/* 
	comment implementation out if the wait seems a bit annoying.
	i think this might cause problems in different formats.
*/
void Game::wait_for_seconds(const long long seconds)
{
	std::this_thread::sleep_for(std::chrono::seconds(seconds));
}

void Game::human_turn()
{
	int input;
	const int INSTRUCTIONS_INPUT = 0, EXIT_INPUT = -1;

	std::cout << "human picks: ";
	std::cin >> input;

	if (input > 0)
	{
		if (game_board->is_move_valid(input))
		{
			game_board->insert_move_at_position(input, HUMAN_ID);
			ingame_front_reload_screen();

			std::cout << "human picks: " << input << "!\n";

			number_of_turns++;
			current_turn = CPU_ID;

			int winner = get_winner();
			if (winner == HUMAN_ID)
			{
				declare_human_winner();
			}
		}
		else
		{
			std::cout << "Move was invalid!\n";
		}
	}
	else if (input == INSTRUCTIONS_INPUT)
	{
		clear_screen();
		instructions();
	}
	else if (input == EXIT_INPUT)
	{
		std::cout << "leaving current game.";
		is_game_active = false;
	}
	else
	{
		std::cout << "input was invalid!\n";
	}
}

//TODO - I am pretty sure I should be making an AI class or something.
void Game::cpu_turn()
{
	std::cout << "cpu picks: ";
	wait_for_seconds(1);

	int best_score = -INT_MAX;
	int best_position_to_move = 1;
	const int TOTAL_POSITIONS = game_board->get_squared_board_size();
	for (int cpu_position = 1; cpu_position <= TOTAL_POSITIONS; cpu_position++)
	{
		if (game_board->is_move_valid(cpu_position))
		{
			game_board->insert_move_at_position(cpu_position, CPU_ID);
			int score = minimax(false, 0);

			//std::cout << score << ' ' << cpu_position << '\n';
			//wait_for_seconds(1);

			game_board->undo_move_at_position(cpu_position);

			if (best_score < score)
			{
				best_score = score;
				best_position_to_move = cpu_position;
			}
		}
	}

	game_board->insert_move_at_position(best_position_to_move, CPU_ID);
	ingame_front_reload_screen();

	std::cout << "cpu picks: ";
	std::cout << best_position_to_move << "!\n";

	number_of_turns++;
	current_turn = HUMAN_ID;

	int winner = get_winner();
	if (winner == CPU_ID)
	{
		declare_cpu_winner();
	}
}

const int Game::get_minimax_score_result(const int result, const int depth) const
{
	int score_result = TIE_ID;
	if (result == HUMAN_ID)
	{
		score_result = depth - 10;
	}
	else if (result == CPU_ID)
	{
		score_result = 10 - depth;
	}

	return score_result;
}

// this is more of a minimax in favor of the AI
// TODO - alpha beta pruning
int Game::minimax(bool is_maximizing, int depth)
{
	int result = get_winner();
	if (result != NONE_ID)
	{
		return get_minimax_score_result(result, depth);
	}

	int best_score = is_maximizing ? -INT_MAX : INT_MAX;
	const int TOTAL_POSITIONS = game_board->get_squared_board_size();
	for (int curr_pos = 1; curr_pos <= TOTAL_POSITIONS; curr_pos++)
	{
		if (game_board->is_move_valid(curr_pos))
		{
			const int PLAYER_ID = is_maximizing ? CPU_ID : HUMAN_ID;
			game_board->insert_move_at_position(curr_pos, PLAYER_ID);

			int score = minimax(!is_maximizing, depth + 1);
			game_board->undo_move_at_position(curr_pos);
			best_score = is_maximizing ?
				std::max(score, best_score) :
				std::min(score, best_score);
		}
	}
	return best_score;
}

void Game::quit()
{
	std::cout << "\n\nGoodbye!!!\n\n";
	wait_for_seconds(2);

	show_catscii();
}

const int Game::get_winner() const
{
	const int HUMAN_WIN = 3, CPU_WIN = -3;
	auto sums = game_board->get_board_sums();
	for (const auto & sum : sums)
	{
		if (sum == HUMAN_WIN)
		{
			return HUMAN_ID;
		}
		else if (sum == CPU_WIN)
		{
			return CPU_ID;
		}
	}

	if (game_board->is_board_filled())
	{
		return TIE_ID;
	}

	return NONE_ID;
}

/* we all know this is futile */
void Game::declare_human_winner()
{
	std::cout << "\n----X wins!----\n";
	human_score++;
	is_game_active = false;
}

void Game::declare_cpu_winner()
{
	std::cout << "\n----O wins!----\n";
	cpu_score++;
	is_game_active = false;
}

void Game::declare_tie()
{
	std::cout << "\n----TIE!!!!----\n";
	tie_score++;
	is_game_active = false;
}