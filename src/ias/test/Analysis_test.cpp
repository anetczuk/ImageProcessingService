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

#include <boost/test/unit_test.hpp>


using namespace ias;


BOOST_AUTO_TEST_SUITE( AnalysisSuite )

    BOOST_AUTO_TEST_CASE( loadImage_not_found ) {
        Analysis object;

        const bool loaded = object.loadImage("not_found.png");
        BOOST_CHECK_EQUAL( loaded, false );
    }

    BOOST_AUTO_TEST_CASE( loadImage_valid ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_CHECK_EQUAL( loaded, true );
    }

    BOOST_AUTO_TEST_CASE( loadImage_corrupt ) {
        Analysis object;

        const bool loaded = object.loadImage("data/yesthisfileiscorrupt.png");
        BOOST_CHECK_EQUAL( loaded, false );
    }


    BOOST_AUTO_TEST_CASE( color_invalid ) {
        Analysis object;

        const cv::Vec3b color = object.color( 0, 0 );
        BOOST_CHECK_EQUAL( color, cv::Vec3b(0, 0, 0) );
    }

    BOOST_AUTO_TEST_CASE( color_valid ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Vec3b color1 = object.color( 45, 0 );
        BOOST_CHECK_EQUAL( color1, cv::Vec3b(7, 37, 249) );     /// red

        const cv::Vec3b color2 = object.color( 0, 45 );
        BOOST_CHECK_EQUAL( color2, cv::Vec3b(37, 249, 5) );     /// green
    }

    BOOST_AUTO_TEST_CASE( color_valid2 ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Vec3b color1 = object.color( cv::Point(0, 45) );
        BOOST_CHECK_EQUAL( color1, cv::Vec3b(7, 37, 249) );             /// red

        const cv::Vec3b color2 = object.color( cv::Point(45, 0) );
        BOOST_CHECK_EQUAL( color2, cv::Vec3b(37, 249, 5) );             /// green
    }


    BOOST_AUTO_TEST_CASE( findRegion_invalid ) {
        Analysis object;

        object.findRegion( cv::Point(0, 0), cv::Vec3b(255, 0, 0) );
        const cv::Mat& region = object.result();
        BOOST_CHECK_EQUAL( region.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findRegion_valid ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Point point(0, 30);
        const cv::Vec3b color(7, 37, 249);
        object.findRegion( point, color, 20 );

        const cv::Mat& region = object.result();
        BOOST_REQUIRE_EQUAL( region.empty(), false );
        BOOST_CHECK_EQUAL( region.channels(), 1 );
        BOOST_CHECK_EQUAL( region.rows, object.image().rows );
        BOOST_CHECK_EQUAL( region.cols, object.image().cols );

        BOOST_CHECK_EQUAL( region.at<uchar>(0, 0), 0 );          /// green RGB(5, 249, 37) background
        BOOST_CHECK_EQUAL( region.at<uchar>(40, 10), 255 );      /// first red RGB(249, 37, 7) rectangle
        BOOST_CHECK_EQUAL( region.at<uchar>(50, 100), 0 );       /// second red rectangle
    }

    BOOST_AUTO_TEST_CASE( findRegion_valid2 ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Point point(100, 50);
        const cv::Vec3b color(7, 37, 249);
        object.findRegion( point, color, 20 );

        const cv::Mat& region = object.result();
        BOOST_REQUIRE_EQUAL( region.empty(), false );
        BOOST_CHECK_EQUAL( region.channels(), 1 );
        BOOST_CHECK_EQUAL( region.rows, object.image().rows );
        BOOST_CHECK_EQUAL( region.cols, object.image().cols );

        BOOST_CHECK_EQUAL( region.at<uchar>(0, 0), 0 );          /// green RGB(5, 249, 37) background
        BOOST_CHECK_EQUAL( region.at<uchar>(40, 10), 0 );        /// first red RGB(249, 37, 7) rectangle
        BOOST_CHECK_EQUAL( region.at<uchar>(50, 100), 255 );     /// second red rectangle
    }


    BOOST_AUTO_TEST_CASE( findPerimeter_invalid_image ) {
        Analysis object;

        const cv::Mat region;
        object.findPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_CHECK_EQUAL( perimeter.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findPerimeter_empty_region ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Mat region;
        object.findPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_CHECK_EQUAL( perimeter.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findPerimeter_region_bad_size ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Mat region( 10, 10, CV_8UC1 );
        object.findPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_CHECK_EQUAL( perimeter.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findPerimeter_valid ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        object.findRegion( cv::Point(0, 30), cv::Vec3b(7, 37, 249), 20 );
        const cv::Mat& region = object.result();
        object.findPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_REQUIRE_EQUAL( perimeter.empty(), false );
        BOOST_CHECK_EQUAL( perimeter.channels(), 1 );
        BOOST_CHECK_EQUAL( perimeter.rows, object.image().rows );
        BOOST_CHECK_EQUAL( perimeter.cols, object.image().cols );

        BOOST_CHECK_EQUAL( perimeter.at<uchar>(0, 0), 0 );         /// green RGB(5, 249, 37) background
        BOOST_CHECK_EQUAL( perimeter.at<uchar>(30, 0), 255 );      /// red RGB(249, 37, 7) rectangle
    }

    BOOST_AUTO_TEST_CASE( findPerimeter_valid2 ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Point point(100, 50);
        const cv::Vec3b color(7, 37, 249);
        object.findRegion( point, color, 20 );
        const cv::Mat& region = object.result();
        object.findPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_REQUIRE_EQUAL( perimeter.empty(), false );
        BOOST_CHECK_EQUAL( perimeter.channels(), 1 );
        BOOST_CHECK_EQUAL( perimeter.rows, object.image().rows );
        BOOST_CHECK_EQUAL( perimeter.cols, object.image().cols );

        BOOST_CHECK_EQUAL( perimeter.at<uchar>(0, 0), 0 );         /// green RGB(5, 249, 37) background
        BOOST_CHECK_EQUAL( perimeter.at<uchar>(30, 0), 0 );        /// red RGB(249, 37, 7) rectangle
    }


    BOOST_AUTO_TEST_CASE( findSmoothPerimeter_invalid_image ) {
        Analysis object;

        const cv::Mat region;
        object.findSmoothPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_CHECK_EQUAL( perimeter.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findSmoothPerimeter_empty_region ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Mat region;
        object.findSmoothPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_CHECK_EQUAL( perimeter.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findSmoothPerimeter_region_bad_size ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        const cv::Mat region( 10, 10, CV_8UC1 );
        object.findSmoothPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_CHECK_EQUAL( perimeter.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( findSmoothPerimeter_valid ) {
        Analysis object;

        const bool loaded = object.loadImage("data/test1.png");
        BOOST_REQUIRE_EQUAL( loaded, true );

        object.findRegion( cv::Point(0, 30), cv::Vec3b(7, 37, 249), 20 );
        const cv::Mat& region = object.result();
        object.findPerimeter( region );

        const cv::Mat& perimeter = object.result();
        BOOST_REQUIRE_EQUAL( perimeter.empty(), false );
//        BOOST_CHECK_EQUAL( perimeter.channels(), 1 );
//        BOOST_CHECK_EQUAL( perimeter.rows, object.image().rows );
//        BOOST_CHECK_EQUAL( perimeter.cols, object.image().cols );
//
//        BOOST_CHECK_EQUAL( perimeter.at<uchar>(0, 0), 0 );         /// green RGB(5, 249, 37) background
//        BOOST_CHECK_EQUAL( perimeter.at<uchar>(30, 0), 255 );      /// red RGB(249, 37, 7) rectangle
    }

BOOST_AUTO_TEST_SUITE_END()
