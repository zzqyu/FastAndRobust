#include "opencv2/opencv.hpp"
using namespace cv;
#pragma once

/**
@note � ������ �� �߿��� �������� ����(float)�� �����ϴ� �Լ�
@param start ���� ���� ��
@param end ���� �� ��
@return ������ ��
**/
float randomDouble(float start, float end);
/**
@note �� ���� ���� ������ ���� ���� �����ϴ� ���� �Լ�
@code if(a>b) 1.0, else if(a==b) 0.0, else -1.0
@param a operand1
@param b operand2
@return result
**/
float signFloat(float a, float b);
/**
@note �� ������ ��� �ȼ��� ���� sign ������ �ϴ� �Լ�
@param src1 ����1
@param src2 ����2
@return result
**/
Mat subtractSign(Mat src1, Mat src2);
/**
@note DHF�� ������ ���ϴ� ������ �ϴ� �Լ�
@param DHF 
@param src ����
@param dest ���� ���
@param isToSR �����ø�(true) �ٿ���ø�(false) ����, false���� default
**/
void mulDHF(SparseMat& DHF, Mat& src, Mat& dest, bool isToSR = false);
/**
��ɿ� �������
@note PSNR�� ���ϴ� �Լ�
@param src1 ����1
@param src2 ����2
@param bb ?
**/
double getPSNR(Mat& src1, Mat& src2, int bb);
