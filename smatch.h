#ifndef SMATCH_H
#define SMATCH_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace sma {
	bool compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2);
	double smatch(cv::Mat input, cv::Mat inputtempl);
}

#endif