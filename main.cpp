#include <opencv2/opencv.hpp>
#include <iostream>
#include <vector>
#include <string>
#include <filesystem>

void encrypt(const cv::Mat& input, cv::Mat& output1, cv::Mat& output2) {
    // Убедимся, что изображение черно-белое
    if (input.type() != CV_8UC1) {
        std::cerr << "Ошибка: входное изображение должно быть черно-белым!" << std::endl;
        return;
    }

    // Создаем выходные изображения с теми же размерами и типом, что и входное
    output1 = cv::Mat::zeros(input.size(), CV_8UC1);
    output2 = cv::Mat::zeros(input.size(), CV_8UC1);

    // Генерируем случайные значения для шифрования
    cv::Mat randomMask = cv::Mat::zeros(input.size(), CV_8UC1);
    cv::randu(randomMask, cv::Scalar(0), cv::Scalar(255)); // Генерируем случайные значения от 0 до 255

    // Проходим по каждому пикселю входного изображения
    for (int y = 0; y < input.rows; ++y) {
        for (int x = 0; x < input.cols; ++x) {
            // Получаем значение пикселя (0-255)
            uchar pixelValue = input.at<uchar>(y, x);
            uchar randomValue = randomMask.at<uchar>(y, x);

            // Шифруем пиксель с использованием XOR
            output1.at<uchar>(y, x) = pixelValue ^ randomValue; // Зашифрованное значение в первой части
            output2.at<uchar>(y, x) = randomValue;             // Случайное значение во второй части
        }
    }
}

cv::Mat decryptImages(const cv::Mat& encryptedImage1, const cv::Mat& encryptedImage2) {
    // Проверка, что размеры изображений совпадают
    if (encryptedImage1.size() != encryptedImage2.size()) {
        throw std::invalid_argument("Размеры изображений должны совпадать!");
    }

    cv::Mat decryptedImage;
    // Выполнение операции XOR для восстановления исходного изображения
    cv::bitwise_xor(encryptedImage1, encryptedImage2, decryptedImage);

    return decryptedImage;
}


int main() {
   // srand(static_cast<unsigned int>(time(0)));
    // Загружаем черно-белое изображение, для того чтоб указать относительный путь, его необходимо прописывать из дериктории build
    cv::Mat inputImage = cv::imread("/home/kali/Desktop/visual_crypto/z.jpeg", cv::IMREAD_GRAYSCALE);
    
    if (inputImage.empty()) {
        std::cerr << "Ошибка: не удалось загрузить изображение!" << std::endl;
        return -1;
    }

    std::string outputDir = "source/";
    std::filesystem::create_directory(outputDir);

    cv::Mat output1, output2;
    encrypt(inputImage, output1, output2);
    cv::imwrite(outputDir + "output1.png", output1);
    cv::imwrite(outputDir + "output2.png", output2);

    // Показываем результаты
    // cv::imshow("Original Image", inputImage);
    // cv::imshow("Encrypted Part 1", output1);
    // cv::imshow("Encrypted Part 2", output2);
    
    cv::Mat original = decryptImages(output1, output2);
    cv::imwrite(outputDir + "returned.png", original);


    //cv::waitKey(0);
    return 0;
}
