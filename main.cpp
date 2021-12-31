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

//egy structba tároljuk az aktuális kép match eredményeit
struct matchresult { 
	double stop = 100;
	double betilos = 100;
	double autopalya = 100;
	double fout = 100;
	double elskot = 100;
	double parkolo = 100;
};

//megkeresi a legjobb eredményt a structból és elõállít egy stringet az alapján
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
		besttempl = "ELSÕBBSÉGADÁS KÖTELEZÕ";
	}

	if (m.parkolo < best) {
		best = m.parkolo;
		besttempl = "PARKOLÓ";
	}

	return besttempl;
}

int main() {
	matchresult match; //aktuális kép match eredményei
	ofstream file("eredmeny.txt"); //fájl amibe kiírjuk az eredményeket

	string path = "kepek/"; //aktuális kép helye

	//a templateek a match-hez
	Mat stop = imread("kepek/template/stop.png");
	Mat betilos = imread("kepek/template/betilos.png");
	Mat autopalya = imread("kepek/template/autopalya.png");
	Mat fout = imread("kepek/template/fout.png");
	Mat elskot = imread("kepek/template/elskot.png");
	Mat parkolo = imread("kepek/template/parkolo.png");

	Mat img, imgred, imgblue, imgyellow; //aktuális kép és az azon végrehajtott szín szûrés eredményeinek tárolására
	int iter;
	cout << "Gepilatas beadando feladat" << endl << "Mathe David Gyorgy" << endl << "R4PBVN" << endl << "Number of pictures: ";
	cin >> iter;

	for (int i = 0; i < iter; i++) {

		//megfelelõ string elõállítása kép beolvasásához
		path += to_string(i);
		path += ".png";

		//kép beolvasása és színre szûrés
		img = imread(path);

		//átméretezés, ha túl kicsi a kép
		while (img.rows < 1000 && img.cols < 1000) {
			resize(img, img, Size(img.cols * 1.5, img.rows * 1.5));
		}

		//színre szûrés
		imgred = pp::red(img);
		imgblue = pp::blue(img);
		imgyellow = pp::yellow(img);

		//megvizsgáljuk, hogy vannak-e keresett alakzatok a képen
		bool hasredtriangle = sma::checkredtriangle(imgred);
		bool hasredcircle = sma::checkredcircle(imgred);
		bool hasredoctagon = sma::checkredoctagon(imgred);
		bool hasbluesign = sma::checkblue(imgblue);
		bool hasyellowsign = sma::checkyellow(imgyellow);

		/* imshow("eredeti", img);
		waitKey(0);
		imshow("piros", imgred);
		waitKey(0);
		imshow("kék", imgblue);
		waitKey(0);
		imshow("sárga", imgyellow);
		waitKey(0); */

		file << "--" << i << "--" << endl;

		//ha van keresett alakzat akkor végrehajtjuk a match algoritmust és kiírjuk az eredményt fájlba
		if (hasredtriangle) {
			match.elskot = sma::smatch(imgred, elskot);

			file << "elsõbbségadás kötelezõ: " << match.elskot << endl;
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
			file << "parkoló: " << match.parkolo << endl;
		}
		
		if (hasyellowsign) {
			match.fout = sma::smatch(imgyellow, fout);

			file << "fõútvonal: " << match.fout << endl;
		}
		
		//ha nincs keresett alakzat a képen akkor nincs tábla a képen
		if (!hasredtriangle && !hasredcircle && !hasredoctagon && !hasbluesign && !hasyellowsign) {
			file << "--> NINCS TÁBLA A KÉPEN" << endl;
		}
		//ha van alakzat, akkor a tárolt eredmények közül megkeressük a legjobban illeszkedõt és azt írjuk be a fájlba eredményként
		else {
			file << "--> " << findbestmatch(match) << endl;
		}
		file << endl;

		//következõ iteráció elõkészítése
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