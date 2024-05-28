#include <algorithm>
#include <iostream>

#include <queue>
#include <unordered_map>

#define state(i, j) state[(i) * W + (j)]
#define child(i, j) child[(i) * W + (j)]
#define paren(i, j) paren[(i) * W + (j)]

int N, W, H, EXIT_row, EXIT_col;

using state_t = std::string;

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
    bool first = true;
    std::vector<std::string> solution;

    while (parent.find (state) != parent.end ())
    {
        state_t paren = parent[state];

        int curr_i, curr_j, prev_i, prev_j;

        for (int i = 0; i < H; i++)
            for (int j = 0; j < W; j++)
                if (state (i, j) != paren (i, j))
                {
                    if (state (i, j) == 'a' || state (i, j) == 'x' || state (i, j) == 'e' || state (i, j) == 'p')
                        curr_i = i, curr_j = j;

                    if (paren (i, j) == 'a' || paren (i, j) == 'x' || paren (i, j) == 'e' || paren (i, j) == 'p')
                        prev_i = i, prev_j = j;
                }

        int len;
        char dir;
        if (prev_i > curr_i)
            dir = 'U', len = prev_i - curr_i;
        else if (prev_i < curr_i)
            dir = 'D', len = curr_i - prev_i;
        else if (prev_j > curr_j)
            dir = 'L', len = prev_j - curr_j;
        else
            dir = 'R', len = curr_j - prev_j;

        if (first)
            len++, first = false;

        solution.push_back (std::to_string (prev_j) + " " + std::to_string (prev_i) + " " + dir + " " + std::to_string (len));

        state = paren;
    }

    std::cout << solution.size () << std::endl;
    std::reverse (solution.begin (), solution.end ());
    for (auto line : solution)
        std::cout << line << std::endl;
    return;
}

state_t
input ()
{
    state_t state;
    std::cin >> W >> H >> N;

    state.resize (H * W);

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
        {
            std::cin >> state (i, j);

            if ((i == 0 || j == 0 || i == H - 1 || j == W - 1) && state (i, j) == '.')
                EXIT_row = i, EXIT_col = j;
        }

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (state (i, j) == 'o')
            {
                int k = 0;
                if (i + 1 < H && state (i + 1, j) == 'o')
                    do
                        state (i, j) = "pqrs"[k], i++, k++;
                    while (i < H && state (i, j) == 'o');
                else
                    do
                        state (i, j) = "efgh"[k], j++, k++;
                    while (j < W && state (i, j) == 'o');

                goto exit;
            }
exit:
    return state;
}

int
h_score (std::string state)
{
    int i = EXIT_row, j = EXIT_col, k = 0;

    if (EXIT_row == 0 || EXIT_row == H - 1)
    {
        int d = EXIT_row == 0 ? 1 : -1;
        for (; state (i, j) != 'p'; i += d)
            if (state (i, j) == 'a' || state (i, j) == 'b' || state (i, j) == 'c' || state (i, j) == 'd')
                k++;
    }
    else
    {
        int d = EXIT_col == 0 ? 1 : -1;
        for (; state (i, j) != 'e'; j += d)
            if (state (i, j) == 'x' || state (i, j) == 'y' || state (i, j) == 'z' || state (i, j) == 'w')
                k++;
    }

    return k;
}

bool
is_goal (state_t state)
{
    int i = EXIT_row, j = EXIT_col;
    return state (i, j) == 'p' || state (i, j) == 'q' || state (i, j) == 'r' || state (i, j) == 's'
           || state (i, j) == 'e' || state (i, j) == 'f' || state (i, j) == 'g' || state (i, j) == 'h';
}

int
main ()
{
    state_t state = input ();

    using pair = std::pair<int, state_t>;
    std::unordered_map<state_t, int> g_score;
    std::unordered_map<state_t, int> f_score;
    std::unordered_map<state_t, state_t> parent;
    std::priority_queue<pair, std::vector<pair>, std::greater<pair> > pq;

    int eps = 50;
    g_score[state] = 0;
    f_score[state] = eps * h_score (state);

    pq.push ({ f_score[state], state });

    while (!pq.empty ())
    {
        int f = pq.top ().first;
        state = pq.top ().second;
        pq.pop ();

        if (is_goal (state))
        {
            reconstruct_path (parent, state);
            return 0;
        }

        if (g_score[state] >= N || f != f_score[state])
            continue;

        for (state_t child : children (state))
            if (g_score.find (child) == g_score.end () || g_score[state] + 1 < g_score[child])
            {
                parent[child] = state;
                g_score[child] = g_score[state] + 1;
                f_score[child] = g_score[child] + eps * h_score (child);
                pq.push ({ f_score[child], child });
            }
    }

    std::cout << "Solution not found!" << std::endl;
    return 0;
}