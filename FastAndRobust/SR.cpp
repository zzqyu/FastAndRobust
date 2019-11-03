#include "SR.h"
#include "Calculate.h"
#include "Function.h"
#include <iostream>

Mat superResolutionImage(Mat ideal, Mat src[], SparseMat DHF[], const int srcNum, int iteration, float beta, float lambda, float alpha, float scale, Size rKernelSize)
{
	//(1)이미지 크기 키워 기준영상 만들기, 추후 구현
	Mat dst(src[0].rows * scale, src[0].cols * scale, src[0].type());
	my_resize(src[0], dst, dst.size());
	my_imWrite("default.raw", dst);
	imshow("default", dst);
	//기준영상 1차원으로 만들기
	Mat defaultVec;
	dst.reshape(3, dst.cols*dst.rows).convertTo(defaultVec, CV_32FC3);

	//steepest descent method for L1 norm minimization
	//L1 표준 최소화를 위한 가장 가파른 하강 방법
	Mat* DHF_tempVecs = new Mat[srcNum];
	for (int i = 0; i < iteration; i++) {
		std::cout << i << std::endl;
		//(2) btvRegularization 
		Mat btvVec = btvRegularization(defaultVec, dst.size(), rKernelSize, alpha, lambda);

		//N개의 저하영상 계산하기
		//(3) DFH X sign(DHF X 기준영상, 저하영상)
		for (int n = 0; n < srcNum; n++) {
			//DHF X 기준영상
			Mat DFH_defaultVec;
			src[n].reshape(3, src[0].cols*src[0].rows).convertTo(DFH_defaultVec, CV_32FC3);
			mulDHF(DHF[n], defaultVec, DFH_defaultVec);

			//sign(DHF X 기준영상, 저하영상)
			Mat temp;
			Mat srcVec;
			src[n].reshape(3, src[0].cols*src[0].rows).convertTo(srcVec, CV_32FC3);

			temp = subtractSign(DFH_defaultVec, srcVec);

			//거기에 DHF 또 곱하기// A = DFH X sign(DHF X 기준영상, 저하영상)
			DHF_tempVecs[n] = Mat(defaultVec.size(), defaultVec.type());
			mulDHF(DHF[n], temp, DHF_tempVecs[n], true);
		}
		//(4) 기준영상 -= (∑(n=0~N)(A_n) X 베타
		/*for (int n = 0; n < srcNum; n++) {
			for (int h = 0; h < dst.rows; h++) {
				for (int w = 0; w < dst.cols; w++) {
					float* d = defaultVec.ptr<float>(h * dst.rows + w);
					float* t = DHF_tempVecs[n].ptr<float>(h * dst.rows + w);
					for (int c = 0; c < 3; c++)
						d[c] -= beta * t[c];
				}
			}
		}*/
		//(4) 기준영상 -= (∑(n=0~N)(A_n) X 베타
		//(4) new_기준영상  = 1.0 x 기준영상 + (- beta) x ∑(n=0~N)(A_n)
		for (int n = 0; n < srcNum; n++) {
			my_addWeighted(defaultVec, 1.0, DHF_tempVecs[n], -beta, defaultVec);
		}
		//(5) 기준영상 -= beta X lambda X 정규화영상
		//(5) new_new_기준영상  = 1.0 x new_기준영상 + (- beta) X lambda X 정규화영상
		if (lambda > 0.0) {
			//addWeighted(defaultVec, 1.0, btvVec, -beta * lambda, 0.0, defaultVec);
			my_addWeighted(defaultVec, 1.0, btvVec, -beta * lambda, defaultVec);
		}
		defaultVec.reshape(3, dst.rows).convertTo(dst, CV_8UC3);

		std::cout << "PSNR" << getPSNR(dst, ideal, 10) << "dB" << std::endl;

		imshow("SRimage", dst); waitKey(30);
		char name[64];
		sprintf_s(name, "iteration%04d.raw", i);
		my_imWrite(name, dst);
	}

	return dst;
}

Mat btvRegularization(Mat& srcVec, Size srcSize, Size kernel, float alpha, float lambda) {
	Mat dstVec = Mat::zeros(srcVec.size(), CV_32FC3);
	if (lambda > 0.0) {
		const int kw = (kernel.width - 1) / 2;
		const int kh = (kernel.height - 1) / 2;

		for (int h = kh; h < srcSize.height - kh; h++) {
			for (int w = kw; w < srcSize.width - kw; w++) {
				//S(x, y)
				float* s = srcVec.ptr<float>(h * srcSize.width + w);
				//D(x, y)
				float* d = dstVec.ptr<float>(h * srcSize.width + w);
				for (int m = 0; m <= kh; m++) {
					for (int l = kw; l + m >= 0; l--) {
						//S(x + l, y + m)
						float* s1 = srcVec.ptr<float>((h + m)* srcSize.width + w + l);
						//S(x - l, y - m)
						float* s2 = srcVec.ptr<float>((h - m)* srcSize.width + w - l);
						//알파^(|m|+|l|)
						float weight = pow(alpha, abs(m) + abs(l));
						//채널별(RGB)로 적용
						for (int c = 0; c < 3; c++)
							d[c] += weight * (signFloat(s[c], s1[c]) - signFloat(s2[c], s[c]));
					}
				}
			}
		}
	}
	return dstVec;
}
