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

#ifndef MASKC1_H_
#define MASKC1_H_

#include <opencv2/core/core.hpp>


namespace ias {

    /**
     * Class implementing basic operations on image, e.g. thresholding, filtering, changing colors etc.
     */
    class MaskC1 {
        cv::Mat mask;

    public:

        MaskC1(): mask() {
        }

        MaskC1(const int width, const int height): mask() {
            mask = cv::Mat::zeros( height, width, CV_8UC1 );
        }

        MaskC1(const int width, const int height, const uchar value): mask() {
            mask = cv::Mat::ones( height, width, CV_8UC1 ) * value;
        }

        MaskC1(const cv::Mat& matrix): mask(matrix) {
        }

        /// binarize RGB image
        MaskC1(const cv::Mat& image, const cv::Vec3b& color, const uchar tolerance);

        const cv::Mat& operator*() const {
            return mask;
        }

//        cv::Mat& operator*() {
//            return mask;
//        }

        bool empty() const {
            return mask.empty();
        }

        const cv::Mat& data() const {
            return mask;
        }

//        cv::Mat& data() {
//            return mask;
//        }

        void invalidate() {
            mask = cv::Mat();
        }

        uchar get(const int x, const int y) const {
            return mask.at<uchar>(y, x);
        }

        void set(const int x, const int y, const uchar val) {
            mask.at<uchar>(y, x) = val;
        }

        void changeColor(const uchar from, const uchar to);

        void floodFill(const cv::Point& startCoords, const uchar color, const uchar target, const uint zero);

        void applyFilter(const cv::Mat& filter);

        void threshold(const uchar thresh);

        void dilate(const int size = 3, const std::size_t repeats = 1);

        void erode(const int size = 3, const std::size_t repeats = 1);


    private:

        double apply(const cv::Mat& filter, const int y, const int x);

    };

} /* namespace ias */
#endif /* MASKC1_H_ */
