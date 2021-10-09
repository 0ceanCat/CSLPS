#include "Converter.h"
using namespace std;

class YUV420Converter : public Converter{
public:
    Mat encode(Mat &img) override{
        int rows = img.rows;
        int cols = img.cols;

        Mat result(rows * 3 / 2, cols, CV_8UC1);
        Mat u_array(rows / 4, cols, CV_8UC1);
        Mat v_array(rows / 4, cols, CV_8UC1);

        int u_row = rows;     // U belongs to the range [img.rows, img.rows*1.25[
        int u_col = 0;
        int v_row = rows * 5 / 4;  // V belongs to the range [img.rows*1.25, img.rows*1.5[
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

                if (Y < 0) Y = 0;
                if (U < 0) U = 0;
                if (V < 0) V = 0;

                if (Y > 255) Y = 255;
                if (U > 255) U = 255;
                if (V > 255) V = 255;

                result.ptr<uchar>(i)[j] = Y;
                if(i%2==0&&j%2 ==0){
                    result.ptr<uchar>(u_row)[u_col++] = U;
                }else{
                    if(j%2==0){
                        result.ptr<uchar>(v_row)[v_col++] = V;
                    }
                }
            }
        }
        return result;
    }

    Mat decode(Mat &img) override{
        int rows = img.rows;
        int cols = img.cols;

        int u_row = rows * 2 / 3;
        int v_row = u_row * 5 / 4;

        int u_cols = 0;
        int v_cols = 0;

        Mat b_array(u_row, cols, CV_8UC1);
        Mat g_array(u_row, cols, CV_8UC1);
        Mat r_array(u_row, cols, CV_8UC1);

        for (int i = 0; i < u_row; i+=2){

            if(u_cols == cols){
                u_row++;
                u_cols = 0;
            }
            if(v_cols == cols){
                v_row++;
                v_cols = 0;
            }
            int Y1;
            int Y5;
            int U;
            int V;
            for (int j = 0; j < cols; j++){
                // Y1 Y2 Y3 Y4
                // Y5 Y6 Y7 Y8
                // U1 U2 U3 U4
                // V1 V2 V3 V4

                //Y1,Y5,Y2,Y6 use U1 and V1
                //for each j, i convert the Y of the line i and the Y of the next line (i+1) at same time
                Y1 = img.ptr<uchar>(i)[j];
                Y5 = img.ptr<uchar>(i + 1)[j];

                if(v_row == rows) break;

                //1 U and 1 V for each 2 cols
                if(j%2 == 0){
                    U = img.ptr<uchar>(u_row)[u_cols++];
                    V = img.ptr<uchar>(v_row)[v_cols++];
                }

                int B1 = Y1 + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
                int G1 = Y1 - 0.714136 * V - 0.344136 * U + 135.459;
                int R1 = Y1 + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

                int B2 = Y5 + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
                int G2 = Y5 - 0.714136 * V - 0.344136 * U + 135.459;
                int R2 = Y5 + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

                if (B1 < 0) B1 = 0;
                if (G1 < 0) G1 = 0;
                if (R1 < 0) R1 = 0;

                if (B1 > 255) B1 = 255;
                if (G1 > 255) G1 = 255;
                if (R1 > 255) R1 = 255;


                if (B2 < 0) B2 = 0;
                if (G2 < 0) G2 = 0;
                if (R2 < 0) R2 = 0;

                if (B2 > 255) B2 = 255;
                if (G2 > 255) G2 = 255;
                if (R2 > 255) R2 = 255;

                b_array.ptr<uchar>(i)[j] = B1;
                g_array.ptr<uchar>(i)[j] = G1;
                r_array.ptr<uchar>(i)[j] = R1;

                b_array.ptr<uchar>(i + 1)[j] = B2;
                g_array.ptr<uchar>(i + 1)[j] = G2;
                r_array.ptr<uchar>(i + 1)[j] = R2;
            }

        }
        Mat result;
        vector<Mat> channels{b_array, g_array, r_array};
        merge(channels, result);
        return result;
    }

};