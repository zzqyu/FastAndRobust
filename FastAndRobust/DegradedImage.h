#include "opencv2/opencv.hpp"
using namespace cv;
#pragma once
/**
@note ǰ�� ���� �̹����� �׿� ���� DHF�� ����� �Լ�
@param DHFs ���� �̹����� �����ϴ� DHF��
@param degradedImages �����̹�����
@param src ���� ����
@param scale ��� ����
@param imageCount ������ ���� �̹��� ����
**/
void createDegradedImagesAndDHFs(SparseMat* DHFs, Mat* degradedImages, Mat src, int scale, int imageCount);
/**
@note DHF�� ����� �Լ�
@param src ���� ����
@param shift ������ �̵���ų ũ��
@param scale ��� ����
@return DHF
**/
SparseMat createDHF(Mat src, Point2d shift, int scale);
/**
@note �����̹����� ����� �Լ�
@param src ���� ����
@param DHF 
@param scale ��� ����
@return ���ϵ� �̹���
**/
Mat createDegradedImage(Mat src, SparseMat DHF, int scale);
