#include "Calculate.h"

float randomDouble(float start, float end) {
	return start + (float)rand() / RAND_MAX * (end - start);
}
float signFloat(float a, float b)
{
	return (a > b) ? 1.0f : (a < b) ? -1.0f : 0.0f;
}
Mat subtractSign(Mat src1, Mat src2){
	Mat dst(src1.size(), src1.type());
	for (int h = 0; h < src1.rows; h++)	{
		float* s1 = src1.ptr<float>(h);
		float* s2 = src2.ptr<float>(h);
		float* d = dst.ptr<float>(h);
		for (int c = 0; c < 3; c++) 
			d[c] = signFloat(s1[c], s2[c]);
	}
	return dst;
}
void mulDHF(SparseMat& DHF, Mat& src, Mat& dest, bool isToSR)
{
	dest.setTo(0); // 결과 이미지 0으로 초기화
	int srcNodeIndex = 0, dstNodeIndex = 1;
	//DHF index 값 조정, ** DHF(큰이미지좌표, 작은이미지 좌표)
	if (isToSR) {
		srcNodeIndex = 1;
		dstNodeIndex = 0;
	}
	SparseMatIterator it = DHF.begin(), it_end = DHF.end();
	for (; it != it_end; ++it) {
		int srcIndex = it.node()->idx[srcNodeIndex];
		int dstIndex = it.node()->idx[dstNodeIndex];

		float* d = dest.ptr<float>(dstIndex);
		float* s = src.ptr<float>(srcIndex);
		for (int c = 0; c < 3; c++){ //각 채널 별로 smat의 값 곱함
			d[c] += it.value<float>() * s[c];
		}
	}
}
double getPSNR(Mat& src1, Mat& src2, int bb)
{
	int i, j;
	double sse, mse, psnr;
	sse = 0.0;

	Mat s1, s2;
	cvtColor(src1, s1, CV_BGR2GRAY);
	cvtColor(src2, s2, CV_BGR2GRAY);

	int count = 0;
	for (j = bb; j < s1.rows - bb; j++)
	{
		uchar* d = s1.ptr(j);
		uchar* s = s2.ptr(j);

		for (i = bb; i < s1.cols - bb; i++)
		{
			sse += ((d[i] - s[i])*(d[i] - s[i]));
			count++;
		}
	}
	if (sse == 0.0 || count == 0)
	{
		return 0;
	}
	else
	{
		mse = sse / (double)(count);
		psnr = 10.0*log10((255 * 255) / mse);
		return psnr;
	}
}