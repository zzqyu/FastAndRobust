#include "DegradedImage.h"
#include "Calculate.h"
#include "Function.h"

void createDegradedImagesAndDHFs(SparseMat* DHFs, Mat* degradedImages, Mat src, int scale, int imageCount){
	//생성할 저하 이미지 만큼 루프
	for (int i = 0; i < imageCount; i++) {
		std::cout << i << std::endl;
		//(1) 이동 크기 지정
		Point2d shift;
		shift.x = randomDouble(0.0, (float)scale);
		shift.y = randomDouble(0.0, (float)scale);
		if (i == 0)// fix first image
			shift = Point2d(0, 0);
		//(2) 이동크기에 따른 DHF 구하기
		DHFs[i] = createDHF(src, shift, scale);
		//(3) DHF에 따른 이미지 손상하기
		degradedImages[i] = createDegradedImage(src, DHFs[i], scale);
		//(4) 노이즈 추가, 직접 구현
		degradedImages[i] = addGaussianNoise(degradedImages[i], 10.0);//add gaussian noise 
		degradedImages[i] = addspikenoise(degradedImages[i], 500);

		imshow("degradedImage", degradedImages[i]); waitKey(30);
		my_imWrite("degradedImage"+std::to_string(i)+".raw", degradedImages[i]);
	}
}

SparseMat createDHF(Mat src, Point2d shift, int scale) {
	int x0 = shift.x, x1 = x0 + 1, 
		y0 = shift.y, y1 = y0 + 1;
	float a1 = shift.x - (float)x0, a0 = 1 - a1,
		b1 = shift.y - (float)y0, b0 = 1 - b1;

	//DHF 객체 생성 (큰이미지픽셀수, 작은이미지픽셀수)
	int size[2] = { src.cols*src.rows, src.cols*src.rows/(scale*scale)};
	SparseMat DHF(2, size, CV_32F); //dims, size, type

	//각 좌표에 더할 값 들
	float values[] = { a0 * b0 / (scale*scale),
		a1 * b0 / (scale*scale),
		a0 * b1 / (scale*scale),
		a1 * b1 / (scale*scale) };
	//x, y는 저하 이미지 기준 좌표
	//X, Y는 원본 이미지 기준 좌표
	for (int y = 0; y < (float)(src.rows - y1 - scale) / scale; y++) {
		if (y < (y1 + 0.0) / scale) continue;

		for (int x = 0; x < (float)(src.cols - x1 - scale) / scale; x++) {
			if(x < (x1 + 0.0) / scale) continue;

			int X = x * scale, Y = y * scale;
			int s = (src.cols / scale) * y + x;//작은 이미지 좌표

			for (int l = 0; l < scale; l++) {
				for (int k = 0; k < scale; k++) {
					//src.cols*(Y + l + y0) + X + k + x0 , (src.cols / scale) * y  + x 
					// 큰이미지(Y + l + y0, X + k + x0), 작은이미지(y, x)
					DHF.ref<float>(src.cols*(Y + l + y0) + X + k + x0, s) += values[0];
					// 큰이미지(Y + l + y0, X + k + x1), 작은이미지(y, x)
					DHF.ref<float>(src.cols*(Y + l + y0) + X + k + x1, s) += values[1];
					// 큰이미지(Y + l + y1, X + k + x0), 작은이미지(y, x)
					DHF.ref<float>(src.cols*(Y + l + y1) + X + k + x0, s) += values[2];
					// 큰이미지(Y + l + y1, X + k + x1), 작은이미지(y, x)
					DHF.ref<float>(src.cols*(Y + l + y1) + X + k + x1, s) += values[3];
				}
			}
		}
	}
	return DHF;
}
Mat createDegradedImage(Mat src, SparseMat DHF, int scale) {
	Mat dst(src.rows / scale, src.cols / scale, CV_8UC3);

	Mat srcVec, dstVec(dst.rows * dst.cols, 1, CV_32FC3);
	src.reshape(3, src.rows * src.cols).convertTo(srcVec, CV_32FC3);

	//DHF x 원본이미지 =  결과
	mulDHF(DHF, srcVec, dstVec);

	dstVec.reshape(3, dst.rows).convertTo(dst, CV_8UC3);

	return dst;
}
