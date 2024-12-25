#ifndef IMAGE_SPLITTER_H
#define IMAGE_SPLITTER_H

#include <opencv2/opencv.hpp>
#include <vector>
#include <iostream>
#include <filesystem>

using namespace cv;
using namespace std;

Mat generateRandomMatrix(const Size& size);
void splitImage(const Mat& image, vector<Mat>& parts, int n, int k);
Mat restoreImage(const vector<Mat>& parts, int k);

#endif