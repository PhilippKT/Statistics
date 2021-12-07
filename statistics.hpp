#include<iostream>
#include<vector>
#include<cmath>

double Medium(const std::vector<double>& data);
double Dispersion(const std::vector<double>& data);
double Minimum(const std::vector<double>& data);
double Maximum(const std::vector<double>& data);
double Covariance(const std::vector<double>& data_x, const std::vector<double>& data_y);
double Correlation(const std::vector<double>& data_x, const std::vector<double>& data_y);

class Variable;
class DoubleVariable;

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
    double GetMediumX() const;
    double GetDispersionX() const;
    double GetMinimumX() const;
    double GetMaximumX() const;
    double GetSumX() const;

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

const std::vector<double>&DoubleVariable::GetDataY() const{
    return y->GetData();
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
