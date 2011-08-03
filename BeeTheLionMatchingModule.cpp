#include <iostream>
#include <string>
#include <fstream>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>

using namespace std;
using namespace cv;

int main()
{
	string strDirFlowerDB = "flowerPicDB\\";
	string strFNameFlowerDB = strDirFlowerDB + "files.txt";
	string strFNameFlower;
	ifstream inFile;
	int count = 0;

	// Find number of photos.
	// ======================
	inFile.open(strFNameFlowerDB.c_str());
	while(inFile >> strFNameFlower)
	{
		count++;
	}
	inFile.close();
	cout << "Number of flower photo = " << count << endl;

	// Extract SURF feature of all photos in the DB.
	// =============================================
	Mat *imgFlowerDB = new Mat[count];
	vector<KeyPoint> *keypointDB = new vector[count];
	SurfFeatureDetector surf(2500);
	count = 0;
	inFile.open(strFNameFlowerDB.c_str());
	while(inFile >> strFNameFlower)
	{
		cout << strFNameFlower << endl;
		*imgFlowerDB[count] = imread(strDirFlowerDB + strFNameFlower);
		surf.detect(*imgFlowerDB[count],*keypointDB[count]);
		count++;
	}
	inFile.close();

	waitKey(0);

	return 0;
}