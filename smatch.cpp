#include "smatch.h"

using namespace cv;
using namespace std;

//k�t contour m�ret�nek �sszehasonl�t�sa
bool sma::compareContourAreas(std::vector<cv::Point> contour1, std::vector<cv::Point> contour2) {
	double i = fabs(contourArea(cv::Mat(contour1)));
	double j = fabs(contourArea(cv::Mat(contour2)));
	return (i > j);
}

//megvizsg�lja, hogy milyen m�rt�kben van a k�pen az adott template
double sma::smatch(Mat input, Mat inputtempl) {
	Mat templ;
	double bestmatch = 100;

	cvtColor(inputtempl, templ, COLOR_BGR2GRAY);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;

	findContours(templ, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE); //megkeresi a kont�rokat a template-ben
	std::sort(contours.begin(), contours.end(), compareContourAreas); //sorba rendezi a kont�rokat

	vector<Point> templatecontour = contours[0]; //kiv�lasztja a legnagyobb kont�rt a vizsg�lathoz

	findContours(input, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE); //megkeresi a kont�rokat a a k�pben

	//�sszehasonl�tja a template kont�rt a k�p �sszes kont�rj�val �s visszaadja a legjobb eredm�nyt
	for (int i = 0; i < contours.size(); i++) {
		if (matchShapes(templatecontour, contours[i], 1, 0.0) < bestmatch) {
			bestmatch = matchShapes(templatecontour, contours[i], 1, 0.0);
		}
	}
	return bestmatch;
}

//piros h�romsz�get keres �ldetekt�l�s �s dilat�ci� ut�n
bool sma::checkredtriangle(Mat redimg) {
	bool redtriangle = false;
	Mat edges;


	Canny(redimg, edges, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(edges, edges, kernel, Point(-1, -1), 3);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> approx;

	findContours(edges, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);

	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area > 500) { //kism�ret�, zajos alakzatok vizsg�lat�nak elker�l�se
			approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.03, true);
			if (approx.size() == 3) {
				redtriangle = true;
			}
		}
	}

	return redtriangle;
}

//piros k�rt keres �ldetekt�l�s �s dilat�ci� ut�n
bool sma::checkredcircle(Mat redimg) {
	bool redcircle = false;
	Mat edges;

	Canny(redimg, edges, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(edges, edges, kernel, Point(-1, -1), 3);

	vector<Vec3f> circles;

	HoughCircles(edges, circles, HOUGH_GRADIENT, 2, 5, 100, 180, 0, 1000);

	if (circles.size() > 0) {
		redcircle = true;
	}

	return redcircle;
}

//piros nyolcsz�get keres �ldetekt�l�s �s dilat�ci� ut�n
bool sma::checkredoctagon(Mat redimg) {
	bool redoctagon = false;
	Mat edges;

	Canny(redimg, edges, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(edges, edges, kernel, Point(-1, -1), 3);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> approx;

	findContours(edges, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area > 500) { //kism�ret�, zajos alakzatok vizsg�lat�nak elker�l�se
			approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
			if (approx.size() == 8) {
				redoctagon = true;
			}
		}
	}

	return redoctagon;
}

//k�k n�gysz�get keres �ldetekt�l�s �s dilat�ci� ut�n
bool sma::checkblue(Mat blueimg) {
	bool bluesign = false;
	Mat edges;

	Canny(blueimg, edges, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(edges, edges, kernel, Point(-1, -1), 3);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> approx;

	findContours(edges, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area > 500) { //kism�ret�, zajos alakzatok vizsg�lat�nak elker�l�se
			approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
			if (approx.size() == 4) {
				bluesign = true;
			}
		}
	}

	return bluesign;
}

//s�rga n�gysz�get keres �ldetekt�l�s �s dilat�ci� ut�n
bool sma::checkyellow(Mat yellowimg) {
	bool yellowsign = false;
	Mat edges = yellowimg;

	Canny(yellowimg, edges, 25, 75);
	Mat kernel = getStructuringElement(MORPH_RECT, Size(2, 2));
	dilate(edges, edges, kernel, Point(-1, -1), 3);

	vector<vector<Point> > contours;
	vector<Vec4i> hierarchy;
	vector<Point> approx;

	findContours(edges, contours, hierarchy, RETR_CCOMP, CHAIN_APPROX_SIMPLE);
	for (int i = 0; i < contours.size(); i++) {
		double area = contourArea(contours[i]);
		if (area > 500) { //kism�ret�, zajos alakzatok vizsg�lat�nak elker�l�se
			approxPolyDP(contours[i], approx, arcLength(contours[i], true) * 0.02, true);
			if (approx.size() == 4) {
				yellowsign = true;
			}
		}
	}

	return yellowsign;
}