#include "../header/Predictor.h"

void Predictor::encodeVideo(string videoPath, Format format) {
    VideoCapture video;
    video.open(videoPath);

    if(!video.isOpened())
    {
        printf("Could not open video\n");
        return;
    }

    Mat frame;
    video.read(frame);
    if (frame.empty()) {
        cerr << "ERROR! blank frame grabbed\n";
        return;
    }

    frame = this->convs.at(format)->encode(frame);
    golomb.encode(frame.rows);
    golomb.encode(frame.cols);
    golomb.encode(frame.channels());
    golomb.encode((int) video.get(CAP_PROP_FRAME_COUNT));
    golomb.encode((int) video.get(CAP_PROP_FPS));
    golomb.encode(format);

    int f = 1;
    for(;;){
        // check if we succeeded
        if (frame.empty()) {
            break;
        }
        encodeFrame(frame);
        video.read(frame);
        frame = this->convs.at(format)->encode(frame);
        cout << "frame " << f << " encoded" << endl;
        f++;
    }
    cout << "encode finished" << endl;
}

void Predictor::encodeFrame(const Mat& frame) {
    int channels = frame.channels();
    int total = 0;
    int e;
    for (int i = 0; i < frame.rows; ++i) {
        for (int j = 0; j < frame.cols; ++j) {
            for (int k = 0; k < channels; ++k) {
                uchar p, v;
                if(channels == 1){
                    p = getPredictValor(frame, -1, i, j);
                    v = frame.ptr<uchar>(i)[j];
                    e = v - p;
                }else{
                    p = getPredictValor(frame, k, i, j);
                    v = frame.ptr<Vec3b>(i)[j][k];
                    e = v - p;
                };
                total += abs(e);
                golomb.encode(e);
            }
        }
    }
    //a idea do Pedro Henrique
    int nextM = 1 << (int) log2(total / (frame.cols * frame.rows * channels) + 1);
    golomb.encode(nextM);
    golomb.setM(nextM);
}

int Predictor::getPredictValor(const Mat& frame, int channel, int line, int col) {
    // -0-0---0---
    // -0|c | b|--
    // -0|a | x|--
    // ------------
    int a, b, c;
    if(channel == -1){ //o frame tem apenas um canal 422,420
        a = col == 0 ? 0 : frame.ptr<uchar>(line)[col - 1];
        b = line == 0 ? 0 : frame.ptr<uchar>(line - 1)[col];
        c = line > 0 and col > 0 ? frame.ptr<uchar>(line - 1)[col - 1] : 0;
    }else{//444 bgr
        a = col == 0 ? 0 : frame.ptr<Vec3b>(line)[col - 1][channel];
        b = line == 0 ? 0 : frame.ptr<Vec3b>(line - 1)[col][channel];
        c = line > 0 and col > 0 ? frame.ptr<Vec3b>(line - 1)[col - 1][channel] : 0;
    }

    switch (type) {
        case 1: return a;
        case 2: return b;
        case 3: return c;
        case 4: return a + b - c;
        case 5: return a + (b- c) / 2;
        case 6: return b + (a - c) / 2;
        case 7: return (a + b) / 2;
    }

    if (c >= max(a, b))
        return min(a, b);

    if (c <= min(a, b))
        return max(a, b);

    return  a + b - c;
}

void Predictor::decodeVideo() {
    golomb.setM(this->m);
    int rows = golomb.decode();
    int cols = golomb.decode();
    int channels = golomb.decode();
    int frames = golomb.decode();
    int fps = golomb.decode();
    int format = golomb.decode();

    cout << "decoding..." << endl;

    if (channels == 3)
        multipleChannelsDecoder(rows, cols, frames, fps, format);
    else
        singleChannelDecoder(rows, cols, frames, fps, format);

}

void Predictor::flip() {
    golomb.flip();
}

void Predictor::multipleChannelsDecoder(int rows, int cols, int frames, int fps, int format) {
    VideoWriter writer;
    writer.open("C:/Users/Predator/Desktop/decoded.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(cols,rows), true);
    for (int f = 0; f < frames; ++f){
        Mat frame = cv::Mat::zeros(rows, cols, CV_8UC3);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int pOfB = getPredictValor(frame,0, i, j);
                int pOfG = getPredictValor(frame, 1, i, j);
                int pOfR = getPredictValor(frame, 2, i, j);
                frame.ptr<Vec3b>(i)[j][0] = pOfB + golomb.decode();
                frame.ptr<Vec3b>(i)[j][1] = pOfG + golomb.decode();
                frame.ptr<Vec3b>(i)[j][2] = pOfR + golomb.decode();
            }
        }
        writer.write(this->convs.at(format)->decode(frame));
        int nextM = golomb.decode();
        golomb.setM(nextM);
        cout << "frame " << f + 1 << " decoded" << endl;
    }
}

void Predictor::singleChannelDecoder(int rows, int cols, int frames, int fps, int format) {
    VideoWriter writer;
    for (int f = 0; f < frames; ++f){
        Mat frame(rows, cols, CV_8UC1);
        for (int i = 0; i < rows; ++i) {
            for (int j = 0; j < cols; ++j) {
                int p = getPredictValor(frame,-1, i, j);
                frame.ptr<uchar>(i)[j] = golomb.decode() + p;
            }
        }
        const Mat &bgr = this->convs.at(format)->decode(frame);
        if (!writer.isOpened()){
            writer.open("C:/Users/Predator/Desktop/decoded.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(bgr.cols, bgr.rows), true);
        }
        writer.write(bgr);
        int nextM = golomb.decode();
        golomb.setM(nextM);
        cout << "frame " << f + 1 << " decoded" << endl;
    }
}
