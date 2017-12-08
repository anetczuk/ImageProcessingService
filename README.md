## Image Analysis Service (ias)

This is an example of simple image processing library developed in C++. Library consists of several _hand-made_ basic image processing operations like:
- erode, 
- dilate, 
- flood fill,
- Laplace filter,
- Gaussian blur

and additional operations defined in *Operations* section.

The library can be accessed by Application Programming Interface and Command Line Interface.

Library can be treated as use-case example of following libraries: 
- Boost.Log
- Boost.UnitTest
- OpenCV


### Requirements

_ias_ library requires Boost Log. Installation:
1. sudo apt-get install libopencv-dev
2. sudo apt-get install libboost-log-dev
3. sudo apt-get install libboost-test-dev


### Building

cmake .
make

Running cli app: ./ias/main/ias
Running tests: ./ias/test/runTests.sh

Application was built under:
- gcc: 4.9.2 and 5.4.0
- clang: 3.5.0 and 3.8.0


### Operations

Implemented operations:
1. *FIND_REGION* - finds nearby pixels whose color is similar creating contiguous area. Algorithm scans whole image for given color and then cuts desired region by _flood filling_ it from rest of image.
2. *FIND_PERIMETER* - finds contour of given region by applying Laplace filter and thresholding the result.
3. *DISPLAY_IMAGE* - pop-up window with loaded image (use OpenCV build-in function).
4. *DISPLAY_PIXELS* - pop-up window with calculated result (use OpenCV build-in function).
5. *SAVE_PIXELS* - store result to file (use OpenCV build-in function).
6. *FIND_SMOOTH_PERIMETER* - finds smooth contour of given region. Smoothing is done by applying Gaussian blur on input region. It's preceded by _erode_ and _dilate_ operations resulting in removal of small artifacts. Final result is obtained by calling Laplace filter. Gaussian smoothing was preferable because of ease of implementation. More sophisticated solution can be obtained by use of OpenCV algorithms. 


### API

Library consists of single class _Analysis_ containing following interface methods:
```cpp
bool Analysis::loadImage(const std::string& imagePath);
```
method loads image from given path. Returs false if file could not be opened, otherwise true

```cpp
void Analysis::findRegion(const cv::Point& pixelCoords, const cv::Vec3f& color, const uchar equalityMargin = 0);
```
performs FIND_REGION operation on opened file. As parameters it takes pixel[x,y] coordinates, color of interest[BGR] and color tolerance[0..255]. Tolerance is calculated for every color component

```cpp
void Analysis::findPerimeter(const cv::Mat& regionsMask);
```
method performs *FIND_PERIMETER* operation on opened file. As argument it takes region mask. Mask has to be in CV_8UC1 format has the same size as loaded image

```cpp
void Analysis::findPerimeter();
```
method performs *FIND_PERIMETER* operation on opened file with mask calculated by previous *FIND_* operation

```cpp
void Analysis::findSmoothPerimeter();
```
method performs *FIND_SMOOTH_PERIMETER* operation on opened file with mask calculated by previous *FIND_* operation

```cpp
void Analysis::displayImage() const;
```
performs *DISPLAY_IMAGE* operation

```cpp
void Analysis::displayPixels() const;
```
performs *DISPLAY_PIXELS* operation on last result of *FIND_* operation

```cpp
void Analysis::displayJoin() const;
```
display both loaded image and calculated result in one window

```cpp
void Analysis::storeResult(const std::string& outputPath) const;
```
performs *SAVE_PIXELS* operation


### Command line interface

Application _ias_ takes following command line arguments:
- --help -- print help message
- --logcout -- print messages to stdout instead of _logger.log_ file
- --image=[path] -- load image from file _path_
- --findRegion=[pX,pY,B,G,R,T] --call *FIND_REGION* operation where:
							   (pX, pY) are coordinates of pixel on image
							   (B,G,R) is color in BGR format
							   T is tolerance of color (for each color component)
- --findPerimeter -- call *FIND_PERIMETER* on loaded image and region calculated by last *FIND_* operation
- --findSmoothPerimeter -- call *FIND_SMOOTH_PERIMETER* on loaded image and region calculated by last *FIND_* operation
- --displayImage -- display loaded image
- --displayPixels -- display calculated result
- --displayJoin -- display both image and result on one window
- --savePixels=[path] -- save image to file _path_

Application supports _streaming_(repeating) all parameters (expect of --help). E.g. it is possible to make following call:
_ias --image=test.png --findRegion=0,0,0,0,0,0 --savePixels=out1.png --findPerimeter --savePixels=out1.png_ 


### Examples of use

API examples can be found in unit tests directory _src/ias/test_.
Command line interface examples can be found in test scripts directory _src/ias/main/test_. 

These scripts illustrates implemented algorithms in action on test images.


### Improvements

1. implementation of FIND_REGION: consider comparing difference between neighbour pixels instead of giving absolute color to search for,
2. reasonable assumption should be made to _normalize_ input image resolution or control granularity of algorithms (e.g. scaling input image),
3. parsing method of command line arguments could be refactored to remove stream of _if_-_else if_ statements.


### References

- OpenCV sample: https://gist.github.com/oblinger/efd6f27da74b6c7846954d3b2c2fffa3
- flood fill: https://en.wikipedia.org/wiki/Flood_fill
- image filters: https://en.wikipedia.org/wiki/Kernel_%28image_processing%29
- OpenCV documentation
- Boost documentation
