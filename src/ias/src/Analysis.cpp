/// MIT License
///
/// Copyright (c) 2017 Arkadiusz Netczuk <dev.arnet@gmail.com>
///
/// Permission is hereby granted, free of charge, to any person obtaining a copy
/// of this software and associated documentation files (the "Software"), to deal
/// in the Software without restriction, including without limitation the rights
/// to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
/// copies of the Software, and to permit persons to whom the Software is
/// furnished to do so, subject to the following conditions:
///
/// The above copyright notice and this permission notice shall be included in all
/// copies or substantial portions of the Software.
///
/// THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
/// IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
/// FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
/// AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
/// LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
/// OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
/// SOFTWARE.
///

#include "ias/Analysis.h"

#include <opencv2/opencv.hpp>


using namespace cv;


namespace ias {

    Analysis::Analysis(): currentImage(), lastResult() {
    }

    Analysis::~Analysis() {
    }

    bool Analysis::loadImage(const std::string& imagePath) {
        currentImage = imread(imagePath, 1);                               /// BGR format
        return !currentImage.empty();
    }

    cv::Vec3b Analysis::color(const int y, const int x ) const {
        if (currentImage.empty()) {
            return cv::Vec3b();
        }
        if (x<0)
            return cv::Vec3b();
        if (y<0)
            return cv::Vec3b();
        if (x>=(currentImage.cols-1))
            return cv::Vec3b();
        if (y>=(currentImage.rows-1))
            return cv::Vec3b();

        return currentImage.at<cv::Vec3b>( y, x );
    }

    cv::Vec3b Analysis::color(const cv::Point& pixel) const {
        if (currentImage.empty()) {
            return cv::Vec3b();
        }
        if (pixel.x<0)
            return cv::Vec3b();
        if (pixel.y<0)
            return cv::Vec3b();
        if (pixel.x>=(currentImage.cols-1))
            return cv::Vec3b();
        if (pixel.y>=(currentImage.rows-1))
            return cv::Vec3b();

        return currentImage.at<cv::Vec3b>( pixel );
    }

    void Analysis::findRegion(const cv::Point& pixelCoords, const cv::Vec3b& color, const uchar tolerance) {
        lastResult.invalidate();
        if (currentImage.empty()) {
            return ;
        }

        lastResult = MaskC1( currentImage, color, tolerance );
        lastResult.floodFill(pixelCoords, 255, 127, 0);
        lastResult.changeColor( 127, 255 );
    }

    void Analysis::findPerimeter(const cv::Mat& regionsMask ) {
        if (currentImage.empty()) {
            lastResult.invalidate();
            return ;
        }
        if (regionsMask.empty()) {
            lastResult.invalidate();
            return ;
        }

        const int nRows = currentImage.rows;
        const int nCols = currentImage.cols;
        if (nRows != regionsMask.rows || nCols != regionsMask.cols) {
            lastResult.invalidate();
            return ;
        }

        lastResult = MaskC1(regionsMask);

        detectEdges();
        lastResult.threshold(128);
    }

    void Analysis::detectEdges() {
        cv::Mat filter = cv::Mat::zeros( 3, 3, CV_64F );

        /// Laplace filter
        filter.at<double>(0,0) = -1;
        filter.at<double>(0,1) = -1;
        filter.at<double>(0,2) = -1;

        filter.at<double>(1,0) = -1;
        filter.at<double>(1,1) =  8;
        filter.at<double>(1,2) = -1;

        filter.at<double>(2,0) = -1;
        filter.at<double>(2,1) = -1;
        filter.at<double>(2,2) = -1;

//        filter.at<double>(0,1) = -1;
//        filter.at<double>(1,0) = -1;
//        filter.at<double>(1,1) =  4;
//        filter.at<double>(1,2) = -1;
//        filter.at<double>(2,1) = -1;

        lastResult.applyFilter(filter);
    }

    void Analysis::findPerimeter() {
        const cv::Mat mask = *lastResult;           /// copy
        findPerimeter(mask);
    }

    void Analysis::findSmoothPerimeter(const cv::Mat& regionsMask) {
        if (currentImage.empty()) {
            lastResult.invalidate();
            return ;
        }
        if (regionsMask.empty()) {
            lastResult.invalidate();
            return ;
        }

        const int nRows = currentImage.rows;
        const int nCols = currentImage.cols;
        if (nRows != regionsMask.rows || nCols != regionsMask.cols) {
            lastResult.invalidate();
            return ;
        }

        lastResult = MaskC1(regionsMask);

        /// remove small artifacts
        lastResult.erode();
        lastResult.dilate();
        lastResult.dilate();
        lastResult.erode();

        cv::Mat filter = cv::Mat::zeros( 3, 3, CV_64F );

        /// Gausian blur
        filter.at<double>(0,0) = 1;
        filter.at<double>(0,1) = 2;
        filter.at<double>(0,2) = 1;
        filter.at<double>(1,0) = 2;
        filter.at<double>(1,1) = 4;
        filter.at<double>(1,2) = 2;
        filter.at<double>(2,0) = 1;
        filter.at<double>(2,1) = 2;
        filter.at<double>(2,2) = 1;
        filter /= 16;

        lastResult.applyFilter(filter);
        lastResult.threshold( 100 );

        detectEdges();
        lastResult.threshold(64);
    }

    void Analysis::findSmoothPerimeter() {
        const cv::Mat mask = *lastResult;       /// copy
        findSmoothPerimeter(mask);
    }

    static void show_mat(const cv::Mat &image, std::string const &win_name) {
        namedWindow(win_name, CV_WINDOW_NORMAL);
        imshow(win_name, image);
        waitKey(0);
    }

    void Analysis::displayImage() const {
        if (currentImage.empty()) {
            ///std::cout << "could not display image(empty)" << std::endl;
            return ;
        }
        show_mat(currentImage, "Input");
    }

    void Analysis::displayPixels() const {
        if (lastResult.empty()) {
            ///std::cout << "could not display image(empty)" << std::endl;
            return ;
        }
        show_mat(*lastResult, "Result");
    }

    void Analysis::displayJoin() const {
        if (lastResult.empty()) {
            ///std::cout << "could not display image(empty)" << std::endl;
            return ;
        }

        cv::Mat result;
        cvtColor(*lastResult, result, CV_GRAY2RGB);

        const Size size1 = currentImage.size();
        const Size size2 = result.size();
        cv::Mat joinImage(size1.height, size1.width+size2.width, CV_8UC3);
        cv::Mat left(joinImage, Rect(0, 0, size1.width, size1.height));
        currentImage.copyTo(left);
        cv::Mat right(joinImage, Rect(size1.width, 0, size2.width, size2.height));
        result.copyTo(right);

        show_mat(joinImage, "Result");
    }

    void Analysis::storeResult(const std::string& outputPath) const {
        storeMat(*lastResult, outputPath);
    }

    void Analysis::displayMat(const cv::Mat& matrix) {
        show_mat(matrix, "Matrix");
    }

    void Analysis::storeMat(const cv::Mat& matrix, const std::string& outputPath) {
        if (matrix.empty())
            return;
        imwrite(outputPath, matrix);
    }

} /* namespace ias */
