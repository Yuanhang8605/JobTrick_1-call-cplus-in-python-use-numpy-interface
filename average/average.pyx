import numpy as np
cimport numpy as np

assert sizeof(int) == sizeof(np.int32_t)

cdef extern from "_average.hpp":
    float _average(np.float32_t*, int)

def average(np.ndarray[np.float32_t, ndim=1] vec):
    cdef int num_elem = vec.shape[0]
    return _average(&vec[0], num_elem)
