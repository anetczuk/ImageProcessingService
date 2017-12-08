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

#include <boost/test/unit_test.hpp>


using namespace ias;


BOOST_AUTO_TEST_SUITE( MaskC1Suite )

    BOOST_AUTO_TEST_CASE( applyFilter_empty_mask ) {
        MaskC1 mask;

        const cv::Mat filter = cv::Mat::ones( 3, 3, CV_64F );
        mask.applyFilter(filter);

        BOOST_CHECK_EQUAL( mask.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( applyFilter_empty_filter ) {
        MaskC1 mask(3, 3);
        mask.set(1,1, 255);

        const cv::Mat filter;
        mask.applyFilter(filter);

        BOOST_CHECK_EQUAL( mask.get(1,1), 255 );
    }

    BOOST_AUTO_TEST_CASE( applyFilter_3x3 ) {
        MaskC1 mask(3, 3);
        mask.set(1,1, 255);

        const cv::Mat filter = cv::Mat::ones( 3, 3, CV_64F ) * 0.2;
        mask.applyFilter(filter);

        BOOST_CHECK_EQUAL( mask.get(1,1), 51 );
    }

    BOOST_AUTO_TEST_CASE( threshold_empty ) {
        MaskC1 mask;

        mask.threshold(130);

        BOOST_CHECK_EQUAL( mask.empty(), true );
    }

    BOOST_AUTO_TEST_CASE( threshold_smaller ) {
        MaskC1 mask(3, 3);
        mask.set(1,1, 120);

        mask.threshold(130);

        BOOST_CHECK_EQUAL( mask.get(1,1), 0 );
    }

    BOOST_AUTO_TEST_CASE( threshold_greater ) {
        MaskC1 mask(3, 3);
        mask.set(1,1, 160);

        mask.threshold(130);

        BOOST_CHECK_EQUAL( mask.get(1,1), 255 );
    }

BOOST_AUTO_TEST_SUITE_END()
