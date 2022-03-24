# Comparison of Different Parallel Methods for Median Filter
Parallel programming final project
---
## Prerequisite
1. CMake
2. CUDA
3. OpenCV

---
## Preprocessing
- We will add pepper and salt (P&S) noise in image
1. Put the image file you want in /
2. Modify the filename and P&S threshold in Line.77 in processing.py
```python
spp = SaltAndPepper(FILENAME, THRESHOLD)
```
3. The output filename will be the original filename with **_noise** suffix

---
## Modify input image
- Copy the image with noise into the method directory.
- Change the filename in .cpp file to use different input file.

---
## Build
1. Move into the directory of different methods
```bash
cd ${DIR_PATH}
```
2. Type build command
```bash
cmake .
make
```

---
## Execute filter
**Because we dont have OpenCV with GPU, so we need to use write.cpp to read value of images then write into .txt, after that we use medianfilter_cuda.cu to compute. Finally, use read.cpp to read .txt and output result image**
1. Move into the directory of different methods
```bash
cd ${DIR_PATH}
```
2. Type running command
```bash
./MedianFilter
```
---
## Data
|Images|Height|Width|Channels|
|----|-----|----|-----|
|4K image|2160|3840|3|
|4K ultra HD image|5459|9705|3|

### Images afore filter
- 4K image with noise
![4K Noise Image](src/4K_noise.jpg?raw=false "4K Noise")
- 4K ultra HD image with noise
![4K Ultra HD Noise Image](src/4K_ultra_noise.jpg?raw=false "4K UHD Noise")

---
## Result
### Speed up
|Methods|4K image|4K ultra HD image|
|----|-----|----|
|Original|1.000x|1.000x|
|SIMD|2.306x|2.139x|
|Threading|3.680x|3.774x|
|OpenMP|3.815x|3.807x|
|CUDA|36.677x|96.468x|
|SIMD+OpenMP|8.360x|7.754x|

### Image after filter
- 4K image after median filter
![4K Noise Image](src/4K_output.jpg?raw=false "4K Noise")
- 4K ultra HD image after median filter
![4K Ultra HD Noise Image](src/4K_ultra_output.jpg?raw=false "4K UHD Noise")
