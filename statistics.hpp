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
    size_t size;
    std::vector<double> data;
    mutable std::pair<bool, double> medium;
    mutable std::pair<bool, double> dispersion;
    mutable std::pair<bool, double> minimum;
    mutable std::pair<bool, double> maximum;
    mutable std::pair<bool, double> sum;
public:
    Variable();
    Variable(const std::vector<double>& data);

    const std::vector<double>& GetData() const;
    std::vector<double> GetSortedUpData() const;
    std::vector<double> GetSortedDownData() const;
    double GetAscendingOrderStatistic(int32_t k) const; // k-ая порядковая статистика по возрастанию (начиная с 1)
    double GetDescendingOrderStatistic(int32_t k)const; // k-ая порядковая статистика по убыванию (начиная с 1)

    double GetMedium() const;
    double GetDispersion() const;
    double GetMinimum() const;
    double GetMaximum() const;
    double GetSum() const;
    size_t GetSize() const;

    friend DoubleVariable;
};

class DoubleVariable{
    Variable* x;
    Variable* y;
    size_t size;
    mutable std::pair<bool, double> covariance;
    mutable std::pair<bool, double> correlation;
    mutable std::pair<bool, double> a, b; // коэффициенты в линейном приближении y = a*x + b
    mutable std::pair<bool, double> a_err, b_err; // их погрешности
public:
    DoubleVariable();
    DoubleVariable(const std::vector<double>& data_x, const std::vector<double>& data_y);
    DoubleVariable(Variable* x, Variable* y);

    const std::vector<double>& GetDataX() const;
    std::vector<double> GetSortedUpDataX() const;
    std::vector<double> GetSortedDownDataX() const;
    std::vector<double> GetSortedUpDataY() const;
    std::vector<double> GetSortedDownDataY() const;
    DoubleVariable GetFilteredData(uint32_t k) const; // k - к-т фильтрации (p(point_bad, point_nearest_to_this) > k * <p(point, point_nearest_to_this)>(среднее))
    double GetAscendingOrderStatisticX(int32_t k)const;
    double GetDescendingOrderStatisticX(int32_t k) const;
    double GetMediumX() const;
    double GetDispersionX() const;
    double GetMinimumX() const;
    double GetMaximumX() const;
    double GetSumX() const;
    double GetAscendingOrderStatisticY(int32_t k) const;
    double GetDescendingOrderStatisticY(int32_t k) const;

    const std::vector<double>& GetDataY() const;
    double GetMediumY() const;
    double GetDispersionY() const;
    double GetMinimumY() const;
    double GetMaximumY() const;
    double GetSumY() const;

    size_t GetSize() const;
    double GetCovariance() const;
    double GetCorrelation() const;
    double GetA() const;
    double GetB() const;
    double GetAError() const;
    double GetBError() const;
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
    sum.first = false;
}

Variable::Variable(const std::vector<double>& data) : data(data){
    size = data.size();
    medium.first = false;
    dispersion.first = false;
    minimum.first = false;
    maximum.first = false;
    sum.first = false;
}

const std::vector<double>&Variable::GetData() const{
    return data;
}

std::vector<double> Variable::GetSortedUpData() const{
    std::vector<double> v = data;
    std::sort(v.begin(), v.end());
    return v;
}

std::vector<double> Variable::GetSortedDownData() const{
    std::vector<double> v = data;
    std::sort(v.rbegin(), v.rend());
    return v;
}

double Variable::GetAscendingOrderStatistic(int32_t k) const{
    if ((k < 1) or (k > size)) throw std::out_of_range("The number of order statistic must be from 1 to the size of data!");
    Selecter<std::less<double>> up(data);
    return up.QuickSelect(k);
}

double Variable::GetDescendingOrderStatistic(int32_t k) const{
    if ((k < 1) or (k > size)) throw std::out_of_range("The number of order statistic must be from 1 to the size of data!");
    Selecter<std::greater<double>> down(data);
    return down.QuickSelect(k);;
}

double Variable::GetMedium() const{
    if(medium.first) return medium.second;
    if(sum.first){
        medium.second = sum.second / size;
        medium.first = true;
        return medium.second;
    }
    sum.second = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        sum.second += *i;
    sum.first = true;
    medium.second = sum.second / size;
    medium.first = true;
    return medium.second;
}

double Variable::GetDispersion() const{
    if(dispersion.first) return dispersion.second;
    dispersion.second = Dispersion(data);
    dispersion.first = true;
    return dispersion.second;
}

double Variable::GetMaximum() const{
    if(maximum.first) return maximum.second;
    maximum.second = Maximum(data);
    maximum.first = true;
    return maximum.second;
}

double Variable::GetMinimum() const{
    if(minimum.first) return minimum.second;
    minimum.second = Minimum(data);
    minimum.first = true;
    return minimum.second;
}

double Variable::GetSum() const{
    if(sum.first) return sum.second;
    sum.second = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        sum.second += *i;
    sum.first = true;
    return sum.second;
}

size_t Variable::GetSize() const{
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

DoubleVariable::DoubleVariable(const std::vector<double>& data_x, const std::vector<double>& data_y){
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

std::vector<double> DoubleVariable::GetSortedUpDataX() const{
    return x->GetSortedUpData();
}

std::vector<double> DoubleVariable::GetSortedDownDataX() const{
    return x->GetSortedDownData();
}

double DoubleVariable::GetMediumX() const{
    return x->GetMedium();
}

double DoubleVariable::GetDispersionX() const{
    return x->GetDispersion();
}

double DoubleVariable::GetMaximumX() const{
    return x->GetMaximum();
}

double DoubleVariable::GetMinimumX() const{
    return x->GetMinimum();
}

double DoubleVariable::GetSumX() const{
    return x->GetSum();
}

double DoubleVariable::GetAscendingOrderStatisticX(int32_t k) const{
    return x->GetAscendingOrderStatistic(k);
}

double DoubleVariable::GetDescendingOrderStatisticX(int32_t k) const{
    return x->GetDescendingOrderStatistic(k);
}

const std::vector<double>&DoubleVariable::GetDataY() const{
    return y->GetData();
}

std::vector<double> DoubleVariable::GetSortedUpDataY() const{
    return y->GetSortedUpData();
}

std::vector<double> DoubleVariable::GetSortedDownDataY() const{
    return y->GetSortedDownData();
}


double DoubleVariable::GetMediumY() const{
    return y->GetMedium();
}

double DoubleVariable::GetDispersionY() const{
    return y->GetDispersion();
}

double DoubleVariable::GetMaximumY() const{
    return y->GetMaximum();
}

double DoubleVariable::GetMinimumY() const{
    return y->GetMinimum();
}

double DoubleVariable::GetSumY() const{
    return y->GetSum();
}

double DoubleVariable::GetAscendingOrderStatisticY(int32_t k) const{
    return y->GetAscendingOrderStatistic(k);
}

double DoubleVariable::GetDescendingOrderStatisticY(int32_t k) const{
    return y->GetDescendingOrderStatistic(k);
}


size_t DoubleVariable::GetSize() const{
    return size;
}

double DoubleVariable::GetCorrelation() const{
    if(correlation.first) return correlation.second;
    correlation.second = Correlation(x->GetData(), y->GetData());
    correlation.first = true;
    return correlation.second;
}

double DoubleVariable::GetCovariance() const{
    if(covariance.first) return covariance.second;
    covariance.second = Covariance(x->GetData(), y->GetData());
    covariance.first = true;
    return covariance.second;
}

double DoubleVariable::GetA() const{
    if(a.first) return a.second;
    a.second = GetCovariance() / GetDispersionX();
    a.first = true;
    return a.second;
}

double DoubleVariable::GetB() const{
    if(b.first) return b.second;
    b.second = GetMediumY() - GetA()*GetMediumX();
    b.first = true;
    return b.second;
}

double DoubleVariable::GetAError() const{
    if(a_err.first) return a_err.second;
    double q = GetDispersionY() / GetDispersionX() - GetA()*GetA();
    if(q < 0) a_err.second = 0;
    else a_err.second = std::sqrt(q) / std::sqrt(size);
    a_err.first = true;
    return a_err.second;
}

double DoubleVariable::GetBError() const{
    if(b_err.first) return b_err.second;
    b_err.second = GetAError() * std::sqrt(GetDispersionX());
    b_err.first = true;
    return b_err.second;
}

DoubleVariable DoubleVariable::GetFilteredData(uint32_t k=2) const{
    std::vector<double> data_x0 = x->GetData(), data_y0 = y->GetData();
    if (size < 2){
        return DoubleVariable(data_x0, data_y0);
    } else{
        std::vector<double> data_x, data_y;
        double a_ = GetA();
        double b_ = GetB();
        double med_p, sum_p = 0;
        std::vector<double> p; // до прямой
        for (uint32_t i = 0; i < size; ++i){
            p.push_back( abs( data_y0[i] - a_*((data_x0)[i]) - b_ ) / std::sqrt(a_*a_ + b_*b_) );
            sum_p += p[i];
        }
        med_p = sum_p / size;
        for (uint32_t i = 0; i < size; ++i){
            if (p[i] < k * med_p){
                data_x.push_back(data_x0[i]);
                data_y.push_back(data_y0[i]);
            }
        }
        return DoubleVariable(data_x, data_y);
    }
}
