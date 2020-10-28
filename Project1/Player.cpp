#include "Player.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include "mythread.h"
using namespace std;

Player::Player()
{
    stop = false;
    option = 100;
    frameIndex = -1;
    isVideo = false;
    morphSize = 0;
    morphOperator = 0;
    morphelem = 0;
    kernelHeight = 1;
    kernelWidth = 1;
    gradient = 0;
    ksize = 1;
    scale = 1;
    delta = 0;
    lowThreshold = 0;
    maxLowthreshold = 100;
    ratio = 3;
    cannyKsize = 3;
    conversion = 0;
}

Player::~Player(){

}

void Player::setLabel(QLabel *label){
    this->label = label;
}

void Player::setOption(int option){
    this->option = option;
    if (!isVideo && !img.empty()){
        displayImg();
    }
}


void Player::displayVideo(){
    stop = false;
    isVideo = true;
    video.set(CAP_PROP_POS_FRAMES,1);
    int fps = video.get(CAP_PROP_FPS);
    int delay = 1000 / fps;
    for(;;){
        if(frameIndex != -1){
            video.set(CAP_PROP_POS_FRAMES,frameIndex);
            frameIndex = -1;
        }
       video.read(img);
       // check if we succeeded
       if (img.empty()) {
           cerr << "ERROR! blank frame grabbed\n";
           break;
       }
       setImage(img);
       displayImg();
       if (waitKey(delay) >= 0 || stop)
           break;
    }
    isVideo = false;
}


void Player::displayImg(){
    Mat result = doOptionWork();
    Mat final;
    cvtColor(result,final,COLOR_BGR2RGB);
    const uchar *qImageBuffer = (const uchar*)final.data;
    // Create QImage with same dimensions as input Mat
    QImage image(qImageBuffer, final.cols, final.rows, final.step, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(image));
    label->setScaledContents(true);
}

void Player::setImage(Mat img){
    if(!isVideo&&this->img.empty()){
        img.copyTo(original);
    }else if(isVideo){
        img.copyTo(original);
    }

    this->img = img;

}

void Player::setVideo(VideoCapture video){
    this->video = video;
    isVideo = true;
}

void Player::setWaterMarkText(String text){
    this->watermarkText = text;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setColorSpace(int colorSpace){
    this->colorSpace  = colorSpace;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setKernelWidth(int w){
    this->kernelWidth = w;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setKernelHeight(int h){
    this->kernelHeight = h;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setMorphOperator(int mp){
    this->morphOperator = mp;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setMorphSize(int ms){
    this->morphSize = ms;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setGradient(int g){
    this->gradient = g;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setMorhElem(int me){
    this->morphelem = me;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setKsize(int ksize){
   this->ksize = ksize < 30 ? ksize+2 : -1;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setScale(int scale){
    this->scale = scale;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setDelta(int delta){
    this->delta = delta;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setLowThreshold(int lt){
    this->lowThreshold = lt >= 100 ? lt : 99;

    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setRatio(int r){
    this->ratio = r;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setCannyKsize(int ck){
    this->cannyKsize = ck;
    if(!isVideo && !img.empty())
        displayImg();
}

void Player::setConversion(int c){
    this->conversion = c;
    if(!isVideo && !img.empty())
        displayImg();
}

bool Player::getIsVideo(){
    return this->isVideo;
}

VideoCapture Player::getVideo(){
    return this->video;
}


Mat Player::myConverter(){
    switch(conversion){
        case 1: return bgr2yuv444();
        case 2: return bgr2yuv422();
        case 3: return bgr2yuv420();
        case 4: return yuv444ToBgr();
        case 5: return yuv422ToBgr();
        case 6: return yuv420ToBgr();
    }

    return getImg();
}

Mat Player::getImg(){
    original.copyTo(img);
    return img;
}

int Player::getOption(){
    return option;
}

Mat Player::doOptionWork(){
    Mat image;
    switch (option) {
        case 1: return waterMark();
        case 2: return changeSpaceAndshowChannels();
        case 3: return colorHistogram();
        case 4: return grayAndHistogram();
        case 5: return gaussianBlurFilters();
        case 6: return segmentation();
        case 7: return morphologyTransformations();
        case 8: return gradients();
        case 9: return canny();
        case 10: return myConverter();
    }
    img.copyTo(image);
    return image;
}

Mat Player::waterMark(){
    Mat img = getImg();
    if (!empty(watermarkPicPath)){
        if (watermarkPic.empty()) watermarkPic = imread(watermarkPicPath);
        Mat imageROI = img(cv::Rect(10,10,watermarkPic.cols,watermarkPic.rows));
        watermarkPic.copyTo(imageROI);
    }

    if(!empty(watermarkText))
        putText(img, watermarkText, Point(20, img.rows - 50), FONT_HERSHEY_COMPLEX,1, Scalar(102, 20, 0), 2);
    return img;
}

Mat Player::changeSpaceAndshowChannels(){
    Mat channels[3];
    Mat image = getImg();
    if(colorSpace == 0)
        cvtColor(image, image, COLOR_BGR2YUV);
    else
        cvtColor(image, image, COLOR_BGR2HSV);
    split(image,channels);

    namedWindow("Red", WINDOW_NORMAL);
    imshow("Red",channels[0]);


    namedWindow("Green", WINDOW_NORMAL);
    imshow("Green",channels[1]);


    namedWindow("Blue", WINDOW_NORMAL);
    imshow("Blue",channels[2]);

    return image;
}

Mat Player::colorHistogram(){
    vector<Mat> bgr_planes;
    Mat src = getImg();
    split( src, bgr_planes );
    int histSize = 256;

    float range[] = { 0, 256 }; //the upper boundary is exclusive
    const float* histRange = { range };

    bool uniform = true, accumulate = false;

    Mat b_hist, g_hist, r_hist;
    calcHist( &bgr_planes[0], 1, 0, Mat(), b_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[1], 1, 0, Mat(), g_hist, 1, &histSize, &histRange, uniform, accumulate );
    calcHist( &bgr_planes[2], 1, 0, Mat(), r_hist, 1, &histSize, &histRange, uniform, accumulate );

    int hist_w = 512, hist_h = 400;
    int bin_w = cvRound( (double) hist_w/histSize );

    Mat histImage( hist_h, hist_w, CV_8UC3, Scalar( 0,0,0) );

    normalize(b_hist, b_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(g_hist, g_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );
    normalize(r_hist, r_hist, 0, histImage.rows, NORM_MINMAX, -1, Mat() );

    for( int i = 1; i < histSize; i++ )
    {
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(b_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(b_hist.at<float>(i)) ),
              Scalar( 255, 0, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(g_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(g_hist.at<float>(i)) ),
              Scalar( 0, 255, 0), 2, 8, 0  );
        line( histImage, Point( bin_w*(i-1), hist_h - cvRound(r_hist.at<float>(i-1)) ),
              Point( bin_w*(i), hist_h - cvRound(r_hist.at<float>(i)) ),
              Scalar( 0, 0, 255), 2, 8, 0  );
    }

    namedWindow("calcHist Demo", WINDOW_NORMAL);
    imshow("calcHist Demo", histImage );
    return src;
}

Mat Player::grayAndHistogram(){
    Mat greyMat;
    Mat image = getImg();
    cvtColor(image, greyMat, COLOR_BGR2GRAY);

    Mat dst;
    equalizeHist(greyMat, dst);
    namedWindow("Equalized Image", WINDOW_NORMAL);
    imshow("Equalized Image", dst);

    return greyMat;
}


Mat Player::gaussianBlurFilters(){
    Mat dst;
    Mat image = getImg();
    GaussianBlur(image, dst, Size(kernelWidth, kernelHeight), 0);
    return dst;
}

/**
 * @brief A alinea f, Player::segmentation
 * @return
 */
Mat Player::segmentation(){
    Mat image = getImg();
    Mat gray, dst;
    cvtColor(image, gray, COLOR_BGR2GRAY); // Convert the image to Gray
    threshold(gray, dst, thresholdValue, 255, thresholdType );
    return dst;
}


void Player::setThresholdType(int type){
    this->thresholdType = type;
    if(!isVideo && !getImg().empty())
        displayImg();
}

void Player::setThresholdValue(int value){
    this->thresholdValue = value;
    if(!isVideo && !getImg().empty())
        displayImg();
}

void Player::setWaterMarkPicPath(String path){
    watermarkPicPath = path;
    watermarkPic = NULL;
    if(!isVideo && !getImg().empty())
        displayImg();
}
//yuv420
Mat Player::bgr2yuv420(){
    Mat img = getImg();
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
              unsigned char B = img.ptr<Vec3b>(i)[j][0];
              unsigned char G = img.ptr<Vec3b>(i)[j][1];
              unsigned char R = img.ptr<Vec3b>(i)[j][2];

              unsigned char Y = 0.299 * R + 0.587 * G + 0.114 * B;
              unsigned char U = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
              unsigned char V = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

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
    setImage(result);
    return result;
}

/**
 * @brief converts a bgr image to yuv444 format
 * @return yuv444 image
 */
Mat Player::bgr2yuv444(){
    int rows = img.rows;
    int cols = img.cols;

    Mat y_array(rows, cols, CV_8UC1);
    Mat u_array(rows, cols, CV_8UC1);
    Mat v_array(rows, cols, CV_8UC1);

    for (int i = 0; i < rows; i++){
        for (int j = 0; j < cols; j++){
            unsigned char B = img.ptr<Vec3b>(i)[j][0];
            unsigned char G = img.ptr<Vec3b>(i)[j][1];
            unsigned char R = img.ptr<Vec3b>(i)[j][2];

            unsigned char Y = 0.299 * R + 0.587 * G + 0.114 * B;
            unsigned char U = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
            unsigned char V = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

            y_array.ptr<uchar>(i)[j] = Y;
            u_array.ptr<uchar>(i)[j] = U;
            v_array.ptr<uchar>(i)[j] = V;
        }
    }
    Mat result;
    vector<Mat> channels{y_array, u_array, v_array};
    merge(channels, result);
    setImage(result);

    return result;
}

/**
 * @brief converts a bgr image to yuv222 format
 * @return yuv222 image
 */
Mat Player::bgr2yuv422(){
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
              unsigned char B = img.ptr<Vec3b>(i)[j][0];
              unsigned char G = img.ptr<Vec3b>(i)[j][1];
              unsigned char R = img.ptr<Vec3b>(i)[j][2];

              unsigned char Y = 0.299 * R + 0.587 * G + 0.114 * B;
              unsigned char U = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
              unsigned char V = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

              result.ptr<uchar>(i)[j] = Y;
              if(j%2 ==0){
                   result.ptr<uchar>(u_row)[u_col++] = U;
                   result.ptr<uchar>(v_row)[v_col++] = V;
              }
        }
    }
    setImage(result);
    return result;
}

/**
 * @brief converts a yuv420 image to bgr format
 * @return bgr image
 */
Mat Player::yuv420ToBgr(){
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
        unsigned  char Y1;
        unsigned char Y2;
        unsigned char U;
        unsigned char V;
        for (int j = 0; j < cols; j++){
            // Y1 Y2 Y3 Y4
            // Y5 Y6 Y7 Y8
            // U1 U2 U3 U4
            // V1 V2 V3 V4


            //Y1 and Y5 use the same U and V
            Y1 = img.ptr<uchar>(i)[j];      //Y1
            Y2 = img.ptr<uchar>(i + 1)[j];  //Y5

            if(v_row == rows) break;

            //1 U and 1 V for each 2 cols
            if(j%2 == 0){
                U = img.ptr<uchar>(u_row)[u_cols++];
                V = img.ptr<uchar>(v_row)[v_cols++];
            }

            unsigned char B1 = Y1 + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
            unsigned char G1 = Y1 - 0.714136 * V - 0.344136 * U + 135.459;
            unsigned char R1 = Y1 + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

            unsigned char B2 = Y2 + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
            unsigned char G2 = Y2 - 0.714136 * V - 0.344136 * U + 135.459;
            unsigned char R2 = Y2 + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

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
    setImage(result);
    return result;
}


Mat Player::yuv422ToBgr(){
    int rows = img.rows;
    int cols = img.cols;
    Mat bmat(rows / 2, cols, CV_8UC1);
    Mat gmat(rows / 2, cols, CV_8UC1);
    Mat rmat(rows / 2, cols, CV_8UC1);

    int u_row = rows / 2;
    int u_col = 0;
    int v_row = u_row * 3 / 2;
    int v_col = 0;
    unsigned char Y;
    unsigned char U;
    unsigned char V;
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
            unsigned char B = Y + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
            unsigned char G = Y - 0.714136 * V - 0.344136 * U + 135.459;
            unsigned char R = Y + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

            bmat.ptr<uchar>(i)[j] = B;
            gmat.ptr<uchar>(i)[j] = G;
            rmat.ptr<uchar>(i)[j] = R;
        }
    }
    Mat result;
    vector<Mat> channels{ bmat, gmat, rmat };

    // Create the output matrix
    merge(channels, result);
    setImage(result);
    return result;
}

/**
 * @brief Player::yuv444ToBgr converter yuv444 para bgr
 * @return
 */
Mat Player::yuv444ToBgr(){
    int rows = img.rows;
    int cols = img.cols;
    Mat result(rows, cols, CV_8UC3);
    unsigned char Y;;
    unsigned char U;
    unsigned char V;

    for (int i = 0; i < rows; i++)
    {
        for (int j = 0; j < cols; j++)
        {
            Y = img.ptr<Vec3b>(i)[j][0];
            U = img.ptr<Vec3b>(i)[j][1];
            V = img.ptr<Vec3b>(i)[j][2];

            unsigned char B = Y + 4.06298 * pow(10, -7) * V + 1.772 * U - 226.816;
            unsigned char G = Y - 0.714136 * V - 0.344136 * U + 135.459;
            unsigned char R = Y + 1.402 * V - 121.889 * pow(10, -6) * U - 179.456;

            result.ptr<Vec3b>(i)[j][0] = B;
            result.ptr<Vec3b>(i)[j][1] = G;
            result.ptr<Vec3b>(i)[j][2] = R;
        }
    }
    setImage(result);
    return result;
}

Mat Player::morphologyTransformations(){
    int operation = morphOperator + 2;

    Mat element = getStructuringElement(morphelem, Size(2*morphSize + 1, 2*morphSize+1), Point(morphSize, morphSize));
    Mat dst;
    morphologyEx(getImg(), dst, operation, element );
    return dst;
}

Mat Player::gradients(){
    int ddepth = CV_16S;

    Mat dst;
    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(getImg(), dst, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // Convert the image to grayscale
    cvtColor(dst, dst, COLOR_BGR2GRAY);
    Mat grad;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

    if(gradient == 0){
        Sobel(dst, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
        Sobel(dst, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
    }

    if(gradient == 1){
        Scharr(dst, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
        Scharr(dst, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
    }

    if(gradient == 2){
        Laplacian(dst, grad_x, ddepth, ksize, scale, delta, BORDER_DEFAULT);
        Laplacian(dst, grad_y, ddepth,  ksize, scale, delta, BORDER_DEFAULT);
    }

    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    return grad;
}


Mat Player::canny(){

    Mat img = getImg();
    Mat dst, img_gray, detected_edges;
    dst.create(img.size(), img.type());

    cvtColor(img, img_gray, COLOR_BGR2GRAY);

    blur( img_gray, detected_edges, Size(3,3) );

    Canny( detected_edges, detected_edges, lowThreshold, lowThreshold*ratio, cannyKsize );

    dst = Scalar::all(0);
    img.copyTo(dst, detected_edges);
    return dst;
}

