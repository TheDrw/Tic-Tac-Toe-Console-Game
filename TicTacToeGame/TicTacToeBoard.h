#pragma once
#include <vector>

class TicTacToeBoard
{
public:
	TicTacToeBoard();
	~TicTacToeBoard();

	void build_board();
	void reset_board();

	const bool is_move_valid(const int set_position) const;
	void insert_move_at_position(const int set_position, const int set_player);
	void undo_move_at_position(const int set_position);

	void instructional_overlay();
	void print_board();

	const bool is_board_filled() const;

	size_t get_squared_board_size() const;
	const std::vector<int> get_board_sums() const;
private:
	char get_current_occupant(const int occupant) const;

private:
	std::vector<std::vector<int>> board;
	size_t board_size;
};