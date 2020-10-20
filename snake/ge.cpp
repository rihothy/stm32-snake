#include <opencv.hpp>
#include <iostream>
#include <fstream>
#include <cstdint>
#include <vector>
#include <map>

using namespace std;

auto rgb888_2_rgb565 = [](auto r, auto g, auto b)
{
    uint16_t ans = 0;
	
    ans = (ans + (r >> 3)) << 6;
    ans = (ans + (g >> 2)) << 5;
    ans = ans + (b >> 3);

    return ans;
};

int main(void)
{
    auto img = cv::imread("node.png");

    vector<cv::Mat> rgb;
    cv::split(img, rgb);

    vector<uint16_t> data;
    data.push_back(img.rows);
    data.push_back(img.cols);

    for (int i = 0; i < img.rows; ++i)
    {
        for (int j = 0; j < img.cols; ++j)
        {
            auto r = rgb[2].ptr<uchar>(i)[j];
            auto g = rgb[1].ptr<uchar>(i)[j];
            auto b = rgb[0].ptr<uchar>(i)[j];

            data.push_back(rgb888_2_rgb565(r, g, b));
        }
    }

    ofstream ostrm("images.h");

    ostrm << "const uint16_t node_img[] = { ";

    int i = 0;
    bool first = false;

    for (auto& ele : data)
    {
        if (i && i % 2 == 0 && not first)
        {
            first = true;
            ostrm << endl << "    ";
        }
        else if (i && (i - 2) % img.cols == 0)
        {
            ostrm << endl << "    ";
        }

        ostrm << "0x";
        ostrm << hex;
        ostrm.width(2);
        ostrm.fill('0');
        ostrm << ele;
        ostrm << ", ";

        ++i;
    }

    ostrm << endl << "};" << endl;

    ostrm.close();

    return 0;
}