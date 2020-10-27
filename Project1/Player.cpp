#include "Player.h"
#include <iostream>
#include <opencv2/imgproc.hpp>
#include "mythread.h"
using namespace std;

Player::Player()
{
    stop = false;
    option = 'a';
    frame = -1;
}

Player::~Player(){

}

void Player::setLabel(QLabel *label){
    this->label = label;
}
void Player::setOption(char option){
    this->option = option;
    if (!isVideo && !img.empty()){
        displayImg();
    }
}


void Player::displayVideo(){
    stop = false;
    video.set(CAP_PROP_POS_FRAMES,1);
    int fps = video.get(CAP_PROP_FPS);
    int delay = 1000 / fps;
    for(;;){
        if(frame != -1){
            video.set(CAP_PROP_POS_FRAMES,frame);
            frame = -1;
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
}


void Player::displayImg(){
    Mat result = doOptionWork();
    cvtColor(result,result,COLOR_BGR2RGB);
    const uchar *qImageBuffer = (const uchar*)result.data;
    // Create QImage with same dimensions as input Mat
    QImage image(qImageBuffer, result.cols, result.rows, result.step, QImage::Format_RGB888);
    label->setPixmap(QPixmap::fromImage(image));
    label->setScaledContents(true);
}

void Player::setImage(Mat img){
    this->img = img;
    img.copyTo(original);
    isVideo = false;
}

void Player::setVideo(VideoCapture video){
    this->video = video;
    isVideo = true;
}

bool Player::getIsVideo(){
    return this->isVideo;
}

VideoCapture Player::getVideo(){
    return this->video;
}

Mat Player::getImg(){
  //  Mat copy;
   // original.copyTo(copy);
    return img;
}

char Player::getOption(){
    return option;
}

Mat Player::doOptionWork(){
    switch (option) {
        case 'a': return waterMark();
        case 'b': return changeSpaceAndshowChannels();
        case 'c': return colorHistogram();
        case 'd': return grayAndHistogram();
        case 'e': return gaussianBlurFilters();
        case 'f': return segmentation();
        case '3': return ex3();
        case 'g': return morphologyTransformations();
        case 'h': return gradients();
        case 'o': Mat image; original.copyTo(image); img = image; return image;
    }
    return img;
}

Mat Player::waterMark(String filepaht, String text){
    Mat logo = imread(filepaht);
  //  Mat image = getImg();
    Mat imageROI = img(cv::Rect(10,10,logo.cols,logo.rows));
    logo.copyTo(imageROI);
    putText(img, text, Point(20, img.rows - 50), FONT_HERSHEY_COMPLEX,1, Scalar(102, 20, 0), 2);
    return img;
}

Mat Player::changeSpaceAndshowChannels(){
    Mat channels[3];
    Mat image = getImg();
    split(img,channels);

    namedWindow("Red", WINDOW_NORMAL);
    imshow("Red",channels[0]);


    namedWindow("Green", WINDOW_NORMAL);
    imshow("Green",channels[1]);


    namedWindow("Blue", WINDOW_NORMAL);
    imshow("Blue",channels[2]);

    cvtColor(img, img, WINDOW_NORMAL);
    return img;
}

Mat Player::colorHistogram(){
    vector<Mat> bgr_planes;
    Mat src = getImg();
    split( img, bgr_planes );
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

    imshow("calcHist Demo", histImage );
    return img;
}

Mat Player::grayAndHistogram(){
    Mat greyMat;
    Mat image = getImg();
    cvtColor(img, greyMat, COLOR_BGR2GRAY);

    Mat dst;
    equalizeHist(greyMat, dst);
    imshow( "Equalized Image", dst);

    return greyMat;
}


Mat Player::gaussianBlurFilters(){
    Mat image_blurred_with_3x3_kernel;
    Mat image = getImg();
    GaussianBlur(img, image_blurred_with_3x3_kernel, Size(3, 3), 0);

    //Blur the image with 5x5 Gaussian kernel
    Mat image_blurred_with_5x5_kernel;
    GaussianBlur(img, image_blurred_with_5x5_kernel, Size(5, 5), 0);

    //Define names of the windows
    String window_name_blurred_with_3x3_kernel = "Image Blurred with 3 x 3 Gaussian Kernel";
    String window_name_blurred_with_5x5_kernel = "Image Blurred with 5 x 5 Gaussian Kernel";

    // Create windows with above names
    namedWindow(window_name_blurred_with_3x3_kernel);
    namedWindow(window_name_blurred_with_5x5_kernel);

    // Show our images inside the created windows.
    imshow(window_name_blurred_with_3x3_kernel, image_blurred_with_3x3_kernel);
    imshow(window_name_blurred_with_5x5_kernel, image_blurred_with_5x5_kernel);
    return img;
}

/**
 * @brief A alinea f, Player::segmentation
 * @return
 */
Mat Player::segmentation(){
    Mat image = getImg();
    Mat gray, dst;
    cvtColor(img, gray, COLOR_BGR2GRAY); // Convert the image to Gray
    threshold(gray, dst, threshold_value, 255, threshold_type );
    return dst;
}

void Player::setFrame(int frame){
    this->frame = frame;
}

void Player::setThresholdType(int type){
    this->threshold_type = type;
    displayImg();
}

void Player::setThresholdValue(int value){
    this->threshold_value = value;
    displayImg();
}

//yuv420
Mat Player::ex3(){
    Mat result(img.rows*3/2, img.cols, CV_8UC1);
    Mat u_array(img.rows/4, img.cols, CV_8UC1);
    Mat v_array(img.rows/4, img.cols, CV_8UC1);

    int u_row = 0;
    int u_col = 0;
    int v_row = 0;
    int v_col = 0;

    for(int i = 0; i < img.rows; i++)
    {
        if(u_col == img.cols){
            u_row++;
            u_col = 0;
        }
        if(v_col == img.cols){
             v_row++;
             v_col = 0;
        }

        for(int j = 0; j < img.cols; j++)
        {
              unsigned char B = img.ptr<Vec3b>(i)[j][0];
              unsigned char G = img.ptr<Vec3b>(i)[j][1];
              unsigned char R = img.ptr<Vec3b>(i)[j][2];

              unsigned char Y = 0.299 * R + 0.587 * G + 0.114 * B;
              unsigned char U = 128 - 0.168736 * R - 0.331264 * G + 0.5 * B;
              unsigned char V = 128 + 0.5 * R - 0.418688 * G - 0.081312 * B;

              result.ptr<uchar>(i)[j] = Y;
              if(i%2==0&&j%2 ==0){
                   u_array.ptr<uchar>(u_row)[u_col++] = U;
              }else{
                  if(j%2==0){
                   v_array.ptr<uchar>(v_row)[v_col++] = V;
                  }
              }
        }
    }

    //preencher os U's no resultado
    for (int i = 0, k = img.rows; i < img.rows/4 && k < img.rows*3/2; i++,k++){
        for (int j = 0; j < img.cols; j++){
            result.ptr<uchar>(k)[j] = u_array.ptr<uchar>(i)[j];
        }
    }

    //preencher os V's no resultado
    for (int i = 0, k = img.rows*3/4; i < img.rows/4 && k < img.rows*3/2; i++,k++){
        for (int j = 0; j < img.cols; j++){
            result.ptr<uchar>(k)[j] = v_array.ptr<uchar>(i)[j];
        }
    }

    return result;
}


Mat Player::morphologyTransformations(){
    int operation = morph_operator + 2;

    Mat element = getStructuringElement(0, Size( 2*morph_size + 1, 2*morph_size+1 ), Point( morph_size, morph_size ) );
    Mat dst;
    morphologyEx(getImg(), dst, operation, element );
    return dst;
}

Mat Player::gradients(){
    int ksize = 1;
    int scale = 1;
    int delta = 0;
    int ddepth = CV_16S;

    Mat dst;
    // Remove noise by blurring with a Gaussian filter ( kernel size = 3 )
    GaussianBlur(getImg(), dst, Size(3, 3), 0, 0, BORDER_DEFAULT);

    // Convert the image to grayscale
    cvtColor(dst, dst, COLOR_BGR2GRAY);
    Mat grad;
    Mat grad_x, grad_y;
    Mat abs_grad_x, abs_grad_y;

  //  Sobel(dst, grad_x, ddepth, 1, 0, ksize, scale, delta, BORDER_DEFAULT);
  //  Sobel(dst, grad_y, ddepth, 0, 1, ksize, scale, delta, BORDER_DEFAULT);
//    Scharr(dst, grad_x, ddepth, 1, 0, scale, delta, BORDER_DEFAULT);
  //  Scharr(dst, grad_y, ddepth, 0, 1, scale, delta, BORDER_DEFAULT);
      Laplacian(dst, grad_x, ddepth, ksize, scale, delta, BORDER_DEFAULT);
      Laplacian(dst, grad_y, ddepth,  ksize, scale, delta, BORDER_DEFAULT);
    // converting back to CV_8U
    convertScaleAbs(grad_x, abs_grad_x);
    convertScaleAbs(grad_y, abs_grad_y);

    addWeighted(abs_grad_x, 0.5, abs_grad_y, 0.5, 0, grad);
    return grad;
}

