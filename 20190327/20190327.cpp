// 20190327.cpp : 此文件包含 "main" 函数。程序执行将在此处开始并结束。
//

#include "CmLib.h"
#include "stack.h"
#include "fun.h"
int main()
{
	char nameorigin[40];
	cout << "Input filename: ";
	cin >> nameorigin;
	Mat sourcePicture;
	sourcePicture = imread(nameorigin);
	cout << sourcePicture.rows <<' '<< sourcePicture.cols << endl;
	

	char nameoutput[20] = "result/";
	char name[60] = { 0 };
	strcpy(name, nameoutput);
	strcat(name, nameorigin);
	char tail[30] = { 0 };

	//灰度处理结果
	Mat blackResult;
	cvtColor(sourcePicture, blackResult, COLOR_BGR2GRAY);

	//分拣区域结果
	Mat areaResult;
	m_pickArea(5, blackResult, areaResult);
	//imshow("areaResult", areaResult);



	char name1[40] = { 0 };
	strcpy(name1, name);
	strcpy(tail, ".areaResult.jpg");
	strcat(name1, tail);
	//imwrite(name1, areaResult);


	Mat correctResult;
	m_getCorrectColor(blackResult, areaResult);
	//imshow("updatedResult", areaResult);

	char name2[40] = { 0 };
	strcpy(name2, name);
	strcpy(tail, ".updatedResult.jpg");
	strcat(name2, tail);
	//imwrite(name2, areaResult);

	//高斯差分处理结果*2
	Mat deltaResult1;
	m_edge(1.52, blackResult, deltaResult1, 0);
	//imshow("deltaResult1", deltaResult1);

	char name3[40] = { 0 };
	strcpy(name3, name);
	strcpy(tail, ".deltaResult1.jpg");
	strcat(name3, tail);
	//imwrite(name3, deltaResult1);

	//Mat deltaResult2;
	//m_edge(1.52, blackResult, deltaResult2, 1);
	//imshow("deltaResult2", deltaResult2);

	char name4[40] = { 0 };
	strcpy(name4, name);
	strcpy(tail, ".deltaResult2.jpg");
	strcat(name4, tail);
	//imwrite(name4, deltaResult2);

	//绘制阴影结果
	Mat finalResult;
	m_addShadow(areaResult, deltaResult1, blackResult, finalResult);
	imshow("finalResult", finalResult);
	//imwrite("finalResult.jpg", finalResult);

	char name5[40] = { 0 };
	strcpy(name5, name);
	strcpy(tail, ".finalResult.jpg");
	strcat(name5, tail);
	cout << name5 << endl;
	imwrite(name5, finalResult);

	waitKey();
	return 0;
	
	//这里是没有被采用的方案
	//滤波处理结果
	Mat filterResult;
	medianBlur(sourcePicture, filterResult, 3);
	GaussianBlur(filterResult, filterResult, Size(3, 3), 1);

	//canny边缘处理
	Mat cannyResult;
	Canny(filterResult, cannyResult, DOWN, UP);
	bitwise_not(cannyResult, cannyResult);
	imshow("cannyResult", cannyResult);
	//imwrite("cannyResult0.jpg", cannyResult);

	//canny+简单填充
	Mat* fillResult = new Mat;
	m_black(cannyResult, *fillResult);
	medianBlur(*fillResult, *fillResult, 3);
	imshow("fillResult", *fillResult);
	//imwrite("fillResult0.jpg", *fillResult);

	//canny+腐蚀变换
	Mat dilateResult;
	m_dilate(cannyResult, dilateResult);
	imshow("dilateResult", dilateResult);
	//imwrite("dilateResult0.jpg", dilateResult);

	waitKey();
	return 0;
}