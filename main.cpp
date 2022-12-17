#include "houghlines.h"
#include "File_header/1.h"
#include <iostream>
#include <chrono>
#include <string>
using namespace std::chrono;

#include <opencv2/core/core.hpp>

// Library to include for
// drawing shapes
#include <opencv2/highgui/highgui.hpp>
#include <opencv2/imgproc.hpp>
#include <fstream>
using namespace cv;

using namespace std;

#define PI (3.1415926535897932384626433832795)
#include <string.h>

float slope(float x1, float y1, float x2, float y2) {
	return (double)-1 * (y2 - y1) / (x2 - x1);
}

int main() {
	int index = 1;
	std::ofstream myfile;
	myfile.open("output_pc.csv", ios::app);
		Mat image;
		image = imread("pic180x100/" + to_string(index) + ".png");
		Mat gray_image;
		int W = image.cols;
		int H = image.rows;
		int image_size = W * H;
		unsigned char* input = new unsigned char[image_size];
		memcpy(input, image.data, image_size);
		vector<line_float_t> Lines;
		boundingbox_t Bbox = { 0,(int)H / 2,W,(int)H / 2 };
		float scalex = 1;
		float scaley = 1;
		int Flag = 0;

		auto start = high_resolution_clock::now();
		Flag = HoughLineDetector(test_pgm, W, H, scalex, scaley, 10, 52, 1, PI / 180, 20, 10, 20, HOUGH_LINE_PROBABILISTIC, Bbox, Lines);
		cout << Flag << endl;

		for (vector<line_float_t>::iterator i = Lines.begin(); i != Lines.end();)
		{
			float s = slope((*i).startx, (*i).starty, (*i).endx, (*i).endy);
			if (!((s > 0.3 && s < 1.74) || (s<-0.7 && s>-1.7))) {
				i = Lines.erase(i);
			}
			else {
				i++;
			}
		}
		auto stop = high_resolution_clock::now();

		auto duration = duration_cast<milliseconds>(stop - start);

		cout << "Time execution: "
			<< duration.count() << " ms" << endl;

		myfile << to_string(index) + ',' + to_string(duration.count()) + '\n';
		
		for (int i = 0; i < Lines.size(); i++)
		{
			line(image, Point(Lines[i].startx, Lines[i].starty), Point(Lines[i].endx, Lines[i].endy), Scalar(0, 0, 255), 2);
		}

		imwrite("output_pc/output_" + to_string(index) + "_" + to_string(duration.count()) + ".png", image);

		//imshow("image1", image);
		//waitKey(0);


}