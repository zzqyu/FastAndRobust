////note: Windows.h 헤더가 opencv 다음에 include되면 오류를 일으킵니다.
#include<Windows.h> 
#include "Function.h"
using namespace std;
/*Mat my_imread(std::string path) {
	//return cv::imread(path);
	FILE * fp_in;
	fopen_s(&fp_in, path.c_str(), "rb");

	// bmp 이미지의 헤더 읽기
	unsigned char* header = new unsigned char[54];
	fread(header, 54, sizeof(unsigned char), fp_in);
	// header에 접근하여 width, height 읽어 오기
	int nHeight_in = *(int*)(header + 22);
	int nWidth_in = *(int*)(header + 18);
	// Mat 이미지 생성하기
	Mat newImage(nHeight_in, nWidth_in, CV_8UC3);

	int Width_rgb = nWidth_in * 3; // RGB 때문에 width * 3
	for (int h = nHeight_in-1; h >=0 ; h--)
		fread(newImage.data + h * Width_rgb, sizeof(unsigned char), nWidth_in * 3,  fp_in);
	fclose(fp_in);
	return newImage;
}*/
Mat my_imread(std::string path) {
	FILE * fp_in;
	fopen_s(&fp_in, path.c_str(), "rb");

	// bmp 이미지의 헤더 읽기
	unsigned char* header = new unsigned char[54];
	fread(header, 54, sizeof(unsigned char), fp_in);
	// header에 접근하여 width, height 읽어 오기
	int nHeight_in = *(int*)(header + 22);
	int nWidth_in = *(int*)(header + 18);

	unsigned char * RGB = new unsigned char[nHeight_in * nWidth_in * 3]; 
	unsigned char * RGB_RVS = new unsigned char[nHeight_in * nWidth_in * 3];

	// RGB value 읽어오기
	fread(RGB, nHeight_in * nWidth_in * 3, sizeof(unsigned char), fp_in);
	fclose(fp_in);

	// 상하 반전
	int Width_rgb = nWidth_in * 3; // RGB 때문에 width * 3
	for (int h = 0; h < nHeight_in; h++) {
		for (int w = 0; w < Width_rgb; w++) {
			int idx_rgb = ((nHeight_in - 1 - h) * Width_rgb + w);
			int idx_rgb_ivs = h * Width_rgb + w;
			RGB_RVS[idx_rgb_ivs] = RGB[idx_rgb];
		}
	}
	// Mat 이미지 생성하기
	Mat newImage = Mat::Mat(nHeight_in, nWidth_in, CV_8UC3, RGB_RVS);
	return newImage;
}

void my_imWrite(std::string path, Mat image) {
	//cv::imwrite(path, image);
	/*int nHeight = image.rows;
	int nWidth = image.cols;

	
	for (int idx = 0; idx < nHeight* nWidth * 3; idx += 3) 
		swap(image.data[idx], image.data[idx+2]);
	
	FILE * fp_out;
	fopen_s(&fp_out, path.c_str(), "wb");
	fwrite(image.data, nHeight * nWidth * 3, sizeof(unsigned char), fp_out);
	fclose(fp_out);*/
	int nHeight = image.rows;
	int nWidth = image.cols;

	unsigned char * RGB = new unsigned char[nHeight * nWidth * 3];
	// RGB = image.data;
	for (int idx = 0; idx < nHeight * nWidth * 3; idx++) {
		if (idx % 3 == 0) { // R
			RGB[idx] = image.data[idx + 2];
		}
		if (idx % 3 == 1) { // G
			RGB[idx] = image.data[idx];
		}
		if (idx % 3 == 2) { // B
			RGB[idx] = image.data[idx - 2];
		}
	}
	FILE * fp_out;
	fopen_s(&fp_out, path.c_str(), "wb");
	fwrite(RGB, nHeight * nWidth * 3, sizeof(unsigned char), fp_out);
	fclose(fp_out);

}

void my_resize(Mat src, Mat dst, Size dst_size) {
	//cv::resize(src, dst, dst_size, INTER_CUBIC);
	double dScaleRate = (dst_size.height + 0.0) / src.rows;

	for (int h = 0; h < dst_size.height; h++)
	{
		for (int w = 0; w < dst_size.width; w++)
		{
			double h_Ori = h / dScaleRate;
			double w_Ori = w / dScaleRate;
			if (h_Ori >= 0 && w_Ori >= 0 && h_Ori <= dst_size.height && w_Ori <= dst_size.width)
			{
				dst.at<Vec3b>(h, w)[0] = BilinearInterpolation(src, h_Ori, w_Ori, 0);
				dst.at<Vec3b>(h, w)[1] = BilinearInterpolation(src, h_Ori, w_Ori, 1);
				dst.at<Vec3b>(h, w)[2] = BilinearInterpolation(src, h_Ori, w_Ori, 2);
			}
		}
	}
}

uchar BilinearInterpolation(Mat src, double h_Cvt, double w_Cvt, int rgb) {

	//interpolation 보간법
	int up_height, up_width;
	int down_height, down_width;
	double reset_height = h_Cvt;
	double reset_width = w_Cvt;
	int interpolation_height;
	int interpolation_width;
	int nWidth_Ori = src.cols, nHeight_Ori = src.rows;

	double d1, d2, d3, d4; // 각 꼭짓점 간의 거리
	double linear1, linear2;
	double final_linear;
	double final_x;
	double final_y;

	up_height = ceil(reset_height);
	up_width = ceil(reset_width);
	down_height = floor(reset_height);
	down_width = floor(reset_width);

	if (down_width == nWidth_Ori - 1 && down_height == nHeight_Ori - 1 || up_width == nWidth_Ori - 1 && up_height == nHeight_Ori - 1) {
		return src.at<Vec3b>(nHeight_Ori - 1, nWidth_Ori - 1)[rgb];
	}
	else if (down_width > nWidth_Ori - 1 || up_width > nWidth_Ori - 1 || down_height > nHeight_Ori - 1 || up_height > nHeight_Ori - 1) {
		return 0;
	}
	else {
		double p1 = src.at<Vec3b>(down_height, down_width)[rgb];
		double p2 = src.at<Vec3b>(down_height, up_width)[rgb];
		double p3 = src.at<Vec3b>(up_height, down_width)[rgb];
		double p4 = src.at<Vec3b>(up_height, up_width)[rgb];

		d1 = pow(down_height - reset_height, 2.0) + pow(down_width - reset_width, 2.0);
		d2 = pow(up_height - reset_height, 2.0) + pow(down_width - reset_width, 2.0);
		d3 = pow(down_height - reset_height, 2.0) + pow(up_width - reset_width, 2.0);
		d4 = pow(up_height - reset_height, 2.0) + pow(up_width - reset_width, 2.0);

		double distance[] = { d1,d2,d3,d4 };
		int f = 0;
		double find_min = distance[f];
		for (f = 1; f < 4; f++)
			if (distance[f] < find_min) find_min = distance[f];

		if (f == 0) {
			interpolation_height = down_height;
			interpolation_width = down_width;
		}
		else if (f == 1) {
			interpolation_height = up_height;
			interpolation_width = down_width;
		}
		else if (f == 2) {
			interpolation_height = down_height;
			interpolation_width = up_width;
		}
		else {
			interpolation_height = up_height;
			interpolation_width = up_width;
		}
		if (w_Cvt > interpolation_width)
			final_x = (w_Cvt - interpolation_width);
		else
			final_x = (interpolation_width - w_Cvt);

		if (h_Cvt > interpolation_height)
			final_y = (h_Cvt - interpolation_height);
		else
			final_y = (interpolation_height - h_Cvt);
		linear1 = (1.0 - final_x)*p1 + final_x * p2;
		linear2 = (1.0 - final_x)*p3 + final_x * p4;
		final_linear = (1.0 - final_y)*linear1 + final_y * linear2;
	}
	return final_linear;
}


void my_addWeighted(Mat src1, double a, Mat src2, double b, Mat dst) {
	//cv::addWeighted(src1, a, src2, b, 0.0, dst);
	if (src1.size() == src2.size()
		&& src2.size() == dst.size()) {
		for (int h = 0; h < src1.rows; h++) {
			int index = h * src1.cols;
			for (int w = 0; w < src2.cols; w++) {
				float* s1 = src1.ptr<float>(index + w);
				float* s2 = src2.ptr<float>(index + w);
				float* d = dst.ptr<float>(index + w);
				for (int ch = 0; ch < 3; ch++)
					d[ch] = s1[ch] * a + s2[ch] * b;
			}
		}
	}
}

//확률밀도 함수를 통해 잡음을 획득하는 함수.
float GetNoise(float *PDF, int nLength) {
	int n;
	int Center = nLength / 2;
	float fRand, fComp, fTemp = 0;
	float x = 0, fDiff;
	float *CDF = new float[nLength];

	CDF[0] = 0;

	fRand = (float)rand() / (RAND_MAX + 1);

	for (n = 1; n < nLength; n++) {
		CDF[n] = (PDF[n] + PDF[n - 1]) / 2 + CDF[n - 1];
		fDiff = fRand - CDF[n];
		if (fDiff < 0) {
			x = ((float)n - Center);
			break;
		}
	}
	fComp = (fRand - CDF[n - 1]) / (CDF[n] - CDF[n - 1]);

	delete[] CDF;

	return x + fComp;
}
//가우시안 확률밀도 분포를 얻는 함수.
void GetGaussianPDF(float* fPDF, int length, float fMean, float fStDev) {
	int n;
	int Center = length / 2;
	float x;

	for (n = 0; n < length; n++) {
		x = (float)(n - Center);//가우시안 확률 분포의 중심으로부터 떨어진 거리(방향값도 가짐)
		//fStDev: 시그마 fMean: u
		fPDF[n] = (1 / ((float)sqrt(2 * PI)*fStDev)) * exp(-pow(x - fMean, 2) / (2 * fStDev * fStDev));
	}
}


Mat addGaussianNoise(Mat& In, double sigma)
{
	int height = In.rows, width = In.cols;  //영상의 높이, 너비 128/128
	float gaussian_value = 0;				//원본 픽셀값 + 잡음값을 저장하는 변수.
	float *fPDF = new float[width] {0.0f, };  //확률밀도함수
	Mat dst(height, width, CV_8UC3);		//반환값, 원본 + 가우시안 잡음
	unsigned char **rgb_uchar = new unsigned char*[3];//rgb를 unsigned char형으로 분리 저장할 공간.
	Mat rgb_mat[3];	//원본의 rgb를 Mat형으로 분리 저장.
	Vec3b rgb;//3채널 픽셀값을 저장할 임시 변수.

	for (int i = 0; i < 3; i++) {//2차원 배열로 3차원 표현 [3][height * width]
		rgb_uchar[i] = new unsigned char[height*width];
		memset(rgb_uchar[i], 0, sizeof(unsigned char)*(height*width));
	}

	GetGaussianPDF(fPDF, width, 0.0, sigma);//가우시안 확률밀도 함수 획득.
	split(In, rgb_mat);//원본 영상을 rgb component로 분리.

	//mat형식의 rgb데이터를 unsigned char형으로 저장.
	for (int h = 0; h < height; h++)
		for (int w = 0; w < width; w++)
			for (int i = 0; i < 3; i++)
				rgb_uchar[i][h*width + w] = rgb_mat[i].at<unsigned char>(h, w);

	srand(GetTickCount());

	//r,g,b채널을 순회하며 각각 잡음을 추가.
	for (int h = 0; h < height; h++)
		for (int w = 0; w < width; w++) {
			for (int i = 0; i < 3; i++) {
				gaussian_value = (float)rgb_uchar[i][h*width + w] + GetNoise(fPDF, width);
				//Veb3b형 변수에 r,g,b각 채널의 픽셀값을 저장한 뒤, 반환할 변수에 대입.
				rgb[i] = saturate_cast<unsigned char>(gaussian_value);
			}
			dst.at<Vec3b>(h, w) = rgb;
		}

	//동적할당 해제
	for (int i = 0; i < 3; i++)
		delete rgb_uchar[i];
	delete rgb_uchar;
	delete fPDF;

	return dst;
}

Mat addspikenoise(Mat& src, int val)
{
	Mat dst(src.rows, src.cols, CV_8UC3);
	int height = src.rows, width = src.cols;
	unsigned char **rgb_uchar = new unsigned char*[3];//rgb데이터를 uchar형으로 저장할 변수.
	Mat rgb_mat[3];	//원본의 rgb를 Mat형으로 분리 저장.
	Vec3b rgb;//3채널 픽셀값을 저장할 임시 변수.

	for (int i = 0; i < 3; i++) {//2차원 배열로 3차원 표현 [3][height * width]
		rgb_uchar[i] = new unsigned char[height*width];
		memset(rgb_uchar[i], 0, sizeof(unsigned char)*(height*width));
	}
	split(src, rgb_mat);//원본 영상을 rgb component로 분리.

	//mat형식의 rgb데이터를 unsigned char형으로 저장.
	for (int h = 0; h < height; h++)
		for (int w = 0; w < width; w++)
			for (int i = 0; i < 3; i++)
				rgb_uchar[i][h*width + w] = rgb_mat[i].at<unsigned char>(h, w);

#pragma omp parallel for
	for (int h = 0; h < height; h++) {
		random_device                  rand_dev;
		mt19937                        generator(rand_dev());
		uniform_int_distribution<int> uniform_rand(0, val);

		for (int w = 0; w < width; w++)
			if (uniform_rand(generator) < 1)
				for (int i = 0; i < 3; i++)
					rgb_uchar[i][h*width + w + 3] = 255;
	}

	for (int h = 0; h < height; h++) {
		for (int w = 0; w < width; w++) {
			for (int i = 0; i < 3; i++) {
				rgb[i] = rgb_uchar[i][h*width + w];
			}
			dst.at<Vec3b>(h, w) = rgb;
		}
	}

	for (int i = 0; i < 3; i++)
		delete rgb_uchar[i];
	delete rgb_uchar;

	return dst;
}
