#include "smatch.h"

using namespace cv;
using namespace std;

bool sma::compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
	double i = fabs(contourArea(cv::Mat(contour1)));
	double j = fabs(contourArea(cv::Mat(contour2)));
	return (i > j);
}

double sma::smatch(Mat input, Mat inputtempl) {
	Mat templ;
	double bestmatch = 100;

	cvtColor(inputtempl, templ, COLOR_BGR2GRAY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(templ, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	std::sort(contours.begin(), contours.end(), compareContourAreas);

	vector<Point> templatecontour = contours[0];

	findContours(input, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		if (matchShapes(templatecontour, contours[i], 1, 0.0) < bestmatch) {
			bestmatch = matchShapes(templatecontour, contours[i], 1, 0.0);
		}
	}
	return bestmatch;
}