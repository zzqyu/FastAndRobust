#include "opencv2/opencv.hpp"
using namespace cv;
#pragma once

/**
@note 여러 개의 저하 이미지를 이용해서 영상을 복원하는 함수
@param ideal PSNR계산을 위해 비교할 원본 영상
@param src 여러 개의 저하 이미지
@param DHF 저하이미지에 대한 DHF 값
@param srcNum 저하이미지의 개수
@param iteration 연산 반복 횟수
@param beta 
@param lambda 
@param alpha 
@param scale 확대 배율
@param rKernelSize BTV정규화 커널 크기
@return 복원 영상
**/
Mat superResolutionImage(Mat ideal, Mat src[], SparseMat DHF[], const int srcNum, int iteration, float beta, float lambda, float alpha, float scale, Size rKernelSize);
/**
@note BTV 정규화를 하는 함수
@param srcVec 정규화할 영상(1차원)
@param srcSize 영상의 사이즈
@param kernel 정규화 커널 크기
@param alpha 
@param lambda 
@return 정규화 결과 영상
**/
Mat btvRegularization(Mat& srcVec, Size srcSize, Size kernel, float alpha, float lambda);
