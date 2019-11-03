#include "opencv2/opencv.hpp"
using namespace cv;
#pragma once
/**
@note 품질 저하 이미지와 그에 따른 DHF를 만드는 함수
@param DHFs 저하 이미지와 대응하는 DHF들
@param degradedImages 저하이미지들
@param src 원본 영상
@param scale 축소 배율
@param imageCount 생성할 저하 이미지 개수
**/
void createDegradedImagesAndDHFs(SparseMat* DHFs, Mat* degradedImages, Mat src, int scale, int imageCount);
/**
@note DHF를 만드는 함수
@param src 원본 영상
@param shift 영상을 이동시킬 크기
@param scale 축소 배율
@return DHF
**/
SparseMat createDHF(Mat src, Point2d shift, int scale);
/**
@note 저하이미지를 만드는 함수
@param src 원본 영상
@param DHF 
@param scale 축소 배율
@return 저하된 이미지
**/
Mat createDegradedImage(Mat src, SparseMat DHF, int scale);
