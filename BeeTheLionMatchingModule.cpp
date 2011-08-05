#include <iostream>
#include <string>
#include <fstream>
#include <vector>
#include <algorithm>
#include <ctime>

#include <opencv2\core\core.hpp>
#include <opencv2\highgui\highgui.hpp>
#include <opencv2\features2d\features2d.hpp>

using namespace std;
using namespace cv;

typedef vector<KeyPoint> vecKey;
typedef vector<DMatch> vecDMatch;

const int numTopSmall = 25; // Keep only top 25 lowest distance.

struct disStruct
{
	string strFNameOfPhoto;
	float distance;
};

ofstream outFile;

bool worseThan(const disStruct & r1,const disStruct & r2);
void ShowResult(const disStruct & rr);

int main()
{
	clock_t tmStart = clock();
	
	string strDirFlowerDB = "flowerPicDB\\";
	string strFNameFlowerDB = strDirFlowerDB + "files.txt";
	string strFNameFlower;
	string strFNameUnknownFlower = "unknownFlower.jpg";
	ifstream inFile;
	int count = 0;

	// Extract SURF feature of unknown flower.
	// =======================================
	Mat imgUnknownFlower;
	vecKey keypointOfUnknownFlower;
	SurfFeatureDetector surf(2500.);
	SurfDescriptorExtractor surfDesc;
	Mat descriptorOfUnknowFlower;
	imgUnknownFlower = imread(strFNameUnknownFlower);
	surf.detect(imgUnknownFlower,keypointOfUnknownFlower);
	surfDesc.compute(imgUnknownFlower,keypointOfUnknownFlower,descriptorOfUnknowFlower);

	// Find number of photos.
	// ======================
	inFile.open(strFNameFlowerDB.c_str());
	while(inFile >> strFNameFlower)
	{
		count++;
	}
	inFile.close();
	inFile.clear(); // If not clear() before it will make second while command finished
					// immediately.
	cout << "Number of flower photo = " << count << endl << endl;

	// Extract SURF feature of all photos in the DB.
	// =============================================
	Mat *imgFlowerDB = new Mat[count];
	vecKey *keypointDB = new vecKey[count];
	Mat *descriptorDB = new Mat[count];
	BruteForceMatcher<L2<float>> matcher;
	vecDMatch *resultOfMatchDB = new vecDMatch[count]; // Similarity between unknown and 
													   // each in DB.
	vector<disStruct> similarity;
	disStruct temp;
	count = 0;
	inFile.open(strFNameFlowerDB.c_str());
	while(inFile >> strFNameFlower)
	{
		cout << strFNameFlower << endl;
		imgFlowerDB[count] = imread(strDirFlowerDB + strFNameFlower);
		surf.detect(imgFlowerDB[count],keypointDB[count]);
		surfDesc.compute(imgFlowerDB[count],keypointDB[count],descriptorDB[count]);

		matcher.match(descriptorOfUnknowFlower,descriptorDB[count]
			,resultOfMatchDB[count]);
		nth_element(resultOfMatchDB[count].begin()
			,resultOfMatchDB[count].begin()+numTopSmall-1
			,resultOfMatchDB[count].end());
		resultOfMatchDB[count].erase(resultOfMatchDB[count].begin()+numTopSmall
			,resultOfMatchDB[count].end());
		float sum = 0;
		for(int i=0;i<numTopSmall;i++)
		{
			sum += resultOfMatchDB[count][i].distance;
		}
		temp.strFNameOfPhoto = strFNameFlower;
		temp.distance = sum;
		similarity.push_back(temp);
		count++;
	}
	inFile.close();

	sort(similarity.begin(),similarity.end(),worseThan);

	outFile.open("Matches.txt");
	for_each(similarity.begin(),similarity.end(),ShowResult);
	outFile.close();

	clock_t tmStop = clock();
	cout << endl << "Total using time = " << (tmStop - tmStart)/CLOCKS_PER_SEC
		<< " sec" << endl << "Pass enter to exit.";
	getchar();
	return 0;
}

bool worseThan(const disStruct & r1,const disStruct & r2)
{
	if(r1.distance < r2.distance)
		return true;
	else
		return false;
}

void ShowResult(const disStruct & rr)
{
	outFile << rr.strFNameOfPhoto << "\t" << rr.distance << endl;
}