from average import average
import numpy as np
import time

a = np.ones((1000000,), dtype = np.float32)

start = time.time()
result = average.average(a)
end = time.time()
print end - start

b = list(range(1000000))

start = time.time()
_sum = 0
for item in b:
    _sum += item
result = _sum / len(b)
end = time.time()
print end - start


start = time.time()
result = sum(b) / len(b)
end = time.time()
print end - start


start = time.time()
result = reduce(lambda x, y: x + y, b)
end = time.time()
print end - start


start = time.time()
result = [item * 2 for item in b]
end = time.time()
print end - start


start = time.time()
for idx, item in enumerate(b):
    b[idx] = 2 * item
end = time.time()
print end - start


start = time.time()
a = 2. * a
end = time.time()
print end - start


