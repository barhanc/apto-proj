#include <algorithm>
#include <iostream>
#include <limits>
#include <map>
#include <queue>
#include <unordered_map>

typedef std::string state_t;

int N, W, H, EXIT_row, EXIT_col;

#define LEN 1024
#define PSS std::pair<int, state_t>

#define state(row, col) state[(row) * W + (col)]
#define child(row, col) child[(row) * W + (col)]

std::vector<state_t>
children (state_t state)
{
    state_t child;
    std::vector<state_t> children;

    for (int row = 0; row < H; row++)
        for (int col = 0; col < W; col++)
            if (state (row, col) == '.')
            {
                int lcol = col, rcol = col;
                int urow = row, drow = row;

                // Go right
                child = state;
                while (rcol < W && state (row, rcol) == '.')
                    rcol++;
                if (rcol < W)
                {
                    if (state (row, rcol) == 'a')
                    {
                        int scol = col;
                        do
                        {
                            std::swap (child (row, rcol), child (row, scol));
                            scol++, rcol++;
                        } while (rcol < W
                                 && (child (row, rcol) == 'b'
                                     || child (row, rcol) == 'c'
                                     || child (row, rcol) == 'd'));
                        children.push_back (child);
                    }
                    else if (state (row, rcol) == 'e')
                    {
                        int scol = col;
                        do
                        {
                            std::swap (child (row, rcol), child (row, scol));
                            scol++, rcol++;
                        } while (rcol < W
                                 && (child (row, rcol) == 'f'
                                     || child (row, rcol) == 'g'
                                     || child (row, rcol) == 'h'));
                        children.push_back (child);
                    }
                }

                // Go left
                child = state;
                while (lcol >= 0 && state (row, lcol) == '.')
                    lcol--;
                if (lcol >= 0)
                {
                    if (state (row, lcol) == 'b' || state (row, lcol) == 'c' || state (row, lcol) == 'd')
                    {
                        int scol = col;
                        do
                        {
                            std::swap (child (row, lcol), child (row, scol));
                            scol--, lcol--;
                        } while (child (row, lcol) != 'a');
                        std::swap (child (row, lcol), child (row, scol));
                        children.push_back (child);
                    }
                    else if (state (row, lcol) == 'f' || state (row, lcol) == 'g' || state (row, lcol) == 'h')
                    {
                        int scol = col;
                        do
                        {
                            std::swap (child (row, lcol), child (row, scol));
                            scol--, lcol--;
                        } while (child (row, lcol) != 'e');
                        std::swap (child (row, lcol), child (row, scol));
                        children.push_back (child);
                    }
                }

                // Go down
                child = state;
                while (drow < H && state (drow, col) == '.')
                    drow++;
                if (drow < H)
                {
                    if (state (drow, col) == 'x')
                    {
                        int srow = row;
                        do
                        {
                            std::swap (child (drow, col), child (srow, col));
                            drow++, srow++;
                        } while (drow < H
                                 && (child (drow, col) == 'y'
                                     || child (drow, col) == 'z'
                                     || child (drow, col) == 'w'));
                        children.push_back (child);
                    }
                    else if (state (drow, col) == 'p')
                    {
                        int srow = row;
                        do
                        {
                            std::swap (child (drow, col), child (srow, col));
                            drow++, srow++;
                        } while (drow < H
                                 && (child (drow, col) == 'q'
                                     || child (drow, col) == 'r'
                                     || child (drow, col) == 's'));
                        children.push_back (child);
                    }
                }

                // Go up
                child = state;
                while (urow >= 0 && state (urow, col) == '.')
                    urow--;
                if (urow >= 0)
                {
                    if (state (urow, col) == 'y' || state (urow, col) == 'z' || state (urow, col) == 'w')
                    {
                        int srow = row;
                        do
                        {
                            std::swap (child (urow, col), child (srow, col));
                            urow--, srow--;
                        } while (child (urow, col) != 'x');
                        std::swap (child (urow, col), child (srow, col));
                        children.push_back (child);
                    }
                    else if (state (urow, col) == 'q' || state (urow, col) == 'r' || state (urow, col) == 's')
                    {
                        int srow = row;
                        do
                        {
                            std::swap (child (urow, col), child (srow, col));
                            urow--, srow--;
                        } while (child (urow, col) != 'p');
                        std::swap (child (urow, col), child (srow, col));
                        children.push_back (child);
                    }
                }
            }

    return children;
}

void
reconstruct_path (std::unordered_map<state_t, state_t> parent, state_t state)
{
    std::vector<std::string> solution;
    bool first = true;
    while (parent.find (state) != parent.end ())
    {
        auto child = parent[state];
        int curr_row, curr_col, prev_row, prev_col;
        for (int row = 0; row < H; row++)
            for (int col = 0; col < W; col++)
            {
                if (state (row, col) != child (row, col)
                    && (state (row, col) == 'a'
                        || state (row, col) == 'x'
                        || state (row, col) == 'e'
                        || state (row, col) == 'p'))
                    curr_row = row, curr_col = col;
                if (state (row, col) != child (row, col)
                    && (child (row, col) == 'a'
                        || child (row, col) == 'x'
                        || child (row, col) == 'e'
                        || child (row, col) == 'p'))
                    prev_row = row, prev_col = col;
            }
        char dir;
        int len;
        if (prev_row > curr_row)
            dir = 'U', len = prev_row - curr_row;
        if (prev_row < curr_row)
            dir = 'D', len = curr_row - prev_row;
        if (prev_col > curr_col)
            dir = 'L', len = prev_col - curr_col;
        if (prev_col < curr_col)
            dir = 'R', len = curr_col - prev_col;
        if (first)
            len++;
        first = false;
        char line[LEN];
        std::sprintf (line, "%d %d %c %d", prev_col, prev_row, dir, len);
        solution.push_back ((std::string)line);

        state = parent[state];
    }

    std::cout << solution.size () << std::endl;
    std::reverse (solution.begin (), solution.end ());
    for (auto line : solution)
        std::cout << line << std::endl;
}

bool
is_goal (state_t state)
{
    return (state (EXIT_row, EXIT_col) == 'p'
            || state (EXIT_row, EXIT_col) == 'q'
            || state (EXIT_row, EXIT_col) == 'r'
            || state (EXIT_row, EXIT_col) == 's'
            || state (EXIT_row, EXIT_col) == 'e'
            || state (EXIT_row, EXIT_col) == 'f'
            || state (EXIT_row, EXIT_col) == 'g'
            || state (EXIT_row, EXIT_col) == 'h');
}

state_t
parse_input ()
{
    std::cin >> W >> H >> N;
    char board[W * H + 1];

    for (int row = 0; row < H; row++)
        for (int col = 0; col < W; col++)
            std::cin >> board[row * W + col];

    board[H * W] = '\0';

    state_t state = board;

    for (int row = 0; row < H; row++)
        for (int col : { 0, W - 1 })
            if (state (row, col) == '.')
                EXIT_row = row, EXIT_col = col;
    for (int col = 0; col < W; col++)
        for (int row : { 0, H - 1 })
            if (state (row, col) == '.')
                EXIT_row = row, EXIT_col = col;

    bool flag = false;
    for (int row = 0; row < H; row++)
    {
        for (int col = 0; col < W; col++)
        {
            if (state (row, col) == 'o' && state (row + 1, col) == 'o')
            {
                int i = 0;
                while (state (row, col) == 'o')
                {
                    state (row, col) = "pqrs"[i];
                    row++, i++;
                }
                flag = true;
                break;
            }
            if (state (row, col) == 'o' && state (row, col + 1) == 'o')
            {
                int i = 0;
                while (state (row, col) == 'o')
                {
                    state (row, col) = "efgh"[i];
                    col++, i++;
                }
                flag = true;
                break;
            }
        }
        if (flag)
            break;
    }
    return state;
}

int
h_score (std::string state)
{
    int i = EXIT_row, j = EXIT_col, k = 0;

    if (EXIT_row == 0)
    {
        while (state (i, j) != 'p')
        {
            if (state (i, j) == 'a'
                || state (i, j) == 'b'
                || state (i, j) == 'c'
                || state (i, j) == 'd')
                k++;
            i++;
        }
    }
    else if (EXIT_row == H - 1)
    {
        while (state (i, j) != 'p')
        {
            if (state (i, j) == 'a'
                || state (i, j) == 'b'
                || state (i, j) == 'c'
                || state (i, j) == 'd')
                k++;
            i--;
        }
    }
    else if (EXIT_col == 0)
    {
        while (state (i, j) != 'e')
        {
            if (state (i, j) == 'x'
                || state (i, j) == 'y'
                || state (i, j) == 'z'
                || state (i, j) == 'w')
                k++;
            j++;
        }
    }
    else if (EXIT_col == W - 1)
    {
        while (state (i, j) != 'e')
        {
            if (state (i, j) == 'x'
                || state (i, j) == 'y'
                || state (i, j) == 'z'
                || state (i, j) == 'w')
                k++;
            j--;
        }
    }

    return k;
}

int
main ()
{
    state_t state = parse_input ();

    std::unordered_map<state_t, state_t> parent;
    std::unordered_map<state_t, int> g_score, f_score;
    std::priority_queue<PSS, std::vector<PSS>, std::greater<PSS> > open_set;

    int eps = 5;
    g_score[state] = 0, f_score[state] = eps * h_score (state);
    open_set.push ({ f_score[state], state });

    while (!open_set.empty ())
    {
        auto f = open_set.top ().first;
        state = open_set.top ().second;
        open_set.pop ();

        if (is_goal (state))
        {
            reconstruct_path (parent, state);
            return 0;
        }

        if (g_score[state] >= N || f != f_score[state])
            continue;

        for (auto child : children (state))
        {
            int tentative_g_score = g_score[state] + 1;
            if (g_score.find (child) == g_score.end () || tentative_g_score < g_score[child])
            {
                parent[child] = state;
                g_score[child] = tentative_g_score;
                f_score[child] = tentative_g_score + eps * h_score (child);
                open_set.push ({ f_score[child], child });
            }
        }
    }

    std::cout << "ERROR" << std::endl;
    return -1;
}