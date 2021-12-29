#ifndef SMATCH_H
#define SMATCH_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace sma {
	bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);
	double smatch(cv::Mat input, cv::Mat inputtempl);
	bool checkredtriangle(cv::Mat redimg);
	bool checkredcircle(cv::Mat redimg);
	bool checkredoctagon(cv::Mat redimg);
	bool checkblue(cv::Mat blueimg);
	bool checkyellow(cv::Mat yellowimg);
}

#endif