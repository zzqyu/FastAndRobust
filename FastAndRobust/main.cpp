#include "opencv2/opencv.hpp"
#include "DegradedImage.h"
#include "SR.h"
#include "Function.h"

using namespace std;
using namespace cv;

int main() {
	Mat image = my_imread("lenna.bmp");//�Է� ����
	Mat result;//��� ����

	const int imageCount = 16;//������ ���� �̹��� ����
	const int scale = 4;//����
	SparseMat DHFs[imageCount];//DHF��
	Mat degImages[imageCount];//�����̹�����

	//�����̹��� �� DHF ����
	createDegradedImagesAndDHFs(DHFs, degImages, image, scale, imageCount);

	//(2) super resolution
	// ��Ÿ: ���� ���ĸ� �ϰ� ����� ���� ��
	// ����: ������ ���� ��ź�� ����� ������ ���� ����ġ �Ķ����
	// ����: btv���� ���� ������ �Ű� ����
	// btv Ŀ�� ũ��: btv ������ Ŀ�� ũ��
	// degimage: ���ϵ� �̹���
	// dest: ���
	// A : DHF
	// image_count: �Է��̹��� ����
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