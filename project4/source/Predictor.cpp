#include "../header/Predictor.h"

void Predictor::encodeVideo(const string& videoPath, Format format) {
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
    //golomb.encode((int) video.get(CAP_PROP_FRAME_COUNT));
    golomb.encode(16);
    golomb.encode((int) video.get(CAP_PROP_FPS));
    golomb.encode(format);

    int f = 1;
    for(;;){
        // check if we succeeded
        if (frame.empty()) {
            break;
        }
        if (f == 1){
            encodeInter(frame);
            lastFrame = frame;
        }else{
            frame = convs.at(format)->encode(frame);
            encode(frame);
        }
        video.read(frame);
        cout << "frame " << f++ << " encoded" << endl;
        if (f == 17)
            break;
    }
    cout << "encode finished" << endl;
}

/**
 * se o numero de bits que {@param frame} precisa for igual o do frame anterior,
 * signica que a cena nao mudou.
 * @param frame
 */
void Predictor::encode(const Mat& frame) {
    int bits = golomb.countNecessaryBit(mediaFrame(frame));
    if (bitsLastFrame != bits) {//houve uma mudanca da cena
        encodeInter(frame);
    }
    else{
        encodeIntra(frame);
    }
    bitsLastFrame = bits;
    lastFrame = frame;

}


void Predictor::encodeInter(const Mat& frame) {
    cout << "Inter" << endl;
    golomb.encode(0); //modo inter
    int bestM = getBestMifUsePredictor(frame);
    golomb.encode(bestM);
    golomb.setM(bestM);
    encodeFrame(frame);
}

void Predictor::encodeIntra(const Mat &frame) {
    cout << "Intra" << endl;
    golomb.encode(1); //modo intra
    bool rowsOk = frame.rows % blockSize == 0;
    bool colsOk = frame.cols % blockSize == 0;
    int lastBlockWidth = blockSize - frame.cols % blockSize;
    int lastBlockHeight = blockSize - frame.rows % blockSize;
    int newBlockSize = min(lastBlockHeight, lastBlockWidth);

    for (int i = 0; i <= frame.rows - lastBlockHeight; i = i + blockSize) {
        for (int j = 0; j <= frame.cols - lastBlockWidth; j = j + blockSize) {
            findBestBlockAndEncode(frame(Rect(j, i, blockSize, blockSize)), j, i);
            // quando o ultimo bloco nao tem pixeis suficientes
            // ou seja, so acontece caso o numero de colunas nao seja multiplo de blockSize
            if (!colsOk && j + blockSize == frame.cols - lastBlockWidth){
                for (int k = 0; k < blockSize / newBlockSize; ++k) {
                    findBestBlockAndEncode(frame(Rect(j + blockSize, i + newBlockSize * k, newBlockSize, newBlockSize)), j + blockSize, i);
                }
                break;
            }
        }
        //caso o numero de linhas nao seja o multiplo de blockSize
        if (!rowsOk && i + blockSize == frame.rows - lastBlockHeight){
            i = i + blockSize;
            for (int j = 0; j <= frame.cols - newBlockSize; j = j + newBlockSize) {
                findBestBlockAndEncode(frame(Rect(j, i, newBlockSize, newBlockSize)), j, i);
            }
            break;
        }
    }
}

/**
 * procurar um bloco com menor diferenca, e codifica-lo
 * @param frame bloco do frame atual
 * @param x posicao x do bloco
 * @param y posicao y do bloco
 */
void Predictor::findBestBlockAndEncode(const Mat& block, int x, int y) {
    vector<int> minBlock;
   // Mat minBlock;
    int shiftX = x;
    int shiftY = y;
    int sumMinBlock = 0;
    int lastBlockWidth = blockSize - block.cols % blockSize;
    int lastBlockHeight = blockSize - block.rows % blockSize;
    int newBlockSize = block.rows;

    // 9 é o numero de elementos do array shifts
    for (int i = 0; i < 9; i++){
        for (int j = 1; j <= ray; j++){
            int newX = shifts[i][0] * j + x;
            int newY = shifts[i][1] * j + y;

            if (newX < 0 || newY < 0 || newX > lastFrame.cols - lastBlockWidth || newY > lastFrame.rows - lastBlockHeight){
                break;
            }

           // Mat difference = block - lastFrame(Rect(newX, newY, newBlockSize, newBlockSize));
            vector<int> difference;
            Mat mat = lastFrame(Rect(newX, newY, newBlockSize, newBlockSize));
            int sum = 0;
            for (int k = 0; k < newBlockSize; ++k) {
                for (int l = 0; l < newBlockSize; ++l) {
                    int val = block.ptr(k)[l] - mat.ptr(k)[l];
                    difference.push_back(val);
                    sum += abs(val);
                }
            }
            if (minBlock.empty()){
                minBlock = difference;
                sumMinBlock = sum;
                shiftX = shifts[i][0] * j;
                shiftY = shifts[i][1] * j;
            }
            else{
               // int sumDiff = sum(difference);
                if (sum == 0){
                    shiftX = shifts[i][0] * j;
                    shiftY = shifts[i][1] * j;
                    encodeBlock(difference, shiftX, shiftY, true);
                    return;
                }

                if (sum < sumMinBlock){
                    minBlock = difference;
                    sumMinBlock = sum;
                    shiftX = shifts[i][0] * j;
                    shiftY = shifts[i][1] * j;
                }
            }
            if (i == 0){ // quando o shift for (0, 0)
                break;
            }
        }
    }

    encodeBlock(minBlock, shiftX, shiftY, false); // 4 é o maior indice do array shifts
}


/**
 * codificar um bloco
 * @param frame bloco
 * @param x shift horizontal
 * @param y shift vertical
 */
void Predictor::encodeBlock(const vector<int>& block, int x, int y, bool sameBlock) {
    if (sameBlock){ // se o bloco for exatemente igual ao bloco do frame anterior
        golomb.encode(-1 * (ray + 1));
        golomb.encode(x);
        golomb.encode(y);
        return;
    }

    golomb.encode(x);
    golomb.encode(y);
    //int bestM = getBestMifUsePredictor(block);
    int bestM = mediaFrame(block) + 1;
    golomb.encode(bestM);
    golomb.setM(bestM);
    //encodeFrame(block);
    for (int i : block) {
        golomb.encode(i);
    }
}

void Predictor::encodeBlock(const Mat& block, int x, int y, bool sameBlock) {
    if (sameBlock){ // se o bloco for exatemente igual ao bloco do frame anterior
        golomb.encode(-1 * (ray + 1));
        golomb.encode(x);
        golomb.encode(y);
        return;
    }

    golomb.encode(x);
    golomb.encode(y);
    //int bestM = getBestMifUsePredictor(block);
    int bestM = mediaFrame(block) + 1;
    golomb.encode(bestM);
    golomb.setM(bestM);
    //encodeFrame(block);
    for (int i = 0; i < block.rows; ++i) {
        for (int j = 0; j < block.cols; ++j) {
            int v = block.ptr(i)[j];
            golomb.encode(v);
        }
    }
}

void Predictor::encodeFrame(const Mat &frame) {
    int channels = frame.channels();
    int e, p, v;
    for (int i = 0; i < frame.rows; ++i) {
        for (int j = 0; j < frame.cols; ++j) {
            for (int k = 0; k < channels; ++k) {
                if(channels == 1){
                    p = getPredictValor(frame, -1, i, j);
                    v = frame.ptr<uchar>(i)[j];
                }else{
                    p = getPredictValor(frame, k, i, j);
                    v = frame.ptr<Vec3b>(i)[j][k];
                };
                e = v - p;
                golomb.encode(e);
            }
        }
    }
}

int Predictor::getPredictValor(const Mat& frame, int channel, int line, int col) const {
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

    VideoWriter writer;

    Mat decoded;
    for (int i = 0; i < frames; ++i) {
        bool inter = golomb.decode() == 0;

        if (inter) {
            decoded = decodeInter(rows, cols, channels);
        }
        else{
            decoded = decodeIntra(rows, cols, channels);
        }
        lastFrame = decoded;
        const Mat &bgr = convs.at(format)->decode(decoded);
        if (!writer.isOpened()){
            writer.open("C:/Users/Predator/Desktop/decoded.avi", cv::VideoWriter::fourcc('M', 'J', 'P', 'G'), fps, Size(bgr.cols, bgr.rows), true);
        }
        writer.write(bgr);
        cout << "frame " << i + 1 << " decoded" << endl;
    }
}

void Predictor::flip() {
    golomb.flip();
}

Mat Predictor::decodeInter(int rows, int cols, int channels) {
    Mat frame;
    if (channels == 3)
        frame = Mat::zeros(rows, cols, CV_8UC3);
    else
        frame = Mat::zeros(rows, cols, CV_8UC1);

    int nextM = golomb.decode();
    golomb.setM(nextM);

    for (int i = 0; i < rows; ++i) {
        for (int j = 0; j < cols; ++j) {
            if (channels == 3){
                frame.ptr<Vec3b>(i)[j][0] = getValidPixelValue(getPredictValor(frame,0, i, j) + golomb.decode());
                frame.ptr<Vec3b>(i)[j][1] = getValidPixelValue(getPredictValor(frame, 1, i, j) + golomb.decode());
                frame.ptr<Vec3b>(i)[j][2] = getValidPixelValue(getPredictValor(frame, 2, i, j) + golomb.decode());
            }else{
                frame.ptr<uchar>(i)[j] = getValidPixelValue(getPredictValor(frame, -1, i, j) + golomb.decode());
            }
        }
    }
    return frame;
}

Mat Predictor::decodeIntra(int rows, int cols, int channels){
    Mat frame;
    if (channels == 3)
        frame = Mat::zeros(rows, cols, CV_8UC3);
    else
        frame = Mat::zeros(rows, cols, CV_8UC1);

    bool rowsOk = rows % blockSize == 0;
    bool colsOk = cols % blockSize == 0;
    int lastBlockHeight = blockSize - rows % blockSize;
    int lastBlockWidth = blockSize - cols % blockSize;
    int newBlockSize = min(lastBlockHeight, lastBlockWidth);

    for (int i = 0; i <= rows - lastBlockHeight; i = i + blockSize) {
        for (int j = 0; j <= cols - lastBlockWidth; j = j + blockSize) {
            decodeBlock(frame, j, i, blockSize);
            if (!colsOk && j + blockSize == cols - lastBlockWidth) {
                for (int k = 0; k < blockSize / newBlockSize; ++k) {
                    decodeBlock(frame, j + blockSize, i + newBlockSize * k, newBlockSize);
                }
                break;
            }
        }

        //caso o numero de linhas nao seja o multiplo de blockSize
        if (!rowsOk && i + blockSize == rows - lastBlockHeight){
            i = i + blockSize;
            for (int j = 0; j <= cols - newBlockSize; j = j + newBlockSize) {
                decodeBlock(frame, j, i, newBlockSize);
            }
            break;
        }
    }
    return frame;
}

void Predictor::decodeBlock(Mat& frame, int x, int y, int size) {
    int shiftX;
    int shiftY;
    int X;
    int Y;
    int channels = frame.channels();
    int flag = golomb.decode();
    bool isSameBlock = false;
    if (flag == -1 * (ray + 1)){
        shiftX = golomb.decode();
        shiftY = golomb.decode();
        isSameBlock = true;
    }else{
        shiftX = flag;
        shiftY = golomb.decode();
        int nextM = golomb.decode();
        golomb.setM(nextM);
    }

    Mat block = lastFrame(Rect(x + shiftX, y + shiftY, size, size));
    Mat diffBlock = Mat::zeros(size, size, frame.type());

    for (int i = 0; i < size; ++i) {
        for (int j = 0; j < size; ++j) {
            Y = y + i;
            X = x + j;
            if (!isSameBlock){
                if (channels == 1){
                   /* int diff = getPredictValor(diffBlock, -1, i, j) + golomb.decode();
                    diffBlock.ptr<uchar>(i)[j] = getValidPixelValue(diff);
                    frame.ptr<uchar>(Y)[X] = getValidPixelValue(diff + block.ptr<uchar>(i)[j]);*/
                    frame.ptr<uchar>(Y)[X] = getValidPixelValue(golomb.decode() + block.ptr<uchar>(i)[j]);
                }
                else{
                    int diff0 = getPredictValor(diffBlock, 0, i, j) + golomb.decode();
                    int diff1 = getPredictValor(diffBlock, 1, i, j) + golomb.decode();
                    int diff2= getPredictValor(diffBlock, 2, i, j) + golomb.decode();

                    diffBlock.ptr<Vec3b>(Y)[X][0] = diff0;
                    diffBlock.ptr<Vec3b>(Y)[X][1] = diff1;
                    diffBlock.ptr<Vec3b>(Y)[X][2] = diff2;

                    frame.ptr<Vec3b>(Y)[X][0] = getValidPixelValue(diff0 + block.ptr<Vec3b>(i)[j][0]);
                    frame.ptr<Vec3b>(Y)[X][1] = getValidPixelValue(diff1 + block.ptr<Vec3b>(i)[j][1]);
                    frame.ptr<Vec3b>(Y)[X][2] = getValidPixelValue(diff2 + block.ptr<Vec3b>(i)[j][2]);
                }
            }else{
                if (channels == 1){
                    frame.ptr<uchar>(Y)[X] = getValidPixelValue(block.ptr<uchar>(i)[j]);;
                }else{
                    frame.ptr<Vec3b>(Y)[X][0] = getValidPixelValue(block.ptr<Vec3b>(i)[j][0]);
                    frame.ptr<Vec3b>(Y)[X][1] = getValidPixelValue(block.ptr<Vec3b>(i)[j][1]);
                    frame.ptr<Vec3b>(Y)[X][2] = getValidPixelValue(block.ptr<Vec3b>(i)[j][2]);
                }
            }
        }
    }
}

int Predictor::sum(const vector<int> &block) {
   /* int total = 0;
    int v = 0;
    int channels = frame.channels();
    for (int i = 0; i < frame.rows; ++i) {
        for (int j = 0; j < frame.cols; ++j) {
            for (int k = 0; k < channels; ++k) {
                if (frame.channels() == 1){
                    v = frame.ptr<uchar>(i)[j];
                }
                else {
                    v = frame.ptr<Vec3b>(i)[j][k];
                }
                total += abs(v);
            }
        }
    }*/
   int total = 0;
    for (int i : block) {
        total += abs(i);
    }
    return total;
}
int Predictor::sum(const Mat &frame) {
     int total = 0;
     int v = 0;
     int channels = frame.channels();
     for (int i = 0; i < frame.rows; ++i) {
         for (int j = 0; j < frame.cols; ++j) {
             for (int k = 0; k < channels; ++k) {
                 if (frame.channels() == 1){
                     v = frame.ptr<uchar>(i)[j];
                 }
                 else {
                     v = frame.ptr<Vec3b>(i)[j][k];
                 }
                 total += abs(v);
             }
         }
     }
    return total;
}
int Predictor::getBestMifUsePredictor(const Mat &frame) {
    int channels = frame.channels();
    int total = 0, e;
    for (int i = 0; i < frame.rows; ++i) {
        for (int j = 0; j < frame.cols; ++j) {
            for (int k = 0; k < channels; ++k) {
                uchar p, v;
                if(channels == 1){
                    p = getPredictValor(frame, -1, i, j);
                    v = frame.ptr<uchar>(i)[j];
                }else{
                    p = getPredictValor(frame, k, i, j);
                    v = frame.ptr<Vec3b>(i)[j][k];
                };

                e = v - p;
                if (e < 0){
                    e = -e * 2 - 1;
                }else{
                    e = e * 2;
                }
                total += e;
            }
        }
    }
    return (total / (frame.rows * frame.cols * frame.channels())) + 1;
}

int Predictor::mediaFrame(const Mat &frame){
    int channels = frame.channels();
    int total = 0, v;
    for (int i = 0; i < frame.rows; ++i) {
        for (int j = 0; j < frame.cols; ++j) {
            for (int k = 0; k < channels; ++k) {
                if(channels == 1){
                    v = frame.ptr<uchar>(i)[j];
                }else{
                    v = frame.ptr<Vec3b>(i)[j][k];
                };

                if (v < 0){
                    v = -v * 2 - 1;
                }else{
                    v = v * 2;
                }
                total += v;
            }
        }
    }
    return total / (frame.rows * frame.cols * frame.channels());
}

int Predictor::mediaFrame(const vector<int> &frame){
    int total = 0;
    for (int v : frame) {
            if (v < 0){
                v = -v * 2 - 1;
            }else{
                v = v * 2;
            }
            total += v;
    }
    return total / (int) frame.size();
}

int Predictor::getValidPixelValue(int val) {
    if (val < 0)
        return 0;

    if (val > 255)
        return 255;
    return val;
}
