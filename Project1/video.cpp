#include "video.h"
Video::Video(VideoCapture capt)
{
    this->capt = capt;
    this->size = Size((int) capt.get(CAP_PROP_FRAME_WIDTH),
                        (int) capt.get(CAP_PROP_FRAME_HEIGHT));

}
