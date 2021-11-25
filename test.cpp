#include"statistics.hpp"

int main()
{
    DoubleVariable v({1, 2, 3, 4, 5, 6, 7, 8, 9}, {1, 2, 3, 4, 5, 6, 7, 8, 9});
    std::cout << v.GetCovariance() << '\n';
    std::cout << v.GetCorrelation() << '\n';
    std::cout << v.GetDispersionX() << '\n';
    std::cout << v.GetDispersionY() << '\n';
    std::cout << v.GetCovariance() << '\n';
    std::cout << "y = ax + b" << '\n';
    std::cout << "a = (" << v.GetA() << " +/- " << v.GetAError() << ")" << '\n';
    std::cout << "b = (" << v.GetB() << " +/- " << v.GetBError() << ")" << '\n';
    return 0;
}
