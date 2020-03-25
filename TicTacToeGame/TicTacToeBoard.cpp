#include "pch.h"
#include "TicTacToeBoard.h"
#include <iostream>
#include <numeric>

TicTacToeBoard::TicTacToeBoard()
{
	// by default 3, is the magic number
	board_size = 3;
	build_board();
}

TicTacToeBoard::~TicTacToeBoard()
{
}

void TicTacToeBoard::build_board()
{
	board = std::vector<std::vector<int>>(board_size, std::vector<int>(board_size, 0));
}

void TicTacToeBoard::reset_board()
{
	build_board();
}

const bool TicTacToeBoard::is_move_valid(const int set_position) const
{
	int position_counter = 1;
	for (size_t row = 0; row < board_size; row++)
	{
		for (size_t col = 0; col < board_size; col++)
		{
			if (position_counter == set_position)
			{
				return board[row][col] == 0 ? true : false;
			}
			else if (position_counter > set_position)
			{
				return false;
			}
			position_counter++;
		}
	}
	return false;
}

/*
	inserts player into the board's position. does not check if move is valid.
	call if move is valid before inserting move
*/
void TicTacToeBoard::insert_move_at_position(const int set_position, const int set_player)
{
	int position_counter = 1;
	for (size_t row = 0; row < board_size; row++)
	{
		for (size_t col = 0; col < board_size; col++)
		{
			if (position_counter == set_position)
			{
				board[row][col] = set_player;
			}
			else if (position_counter > set_position)
			{
				return;
			}
			position_counter++;
		}
	}
}

void TicTacToeBoard::undo_move_at_position(const int set_position)
{
	int position_counter = 1;
	for (size_t row = 0; row < board_size; ++row)
	{
		for (size_t col = 0; col < board_size; ++col)
		{
			if (position_counter == set_position)
			{
				board[row][col] = 0;
				return;
			}
			else if (position_counter > set_position)
			{
				return;
			}
			position_counter++;
		}
	}
}

void TicTacToeBoard::instructional_overlay()
{
	int position = 1;
	for (size_t row = 0; row < board_size; ++row)
	{
		for (size_t col = 0; col < board_size; ++col)
		{
			std::cout << position;
			if (col + 1 != board_size)
			{
				std::cout << " | ";
			}

			position++;
		}
		std::cout << "\n";
	}
	std::cout << "\n";
}

void TicTacToeBoard::print_board()
{
	for (auto row = board.cbegin(); row != board.cend(); ++row)
	{
		for (auto col = row->cbegin(); col != row->cend(); ++col)
		{
			auto occupant = get_current_occupant(*col);
			std::cout << occupant;

			if (col + 1 != row->end())
			{
				std::cout << " | ";
			}
		}
		std::cout << '\n';
	}
	std::cout << '\n';
}

const bool TicTacToeBoard::is_board_filled() const
{
	for (auto row = board.cbegin(); row != board.cend(); ++row)
	{
		for (auto col = row->cbegin(); col != row->cend(); ++col)
		{
			if (*col == 0)
			{
				return false;
			}
		}
	}
	return true;
}

size_t TicTacToeBoard::get_squared_board_size() const
{
	return board_size * board_size;
}

// TODO - not sure about this approach.
const std::vector<int> TicTacToeBoard::get_board_sums() const
{
	std::vector<int> board_sums;

	// sum for each row
	for (auto row = board.cbegin(); row != board.cend(); ++row)
	{
		board_sums.emplace_back(std::accumulate(row->cbegin(), row->cend(), 0));
	}

	// sum for each column
	auto col_sums = std::vector<int>(board_size, 0);
	for (size_t col = 0; col < board_size; ++col)
	{
		for (size_t row = 0; row < board_size; ++row)
		{
			col_sums[row] += board[col][row];
		}
	}

	// put all the column sums into board sums
	for (const auto &sum : col_sums)
	{
		board_sums.emplace_back(sum);
	}

	// top left to bottom right diagonal
	int top_left_diagonal_sum = 0;
	for (size_t i = 0, j = 0; i < board_size; i++, j++)
	{
		top_left_diagonal_sum += board[i][j];
	}
	board_sums.emplace_back(top_left_diagonal_sum);

	// bottom left to top right diagonal sum
	int bottom_left_diagonal_sum = 0;
	for (size_t i = board_size - 1, j = 0; j < board_size; i--, j++)
	{
		bottom_left_diagonal_sum += board[i][j];
	}
	board_sums.emplace_back(bottom_left_diagonal_sum);

	return board_sums;
}

/* used mainly for when printing the board*/
char TicTacToeBoard::get_current_occupant(const int occupant) const
{
	char current_occupant = [&]() {
		switch (occupant)
		{
		case -1:	return 'O'; // cpu
		case 0:		return ' '; // no one
		case 1:		return 'X'; // human
		default:	return 'F'; // to pay respects for error - shouldn't hit here
		}
	}();

	return current_occupant;
}