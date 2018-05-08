#include <iostream>
#include <string>
#include <sstream>
#include <opencv2/opencv.hpp>

using namespace std;

void depth2color(cv::Mat & color, const cv::Mat & depth, const double max, const double min)
{
	cv::Mat grayImage;
	double alpha = 255.0 / (max - min);
	depth.convertTo(grayImage, CV_8UC1, alpha, -alpha * min);// expand your range to 0..255. Similar to histEq();
	cv::applyColorMap(grayImage, color, cv::COLORMAP_JET);// this is great. It converts your grayscale image into a tone-mapped one, much more pleasing for the eye function is found in contrib module, so include contrib.hpp  and link accordingly
}

void getDepthMinMax(const cv::Mat & depthTmp, double &imax, double &imin) {
	int imrow = depthTmp.rows;
	int imcol = depthTmp.cols * depthTmp.channels();
	for (int i = 0; i < imrow; i++)
	{
		for (int j = 0; j < imcol; j++)
		{
			ushort data = depthTmp.at<ushort>(i, j);
			if (imin >= data && data != 0)
			{
				imin = data;
			}
			if (imax <= data)
			{
				imax = data;
			}

		}
	}
}

int main() {
	int count = 182;
	double imax = 0, imin = 70000;
	cv::Mat colorimg;
	char colorname[100], rgbname[100], depthname[100];

	string in_imgpath = "geely/05/";
	string out_imgpath = "geelypart/";

	for (int i = 0; i <= 142; ++i) {
		ostringstream ss;
		ss << i;
		cv::Mat depthimg = cv::imread(in_imgpath + "depth/depth"+ss.str()+".png", cv::IMREAD_UNCHANGED);
		cv::Mat rgbimg = cv::imread(in_imgpath + "rgb/rgb"+ss.str()+".png", cv::IMREAD_UNCHANGED);
		if (depthimg.empty() || rgbimg.empty()) {
			cout << "read img failed\n";
			system("pause");
			return -1;
		}
		sprintf_s(colorname, sizeof(colorname), (out_imgpath + "color/color%03d.png").c_str(), count);
		sprintf_s(rgbname, sizeof(rgbname), (out_imgpath + "rgb/rgb%03d.png").c_str(), count);
		sprintf_s(depthname, sizeof(depthname), (out_imgpath + "depth/depth%03d.png").c_str(), count++);

		getDepthMinMax(depthimg, imax, imin);
		depth2color(colorimg, depthimg, imax, imin);
		//cv::imshow("depth2color", colorimg);
		cv::imwrite(colorname, colorimg);
		cv::imwrite(rgbname, rgbimg);
		cv::imwrite(depthname, depthimg);
	}
	//cv::waitKey(0);
	cout << "depth2color finished!\n";
	cout << "the image count is " << count << endl;
	system("pause");
	return 0;
}