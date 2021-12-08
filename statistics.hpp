#include<iostream>
#include<vector>
#include<cmath>
#include<algorithm>
#include <cstdlib>

double Medium(const std::vector<double>& data);
double Dispersion(const std::vector<double>& data);
double Minimum(const std::vector<double>& data);
double Maximum(const std::vector<double>& data);
double Covariance(const std::vector<double>& data_x, const std::vector<double>& data_y);
double Correlation(const std::vector<double>& data_x, const std::vector<double>& data_y);

class Segment;
template <class>
class Selecter;
class Variable;
class DoubleVariable;

class Segment{
private:
    double lb, rb;
public:
    Segment(double lb, double rb): lb(lb), rb(rb){}
    bool Contains(double x){
        if ((x >= lb) and (x <= rb)){
            return true;
        } else{
            return false;
        }
    }
    double RightBorder(){
        return rb;
    }
    double LeftBorder(){
        return lb;
    }
};

template <class TCmp=std::less<double>>
class Selecter{
private:
    std::vector<double> data;
    TCmp cmp;
    Segment Partition(uint32_t l, uint32_t r){
        uint32_t support_i = l + rand() % (r - l + 1);
        double support_el = data[support_i];
        uint32_t col = 0; // Количество опорных элементов минус 1.
        while(l < r){
            while ( cmp(data[l], support_el) and (data[l] != support_el) ){
                l++;
            }
            while ( cmp(support_el, data[r]) and (data[r] != support_el) ){
                r--;
            }
            if (l == r){
                break;
            }
            if (data[l] == data[r]){ // Это возможно только в случае data[l] == data[r] == support_el.
                col++;  //  /Операция сдвига влево. (просто игнорируем элементы == support_el) (у нас l < r)
                r--;    //  \В конце мы получим разбитый массив; в левой части от опорного элемента(support_el) элементы будут > support_el; в правой части -- <= support_el.
            } else{
                std::swap(data[l], data[r]);
            }
        }
        return Segment(l + 1, l + 1 + col);  //  Выражено через l, т.к. сдвигали влево. ((l+1) - наименьшая статистика опорного элемента, если он не единственный; (l+1+col) -- наибольшая)
    }
public:
    Selecter(){};
    Selecter(std::vector<double> data): data(data){}
    double QuickSelect(uint32_t k, uint32_t l, uint32_t r){
        Segment midseg = Partition(l, r);
        double mid = midseg.LeftBorder();
        if (midseg.Contains(k)){
            return data[mid - 1];
        }
        if (k < mid){
            return QuickSelect(k, l, mid - 2);
        }
        if (k > mid){
            return QuickSelect(k, mid, r);
        }
    }
    double QuickSelect(uint32_t k){
        return QuickSelect(k, 0, data.size() - 1);
    }
    friend Variable;
};

class Variable{
private:
    Selecter<std::less<double>> up;
    Selecter<std::greater<double>> down;
    size_t size;
    std::vector<double> data;
    std::vector<double> data_sorted_up = {};
    std::vector<double> data_sorted_down = {};
    std::vector<std::pair<bool, double>> data_ascending_order_statistics; // уже посчитанные порядковые статистики по возрастанию. Причём выгоднее становится сначала отсортировать данные(n = size), а потом взять m статистик (nlogn + m), чем просто взять m статистик QuickSelect-ом (n * O(n)), когда количество запросов статистик m достигает: nm > nlogn + m <=> m > (1+1/(n-1))*logn ~ logn (при n >> 1)
    std::pair<bool, double> medium;
    std::pair<bool, double> dispersion;
    std::pair<bool, double> minimum;
    std::pair<bool, double> maximum;
    std::pair<bool, double> summ;
public:
    Variable();
    Variable(std::vector<double> data);

    const std::vector<double>& GetData() const;
    const std::vector<double>& GetSortedUpData();
    const std::vector<double>& GetSortedDownData();
    double GetAscendingOrderStatistic(int32_t k); // k-ая порядковая статистика по возрастанию (начиная с 1)
    double GetDescendingOrderStatistic(int32_t k); // k-ая порядковая статистика по убыванию (начиная с 1)
    double GetMedium();
    double GetDispersion();
    double GetMinimum();
    double GetMaximum();
    double GetSumm();
    size_t GetSize();

    friend DoubleVariable;
};

class DoubleVariable{
    Variable* x;
    Variable* y;
    size_t size;
    std::pair<bool, double> covariance;
    std::pair<bool, double> correlation;
    std::pair<bool, double> a, b; // коэффициенты в линейном приближении y = a*x + b
    std::pair<bool, double> a_err, b_err; // их погрешности
public:
    DoubleVariable();
    DoubleVariable(std::vector<double> data_x, std::vector<double> data_y);
    DoubleVariable(Variable* x, Variable* y);

    const std::vector<double>& GetDataX() const;
    const std::vector<double>& GetSortedUpDataX();
    const std::vector<double>& GetSortedDownDataX();
    double GetMediumX();
    double GetDispersionX();
    double GetMinimumX();
    double GetMaximumX();
    double GetSummX();
    double GetAscendingOrderStatisticX(int32_t k);
    double GetDescendingOrderStatisticX(int32_t k);

    const std::vector<double>& GetDataY() const;
    const std::vector<double>& GetSortedUpDataY();
    const std::vector<double>& GetSortedDownDataY();
    double GetMediumY();
    double GetDispersionY();
    double GetMinimumY();
    double GetMaximumY();
    double GetSummY();
    double GetAscendingOrderStatisticY(int32_t k);
    double GetDescendingOrderStatisticY(int32_t k);

    size_t GetSize();
    double GetCovariance();
    double GetCorrelation();
    double GetA();
    double GetB();
    double GetAError();
    double GetBError();
};

double Medium(const std::vector<double>& data){
    double sum = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        sum += *i;
    return sum / data.size();
}

double Dispersion(const std::vector<double>& data){
    double med = Medium(data);
    double s = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        s += (*i - med)*(*i - med);
    return s / data.size();
}

double Minimum(const std::vector<double>& data){
    auto i = data.begin();
    auto en = data.end();
    double minimum = *i;
    i++;
    for(;i != en; ++i)
        if(*i < minimum) minimum = *i;
    return minimum;
}

double Maximum(const std::vector<double>& data){
    auto i = data.begin();
    auto en = data.end();
    double maximum = *i;
    i++;
    for(;i != en; ++i)
        if(*i > maximum) maximum = *i;
    return maximum;
}

double Covariance(const std::vector<double>& data_x, const std::vector<double>& data_y){
    if(data_x.size() != data_y.size()) throw std::runtime_error("Size of data_x != size of data_y");
    else{
        double xy = 0;
        size_t sz = data_x.size();
        for(size_t i = 0; i != sz; i++)
            xy += data_x[i] * data_y[i];
        double x = Medium(data_x);
        double y = Medium(data_y);
        return xy/sz - x*y;
    }
}

double Correlation(const std::vector<double>& data_x, const std::vector<double>& data_y){
    return Covariance(data_x, data_y) / std::sqrt(Dispersion(data_x) * Dispersion(data_y));
}


Variable::Variable(){
    size = 0;
    medium.first = false;
    dispersion.first = false;
    minimum.first = false;
    maximum.first = false;
    summ.first = false;
}

Variable::Variable(std::vector<double> data) : data(data), up(data), down(data){
    size = data.size();
    medium.first = false;
    dispersion.first = false;
    minimum.first = false;
    maximum.first = false;
    summ.first = false;
    for(uint32_t i = 0; i < size; ++i){
        std::pair<bool, double> p(false, 0);
        data_ascending_order_statistics.push_back(p);
    }
}

const std::vector<double>&Variable::GetData() const{
    return data;
}

const std::vector<double>&Variable::GetSortedUpData(){
    if (data_sorted_up.size() > 0){
        return data_sorted_up;
    }
    if (data_sorted_down.size() > 0){
        std::vector<double> v;
        for (uint32_t i = size - 1; i >= 0; i--){
            v.push_back(data_sorted_down[i]);
        }
        data_sorted_up = v;
        return data_sorted_up;
    }
    std::vector<double> v = data;
    std::sort(v.begin(), v.end());
    data_sorted_up = v;
    return data_sorted_up;
}

const std::vector<double>&Variable::GetSortedDownData(){
    if (data_sorted_down.size() > 0){
        return data_sorted_down;
    }
    if (data_sorted_up.size() > 0){
        std::vector<double> v;
        for (uint32_t i = size - 1; i >= 0; i--){
            v.push_back(data_sorted_up[i]);
        }
        data_sorted_down = v;
        return data_sorted_down;
    }
    std::vector<double> v = data;
    std::sort(v.rbegin(), v.rend());
    data_sorted_down = v;
    return data_sorted_down;
}

double Variable::GetAscendingOrderStatistic(int32_t k){  // k-ая порядковая статистика по возрастанию (начиная с 1)
    if ((k < 1) or (k > size)) throw std::out_of_range("The number of order statistic must be from 1 to the size of data!");
    if (data_ascending_order_statistics[k - 1].first == true){
        return data_ascending_order_statistics[k - 1].second;
    }
    if (data_sorted_up.size() > 0){
        return data_sorted_up[k - 1];
    }
    if (data_sorted_down.size() > 0){
        return data_sorted_down[size - k];
    }
    data_ascending_order_statistics[k - 1].first = true;
    data_ascending_order_statistics[k - 1].second = up.QuickSelect(k);
    return data_ascending_order_statistics[k - 1].second;
}

double Variable::GetDescendingOrderStatistic(int32_t k){  // k-ая порядковая статистика по убыванию (начиная с 1)
    if ((k < 1) or (k > size)) throw std::out_of_range("The number of order statistic must be from 1 to the size of data!");
    if (data_ascending_order_statistics[size - k].first == true){
        return data_ascending_order_statistics[size - k].second;
    }
    if (data_sorted_up.size() > 0){
        return data_sorted_up[size - k];
    }
    if (data_sorted_down.size() > 0){
        return data_sorted_down[k - 1];
    }
    data_ascending_order_statistics[size - k].first = true;
    data_ascending_order_statistics[size - k].second = down.QuickSelect(k);
    return data_ascending_order_statistics[size - k].second;
}

double Variable::GetMedium(){
    if(medium.first) return medium.second;
    if(summ.first){
        medium.second = summ.second / size;
        medium.first = true;
        return medium.second;
    }
    summ.second = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        summ.second += *i;
    summ.first = true;
    medium.second = summ.second / size;
    medium.first = true;
    return medium.second;
}

double Variable::GetDispersion(){
    if(dispersion.first) return dispersion.second;
    dispersion.second = Dispersion(data);
    dispersion.first = true;
    return dispersion.second;
}

double Variable::GetMaximum(){
    if(maximum.first) return maximum.second;
    maximum.second = Maximum(data);
    maximum.first = true;
    return maximum.second;
}

double Variable::GetMinimum(){
    if(minimum.first) return minimum.second;
    minimum.second = Minimum(data);
    minimum.first = true;
    return minimum.second;
}

double Variable::GetSumm(){
    if(summ.first) return summ.second;
    summ.second = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        summ.second += *i;
    summ.first = true;
    return summ.second;
}

size_t Variable::GetSize(){
    return size;
}

DoubleVariable::DoubleVariable(){
    size = 0;
    covariance.first = false;
    correlation.first = false;
}

DoubleVariable::DoubleVariable(Variable* x1, Variable* x2){
    if(x1->size != x2->size) throw std::runtime_error("Size of data_x != size of data_y");
    else{
        x = x1;
        y = x2;
        size = x->size;
        covariance.first = false;
        correlation.first = false;
    }
}

DoubleVariable::DoubleVariable(std::vector<double> data_x, std::vector<double> data_y){
    if(data_x.size() != data_y.size()) throw std::runtime_error("Size of data_x != size of data_y");
    else{
        x = new Variable(data_x);
        y = new Variable(data_y);
        size = data_x.size();
        covariance.first = false;
        correlation.first = false;
    }
}

const std::vector<double>&DoubleVariable::GetDataX() const{
    return x->GetData();
}

const std::vector<double>&DoubleVariable::GetSortedUpDataX(){
    return x->GetSortedUpData();
}

const std::vector<double>&DoubleVariable::GetSortedDownDataX(){
    return x->GetSortedDownData();
}

double DoubleVariable::GetMediumX(){
    return x->GetMedium();
}

double DoubleVariable::GetDispersionX(){
    return x->GetDispersion();
}

double DoubleVariable::GetMaximumX(){
    return x->GetMaximum();
}

double DoubleVariable::GetMinimumX(){
    return x->GetMinimum();
}

double DoubleVariable::GetSummX(){
    return x->GetSumm();
}

double DoubleVariable::GetAscendingOrderStatisticX(int32_t k){
    return x->GetAscendingOrderStatistic(k);
}

double DoubleVariable::GetDescendingOrderStatisticX(int32_t k){
    return x->GetDescendingOrderStatistic(k);
}

const std::vector<double>&DoubleVariable::GetDataY() const{
    return y->GetData();
}

const std::vector<double>&DoubleVariable::GetSortedUpDataY(){
    return y->GetSortedUpData();
}

const std::vector<double>&DoubleVariable::GetSortedDownDataY(){
    return y->GetSortedDownData();
}

double DoubleVariable::GetMediumY(){
    return y->GetMedium();
}

double DoubleVariable::GetDispersionY(){
    return y->GetDispersion();
}

double DoubleVariable::GetMaximumY(){
    return y->GetMaximum();
}

double DoubleVariable::GetMinimumY(){
    return y->GetMinimum();
}

double DoubleVariable::GetSummY(){
    return y->GetSumm();
}

double DoubleVariable::GetAscendingOrderStatisticY(int32_t k){
    return y->GetAscendingOrderStatistic(k);
}

double DoubleVariable::GetDescendingOrderStatisticY(int32_t k){
    return y->GetDescendingOrderStatistic(k);
}

size_t DoubleVariable::GetSize(){
    return size;
}

double DoubleVariable::GetCorrelation(){
    if(correlation.first) return correlation.second;
    correlation.second = Correlation(x->GetData(), y->GetData());
    correlation.first = true;
    return correlation.second;
}

double DoubleVariable::GetCovariance(){
    if(covariance.first) return covariance.second;
    covariance.second = Covariance(x->GetData(), y->GetData());
    covariance.first = true;
    return covariance.second;
}

double DoubleVariable::GetA(){
    if(a.first) return a.second;
    a.second = GetCovariance() / GetDispersionX();
    a.first = true;
    return a.second;
}

double DoubleVariable::GetB(){
    if(b.first) return b.second;
    b.second = GetMediumY() - GetA()*GetMediumX();
    b.first = true;
    return b.second;
}

double DoubleVariable::GetAError(){
    if(a_err.first) return a_err.second;
    double q = GetDispersionY() / GetDispersionX() - GetA()*GetA();
    if(q < 0) a_err.second = 0;
    else a_err.second = std::sqrt(q) / std::sqrt(size);
    a_err.first = true;
    return a_err.second;
}

double DoubleVariable::GetBError(){
    if(b_err.first) return b_err.second;
    b_err.second = GetAError() * std::sqrt(GetDispersionX());
    b_err.first = true;
    return b_err.second;
}
