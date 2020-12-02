#include "Converter.h"
using namespace std;

class YUV444Converter : public Converter{
public:
    Mat encode(Mat &img) override{
        int rows = img.rows;
        int cols = img.cols;

        Mat y_array(rows, cols, CV_8UC1);
        Mat u_array(rows, cols, CV_8UC1);
        Mat v_array(rows, cols, CV_8UC1);

        for (int i = 0; i < rows; i++){
            for (int j = 0; j < cols; j++){
                int B = img.ptr<Vec3b>(i)[j][0];
                int G = img.ptr<Vec3b>(i)[j][1];
                int R = img.ptr<Vec3b>(i)[j][2];

                int Y = 0.299 * R + 0.587 * G + 0.114 * B;
                int U = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
                int V = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

                if (Y < 0) Y = 0;
                if (U < 0) U = 0;
                if (V < 0) V = 0;

                if (Y > 255) Y = 255;
                if (U > 255) U = 255;
                if (V > 255) V = 255;

                y_array.ptr<uchar>(i)[j] = Y;
                u_array.ptr<uchar>(i)[j] = U;
                v_array.ptr<uchar>(i)[j] = V;
            }
        }
        Mat result;
        vector<Mat> channels{y_array, u_array, v_array};
        merge(channels, result);
        return result;
    }

    Mat decode(Mat &img) override{
        int rows = img.rows;
        int cols = img.cols;
        Mat result(rows, cols, CV_8UC3);
        int Y;
        int U;
        int V;

        for (int i = 0; i < rows; i++)
        {
            for (int j = 0; j < cols; j++)
            {
                Y = img.ptr<Vec3b>(i)[j][0];
                U = img.ptr<Vec3b>(i)[j][1];
                V = img.ptr<Vec3b>(i)[j][2];

                int B = Y + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
                int G = Y - 0.714136 * V - 0.344136 * U + 135.459;
                int R = Y + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

                if (B < 0) B = 0;
                if (G < 0) G = 0;
                if (R < 0) R = 0;

                if (B > 255) B = 255;
                if (G > 255) G = 255;
                if (R > 255) R = 255;

                result.ptr<Vec3b>(i)[j][0] = B;
                result.ptr<Vec3b>(i)[j][1] = G;
                result.ptr<Vec3b>(i)[j][2] = R;
            }
        }
        return result;
    }
};
