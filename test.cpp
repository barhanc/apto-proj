#include <iostream>
#include <queue>
#include <set>

int
main ()
{
    using pis = std::pair<int, std::string>;
    std::priority_queue<pis, std::vector<pis>, std::greater<pis> > q;
    std::set<pis> s;

    q.push ({ 1, "xyafa" });
    q.push ({ 4, "bcad" });
    q.push ({ 3, "xyzd" });
    q.push ({ 2, "asfs" });

    s.insert ({ 1, "xyafa" });
    s.insert ({ 4, "bcad" });
    s.insert ({ 3, "xyzd" });
    s.insert ({ 2, "asfs" });

    while (!s.empty ())
    {
        auto el = *s.begin ();
        std::cout << el.second << std::endl;
        s.erase (el);
    }
}