from gen_geo_map import gen_geo_map
import numpy as np
import time
import matplotlib.pyplot as plt

score_map = np.zeros((384, 384), dtype=np.float32)
geo_map = np.zeros((384, 384, 2), dtype=np.float32)

gt_bboxes = np.array([
        [[10., 10.], [50., 10.], [50., 50.], [10., 50.]], 
        [[40., 10.], [90., 10.], [90., 50.], [40., 50.]] 
    ], dtype= np.float32)

gt_masks = np.array([1., 1.], dtype=np.float32)
gen_geo_map.genGeoMap(score_map, geo_map, gt_bboxes, gt_masks, 2)


fig, axs = plt.subplots(2, 2, figsize=(20, 30))
axs[0, 0].imshow(score_map)
axs[0, 0].set_xticks([])
axs[0, 0].set_yticks([])

axs[0, 1].imshow(geo_map[::, ::, 0])
axs[0, 1].set_xticks([])
axs[0, 1].set_yticks([])

axs[1, 0].imshow(geo_map[::, ::, 1])
axs[1, 0].set_xticks([])
axs[1, 0].set_yticks([])

plt.tight_layout()
plt.show()
plt.close()     
