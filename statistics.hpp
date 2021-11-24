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
    std::pair<bool, double> medium;
    std::pair<bool, double> dispersion;
    std::pair<bool, double> minimum;
    std::pair<bool, double> maximum;
    std::pair<bool, double> summ;
public:
    Variable();
    Variable(std::vector<double> data);

    const std::vector<double>& Get_Data() const;
    double Get_Medium();
    double Get_Dispersion();
    double Get_Minimum();
    double Get_Maximum();
    double Get_Summ();
    size_t Get_Size();

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

    const std::vector<double>& Get_Data_x() const;
    double Get_Medium_x();
    double Get_Dispersion_x();
    double Get_Minimum_x();
    double Get_Maximum_x();
    double Get_Summ_x();

    const std::vector<double>& Get_Data_y() const;
    double Get_Medium_y();
    double Get_Dispersion_y();
    double Get_Minimum_y();
    double Get_Maximum_y();
    double Get_Summ_y();

    size_t Get_Size();
    double Get_Covariance();
    double Get_Correlation();
    double Get_a();
    double Get_b();
    double Get_a_error();
    double Get_b_error();
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

Variable::Variable(std::vector<double> data) : data(data){
    size = data.size();
    medium.first = false;
    dispersion.first = false;
    minimum.first = false;
    maximum.first = false;
    summ.first = false;
}

const std::vector<double>&Variable::Get_Data() const{
    return data;
}

double Variable::Get_Medium(){
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

double Variable::Get_Dispersion(){
    if(dispersion.first) return dispersion.second;
    dispersion.second = Dispersion(data);
    dispersion.first = true;
    return dispersion.second;
}

double Variable::Get_Maximum(){
    if(maximum.first) return maximum.second;
    maximum.second = Maximum(data);
    maximum.first = true;
    return maximum.second;
}

double Variable::Get_Minimum(){
    if(minimum.first) return minimum.second;
    minimum.second = Minimum(data);
    minimum.first = true;
    return minimum.second;
}

double Variable::Get_Summ(){
    if(summ.first) return summ.second;
    summ.second = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        summ.second += *i;
    summ.first = true;
    return summ.second;
}

size_t Variable::Get_Size(){
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

const std::vector<double>&DoubleVariable::Get_Data_x() const{
    return x->Get_Data();
}

double DoubleVariable::Get_Medium_x(){
    return x->Get_Medium();
}

double DoubleVariable::Get_Dispersion_x(){
    return x->Get_Dispersion();
}

double DoubleVariable::Get_Maximum_x(){
    return x->Get_Maximum();
}

double DoubleVariable::Get_Minimum_x(){
    return x->Get_Minimum();
}

double DoubleVariable::Get_Summ_x(){
    return x->Get_Summ();
}

const std::vector<double>&DoubleVariable::Get_Data_y() const{
    return y->Get_Data();
}

double DoubleVariable::Get_Medium_y(){
    return y->Get_Medium();
}

double DoubleVariable::Get_Dispersion_y(){
    return y->Get_Dispersion();
}

double DoubleVariable::Get_Maximum_y(){
    return y->Get_Maximum();
}

double DoubleVariable::Get_Minimum_y(){
    return y->Get_Minimum();
}

double DoubleVariable::Get_Summ_y(){
    return y->Get_Summ();
}

size_t DoubleVariable::Get_Size(){
    return size;
}

double DoubleVariable::Get_Correlation(){
    if(correlation.first) return correlation.second;
    correlation.second = Correlation(x->Get_Data(), y->Get_Data());
    correlation.first = true;
    return correlation.second;
}

double DoubleVariable::Get_Covariance(){
    if(covariance.first) return covariance.second;
    covariance.second = Covariance(x->Get_Data(), y->Get_Data());
    covariance.first = true;
    return covariance.second;
}

double DoubleVariable::Get_a(){
    if(a.first) return a.second;
    a.second = Get_Covariance() / Get_Dispersion_x();
    a.first = true;
    return a.second;
}

double DoubleVariable::Get_b(){
    if(b.first) return b.second;
    b.second = Get_Medium_y() - Get_a()*Get_Medium_x();
    b.first = true;
    return b.second;
}

double DoubleVariable::Get_a_error(){
    if(a_err.first) return a_err.second;
    double q = Get_Dispersion_y() / Get_Dispersion_x() - Get_a()*Get_a();
    if(q < 0) a_err.second = 0;
    else a_err.second = std::sqrt(q) / std::sqrt(size);
    a_err.first = true;
    return a_err.second;
}

double DoubleVariable::Get_b_error(){
    if(b_err.first) return b_err.second;
    b_err.second = Get_a_error() * std::sqrt(Get_Dispersion_x());
    b_err.first = true;
    return b_err.second;
}
