/* Program to print and play checker games.

 Skeleton program written by Artem Polyvyanyy, artem.polyvyanyy@unimelb.edu.au,
 September 2021, with the intention that it be modified by students
 to add functionality, as required by the assignment specification.
 Student Authorship Declaration:

 (1) I certify that except for the code provided in the initial skeleton file,
 the program contained in this submission is completely my own individual
 work, except where explicitly noted by further comments that provide details
 otherwise. I understand that work that has been developed by another student,
 or by me in collaboration with other students, or by non-students as a result
 of request, solicitation, or payment, may not be submitted for assessment in
 this subject. I understand that submitting for assessment work developed by
 or in collaboration with other students or non-students constitutes Academic
 Misconduct, and may be penalized by mark deductions, or by other penalties
 determined via the University of Melbourne Academic Honesty Policy, as
 described at https://academicintegrity.unimelb.edu.au.

 (2) I also certify that I have not provided a copy of this work in either
 softcopy or hardcopy or any other form to any other student, and nor will I
 do so until after the marks are released. I understand that providing my work
 to other students, regardless of my intention or any undertakings made to me
 by that other student, is also Academic Misconduct.

 (3) I further understand that providing a copy of the assignment specification
 to any form of code authoring or assignment tutoring service, or drawing the
 attention of others to such services and code that may have been made
 available via such a service, may be regarded as Student General Misconduct
 (interfering with the teaching activities of the University and/or inciting
 others to commit Academic Misconduct). I understand that an allegation of
 Student General Misconduct may arise regardless of whether or not I personally
 make use of such solutions or sought benefit from such actions.
 Signed by: Daniel Nappa 1170717
 Dated: 6/09/2021
*/

#include <ctype.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <limits.h>
#include <math.h>

/* some #define's from my sample solution ------------------------------------*/
#define BOARD_SIZE 8       // board size
#define ROWS_WITH_PIECES 3 // number of initial rows with pieces
#define CELL_EMPTY '.'     // empty cell character
#define CELL_BPIECE 'b'    // black piece character
#define CELL_WPIECE 'w'    // white piece character
#define CELL_BTOWER 'B'    // black tower character
#define CELL_WTOWER 'W'    // white tower character
#define COST_PIECE 1       // one piece cost
#define COST_TOWER 3       // one tower cost
#define TREE_DEPTH 3       // minimax tree depth
#define COMP_ACTIONS 10    // number of computed actions

/* #define's from student ----------------------------------------------------*/
#define max(a,b) (((a) > (b)) ? (a) : (b))
#define min(a,b) (((a) < (b)) ? (a) : (b))

#define SEPARATOR "+---+---+---+---+---+---+---+---+"
#define ACTION "====================================="
#define WHITESPACE " "
#define PIPE "|"
#define BLACK_ACTION "BLACK ACTION"
#define WHITE_ACTION "WHITE ACTION"
#define BOARD_COST "BOARD COST"
#define BLACK_WIN "BLACK WIN!"
#define WHITE_WIN "WHITE WIN!"
#define STAGE_1 "***"
#define ERROR_1 "ERROR: Source cell is outside of the board."
#define SRC_OUTSIDE 1
#define ERROR_2 "ERROR: Target cell is outside of the board."
#define TAR_OUTSIDE 2
#define ERROR_3 "ERROR: Source cell is empty."
#define SRC_EMPTY 3
#define ERROR_4 "ERROR: Target cell is not empty."
#define TAR_NOT_EMPTY 4
#define ERROR_5 "ERROR: Source cell holds opponent's piece/tower."
#define SRC_OPP 5
#define ERROR_6 "ERROR: Illegal action."
#define CAPTURE 2  // cells moved for capture
#define SINGLE 1   // cells moved for single
#define ILLEGAL 6  // illegal error code
#define LEGAL 0    // move legality
#define LENGTH 5   // character limit to describe a move
#define ASCII_A 65 // ASCII representation of A
#define ASCII_I 73 // ASCII representation of I
#define BOUND 0    // first row number
#define BLACK 0    // player number for black
#define WHITE 1    // player number for white
#define SIZE 2
#define TRUE 1
#define FALSE 0
#define MOVE 6 // string length for reading from stdin
/* one type definition from my sample solution -------------------------------*/
typedef unsigned char board_t[BOARD_SIZE][BOARD_SIZE]; // board type
typedef struct node node_t;

/* struct to represent position on board */
typedef struct
{
    int row;
    char cell;
} position_t;

/* struct to represent a whole move */
typedef struct
{
    position_t src;
    position_t tar;
} move_t;
typedef struct
{
    size_t _size;
    int n;
    move_t *array;
} movestruct_t;

/* struct to represent index position on board */
typedef struct
{
    int row;
    int cell;
} index_t;

/* node structure for minimax algorithm */
struct node
{
    int score;
    board_t board;
    size_t _size;
    int n;
    movestruct_t *moves;
    node_t **nodes;
};
void fill_board(board_t board);
int place_pieces(board_t board, int i, int i_guard, int j, int j_guard,
                 char piece);
void print_b(board_t board);
int board_cost(board_t board);
int count_pieces(board_t board, char piece);
void change_turn(int *turn);
void scan_moves(board_t board);
void print_win(board_t board);
void stage_0(board_t board, move_t *move, int turn);
int move_illegal(board_t board, move_t *move, int turn);
index_t pos2idx(position_t *pos);
position_t idx2pos(index_t *idx);
void print_m(board_t board, move_t *move, int turn, int turn_n, int stage_1);
void swap(board_t board, move_t *move);
movestruct_t *generate_moves(board_t board, int turn);
void add_move(movestruct_t *moves, move_t *next);
int stage_1(board_t board, int turn, int depth, node_t *par);
node_t *make_node(board_t board);
int game_ended(board_t board);
int found_move(board_t board, char piece, char tower, int turn);
void free_nodes(node_t *par);
void free_movestruct(movestruct_t *moves);
move_t next_action(node_t *par);
int main(int argc, char *argv[])
{    
    board_t board;
    fill_board(board);
    print_b(board);
    scan_moves(board);
    return EXIT_SUCCESS; 
}

/* board must be populated before game starts */
void fill_board(board_t board)
{
    printf("BOARD SIZE: %dx%d\n", BOARD_SIZE, BOARD_SIZE);
    int n_bpiece = 0;
    int n_wpiece = 0;
    /* fill board with empty cells */
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            board[i][j] = CELL_EMPTY;
        }
    }

    /* place black pieces on board */
    n_bpiece = place_pieces(board, BOARD_SIZE - ROWS_WITH_PIECES,
                            BOARD_SIZE, 1, BOARD_SIZE, CELL_BPIECE);
    printf("#BLACK PIECES: %d\n", n_bpiece);
    
    /* place white pieces on board */
    n_wpiece = place_pieces(board, 0, ROWS_WITH_PIECES, 1, BOARD_SIZE,
                            CELL_WPIECE);
    printf("#WHITE PIECES: %d\n", n_wpiece);
}

/* helper function for fill_board that places pieces on the board */
int place_pieces(board_t board, int i, int i_guard, int j, int j_guard,
                 char piece)
{
    int num_piece = 0;
    for (int row = i; row < i_guard; row++)
    {
        if (!((row + 1) % SIZE))
        {
            for (int cell = 0; cell < j_guard; cell += SIZE)
            {
                board[row][cell] = piece;
                num_piece++;
            }
        }
        else
        {
            for (int cell = j; cell < j_guard; cell += SIZE)
            {
                board[row][cell] = piece;
                num_piece++;
            }
        }
    }
    return num_piece;
}

/* print the board */
void print_b(board_t board)
{

    /* output column letters */
    for (int i = ASCII_A; i < ASCII_I; i++)
    {
        if (i == ASCII_A)
        {
            printf("%6c", (char)i);
        }
        else
        {
            printf("%4c", (char)i);
        }
    }
    printf("\n");

    /* output board grid */
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        printf("%3s%s\n", WHITESPACE, SEPARATOR);
        printf("%2d%2s", row + 1, PIPE);
        for (int cell = 0; cell < BOARD_SIZE; cell++)
        {
            printf("%2c%2s", (char)board[row][cell], PIPE);
        }
        printf("\n");
    }
    printf("%3s%s\n", WHITESPACE, SEPARATOR);
}

/* calculates board cost */
int board_cost(board_t board)
{
    return (count_pieces(board, CELL_BPIECE) +
            3 * count_pieces(board, CELL_BTOWER) -
            count_pieces(board, CELL_WPIECE) -
            3 * count_pieces(board, CELL_WTOWER));
}

/* helper function for board_cost to count pieces of a given type */
int count_pieces(board_t board, char piece)
{
    int count = 0;
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int cell = 0; cell < BOARD_SIZE; cell++)
        {
            if (board[row][cell] == piece)
            {
                count++;
            }
        }
    }
    return count;
}

/* coverts an index_t to a position_t */
position_t idx2pos(index_t *idx)
{
    position_t new;
    new.cell = (char)(idx->cell + ASCII_A);
    new.row = idx->row + 1;
    return new;
}

/* helper function used in stage1 to convert board indexes to a move */
move_t idxs2move(int src_row, int src_cell, int tar_row, int tar_cell)
{
    move_t new;
    index_t src_i;
    index_t tar_i;
    position_t src_p;
    position_t tar_p;
    src_i.row = src_row;
    src_i.cell = src_cell;
    tar_i.row = tar_row;
    tar_i.cell = tar_cell;
    src_p = idx2pos(&src_i);
    tar_p = idx2pos(&tar_i);
    new.src = src_p;
    new.tar = tar_p;
    return new;
}

/* helper function for move_illegal to convert positions into indices */
index_t pos2idx(position_t *pos)
{
    index_t new;
    new.cell = (int)pos->cell - ASCII_A;
    new.row = pos->row - 1;
    return new;
}

/* checks whether the given move is legal */
int move_illegal(board_t board, move_t *move, int turn)
{
    index_t src = pos2idx(&move->src);
    index_t tar = pos2idx(&move->tar);

    /* source cell is outside of board */
    if (!(BOUND <= src.cell && src.cell < BOARD_SIZE) || !(BOUND <= src.row && src.row < BOARD_SIZE))
    {
        return SRC_OUTSIDE;
    }

    /* target cell is outside of board */
    if (!(BOUND <= tar.cell && tar.cell < BOARD_SIZE) || !(BOUND <= tar.row && tar.row < BOARD_SIZE))
    {
        return TAR_OUTSIDE;
    }

    /* source cell is empty */
    if (board[src.row][src.cell] == CELL_EMPTY)
    {
        return SRC_EMPTY;
    }

    /* source cell holds opponents peice/tower */
    if ((turn == BLACK && ((board[src.row][src.cell] == CELL_WPIECE) || (board[src.row][src.cell] == CELL_WTOWER))) || (turn == WHITE && ((board[src.row][src.cell] == CELL_BPIECE) || (board[src.row][src.cell] == CELL_BTOWER))))
    {
        return SRC_OPP;
    }

    /* target cell is not empty */
    if (board[tar.row][tar.cell] != CELL_EMPTY)
    {
        return TAR_NOT_EMPTY;
    }

    /* illegal action */
    if (turn == BLACK)
    {
        if (board[src.row][src.cell] == CELL_BPIECE)
        {
            /* illegal capture */
            if (tar.row - src.row == -CAPTURE && abs(tar.cell - src.cell) == CAPTURE)
            {
                int mid_row = (int)(tar.row + src.row) / 2;
                int mid_cell = (int)(tar.cell + src.cell) / 2;
                if (board[mid_row][mid_cell] != CELL_WPIECE && board[mid_row][mid_cell] != CELL_WTOWER)
                {
                    return ILLEGAL;
                }
            }
            /* illegal move*/
            else if ((tar.row - src.row != -SINGLE) || abs(tar.cell - src.cell) != SINGLE)
            {
                return ILLEGAL;
            }
        }
        else if (board[src.row][src.cell] == CELL_BTOWER)
        {
            /* illegal capture */
            if (abs(tar.row - src.row) == CAPTURE && abs(tar.cell - src.cell) == CAPTURE)
            {
                int mid_row = (int)(tar.row + src.row) / 2;
                int mid_cell = (int)(tar.cell + src.cell) / 2;
                if (board[mid_row][mid_cell] != CELL_WPIECE && board[mid_row][mid_cell] != CELL_WTOWER)
                {
                    return ILLEGAL;
                }
            }
            /* illegal move */
            else if (abs(tar.row - src.row) != SINGLE || abs(tar.cell - src.cell) != SINGLE)
            {
                return ILLEGAL;
            }
        }
    }
    else if (turn == WHITE)
    {
        if (board[src.row][src.cell] == CELL_WPIECE)
        {
            /* illegal capture */
            if (tar.row - src.row == CAPTURE && abs(tar.cell - src.cell) == CAPTURE)
            {
                int mid_row = (int)(tar.row + src.row) / 2;
                int mid_cell = (int)(tar.cell + src.cell) / 2;
                if (board[mid_row][mid_cell] != CELL_BPIECE && board[mid_row][mid_cell] != CELL_BTOWER)
                {
                    return ILLEGAL;
                }
            }
            /* illegal move*/
            else if ((tar.row - src.row != SINGLE) || abs(tar.cell - src.cell) != SINGLE)
            {
                return ILLEGAL;
            }
        }
        else if (board[src.row][src.cell] == CELL_WTOWER)
        {
            /* illegal capture */
            if (abs(tar.row - src.row) == CAPTURE && abs(tar.cell - src.cell) == CAPTURE)
            {
                int mid_row = (int)(tar.row + src.row) / 2;
                int mid_cell = (int)(tar.cell + src.cell) / 2;
                if (board[mid_row][mid_cell] != CELL_BPIECE && board[mid_row][mid_cell] != CELL_BTOWER)
                {
                    return ILLEGAL;
                }
            }
            /* illegal move */
            else if (abs(tar.row - src.row) != SINGLE || abs(tar.cell - src.cell) != SINGLE)
            {
                return ILLEGAL;
            }
        }
    }
    return LEGAL;
}

/* helper function to game_ended to determine if a possible move is found */
int found_move(board_t board, char piece, char tower, int turn)
{
    move_t move;
    for (int i = 0; i < BOARD_SIZE; i++)
    {
        for (int j = 0; j < BOARD_SIZE; j++)
        {
            if (board[i][j] == piece || board[i][j] == tower)
            {
                for (int k = 0; k < BOARD_SIZE; k++)
                {
                    for (int l = 0; l < BOARD_SIZE; l++)
                    {
                        move = idxs2move(i, j, k, l);
                        if (!move_illegal(board, &move, turn))
                        {
                            return 1;
                        }
                    }
                }
            }
        }
    }
    return 0;
}

/* determine if either team has won the game, returns 0 if still going */
int game_ended(board_t board)
{
    /* if either team has no pieces or cannot make any moves */
    if ((!count_pieces(board, CELL_BPIECE) && !count_pieces(board, CELL_BTOWER)) || (!found_move(board, CELL_BPIECE, CELL_BTOWER, BLACK)))
    {
        return INT_MIN;
    }
    if ((!count_pieces(board, CELL_WPIECE) && !count_pieces(board, CELL_WTOWER)) || (!found_move(board, CELL_WPIECE, CELL_WTOWER, WHITE)))
    {
        return INT_MAX;
    }
    return 0;
}

void change_turn(int *turn)
{
    if (*turn == BLACK)
    {
        *turn = WHITE;
    }
    else if (*turn == WHITE)
    {
        *turn = BLACK;
    }
}

/* helper function to scan_moves to print the winner */
void print_win(board_t board)
{
    if (game_ended(board) == INT_MIN)
    {
        printf("%s\n", WHITE_WIN);
        exit(EXIT_FAILURE);
    }
    if (game_ended(board) == INT_MAX)
    {
        printf("%s\n", BLACK_WIN);
        exit(EXIT_FAILURE);
    }
}

/* function to scan moves for stage 0 */
void scan_moves(board_t board)
{
    int turn = BLACK;    /* black starts the game */
    size_t _size = SIZE; /* initial size for moves array */
    move_t move;         /* individual move */
    move_t *moves;
    moves = malloc(_size * sizeof(move_t));
    int n = 0;         /* number of moves read into moves array */
    char line[MOVE];   /* line of input text */
    char next = FALSE; /* next action afer reading all moves */

    /* read all moves from stdin into an array of moves */
    while (scanf("%s\n", line) != EOF)
    {
        if (strlen(line) == 5 && isalpha(*line) && isdigit(line[1]) && line[2] == '-' && isalpha(line[3]) && isdigit(line[4]))
        {
            move.src.cell = *line;
            move.src.row = atoi(&line[1]);
            move.tar.cell = line[3];
            move.tar.row = atoi(&line[4]);
        }
        if (strlen(line) == 1 && (*line == 'A' || *line == 'P'))
        {
            next = *line;
            break;
        }
        if (n == _size)
        {
            _size *= SIZE;
            moves = realloc(moves, _size * sizeof(*moves));
        }
        stage_0(board, &move, turn);
        *(moves + n) = move;
        n++;
        print_m(board, &move, turn, n, FALSE);
        print_win(board);
        change_turn(&turn);
    }

    /* if end of file contains next action after stage 0 */
    if (next == 'A')
    {
        node_t *par = make_node(board);
        stage_1(board, turn, TREE_DEPTH, par);
        move = next_action(par);
        print_m(board, &move, turn, ++n, TRUE);
        print_win(board);
        free_nodes(par);
    }
    else if (next == 'P')
    {
        for (int i = 0; i < COMP_ACTIONS; i++)
        {
            node_t *par = make_node(board);
            stage_1(board, turn, TREE_DEPTH, par);
            move = next_action(par);
            print_m(board, &move, turn, ++n, TRUE);
            print_win(board);
            change_turn(&turn);
            free_nodes(par);
        }
    }
    free(moves);
    moves = NULL;
}

/* parent function to move_illegal primarily for error messages */
void stage_0(board_t board, move_t *move, int turn)
{
    if (move_illegal(board, move, turn) == SRC_OUTSIDE)
    {
        printf("%s\n", ERROR_1);
        exit(EXIT_FAILURE);
    }
    if (move_illegal(board, move, turn) == TAR_OUTSIDE)
    {
        printf("%s\n", ERROR_2);
        exit(EXIT_FAILURE);
    }
    if (move_illegal(board, move, turn) == SRC_EMPTY)
    {
        printf("%s\n", ERROR_3);
        exit(EXIT_FAILURE);
    }
    if (move_illegal(board, move, turn) == TAR_NOT_EMPTY)
    {
        printf("%s\n", ERROR_4);
        exit(EXIT_FAILURE);
    }
    if (move_illegal(board, move, turn) == SRC_OPP)
    {
        printf("%s\n", ERROR_5);
        exit(EXIT_FAILURE);
    }
    if (move_illegal(board, move, turn) == ILLEGAL)
    {
        printf("%s\n", ERROR_6);
        exit(EXIT_FAILURE);
    }
}

/* displays the next move */
void print_m(board_t board, move_t *move, int turn, int turn_n, int stage_1)
{
    printf("%s\n", ACTION);
    if (stage_1)
    {
        printf("%s ", STAGE_1);
    }
    if (turn == BLACK)
    {
        printf("%s #%d: %c%d-%c%d\n", BLACK_ACTION, turn_n, move->src.cell,
               move->src.row, move->tar.cell, move->tar.row);
    }
    else
    {
        printf("%s #%d: %c%d-%c%d\n", WHITE_ACTION, turn_n, move->src.cell,
               move->src.row, move->tar.cell, move->tar.row);
    }
    swap(board, move);
    printf("%s: %d\n", BOARD_COST, board_cost(board));
    print_b(board);
}

/* manipulates array to perform a move by swapping and modifying elements */
void swap(board_t board, move_t *move)
{
    index_t src = pos2idx(&move->src);
    index_t tar = pos2idx(&move->tar);
    char tmp;
    /* capture */
    if (abs(tar.row - src.row) == CAPTURE && abs(tar.cell - src.cell) == CAPTURE)
    {
        int mid_row = (int)(tar.row + src.row) / 2;
        int mid_cell = (int)(tar.cell + src.cell) / 2;
        board[mid_row][mid_cell] = CELL_EMPTY;
    }
    /* become tower */
    if (board[src.row][src.cell] == CELL_BPIECE && !tar.row)
    {
        tmp = board[tar.row][tar.cell];
        board[tar.row][tar.cell] = CELL_BTOWER;
        board[src.row][src.cell] = tmp;
    }
    else if (board[src.row][src.cell] == CELL_WPIECE && tar.row == BOARD_SIZE - 1)
    {
        tmp = board[tar.row][tar.cell];
        board[tar.row][tar.cell] = CELL_WTOWER;
        board[src.row][src.cell] = tmp;
    }
    else
    {
        tmp = board[tar.row][tar.cell];
        board[tar.row][tar.cell] = board[src.row][src.cell];
        board[src.row][src.cell] = tmp;
    }
}

/* we're going to generate all possible legal moves for a given turn */
movestruct_t *generate_moves(board_t board, int turn)
{
    char piece, tower;
    movestruct_t *moves = malloc(sizeof(*moves));
    moves->_size = SIZE;
    moves->n = 0;
    moves->array = malloc(moves->_size * sizeof(move_t));
    move_t next; /* next possible move */
    if (turn == BLACK)
    {
        piece = CELL_BPIECE;
        tower = CELL_BTOWER;
    }
    else
    {
        piece = CELL_WPIECE;
        tower = CELL_WTOWER;
    }
    /* for a given turn, we need to determine each possible move for each piece
    and add them into the array */
    for (int row = 0; row < BOARD_SIZE; row++)
    {
        for (int cell = 0; cell < BOARD_SIZE; cell++)
        {
            if (board[row][cell] == piece || board[row][cell] == tower)
            {
                /* check captures */
                next = idxs2move(row, cell, row - CAPTURE, cell + CAPTURE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                next = idxs2move(row, cell, row + CAPTURE, cell + CAPTURE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                next = idxs2move(row, cell, row + CAPTURE, cell - CAPTURE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                next = idxs2move(row, cell, row - CAPTURE, cell - CAPTURE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                /* check moves */
                next = idxs2move(row, cell, row - SINGLE, cell + SINGLE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                next = idxs2move(row, cell, row + SINGLE, cell + SINGLE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                next = idxs2move(row, cell, row + SINGLE, cell - SINGLE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
                next = idxs2move(row, cell, row - SINGLE, cell - SINGLE);
                if (!move_illegal(board, &next, turn))
                {
                    add_move(moves, &next);
                }
            }
        }
    }
    return moves;
}

/* helper function to generate_moves */
void add_move(movestruct_t *moves, move_t *next)
{
    if (moves->_size == moves->n)
    {
        moves->_size *= SIZE;
        moves->array = realloc(moves->array,
                               moves->_size * sizeof(move_t));
    }
    *(moves->array + moves->n) = *next;
    moves->n++;
}

/* contruct minimax tree */
int stage_1(board_t board, int turn, int depth, node_t *par)
{
    /* if depth is 0 or winning state */
    if (!depth || game_ended(board) == INT_MAX || game_ended(board) == INT_MIN)
    {
        par->score = game_ended(board);
        return board_cost(board);
    }
    int value;
    if (turn == BLACK)
    {
        value = INT_MIN;
    }
    else
    {
        value = INT_MAX;
    }
    /* generate_moves all possible moves for the given turn */
    movestruct_t *moves = generate_moves(board, turn);
    par->moves = moves;
    for (int i = 0; i < moves->n; i++)
    {
        /* make a child board for the move */
        board_t *child_board = malloc(sizeof(board_t));
        /* copy parent board into child board */
        memcpy(*child_board, board, sizeof(board_t));
        /* do the move on the child board */
        swap(*child_board, moves->array + i);
        if (par->n == par->_size)
        {
            par->_size *= SIZE;
            par->nodes = realloc(par->nodes, par->_size * sizeof(node_t *));
        }
        node_t *child = make_node(*child_board);
        *(par->nodes + i) = child;
        (par->n)++;
        /* recursive call for the child node */
        if (turn == BLACK)
        {
            value = max(value, stage_1(*child_board, WHITE, depth - 1, child));
        }
        else
        {
            value = min(value, stage_1(*child_board, BLACK, depth - 1, child));
        }
    }
    par->score = value;
    return value;
}

/* allocate memory in the heap for a node and return its pointer */
node_t *make_node(board_t board)
{
    node_t *node;
    node = malloc(sizeof(*node));
    node->_size = SIZE;
    node->n = 0;
    node->nodes = malloc(node->_size * sizeof(node_t *));
    memcpy(node->board, board, sizeof(board_t));
    node->moves = NULL;
    return node;
}

/* helper function to stage 1 to free movestruct from heap */
void free_movestruct(movestruct_t *moves)
{
    free(moves->array);
    moves->array = NULL;
    free(moves);
}

/* free all the node pointers that have been malloc'd */
void free_nodes(node_t *par)
{
    if (par)
    {
        for (int i = 0; i < par->n; i++)
        {
            free_nodes(*(par->nodes + i));
        }
        free(par);
    }
}

/* take best action after generating mimimax tree */
move_t next_action(node_t *par)
{
    /* propagated score we need to match when going through moves */
    int score = par->score;
    move_t next;
    for (int i = 0; i < par->n; i++)
    {
        if (score == (*(par->nodes + i))->score)
        {
            next = *(par->moves->array + i);
            break;
        }
    }
    return next;
}
/* algorithms are fun */
/* THE END -------------------------------------------------------------------*/