#include "opencv2/opencv.hpp"
using namespace cv;
#pragma once

/**
@note 어떤 범위의 수 중에서 랜덤으로 숫자(float)를 생성하는 함수
@param start 범위 시작 수
@param end 범위 끝 수
@return 생성된 수
**/
float randomDouble(float start, float end);
/**
@note 두 수를 비교해 다음과 같은 값을 리턴하는 연산 함수
@code if(a>b) 1.0, else if(a==b) 0.0, else -1.0
@param a operand1
@param b operand2
@return result
**/
float signFloat(float a, float b);
/**
@note 두 영상의 모든 픽셀에 대해 sign 연산을 하는 함수
@param src1 영상1
@param src2 영상2
@return result
**/
Mat subtractSign(Mat src1, Mat src2);
/**
@note DHF와 영상을 곱하는 연산을 하는 함수
@param DHF 
@param src 영상
@param dest 연산 결과
@param isToSR 업샘플링(true) 다운샘플링(false) 여부, false값이 default
**/
void mulDHF(SparseMat& DHF, Mat& src, Mat& dest, bool isToSR = false);
/**
기능에 상관없음
@note PSNR을 구하는 함수
@param src1 영상1
@param src2 영상2
@param bb ?
**/
double getPSNR(Mat& src1, Mat& src2, int bb);
