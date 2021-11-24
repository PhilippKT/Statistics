#include"statistics.hpp"

int main()
{
    DoubleVariable v({1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << v.Get_Covariance() << '\n';
    std::cout << v.Get_Correlation() << '\n';
    return 0;
}
