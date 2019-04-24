#include "_average.hpp"
#include <algorithm>

float _average(float* vec, int num_elem){
    float sum = std::accumulate(vec, vec+num_elem, 0.0);
    return sum / num_elem;
}
