//
//  main.cpp
//  HW6
//
//  Created by WeiTingChen on 11/17/16.
//  Copyright © 2016 WeiTingChen. All rights reserved.
//

#include <iostream>
#include "FFT.hpp"
#include "OpenRawFile.hpp"
#include "gaussianNoise.hpp"

void createFilter(double gKernel[][5])
{
    // set standard deviation to 1.0
    double sigma = 1.0;
    double r, s = 2.0 * sigma * sigma;
    
    // sum is for normalization
    double sum = 0.0;
    
    // generate 5x5 kernel
    for (int x = -2; x <= 2; x++)
    {
        for(int y = -2; y <= 2; y++)
        {
            r = sqrt(x*x + y*y);
            gKernel[x + 2][y + 2] = (exp(-(r*r)/s))/(M_PI * s);
            sum += gKernel[x + 2][y + 2];
        }
    }
    
    // normalize the Kernel
    for(int i = 0; i < 5; ++i)
        for(int j = 0; j < 5; ++j)
            gKernel[i][j] /= sum;
    
}

int main(int argc, const char * argv[]) {
    // insert code here...
    std::cout << "This is homework6_2_a.\n\n";
    
    char str_input_image_1[] = "../../doc/HW6/lena_256.raw";
//    char str_input_image_1[] = "../../doc/HW4/lena_512.raw";


    int sizeSrc_width = 256;
    int sizeSrc_height = 256;
    
    CvMat *image_cvMat_1 = cvCreateMat(sizeSrc_width, sizeSrc_height, CV_8UC1);
    
    OpenRAW(str_input_image_1, image_cvMat_1);
    
    // Chamge the format to Mat
    Mat inputImg_1 = cv::cvarrToMat(image_cvMat_1, true);
    
    
//    // add Gaussian noise
////    Gaussian_Noise(inputImg_1);
//    
//    copyMakeBorder(inputImg_1, inputImg_1,
//                   2, 2, 2, 2,
//                   BORDER_CONSTANT, // 外推的值為常數，常在仿射變換、透視變換中使用
//                   Scalar::all(0)); // 為了效率，所以對影像邊界拓展
//    
//    for(int i = 2; i < (inputImg_1.rows - 2); i++)
//        for(int j = 2; j < (inputImg_1.cols - 2); j++){
//            
//        }
//    
//    double gKernel[5][5];
//    createFilter(gKernel);
//    for(int i = 0; i < 5; ++i)
//    {
//        for (int j = 0; j < 5; ++j)
//            cout<<gKernel[i][j]<<"\t";
//        cout<<endl;
//    }
    
    
    imshow("Imput_1", inputImg_1);
    moveWindow("Imput_1", 0, 0);
    
    waitKey(0);

    // Save image
    imwrite("../../HW6/output/lena_256.jpg", inputImg_1);
    
    return 0;
}
