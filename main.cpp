#include "main.h"

Mat generateRandomMatrix(const Size& size) {

    Mat randomMat = Mat::zeros(size, CV_8U);

    randu(randomMat, Scalar(0), Scalar(256));

    return randomMat;
}

void splitImage(const Mat& image, vector<Mat>& parts, int n) {
    Mat gray;

    if (image.channels() > 1) {
        cvtColor(image, gray, COLOR_BGR2GRAY);
    }

    else {
        gray = image;
    }

    vector<Mat> randomParts(n - 1);

    for (int i = 0; i < n - 1; i++) {
        randomParts[i] = generateRandomMatrix(gray.size());
    }

    Mat lastPart = gray.clone();

    for (int i = 0; i < gray.rows; i++) {
        for (int j = 0; j < gray.cols; j++) {
            uchar value = 0;
            for (int p = 0; p < n - 1; p++) {
                value ^= randomParts[p].at<uchar>(i, j);
            }
            lastPart.at<uchar>(i, j) ^= value;
        }
    }

    parts.push_back(lastPart);

    for (int i = 0; i < n - 1; i++) {
        parts.push_back(randomParts[i]);
    }
}

Mat restoreImage(const vector<Mat>& parts, int k) {

    Mat restoredImage = Mat::zeros(parts[0].size(), CV_8U);

    for (int i = 0; i < k; i++) {
        for (int row = 0; row < parts[i].rows; row++) {
            for (int col = 0; col < parts[i].cols; col++) {
                restoredImage.at<uchar>(row, col) ^= parts[i].at<uchar>(row, col);
            }
        }
    }

    return restoredImage;
}

int main() {

    string imagePath;

    string outputDir;
    outputDir = "/home/kali/Desktop/visual_crypto/images";

    if (!filesystem::exists(outputDir)) {
        filesystem::create_directories(outputDir); // Создает все необходимые директории
    }

    cout << "Enter path to image: ";
    getline(cin, imagePath); // Считываем путь к изображению

    Mat image = imread(imagePath);
    if (image.empty()) {
        cout << "Error while download image" << endl;
        return -1;
    }

    if (image.empty()) {
        cout << "Error while image uploading" << endl;
        return -1;
    }


    int n = 5;
    vector<Mat> parts;

    splitImage(image, parts, n);

    for (int i = 0; i < n; i++) {
        imwrite(outputDir + "/part" + to_string(i + 1) + ".jpg", parts[i]);
    }

    Mat restoredImage = restoreImage(parts, n);

    imshow("Original Image", image);

    imshow("Restored Image", restoredImage);

    waitKey(0);
    
    return 0;
}
