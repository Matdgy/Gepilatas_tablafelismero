#include "preprocess.h"

using namespace std;
using namespace cv;

//trackbart hoz létre a színszûrés megfelelõ határértékeinek megtalálásához
void pp::trackbar(Mat input) {
	Mat mask, inputHSV;
	cvtColor(input, inputHSV, COLOR_BGR2HSV);
	int hmin = 0, smin = 0, vmin = 0;
	int hmax = 0, smax = 0, vmax = 0;

	namedWindow("Trackbars", (640, 200));

	createTrackbar("hmin", "Trackbars", &hmin, 179);
	createTrackbar("smin", "Trackbars", &smin, 255);
	createTrackbar("vmin", "Trackbars", &vmin, 255);

	createTrackbar("hmax", "Trackbars", &hmax, 179);
	createTrackbar("smax", "Trackbars", &smax, 255);
	createTrackbar("vmax", "Trackbars", &vmax, 255);

	while (true) {
		inRange(inputHSV, Scalar(hmin, smin, vmin), Scalar(hmax, smax, vmax), mask);
		bitwise_not(mask, mask);

		imshow("Mask", mask);
		waitKey(1);
	}
}

//piros színre szûr
Mat pp::red(Mat input) {
	Mat mask1, mask2, inputHSV;
	cvtColor(input, inputHSV, COLOR_BGR2HSV);
	//GaussianBlur(inputHSV, inputHSV, Size(5, 5), 0);

	inRange(inputHSV, Scalar(0, 170, 70), Scalar(10, 255, 255), mask1);
	inRange(inputHSV, Scalar(170, 170, 70), Scalar(180, 255, 255), mask2);

	Mat1b mask = mask1 | mask2;
	bitwise_not(mask, mask);

	//Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	//erode(mask, mask, kernel, Point(-1,-1), 3);
	return mask;
}

//kék színre szûr
Mat pp::blue(Mat input) {
	Mat mask, inputHSV;
	cvtColor(input, inputHSV, COLOR_BGR2HSV);
	//GaussianBlur(inputHSV, inputHSV, Size(5, 5), 0);

	inRange(inputHSV, Scalar(100, 170, 70), Scalar(120, 255, 255), mask);

	bitwise_not(mask, mask);

	//Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	//erode(mask, mask, kernel, Point(-1, -1), 3);
	return mask;
}

//sárga színre szûr
Mat pp::yellow(Mat input) {
	Mat mask, inputHSV;
	cvtColor(input, inputHSV, COLOR_BGR2HSV);
	//GaussianBlur(inputHSV, inputHSV, Size(5, 5), 0);

	inRange(inputHSV, Scalar(20, 170, 70), Scalar(30, 255, 255), mask);

	bitwise_not(mask, mask);

	//Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	//erode(mask, mask, kernel, Point(-1, -1), 3);
	return mask;
}