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

#include "ias/MaskC1.h"


using namespace cv;


namespace ias {

    /// compare each color component separately
    inline bool isColorSame(const cv::Vec3b& color, const Vec3b& pixel, const uchar tolerance ) {
        for (int i = 0; i < 3; ++i) {
            const uchar diff = std::abs( color(i) - pixel(i) );
            if (diff > tolerance)
                return false;
        }
        return true;
    }

    MaskC1::MaskC1(const cv::Mat& image, const cv::Vec3b& color, const uchar tolerance): mask() {
        mask = cv::Mat::zeros( image.rows, image.cols, CV_8UC1 );

        const int nRows = image.rows;
        const int nCols = image.cols;
        for (int y = 0; y < nRows; ++y) {
            const Vec3b* inrow = image.ptr<Vec3b>(y);
            uchar* outrow = mask.ptr<uchar>(y);
            for (int x = 0; x < nCols; ++x) {
                if (isColorSame(color, inrow[x], tolerance)) {
                    outrow[x] = 255;
                }
            }
        }
    }

    void MaskC1::changeColor(const uchar from, const uchar to) {
        const int nRows = mask.rows;
        const int nCols = mask.cols;
        for (int y = 0; y < nRows; ++y) {
            for (int x = 0; x < nCols; ++x) {
                uchar& pixel = mask.at<uchar>(y, x);
                if (pixel == from) {
                    pixel = to;
                }
            }
        }
    }

    static inline bool fillColor(cv::Mat& image, const cv::Point& pixel, const uchar color, const uchar target, const uint zero) {
        uchar& currColor = image.at<uchar>( pixel );
        if ( currColor == target ) {
            /// already changed
            return false;
        } else if ( currColor == color ) {
            currColor = target;
            return true;
        } else {
            currColor = zero;
            return false;
        }
    }

    void MaskC1::floodFill(const cv::Point& startCoords, const uchar color, const uchar target, const uint zero) {
        if (color == target) {
            return;
        }

        const int nRows = mask.rows;
        const int nCols = mask.cols;

        std::vector<cv::Point> queue;
        queue.push_back( startCoords );
        while( !queue.empty() ) {
            const cv::Point node = queue.back();
            queue.pop_back();

            /// going west
            for( int x=node.x-1; x>=0; --x ) {
                const cv::Point pixel( x, node.y );
                if( fillColor(mask, pixel, color, target, zero) ) {
                    if (node.y > 0)
                        queue.push_back( cv::Point(x, node.y-1) );
                    if (node.y < (nRows-1) )
                        queue.push_back( cv::Point(x, node.y+1) );
                } else {
                    break;
                }
            }

            /// going east
            for( int x=node.x; x<nCols; ++x ) {
                const cv::Point pixel( x, node.y );
                if( fillColor(mask, pixel, color, target, zero) ) {
                    if (node.y > 0)
                        queue.push_back( cv::Point(x, node.y-1) );
                    if (node.y < (nRows-1) )
                        queue.push_back( cv::Point(x, node.y+1) );
                } else {
                    break;
                }
            }
        }

        changeColor(color, zero);
    }

    void MaskC1::applyFilter(const cv::Mat& filter) {
        if (mask.empty()) {
            return ;
        }
        if (filter.empty()) {
            return ;
        }

        const int nRows = mask.rows;
        const int nCols = mask.cols;

        cv::Mat result = cv::Mat::zeros( nRows, nCols, CV_8UC1 );

        for (int y = 0; y < nRows; ++y) {
            for (int x = 0; x < nCols; ++x) {
                const double sum = apply(filter, y, x);
                if (sum < 0)
                    result.at<uchar>( y, x ) = 0;
                else if (sum > 255)
                    result.at<uchar>( y, x ) = 255;
                else
                    result.at<uchar>( y, x ) = sum;
            }
        }

        mask = result;
    }

    void MaskC1::threshold(const uchar thresh) {
        const int nRows = mask.rows;
        const int nCols = mask.cols;
        for (int y = 0; y < nRows; ++y) {
            for (int x = 0; x < nCols; ++x) {
                uchar& value = mask.at<uchar>( y, x );
                if (value < thresh) {
                    value = 0;
                } else {
                    value = 255;
                }
            }
        }
    }

    void MaskC1::dilate(const int size, const std::size_t repeats) {
        const cv::Mat filter = cv::Mat::ones( size, size, CV_64F );
        for(std::size_t i=0; i<repeats; ++i) {
            applyFilter(filter);
        }
    }

    void MaskC1::erode(const int size, const std::size_t repeats) {
        const cv::Mat filter = cv::Mat::ones( size, size, CV_64F ) / 9;
        for(std::size_t i=0; i<repeats; ++i) {
            applyFilter(filter);
            threshold(254);
        }
    }

    double MaskC1::apply(const cv::Mat& filter, const int y, const int x) {
        const int fRows = filter.rows;
        const int fCols = filter.cols;

        const int fxm = filter.cols / 2;
        const int fym = filter.rows / 2;

        double sum = 0.0;

        for (int fy = 0; fy < fRows; ++fy) {
            const int my = y + (fy-fym);
            if (my<0)
                continue;
            else if (my>=mask.rows)
                continue;

            for (int fx = 0; fx < fCols; ++fx) {
                const int mx = x + (fx-fxm);
                if (mx<0)
                    continue;
                else if (mx>=mask.cols)
                    continue;

                sum += mask.at<uchar>(my, mx) * filter.at<double>(fy, fx);
            }
        }

        return sum;
    }

} /* namespace ias */
