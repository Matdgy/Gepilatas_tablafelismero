#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

#include "preprocess.h"
#include "smatch.h"

using namespace std;
using namespace cv;

struct matchresult {
	double stop;
	double betilos;
	double autopalya;
	double fout;
	double elskot;
	double parkolo;
};

string findbestmatch(matchresult m) {
	double best = 100;
	string besttempl;

	if (m.stop < best) {
		best = m.stop;
		besttempl = "STOP";
	}

	if (m.betilos < best) {
		best = m.betilos;
		besttempl = "BEHAJTANI TILOS";
	}

	if (m.autopalya < best) {
		best = m.autopalya;
		besttempl = "AUTÓPÁLYA";
	}

	if (m.fout < best) {
		best = m.fout;
		besttempl = "FÕÚTVONAL";
	}

	if (m.elskot < best) {
		best = m.elskot;
		besttempl = "ELSÖBBSÉGADÁS KÖTELEZÕ";
	}

	if (m.parkolo < best) {
		best = m.parkolo;
		besttempl = "PARKOLÓ";
	}

	return besttempl;
}

int main() {
	matchresult match[13];
	ofstream file("eredmeny.txt");

	string path = "kepek/";

	Mat stop = imread("kepek/template/stop.png");
	Mat betilos = imread("kepek/template/betilos.png");
	Mat autopalya = imread("kepek/template/autopalya.png");
	Mat fout = imread("kepek/template/fout.png");
	Mat elskot = imread("kepek/template/elskot.png");
	Mat parkolo = imread("kepek/template/parkolo.png");

	Mat img, imgred, imgblue, imgyellow;

	for (int i = 0; i < 13; i++) {

		path += to_string(i);
		path += ".png";

		cout << path;
		img = imread(path);
		imgred = pp::red(img);
		imgblue = pp::blue(img);
		imgyellow = pp::yellow(img);

		match[i].stop = sma::smatch(imgred, stop);
		match[i].betilos = sma::smatch(imgred, betilos);
		match[i].autopalya = sma::smatch(imgblue, autopalya);
		match[i].fout = sma::smatch(imgyellow, fout);
		match[i].elskot = sma::smatch(imgred, elskot);
		match[i].parkolo = sma::smatch(imgblue, parkolo);

		imshow(path, img);
		waitKey(0);
		imshow(path, imgred);
		waitKey(0);
		imshow(path, imgblue);
		waitKey(0);
		imshow(path, imgyellow);
		waitKey(0);

		path = "kepek/";

		file << "--" << i << "--" << endl;
		file << "stop: " << match[i].stop << endl;
		file << "behajtani tilos: " << match[i].betilos << endl;
		file << "autopalya: " << match[i].autopalya << endl;
		file << "fõútvonal: " << match[i].fout << endl;
		file << "elsöbbségadás kötelezõ: " << match[i].elskot << endl;
		file << "parkoló: " << match[i].parkolo << endl;
		file << endl;
		file << "--> " << findbestmatch(match[i]) << endl;
		file << endl;
	}

	file.close();

}