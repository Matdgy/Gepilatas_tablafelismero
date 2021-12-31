#include <opencv2/imgcodecs.hpp>
#include <opencv2/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <iostream>
#include <fstream>

#include "preprocess.h"
#include "smatch.h"
//includeok

using namespace std;
using namespace cv;

//egy structba t�roljuk az aktu�lis k�p match eredm�nyeit
struct matchresult { 
	double stop = 100;
	double betilos = 100;
	double autopalya = 100;
	double fout = 100;
	double elskot = 100;
	double parkolo = 100;
};

//megkeresi a legjobb eredm�nyt a structb�l �s el��ll�t egy stringet az alapj�n
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
		besttempl = "AUT�P�LYA";
	}

	if (m.fout < best) {
		best = m.fout;
		besttempl = "F��TVONAL";
	}

	if (m.elskot < best) {
		best = m.elskot;
		besttempl = "ELS�BBS�GAD�S K�TELEZ�";
	}

	if (m.parkolo < best) {
		best = m.parkolo;
		besttempl = "PARKOL�";
	}

	return besttempl;
}

int main() {
	matchresult match; //aktu�lis k�p match eredm�nyei
	ofstream file("eredmeny.txt"); //f�jl amibe ki�rjuk az eredm�nyeket

	string path = "kepek/"; //aktu�lis k�p helye

	//a templateek a match-hez
	Mat stop = imread("kepek/template/stop.png");
	Mat betilos = imread("kepek/template/betilos.png");
	Mat autopalya = imread("kepek/template/autopalya.png");
	Mat fout = imread("kepek/template/fout.png");
	Mat elskot = imread("kepek/template/elskot.png");
	Mat parkolo = imread("kepek/template/parkolo.png");

	Mat img, imgred, imgblue, imgyellow; //aktu�lis k�p �s az azon v�grehajtott sz�n sz�r�s eredm�nyeinek t�rol�s�ra
	int iter;
	cout << "Gepilatas beadando feladat" << endl << "Mathe David Gyorgy" << endl << "R4PBVN" << endl << "Number of pictures: ";
	cin >> iter;

	for (int i = 0; i < iter; i++) {

		//megfelel� string el��ll�t�sa k�p beolvas�s�hoz
		path += to_string(i);
		path += ".png";

		//k�p beolvas�sa �s sz�nre sz�r�s
		img = imread(path);

		//�tm�retez�s, ha t�l kicsi a k�p
		while (img.rows < 1000 && img.cols < 1000) {
			resize(img, img, Size(img.cols * 1.5, img.rows * 1.5));
		}

		//sz�nre sz�r�s
		imgred = pp::red(img);
		imgblue = pp::blue(img);
		imgyellow = pp::yellow(img);

		//megvizsg�ljuk, hogy vannak-e keresett alakzatok a k�pen
		bool hasredtriangle = sma::checkredtriangle(imgred);
		bool hasredcircle = sma::checkredcircle(imgred);
		bool hasredoctagon = sma::checkredoctagon(imgred);
		bool hasbluesign = sma::checkblue(imgblue);
		bool hasyellowsign = sma::checkyellow(imgyellow);

		/* imshow("eredeti", img);
		waitKey(0);
		imshow("piros", imgred);
		waitKey(0);
		imshow("k�k", imgblue);
		waitKey(0);
		imshow("s�rga", imgyellow);
		waitKey(0); */

		file << "--" << i << "--" << endl;

		//ha van keresett alakzat akkor v�grehajtjuk a match algoritmust �s ki�rjuk az eredm�nyt f�jlba
		if (hasredtriangle) {
			match.elskot = sma::smatch(imgred, elskot);

			file << "els�bbs�gad�s k�telez�: " << match.elskot << endl;
		}

		if (hasredcircle) {
			match.betilos = sma::smatch(imgred, betilos);

			file << "behajtani tilos: " << match.betilos << endl;
		}

		if (hasredoctagon) {
			match.stop = sma::smatch(imgred, stop);

			file << "stop: " << match.stop << endl;
		}
		
		if (hasbluesign) {
			match.autopalya = sma::smatch(imgblue, autopalya);
			match.parkolo = sma::smatch(imgblue, parkolo);

			file << "autopalya: " << match.autopalya << endl;
			file << "parkol�: " << match.parkolo << endl;
		}
		
		if (hasyellowsign) {
			match.fout = sma::smatch(imgyellow, fout);

			file << "f��tvonal: " << match.fout << endl;
		}
		
		//ha nincs keresett alakzat a k�pen akkor nincs t�bla a k�pen
		if (!hasredtriangle && !hasredcircle && !hasredoctagon && !hasbluesign && !hasyellowsign) {
			file << "--> NINCS T�BLA A K�PEN" << endl;
		}
		//ha van alakzat, akkor a t�rolt eredm�nyek k�z�l megkeress�k a legjobban illeszked�t �s azt �rjuk be a f�jlba eredm�nyk�nt
		else {
			file << "--> " << findbestmatch(match) << endl;
		}
		file << endl;

		//k�vetkez� iter�ci� el�k�sz�t�se
		path = "kepek/";
		match.stop = 100;
		match.betilos = 100;
		match.autopalya = 100;
		match.fout = 100;
		match.elskot = 100;
		match.parkolo = 100;

	}

	file.close();

}