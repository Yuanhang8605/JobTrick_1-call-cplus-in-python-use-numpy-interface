import numpy as np
cimport numpy as np

assert sizeof(int) == sizeof(np.int32_t)

cdef extern from "_gen_geo_map.hpp":
    float _genGeoMap(np.float32_t*, np.float32_t*, np.float32_t*, np.float32_t*, int, int, int)

def genGeoMap(np.ndarray[np.float32_t, ndim=2] score_map,     \
                np.ndarray[np.float32_t, ndim=3] geo_map,       \
                np.ndarray[np.float32_t, ndim=3] gt_bboxes,     \
                np.ndarray[np.float32_t, ndim=1] gt_masks,     \
                np.int32_t num_gt_bboxes):
    cdef int H = score_map.shape[0]
    cdef int W = score_map.shape[1]

    _genGeoMap(&score_map[0,0], &geo_map[0,0,0], &gt_bboxes[0,0,0], &gt_masks[0], H, W, num_gt_bboxes)

