#include <algorithm>
#include <iostream>

#include <queue>
#include <unordered_map>

#define state(i, j) state[(i) * W + (j)]
#define child(i, j) child[(i) * W + (j)]
#define paren(i, j) paren[(i) * W + (j)]

using state_t = std::string;
int N, W, H, EXIT_i, EXIT_j;

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
                EXIT_i = i, EXIT_j = j;
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

std::vector<state_t>
children (state_t state)
{
    char c;
    state_t child;
    std::vector<state_t> children;

    for (int i = 0; i < H; i++)
        for (int j = 0; j < W; j++)
            if (state (i, j) == '.')
            {
                int ui = i, di = i, lj = j, rj = j;

                // GO RIGHT
                child = state;
                while (rj < W && (c = state (i, rj)) == '.')
                    rj++;

                if (rj < W && (c == 'a' || c == 'e'))
                {
                    int sj = j;
                    do
                        std::swap (child (i, rj), child (i, sj)), sj++, rj++;
                    while (rj < W
                           && (child (i, rj) == (c == 'a' ? 'b' : 'f')
                               || child (i, rj) == (c == 'a' ? 'c' : 'g')
                               || child (i, rj) == (c == 'a' ? 'd' : 'h')));

                    children.push_back (child);
                }

                // GO DOWN
                child = state;
                while (di < H && (c = state (di, j)) == '.')
                    di++;

                if (di < H && (c == 'x' || c == 'p'))
                {
                    int si = i;
                    do
                        std::swap (child (di, j), child (si, j)), si++, di++;
                    while (di < H
                           && (child (di, j) == (c == 'x' ? 'y' : 'q')
                               || child (di, j) == (c == 'x' ? 'z' : 'r')
                               || child (di, j) == (c == 'x' ? 'w' : 's')));

                    children.push_back (child);
                }

                // GO LEFT
                child = state;
                while (lj >= 0 && (c = state (i, lj)) == '.')
                    lj--;

                if (lj >= 0 && (c == 'b' || c == 'c' || c == 'd' || c == 'f' || c == 'g' || c == 'h'))
                {
                    int sj = j;
                    do
                        std::swap (child (i, lj), child (i, sj)), sj--, lj--;
                    while (lj >= 0 && child (i, lj) != (c == 'b' || c == 'c' || c == 'd' ? 'a' : 'e'));
                    std::swap (child (i, lj), child (i, sj));

                    children.push_back (child);
                }

                // GO UP
                child = state;
                while (ui >= 0 && (c = state (ui, j)) == '.')
                    ui--;

                if (ui >= 0 && (c == 'y' || c == 'z' || c == 'w' || c == 'q' || c == 'r' || c == 's'))
                {
                    int si = i;
                    do
                        std::swap (child (ui, j), child (si, j)), si--, ui--;
                    while (ui >= 0 && child (ui, j) != (c == 'y' || c == 'z' || c == 'w' ? 'x' : 'p'));
                    std::swap (child (ui, j), child (si, j));

                    children.push_back (child);
                }
            }

    return children;
}

int
h_score (state_t state)
{
    int i = EXIT_i, j = EXIT_j, cnt = 0;
    int di, dj;
    char h;
    std::string c;

    if (EXIT_i == 0 || EXIT_i == H - 1)
        di = EXIT_i == 0 ? 1 : -1, dj = 0, c = "abcd", h = 'p';
    else
        dj = EXIT_j == 0 ? 1 : -1, di = 0, c = "xyzw", h = 'e';

    for (; state (i, j) != h; i += di, j += dj)
        if (state (i, j) == c[0] || state (i, j) == c[1] || state (i, j) == c[2] || state (i, j) == c[3])
            cnt++;

    return cnt;
}

bool
is_goal (state_t state)
{
    int i = EXIT_i, j = EXIT_j;
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