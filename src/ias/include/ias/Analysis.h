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

#ifndef ANALYSIS_H_
#define ANALYSIS_H_

#include <string>

#include "ias/MaskC1.h"


namespace ias {

    /**
     *
     */
    class Analysis {

        cv::Mat currentImage;
        MaskC1 lastResult;


    public:

        Analysis();

        virtual ~Analysis();

        const cv::Mat& image() const {
            return currentImage;
        }

        const cv::Mat& result() const {
            return lastResult.data();
        }

        bool loadImage(const std::string& imagePath);

        cv::Vec3b color(const int y, const int x ) const;

        cv::Vec3b color(const cv::Point& pixel) const;

        /**
         * Get mask representing found region.
         * "color" in BGR format
         * Returns empty mask if failed, otherwise single channel mask in size of image.
         */
        void findRegion(const cv::Point& pixelCoords, const cv::Vec3b& color, const uchar tolerance = 0);

        /**
         * Get mask representing found perimeter(s).
         * Returns empty mask if failed, otherwise single channel mask in size of image.
         */
        void findPerimeter(const cv::Mat& regionsMask);

        void findPerimeter();

        void findSmoothPerimeter(const cv::Mat& regionsMask);

        void findSmoothPerimeter();

        void displayImage() const;

        void displayPixels() const;

        void displayJoin() const;

        void storeResult(const std::string& outputPath) const;


    private:

        void detectEdges();


    public:

        static void displayMat(const cv::Mat& matrix);

        static void storeMat(const cv::Mat& matrix, const std::string& outputPath);

    };

} /* namespace ias */
#endif /* ANALYSIS_H_ */
