#ifndef PREPROCESS_H
#define PREPROCESS_H

#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>

namespace pp {
	void trackbar(cv::Mat input);
	cv::Mat red(cv::Mat input);
	cv::Mat blue(cv::Mat input);
	cv::Mat yellow(cv::Mat input);
}

#endif