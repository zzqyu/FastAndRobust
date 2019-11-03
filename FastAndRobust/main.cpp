#include "opencv2/opencv.hpp"
#include "DegradedImage.h"
#include "SR.h"
#include "Function.h"

using namespace std;
using namespace cv;

int main() {
	Mat image = my_imread("lenna.bmp");//입력 영상
	Mat result;//결과 영상

	const int imageCount = 16;//생성할 저하 이미지 개수
	const int scale = 4;//배율
	SparseMat DHFs[imageCount];//DHF들
	Mat degImages[imageCount];//저하이미지들

	//저하이미지 및 DHF 생성
	createDegradedImagesAndDHFs(DHFs, degImages, image, scale, imageCount);

	//(2) super resolution
	// 베타: 가장 가파른 하강 방법의 증상 값
	// 람다: 데이터 용어와 평탄도 용어의 균형을 위한 가중치 파라미터
	// 알파: btv에서 공간 분포의 매개 변수
	// btv 커널 크기: btv 필터의 커널 크기
	// degimage: 저하된 이미지
	// dest: 결과
	// A : DHF
	// image_count: 입력이미지 개수
	result = superResolutionImage(image, degImages, DHFs, imageCount,
		180,//number of iteration
		1.3f,//beta
		0.03f,//lambda
		0.7f,//alpha
		scale,
		Size(7, 7)//btv kernel size
	);

	my_imWrite("sr.raw", result);
	return 0;
}