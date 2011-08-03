#include <iostream>
#include <string>
#include <fstream>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>

using namespace std;
using namespace cv;

int main()
{
	string strDirFlowerDB = "flowerPicDB\\";
	string strFNameFlowerDB = strDirFlowerDB + "files.txt";
	string strFNameFlower;
	ifstream inFile;
	int count = 0;

	inFile.open(strFNameFlowerDB.c_str());
	while(inFile >> strFNameFlower)
	{
		count++;
	}
	inFile.close();
	cout << "Number of flower photo = " << count << endl;

	Mat *imgFlower = new Mat[count];
	/*inFile.open(strFNameFlowerDB);
	while(inFile >> strFNameFlower)
	{
		cout << strFNameFlower << endl;
		imgFlower = imread(strDirFlowerDB + strFNameFlower);
		namedWindow(strFNameFlower);
		imshow(strFNameFlower,imgFlower);
	}
	inFile.close();*/

	waitKey(0);

	return 0;
}