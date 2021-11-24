#include<iostream>
#include<vector>
#include<cmath>

float Medium(const std::vector<float>& data);
float Dispersion(const std::vector<float>& data);
float Minimum(const std::vector<float>& data);
float Maximum(const std::vector<float>& data);
float Covariance(const std::vector<float>& data_x, const std::vector<float>& data_y);
float Correlation(const std::vector<float>& data_x, const std::vector<float>& data_y);

class Variable;
class DoubleVariable;

class Variable{
private:
    size_t size;
    std::vector<float> data;
    std::pair<bool, float> medium;
    std::pair<bool, float> dispersion;
    std::pair<bool, float> minimum;
    std::pair<bool, float> maximum;
    std::pair<bool, float> summ;
public:
    Variable();
    Variable(std::vector<float> data);

    const std::vector<float>& Get_Data() const;
    float Get_Medium();
    float Get_Dispersion();
    float Get_Minimum();
    float Get_Maximum();
    float Get_Summ();
    size_t Get_Size();

    friend DoubleVariable;
};

class DoubleVariable{
    Variable* x;
    Variable* y;
    size_t size;
    std::pair<bool, float> covariance;
    std::pair<bool, float> correlation;
public:
    DoubleVariable();
    DoubleVariable(std::vector<float> data_x, std::vector<float> data_y);
    DoubleVariable(Variable* x, Variable* y);

    const std::vector<float>& Get_Data_x() const;
    float Get_Medium_x();
    float Get_Dispersion_x();
    float Get_Minimum_x();
    float Get_Maximum_x();
    float Get_Summ_x();

    const std::vector<float>& Get_Data_y() const;
    float Get_Medium_y();
    float Get_Dispersion_y();
    float Get_Minimum_y();
    float Get_Maximum_y();
    float Get_Summ_y();

    size_t Get_Size();
    float Get_Covariance();
    float Get_Correlation();
};

float Medium(const std::vector<float>& data){
    float sum = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        sum += *i;
    return sum / data.size();
}

float Dispersion(const std::vector<float>& data){
    float med = Medium(data);
    float s = 0;
    auto i = data.begin();
    auto en = data.end();
    for(;i != en; ++i)
        s += (*i - med)*(*i - med);
    return s / data.size();
}

float Minimum(const std::vector<float>& data){
    auto i = data.begin();
    auto en = data.end();
    float minimum = *i;
    i++;
    for(;i != en; ++i)
        if(*i < minimum) minimum = *i;
    return minimum;
}

float Maximum(const std::vector<float>& data){
    auto i = data.begin();
    auto en = data.end();
    float maximum = *i;
    i++;
    for(;i != en; ++i)
        if(*i > maximum) maximum = *i;
    return maximum;
}

float Covariance(const std::vector<float>& data_x, const std::vector<float>& data_y){
    if(data_x.size() != data_y.size()) throw std::runtime_error("Size of data_x != size of data_y");
    else{
        float xy = 0;
        size_t sz = data_x.size();
        for(size_t i = 0; i != sz; i++)
            xy += data_x[i] * data_y[i];
        float x = Medium(data_x);
        float y = Medium(data_y);
        return xy/sz - x*y;
    }
}


float Correlation(const std::vector<float>& data_x, const std::vector<float>& data_y){
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

Variable::Variable(std::vector<float> data) : data(data){
    size = data.size();
    medium.first = false;
    dispersion.first = false;
    minimum.first = false;
    maximum.first = false;
    summ.first = false;
}

const std::vector<float>&Variable::Get_Data() const{
    return data;
}

float Variable::Get_Medium(){
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

float Variable::Get_Dispersion(){
    if(dispersion.first) return dispersion.second;
    dispersion.second = Dispersion(data);
    dispersion.first = true;
    return dispersion.second;
}

float Variable::Get_Maximum(){
    if(maximum.first) return maximum.second;
    maximum.second = Maximum(data);
    maximum.first = true;
    return maximum.second;
}

float Variable::Get_Minimum(){
    if(minimum.first) return minimum.second;
    minimum.second = Minimum(data);
    minimum.first = true;
    return minimum.second;
}

float Variable::Get_Summ(){
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

DoubleVariable::DoubleVariable(std::vector<float> data_x, std::vector<float> data_y){
    if(data_x.size() != data_y.size()) throw std::runtime_error("Size of data_x != size of data_y");
    else{
        x = new Variable(data_x);
        y = new Variable(data_y);
        size = data_x.size();
        covariance.first = false;
        correlation.first = false;
    }
}

const std::vector<float>&DoubleVariable::Get_Data_x() const{
    return x->Get_Data();
}

float DoubleVariable::Get_Dispersion_x(){
    return x->Get_Dispersion();
}

float DoubleVariable::Get_Maximum_x(){
    return x->Get_Maximum();
}

float DoubleVariable::Get_Minimum_x(){
    return x->Get_Minimum();
}

float DoubleVariable::Get_Summ_x(){
    return x->Get_Summ();
}

const std::vector<float>&DoubleVariable::Get_Data_y() const{
    return y->Get_Data();
}

float DoubleVariable::Get_Dispersion_y(){
    return y->Get_Dispersion();
}

float DoubleVariable::Get_Maximum_y(){
    return y->Get_Maximum();
}

float DoubleVariable::Get_Minimum_y(){
    return y->Get_Minimum();
}

float DoubleVariable::Get_Summ_y(){
    return y->Get_Summ();
}

size_t DoubleVariable::Get_Size(){
    return size;
}

float DoubleVariable::Get_Correlation(){
    return Correlation(x->Get_Data(), y->Get_Data());
}

float DoubleVariable::Get_Covariance(){
    return Covariance(x->Get_Data(), y->Get_Data());
}
