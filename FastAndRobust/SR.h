#include "opencv2/opencv.hpp"
using namespace cv;
#pragma once

/**
@note ���� ���� ���� �̹����� �̿��ؼ� ������ �����ϴ� �Լ�
@param ideal PSNR����� ���� ���� ���� ����
@param src ���� ���� ���� �̹���
@param DHF �����̹����� ���� DHF ��
@param srcNum �����̹����� ����
@param iteration ���� �ݺ� Ƚ��
@param beta 
@param lambda 
@param alpha 
@param scale Ȯ�� ����
@param rKernelSize BTV����ȭ Ŀ�� ũ��
@return ���� ����
**/
Mat superResolutionImage(Mat ideal, Mat src[], SparseMat DHF[], const int srcNum, int iteration, float beta, float lambda, float alpha, float scale, Size rKernelSize);
/**
@note BTV ����ȭ�� �ϴ� �Լ�
@param srcVec ����ȭ�� ����(1����)
@param srcSize ������ ������
@param kernel ����ȭ Ŀ�� ũ��
@param alpha 
@param lambda 
@return ����ȭ ��� ����
**/
Mat btvRegularization(Mat& srcVec, Size srcSize, Size kernel, float alpha, float lambda);
