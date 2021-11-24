#include"statistics.hpp"

int main()
{
    Variable v({1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << v.Get_Medium() << '\n';
    std::cout << v.Get_Maximum() << '\n';
    std::cout << v.Get_Minimum() << '\n';
    std::cout << v.Get_Summ() << '\n';
    std::cout << v.Get_Dispersion() << '\n';
    return 0;
}
