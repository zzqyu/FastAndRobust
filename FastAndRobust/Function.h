#include <algorithm>
#include <opencv2/opencv.hpp>
#include <random>
using namespace std;
using namespace cv;
#define PI 3.1415926535f

#define PI 3.1415926535f
#pragma once
/**
@note 3채널 RGB 영상 중에서 bmp, raw파일 형식을 읽어오는 함수이다. 
@param path 읽어올 bmp형식의 파일의 경로
@return Mat 형식의 읽어온 이미지 영상
**/
cv::Mat my_imread(std::string path);
/**
@note Mat 형식의 3채널 RGB영상을 raw파일로 저장하는 함수이다.
@param path 저장할 raw형식의 파일의 경로
@param image 저장할 Mat 형식의 이미지 영상
**/
void my_imWrite(std::string path, cv::Mat image);
/**
@note Mat 형식의 3채널 RGB영상을 리사이징 하는 함수
@param src 리사이징할 원본 영상
@param dst 리사이징한 영상
@param dst_size 리사이징 영상 사이즈
**/
void my_resize(cv::Mat src, cv::Mat dst, cv::Size dst_size);
/**
@note Bilinear방식의 보간법을 적용하는 함수
@param src 리사이징할 원본 영상
@param h_Cvt 리사이징한 영상과 대응하는 원본영상의 y좌표
@param w_Cvt 리사이징한 영상과 대응하는 원본영상의 x좌표
@param rgb rgb인덱스
@return 보간계산한 픽셀 값
**/
uchar BilinearInterpolation(cv::Mat src, double h_Cvt, double w_Cvt, int rgb);
/**
@note dst = src1 * a + src2 * b;
@param src1 Mat 영상 1
@param a Mat 영상 1에 곱할 상수
@param src2 Mat 영상 2
@param a Mat 영상 2에 곱할 상수
@param dst 계산 결과 Mat 영상
**/
void my_addWeighted(cv::Mat src1, double a, cv::Mat src2, double b, cv::Mat dst);
cv::Mat addGaussianNoise(cv::Mat& src, double sigma);
cv::Mat addspikenoise(cv::Mat& src, int val);