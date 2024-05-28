#include <algorithm>
#include <iostream>
#include <stack>

#define INF 1000000
#define STATE(i, j) state[(i) * W + (j)]
#define PRINT(mat, N, M)                            \
    {                                               \
        for (int i = 0; i < N; i++)                 \
        {                                           \
            for (int j = 0; j < M; j++)             \
                std::cout << mat[i * M + j] << " "; \
            std::cout << std::endl;                 \
        }                                           \
    }
#define MOVE(row, col, d, c) std::to_string (col) + " " + std::to_string (row) + " " + c + " " + std::to_string (d)

int H, W, N, EXIT_i, EXIT_j;
std::vector<std::string> START;

std::vector<std::tuple<std::string, std::vector<std::string> > >
children (std::vector<std::string> state)
{
    std::vector<std::string> child;
    std::vector<std::tuple<std::string, std::vector<std::string> > > children;

    for (int row = 0; row < H; row++)
        for (int col = 0; col < W; col++)
            if (state[row * W + col][0] == '.')
            {
                int mcol, mrow;
                int lcol = col, rcol = col;
                int urow = row, drow = row;

                // Go right
                child = state;
                while (rcol < W && state[row * W + rcol][0] == '.')
                    rcol++;
                if (rcol < W && state[row * W + rcol] == START[row * W + rcol])
                {
                    if (state[row * W + rcol][0] == 'a')
                    {
                        mcol = rcol, mrow = row;
                        int scol = col;
                        do
                        {
                            std::swap (child[row * W + rcol], child[row * W + scol]);
                            scol++, rcol++;
                        } while (rcol < W
                                 && (child[row * W + rcol][0] == 'b'
                                     || child[row * W + rcol][0] == 'c'
                                     || child[row * W + rcol][0] == 'd'));
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, mcol - col, "L"), child));
                    }
                    else if (state[row * W + rcol][0] == 'e')
                    {
                        mcol = rcol, mrow = row;
                        int scol = col;
                        do
                        {
                            std::swap (child[row * W + rcol], child[row * W + scol]);
                            scol++, rcol++;
                        } while (rcol < W
                                 && (child[row * W + rcol][0] == 'f'
                                     || child[row * W + rcol][0] == 'g'
                                     || child[row * W + rcol][0] == 'h'));
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, mcol - col + 1, "L"), child));
                    }
                }

                // Go left
                child = state;
                while (lcol >= 0 && state[row * W + lcol][0] == '.')
                    lcol--;
                if (lcol >= 0 && state[row * W + lcol] == START[row * W + lcol])
                {
                    if (state[row * W + lcol][0] == 'b' || state[row * W + lcol][0] == 'c' || state[row * W + lcol][0] == 'd')
                    {
                        mcol = lcol, mrow = row;
                        int scol = col;
                        do
                        {
                            std::swap (child[row * W + lcol], child[row * W + scol]);
                            scol--, lcol--;
                        } while (child[row * W + lcol][0] != 'a');
                        std::swap (child[row * W + lcol], child[row * W + scol]);
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, col - mcol, "R"), child));
                    }
                    else if (state[row * W + lcol][0] == 'f' || state[row * W + lcol][0] == 'g' || state[row * W + lcol][0] == 'h')
                    {
                        mcol = lcol, mrow = row;
                        int scol = col;
                        do
                        {
                            std::swap (child[row * W + lcol], child[row * W + scol]);
                            scol--, lcol--;
                        } while (child[row * W + lcol][0] != 'e');
                        std::swap (child[row * W + lcol], child[row * W + scol]);
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, col - mcol + 1, "R"), child));
                    }
                }

                // Go down
                child = state;
                while (drow < H && state[drow * W + col][0] == '.')
                    drow++;
                if (drow < H && state[drow * W + col] == START[drow * W + col])
                {
                    if (state[drow * W + col][0] == 'x')
                    {
                        mcol = col, mrow = drow;
                        int srow = row;
                        do
                        {
                            std::swap (child[drow * W + col], child[srow * W + col]);
                            drow++, srow++;
                        } while (drow < H
                                 && (child[drow * W + col][0] == 'y'
                                     || child[drow * W + col][0] == 'z'
                                     || child[drow * W + col][0] == 'w'));
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, mrow - row, "U"), child));
                    }
                    else if (state[drow * W + col][0] == 'p')
                    {
                        mcol = col, mrow = drow;
                        int srow = row;
                        do
                        {
                            std::swap (child[drow * W + col], child[srow * W + col]);
                            drow++, srow++;
                        } while (drow < H
                                 && (child[drow * W + col][0] == 'q'
                                     || child[drow * W + col][0] == 'r'
                                     || child[drow * W + col][0] == 's'));
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, mrow - row + 1, "U"), child));
                    }
                }

                // Go up
                child = state;
                while (urow >= 0 && state[urow * W + col][0] == '.')
                    urow--;
                if (urow >= 0 && state[urow * W + col] == START[urow * W + col])
                {
                    if (state[urow * W + col][0] == 'y' || state[urow * W + col][0] == 'z' || state[urow * W + col][0] == 'w')
                    {
                        mcol = col, mrow = urow;
                        int srow = row;
                        do
                        {
                            std::swap (child[urow * W + col], child[srow * W + col]);
                            urow--, srow--;
                        } while (child[urow * W + col][0] != 'x');
                        std::swap (child[urow * W + col], child[srow * W + col]);
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, row - mrow, "D"), child));
                    }
                    else if (state[urow * W + col][0] == 'q' || state[urow * W + col][0] == 'r' || state[urow * W + col][0] == 's')
                    {
                        mcol = col, mrow = urow;
                        int srow = row;
                        do
                        {
                            std::swap (child[urow * W + col], child[srow * W + col]);
                            urow--, srow--;
                        } while (child[urow * W + col][0] != 'p');
                        std::swap (child[urow * W + col], child[srow * W + col]);
                        children.push_back (std::make_tuple (MOVE (mrow, mcol, row - mrow + 1, "D"), child));
                    }
                }
            }

    return children;
}

struct node
{
    int n;
    std::vector<std::string> state;
    std::string move;
    struct node *prev;
};

const int eps = 5;

int
h (std::vector<std::string> state)
{
    int i = EXIT_i, j = EXIT_j, k = 0;
    if (EXIT_i == 0)
    {
        while (STATE (i, j)[0] != 'p')
        {
            if (STATE (i, j)[0] == 'a'
                || STATE (i, j)[0] == 'b'
                || STATE (i, j)[0] == 'c'
                || STATE (i, j)[0] == 'd')
                k++;
            i++;
        }
    }
    else if (EXIT_i == H - 1)
    {
        while (STATE (i, j)[0] != 'p')
        {
            if (STATE (i, j)[0] == 'a'
                || STATE (i, j)[0] == 'b'
                || STATE (i, j)[0] == 'c'
                || STATE (i, j)[0] == 'd')
                k++;
            i--;
        }
    }
    else if (EXIT_j == 0)
    {
        while (STATE (i, j)[0] != 'p')
        {
            if (STATE (i, j)[0] == 'x'
                || STATE (i, j)[0] == 'y'
                || STATE (i, j)[0] == 'z'
                || STATE (i, j)[0] == 'w')
                k++;
            j++;
        }
    }
    else if (EXIT_j == W - 1)
    {
        while (STATE (i, j)[0] != 'p')
        {
            if (STATE (i, j)[0] == 'x'
                || STATE (i, j)[0] == 'y'
                || STATE (i, j)[0] == 'z'
                || STATE (i, j)[0] == 'w')
                k++;
            j--;
        }
    }

    return k;
}

bool
comp (node a, node b)
{
    return (a.n + eps * h (a.state) < b.n + eps * h (b.state));
}

int
main ()
{
    /**
     * ================== PARSE INPUT ==================
     */

    std::cin >> W >> H >> N;

    char c;
    std::vector<std::string> state (H * W);
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            std::cin >> c, STATE (i, j) = c;

    /**
     * ================== FIND EXIT ==================
     */

    for (int i = 0; i < H; i++)
        for (auto j : { 0, W - 1 })
            if (STATE (i, j)[0] == '.')
                EXIT_i = i, EXIT_j = j;

    for (int j = 0; j < W; j++)
        for (auto i : { 0, H - 1 })
            if (STATE (i, j)[0] == '.')
                EXIT_i = i, EXIT_j = j;

    /**
     * ================== MAKE CARS UNIQUE ==================
     * A horizontal car is reprezented by a proper prefix of "abcd"
     * A vertical car is represented by a proper predfix od "xyzw"
     */

    int id = 0;
    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (STATE (i, j)[0] == 'a')
            {
                do
                    STATE (i, j) += std::to_string (id), j++;
                while (j < W && (STATE (i, j)[0] == 'b' || STATE (i, j)[0] == 'c' || STATE (i, j)[0] == 'd'));
                id++;
            }
    id = 0;
    for (int j = 0; j < W; j++)
        for (int i = 0; i < H; i++)
            if (STATE (i, j)[0] == 'x')
            {
                do
                    STATE (i, j) += std::to_string (id), i++;
                while (i < H && (STATE (i, j)[0] == 'y' || STATE (i, j)[0] == 'z' || STATE (i, j)[0] == 'w'));
                id++;
            }

    /**
     * ================== TRANSFORM DEAN'S CAR ==================
     * Horizontal dean's car is represented by a proper prefix of "efgh"
     * Vertical dean's car is represented by a proper prefix of "pqrs"
     */

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (STATE (i, j)[0] == 'o')
            {
                int k = 0;
                if (i + 1 < H && STATE (i, j)[0] == 'o')
                    do
                        STATE (i, j) = "pqrs"[k], i++, k++;
                    while (i < H && STATE (i, j)[0] == 'o');
                else
                    do
                        STATE (i, j) = "efgh"[k], j++, k++;
                    while (j < W && STATE (i, j)[0] == 'o');
                break;
            }

    // Save starting state
    START = state;

    node pn;
    pn.state = START;
    pn.move = "";
    pn.prev = NULL;
    pn.n = 0;

    std::vector<node> stack;
    stack.push_back (pn);

    while (stack.size () > 0)
    {
        pn = *std::max_element (stack.begin (), stack.end (), comp);
        state = pn.state;

        std::cout << pn.n << std::endl;

        if (STATE (EXIT_i, EXIT_j)[0] == 'p'
            || STATE (EXIT_i, EXIT_j)[0] == 'q'
            || STATE (EXIT_i, EXIT_j)[0] == 'r'
            || STATE (EXIT_i, EXIT_j)[0] == 's'
            || STATE (EXIT_i, EXIT_j)[0] == 'e'
            || STATE (EXIT_i, EXIT_j)[0] == 'f'
            || STATE (EXIT_i, EXIT_j)[0] == 'g'
            || STATE (EXIT_i, EXIT_j)[0] == 'h')
        {
            std::cout << "FOUND" << std::endl;
            while (pn.prev != NULL)
            {
                std::cout << pn.move << std::endl;
                pn = *pn.prev;
            }
        }

        if (pn.n == N)
            continue;

        for (auto [move, next] : children (state))
        {
            node nn = { .n = pn.n + 1, .state = next, .move = move, .prev = &pn };
            stack.push_back (nn);
        }
    }

    return -1;
}