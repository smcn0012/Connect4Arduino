# This the start of a digital connect 4 implementation that never took off, it is here in case I need inspiration for the Arduino implementation

def player_turn(current_board, player_token):
    column = int(input("Enter column: "))-1
    if column not in range(7) or "." not in current_board[column]:
        print("Invalid column!")
        return player_turn(current_board, player_token)
    else:
        current_board, row = board_edit(current_board, column, player_token)
        return current_board, win_check(current_board, column, row, player_token)


def board_edit(current_board, column_number, player_token):
    for row, token in enumerate(current_board[column_number]):
        if token != ".":
            current_board[column_number][row - 1] = player_token
            return current_board, row
    current_board[column_number][5] = player_token
    return current_board, 5

def win_check(current_board, column, row, player_token):
    direction_list = [(-1, 1), (0, 1), (1, 1), (1, 0)]
    possible_win_directions = [True for i in range(4)]
    chain_length = [0 for i in range(4)]
    distance = -3
    while True in possible_win_directions and distance <= 3:
        for index, direction in enumerate(direction_list):
            if possible_win_directions[index]:
                if 0 <= column + distance * direction_list[index][0] <= 6 and 0 <= row + distance * direction_list[index][1] <= 5:
                    if current_board[column + distance * direction_list[index][0]][row + distance * direction_list[index][1]] != player_token:
                        chain_length[index] = 0
                    else:
                        chain_length[index] += 1
                        if chain_length[index] == 4:
                            return True
                        elif chain_length[index] < distance - 3:
                            possible_win_directions[index] = False
                else:
                    chain_length[index] = 0
        distance += 1
    return False

def print_board(current_board):
    for row in range(6):
        row_string = "|"
        for column in range(7):
            row_string += " " + current_board[column][row] + " |"
        print(row_string)

def bot_turn(current_board, player_token):
    pass

board = [["." for i in range(6)] for j in range(7)]
print_board(board)
for turn in range(42):
    board, player_win = player_turn(board, "@")
    print_board(board)
    if player_win:
        print("@ Wins!")
        break

    board, bot_win = player_turn(board, "O")
    print_board(board)
    if bot_win:
        print("O Wins!")
        break


