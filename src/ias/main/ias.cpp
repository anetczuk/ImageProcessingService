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

#include <sstream>

#include <boost/algorithm/string.hpp>
#include <boost/log/core.hpp>
#include <boost/log/trivial.hpp>
#include <boost/log/utility/setup/file.hpp>

#include "ias/Analysis.h"


static bool findFlag(int argc, char **argv, const std::string& flag) {
    for(int i=1; i<argc; ++i) {
        const std::string param = argv[i];
        if (param.compare( flag ) == 0)
            return true;
    }
    return false;
}


class RegionParams {
public:

    bool valid;

    cv::Point pixelCoords;
    cv::Vec3b color;
    uchar equalityMargin;


private:

    std::istringstream iss;


public:

    RegionParams(const std::string& input): valid(false), pixelCoords(), color(), equalityMargin(), iss(input) {
        const int x = read<int>(); readSeparator();
        const int y = read<int>(); readSeparator();
        pixelCoords = cv::Point(x, y);

        const int b = read<int>(); readSeparator();
        const int g = read<int>(); readSeparator();
        const int r = read<int>(); readSeparator();
        color = cv::Vec3b(b, g, r);

        equalityMargin = read<int>();

        valid = true;
    }


private:

    template<typename Type>
    Type read() {
        Type value;
        iss >> value;
        return value;
    }

    void readSeparator() {
        read<char>();
    }

};


static int handleParam(ias::Analysis& object, const std::string& option) {
    std::vector<std::string> words;
    boost::split(words, option, boost::is_any_of("="));

    if (words.empty()) {
        BOOST_LOG_TRIVIAL(error) << "bad argument: " << option;
        return 1;
    }

    const std::string& param = words[0];
    if ( param.compare("--image") == 0 ) {
        const std::string& path = words[1];
        BOOST_LOG_TRIVIAL(info) << "loading image: " << path;
        const bool valid = object.loadImage(path);
        if (valid == false) {
            BOOST_LOG_TRIVIAL(error) << "unable to load file: " << path;
            return 1;
        }

    } else if ( param.compare("--findRegion") == 0 ) {
        const std::string& input = words[1];
        RegionParams regionParams(input);
        if (regionParams.valid == false) {
            BOOST_LOG_TRIVIAL(error) << "unable to parse: " << option;
            return 1;
        }
        BOOST_LOG_TRIVIAL(info) << "calculating region: " << input;
        object.findRegion( regionParams.pixelCoords, regionParams.color, regionParams.equalityMargin );
        return 0;

    } else if ( param.compare("--findPerimeter") == 0 ) {
        BOOST_LOG_TRIVIAL(info) << "calculating perimeter";
        object.findPerimeter();
        return 0;

    } else if ( param.compare("--findSmoothPerimeter") == 0 ) {
        BOOST_LOG_TRIVIAL(info) << "calculating smooth perimeter";
        object.findSmoothPerimeter();
        return 0;

    } else if ( param.compare("--displayImage") == 0 ) {
        BOOST_LOG_TRIVIAL(info) << "displaying image";
        object.displayImage();

    } else if ( param.compare("--displayPixels") == 0 ) {
        BOOST_LOG_TRIVIAL(info) << "displaying result";
        object.displayPixels();

    } else if ( param.compare("--displayJoin") == 0 ) {
        BOOST_LOG_TRIVIAL(info) << "displaying image and result on one window";
        object.displayJoin();

    } else if ( param.compare("--savePixels") == 0 ) {
        const std::string& path = words[1];
        BOOST_LOG_TRIVIAL(info) << "saving result to file: " << path;
        object.storeResult(path);
    }

    return 0;
}


int main(int argc, char **argv) {
    if (findFlag(argc, argv, "--logcout") == false) {
        boost::log::add_file_log("logger.log");
    }

    if (argc < 2) {
        BOOST_LOG_TRIVIAL(error) << "no parameres given";
        return 1;
    }

    if (findFlag(argc, argv, "--help")) {
        std::cout << "Options:" << std::endl;
        std::cout << "  --help                          Help screen" << std::endl;
        std::cout << "  --logcout                       Output to console" << std::endl;
        std::cout << "  --image=[path]                  Open image from file 'path'" << std::endl;
        std::cout << "  --findRegion=[pX,pY,B,G,R,T]    Calculate region of region calculated by --findRegion command where:" << std::endl;
        std::cout << "                                  -- pX,pY are coordinates of pixel on loaded image" << std::endl;
        std::cout << "                                  -- B,G,R are components of color to find" << std::endl;
        std::cout << "                                  -- T      is tolerance of color" << std::endl;
        std::cout << "  --findPerimeter                 Calculate perimeter of region calculated by --findRegion command" << std::endl;
        std::cout << "  --displayImage                  Display opened image" << std::endl;
        std::cout << "  --displayPixels                 Display result of find* command" << std::endl;
        std::cout << "  --savePixels=[path]             Save result of find* command to file 'path'" << std::endl;
        return 0;
    }

    ias::Analysis object;

    for(int i=1; i<argc; ++i) {
        const std::string param = argv[i];
        const int ret = handleParam(object, param);
        if (ret != 0)
            return ret;
    }

    return 0;
}
