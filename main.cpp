#include <iostream>
#include <opencv2/opencv.hpp>

using namespace cv;

void salt(Mat image){  // Соль и перець
    RNG rng;
    double x = image.rows * image.cols * 0.1, y = image.rows * image.cols * 0.1;   //для уровня шума
    for(int i = 0; i < x; i++)    image.at<uchar>(rng.uniform(0, image.rows), rng.uniform(0, image.cols)) = 0;
    for(int i = 0; i < y; i++)    image.at<uchar>(rng.uniform(0, image.rows), rng.uniform(0, image.cols)) = 255;
}

void shum(Mat image, int n) { // Для шума
    uchar * data = image.data;
    vector<int> values;
    for (int x = 0; x < (image.rows * image.cols); x++) {
        for (size_t i = 0; i < n; i++) {
            values.push_back(data[x + i]);
        }
        sort(values.begin(), values.end());

        image.data[x] = values[4];
        values.clear();
    }
}

Mat gauss(Mat image){ // Размытие Гаусса

    Mat final = image.clone();
    int filter [3] [3] = {{1, 2, 1}, // Матрица
                          {2, 4, 2},
                          {1, 2, 1}
    };
    for (int x = 1; x < image.rows-1; x++){
        for (int y = 1; y < image.cols-1; y++){
            int sum = 0;
            sum += image.at<uchar>(x-1,y-1)*filter[0][0];
            sum += image.at<uchar>(x-1,y)*filter[0][1];
            sum += image.at<uchar>(x-1,y+1)*filter[0][2];
            sum += image.at<uchar>(x,y-1)*filter[1][0];
            sum += image.at<uchar>(x,y)*filter[1][1];
            sum += image.at<uchar>(x,y+1)*filter[1][2];

            sum += image.at<uchar>(x+1,y-1)*filter[2][0];
            sum += image.at<uchar>(x+1,y)*filter[2][1];
            sum += image.at<uchar>(x+1,y+1)*filter[2][2];
            sum/=16; //Нормирующий коэффицент 16
            final.at<uchar>(x,y) = sum;
        }

    }
    return final;
}

int main(int argc, char** argv ) {
    Mat out, image = imread("/home/o2b3k/losh.jpg", 1);
    if (!image.data) return -1;

    cvtColor(image, image, CV_RGB2GRAY, 1);
    namedWindow("Original", WINDOW_AUTOSIZE);
    imshow("Original", image);

    salt(image);
    namedWindow("Salt and pepper", WINDOW_AUTOSIZE);
    imshow("Salt and pepper", image);

    shum(image, 9);
    Mat final =  gauss(image);
    namedWindow("Gauss", CV_WINDOW_AUTOSIZE);
    imshow("Gauss", final);

    waitKey(0);
    return 0;
}