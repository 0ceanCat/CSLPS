#include "Converter.h"
using namespace std;

class YUV422Converter : public Converter{
public:
    Mat encode(Mat &img) override{
        int rows = img.rows;
        int cols = img.cols;

        Mat result(rows * 2, cols, CV_8UC1);
        Mat u_array(rows / 2, cols, CV_8UC1);
        Mat v_array(rows / 2, cols, CV_8UC1);

        int u_row = rows;
        int u_col = 0;
        int v_row = rows * 3 / 2;
        int v_col = 0;

        for(int i = 0; i < rows; i++)
        {
            if(u_col == cols){
                u_row++;
                u_col = 0;
            }
            if(v_col == cols){
                v_row++;
                v_col = 0;
            }

            for(int j = 0; j < cols; j++)
            {
                int B = img.ptr<Vec3b>(i)[j][0];
                int G = img.ptr<Vec3b>(i)[j][1];
                int R = img.ptr<Vec3b>(i)[j][2];

                int Y = 0.299 * R + 0.587 * G + 0.114 * B;
                int U = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
                int V = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

                result.ptr<uchar>(i)[j] = Y;
                if(j%2 ==0){
                    result.ptr<uchar>(u_row)[u_col++] = U;
                    result.ptr<uchar>(v_row)[v_col++] = V;
                }
            }
        }
        return result;
    }

    Mat decode(Mat &img) override{
        int rows = img.rows;
        int cols = img.cols;
        Mat bmat(rows / 2, cols, CV_8UC1);
        Mat gmat(rows / 2, cols, CV_8UC1);
        Mat rmat(rows / 2, cols, CV_8UC1);

        int u_row = rows / 2;
        int u_col = 0;
        int v_row = u_row * 3 / 2;
        int v_col = 0;
        int Y;
        int U;
        int V;
        for (int i = 0; i < rows / 2; i++)
        {
            if (u_col == cols) {
                u_row++;
                u_col = 0;
            }
            if (v_col == cols) {
                v_row++;
                v_col = 0;
            }

            for (int j = 0; j < cols; j++)
            {
                Y = img.ptr<uchar>(i)[j];
                if (j % 2 == 0) {
                    U = img.ptr<uchar>(u_row)[u_col++];
                    V = img.ptr<uchar>(v_row)[v_col++];
                }
                int B = Y + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
                int G = Y - 0.714136 * V - 0.344136 * U + 135.459;
                int R = Y + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

                if (B < 0) B = 0;
                if (G < 0) G = 0;
                if (R < 0) R = 0;

                if (B > 255) B = 255;
                if (G > 255) G = 255;
                if (R > 255) R = 255;

                bmat.ptr<uchar>(i)[j] = B;
                gmat.ptr<uchar>(i)[j] = G;
                rmat.ptr<uchar>(i)[j] = R;
            }
        }
        Mat result;
        vector<Mat> channels{ bmat, gmat, rmat };
        merge(channels, result);
        return result;
    }
};


