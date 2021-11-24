#include<iostream>
#include<vector>

class Variable{
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
