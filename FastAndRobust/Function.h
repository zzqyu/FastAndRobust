#include <algorithm>
#include <opencv2/opencv.hpp>
#include <random>
using namespace std;
using namespace cv;
#define PI 3.1415926535f

#define PI 3.1415926535f
#pragma once
/**
@note 3ä�� RGB ���� �߿��� bmp, raw���� ������ �о���� �Լ��̴�. 
@param path �о�� bmp������ ������ ���
@return Mat ������ �о�� �̹��� ����
**/
cv::Mat my_imread(std::string path);
/**
@note Mat ������ 3ä�� RGB������ raw���Ϸ� �����ϴ� �Լ��̴�.
@param path ������ raw������ ������ ���
@param image ������ Mat ������ �̹��� ����
**/
void my_imWrite(std::string path, cv::Mat image);
/**
@note Mat ������ 3ä�� RGB������ ������¡ �ϴ� �Լ�
@param src ������¡�� ���� ����
@param dst ������¡�� ����
@param dst_size ������¡ ���� ������
**/
void my_resize(cv::Mat src, cv::Mat dst, cv::Size dst_size);
/**
@note Bilinear����� �������� �����ϴ� �Լ�
@param src ������¡�� ���� ����
@param h_Cvt ������¡�� ����� �����ϴ� ���������� y��ǥ
@param w_Cvt ������¡�� ����� �����ϴ� ���������� x��ǥ
@param rgb rgb�ε���
@return ��������� �ȼ� ��
**/
uchar BilinearInterpolation(cv::Mat src, double h_Cvt, double w_Cvt, int rgb);
/**
@note dst = src1 * a + src2 * b;
@param src1 Mat ���� 1
@param a Mat ���� 1�� ���� ���
@param src2 Mat ���� 2
@param a Mat ���� 2�� ���� ���
@param dst ��� ��� Mat ����
**/
void my_addWeighted(cv::Mat src1, double a, cv::Mat src2, double b, cv::Mat dst);
cv::Mat addGaussianNoise(cv::Mat& src, double sigma);
cv::Mat addspikenoise(cv::Mat& src, int val);