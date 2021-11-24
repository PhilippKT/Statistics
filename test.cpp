#include"statistics.hpp"

int main()
{
    DoubleVariable v({1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << v.Get_Covariance() << '\n';
    std::cout << v.Get_Correlation() << '\n';
    std::cout << v.Get_Dispersion_x() << '\n';
    std::cout << v.Get_Dispersion_y() << '\n';
    std::cout << v.Get_Covariance() << '\n';
    std::cout << "y = ax + b" << '\n';
    std::cout << "a = (" << v.Get_a() << " +/- " << v.Get_a_error() << ")" << '\n';
    std::cout << "b = (" << v.Get_b() << " +/- " << v.Get_b_error() << ")" << '\n';
    return 0;
}
