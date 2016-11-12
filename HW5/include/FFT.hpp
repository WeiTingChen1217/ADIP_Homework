//
//  FFT.hpp
//  ADIP_Homework
//
//  Created by WeiTingChen on 11/11/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#ifndef FFT_hpp
#define FFT_hpp

#include <stdio.h>
#include "opencv2/core/core.hpp"
#include "opencv2/imgproc/imgproc.hpp"
#include "opencv2/highgui/highgui.hpp"

using namespace cv;
using namespace std;

Mat FFT_opencv(Mat src, Mat dst);


#endif /* FFT_hpp */
