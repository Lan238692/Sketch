#include"fun.h"

void m_shadow(Mat& des, uchar strength, double direction)
{
	Point p1, p2;
	p1.x = 0; p1.y = 0;
	p2.x = des.cols - 1; p2.y = p1.y - direction * (double)des.cols;
	for (int i = 0; i < des.rows; i++)
		for (int j = 0; j < des.cols; j++)
		{
			des.at<uchar>(i, j) = 255;
		}
	double size = des.cols*des.rows;
	double resize = size / 800.0 / 600.0;
	double sd = pow(direction*direction + 1.0, 0.5);
	while (true)
	{
		if (p2.y > des.rows)
		{
			//imshow("shadow", des);
			return;
		}
		line(des, p1, p2, 0, 1);
		double delta = strength /10.0;
		if (delta < 10.0)//*resize)
		delta = 10.0;// *resize;
		p1.y += delta * sd;// *resize;
		p2.y += delta * sd;// *resize;
	}
}
void getColor(stack* s, uchar color, uchar base, int the, int x, int y, Mat src, Mat* des, Mat* check)
{
	if (x<0 || y<0 || x>src.rows - 1 || y>src.cols - 1)
		return;
	if (check->at<uchar>(x, y) == 1)
		return;
	uchar newColor = src.at<uchar>(x, y);
	if (abs((int)newColor - (int)color) > the)
		return;
	des->at<uchar>(x, y) = base;
	check->at<uchar>(x, y) = 1;
	if (x > 0 && check->at<uchar>(x - 1, y) != 1)
		s->push(x - 1, y, newColor);

	if (x < src.rows - 1 && check->at<uchar>(x + 1, y) != 1)
		s->push(x + 1, y, newColor);

	if (y > 0 && check->at<uchar>(x, y - 1) != 1)
		s->push(x, y - 1, newColor);

	if (y < src.cols - 1 && check->at<uchar>(x, y + 1) != 1)
		s->push(x, y + 1, newColor);

	//if (y + 5 < src.cols - 1 && check->at<uchar>(x, y + 6) != 1)
		//s->push(x, y + 6, newColor);
	//if (x + 5 < src.rows - 1 && check->at<uchar>(x + 6, y) != 1)
		//s->push(x + 6, y, newColor);

	return;
}
void getRealColor(stack* s, Mat areaSrc, Mat src, Mat* check, int recent, int x, int y, double* count, double* sum)
{
	*count+=1.0;
	uchar tempValue = areaSrc.at<uchar>(x, y);
	uchar tempReal = src.at<uchar>(x, y);
	*sum += (int)tempReal;
	check->at<int>(x, y) = recent;
	if (x > 0 && check->at<int>(x - 1, y) == 0 && areaSrc.at<uchar>(x - 1, y) == tempValue)
		s->push(x - 1, y, 0);

	if (x < src.rows - 1 && check->at<int>(x + 1, y) == 0 && areaSrc.at<uchar>(x + 1, y) == tempValue)
		s->push(x + 1, y, 0);

	if (y > 0 && check->at<int>(x, y - 1) == 0 && areaSrc.at<uchar>(x, y - 1) == tempValue)
		s->push(x, y - 1, 0);

	if (y < src.cols - 1 && check->at<int>(x, y + 1) == 0 && areaSrc.at<uchar>(x, y + 1) == tempValue)
		s->push(x, y + 1, 0);
}
void changeColor(stack* s, Mat* areaSrc, Mat* check, int recent, int x, int y, uchar value)
{
	areaSrc->at<uchar>(x, y) = value;
	check->at<int>(x, y) = -1;
	if (x > 0 && check->at<int>(x - 1, y) == recent)
		s->push(x - 1, y, 0);

	if (x < check->rows - 1 && check->at<int>(x + 1, y) == recent)
		s->push(x + 1, y, 0);

	if (y > 0 && check->at<int>(x, y - 1) == recent)
		s->push(x, y - 1, 0);

	if (y < check->cols - 1 && check->at<int>(x, y + 1) == recent)
		s->push(x, y + 1, 0);

}
void m_getCorrectColor(Mat src, Mat& areaSrc)
{
	Mat check(src.size(), CV_32SC1);
	int recent = 1;
	double count;
	double sum;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			check.at<int>(i, j) = 0;
		}
	}
	stack s;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			if (check.at<int>(i, j) == 0)
			{
				//cout << i <<' '<< j << endl;
				count = 0;
				sum = 0;
				getRealColor(&s, areaSrc, src, &check, recent, i, j, &count, &sum);
				while (!s.isEmpty())
				{
					int tempr, tempc;
					uchar tempcl;
					s.pop(tempr, tempc, tempcl);
					getRealColor(&s, areaSrc, src, &check, recent, tempr, tempc, &count, &sum);
				}
				if (count == 0)
					cout << "count = 0 error" << endl;
				double realColor = sum / count;
				if (realColor < 0)
				{
					cout << "color < 0: " <<realColor<< endl;
					realColor = 0;
				}
				if (realColor > 255)
				{
					cout << "color > 255: " << realColor << endl;
					realColor = 255;
				}
				changeColor(&s, &areaSrc, &check, recent, i, j, realColor);
				while (!s.isEmpty())
				{
					int tempr, tempc;
					uchar tempcl;
					s.pop(tempr, tempc, tempcl);
					changeColor(&s, &areaSrc, &check, recent, tempr, tempc, realColor);
				}
				recent++;
			}
		}
	}

}
void m_pickArea(int the, Mat src, Mat& des)
{
	Mat temp(src.size(), src.type());
	Mat check(src.size(), CV_8UC1);
	stack s;

	int m_the = the;
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			check.at<uchar>(i, j) = 0;
		}
	}


	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (check.at<uchar>(i, j) == 0)
			{
				int tempi, tempj;
				unsigned char color, base;
				color = src.at<uchar>(i, j);
				base = src.at<uchar>(i, j);
				getColor(&s, color, base, m_the, i, j, src, &temp, &check);
				while (!s.isEmpty())
				{
					s.pop(tempi, tempj, color);
					getColor(&s, color, base, m_the, tempi, tempj, src, &temp, &check);
				}
			}
		}
	temp.copyTo(des);
	//imshow("areaResult", des);
}
void m_addShadow(Mat areaSrc, Mat edgeSrc, Mat originSrc, Mat& des)
{
	srand(time(NULL));
	Mat temp(originSrc.size(), originSrc.type());
	edgeSrc.copyTo(temp);

	int count[256] = { 0 };
	double s = areaSrc.rows*areaSrc.cols;
	for (int i = 0; i < areaSrc.rows; i++)
	{
		for (int j = 0; j < areaSrc.cols; j++)
		{
			count[(int)areaSrc.at<uchar>(i, j)]++;
		}
	}
	
	for (int i = GRAY_DOWN; i < GRAY_UP; i++)
	{
		if (i >= 85 && i <= 92)
			continue;
		if (count[i] >= 500 && count[i] <= UP_PERCENT * s)
		{
			Mat shadowTemp(originSrc.size(), originSrc.type());
			m_shadow(shadowTemp, i, (double)(rand() % 100) / 50.0+0.55);// chu 100 +0.65
			for (int x = 0; x < areaSrc.rows; x++)
			{
				for (int y = 0; y < areaSrc.cols; y++)
				{
					if (shadowTemp.at<char>(x, y) == 0 && areaSrc.at<uchar>(x, y) == i)
						temp.at<uchar>(x, y) = 0;
				}
			}
		}
	}
	Mat element = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
	//dilate(temp, temp, element);
	//imshow("temp", temp);
	temp.copyTo(des);
}
void m_inv(int the, Mat src, Mat& des)
{
	des = src.clone();
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if ((int)src.at<uchar>(i, j) < the)
				des.at<uchar>(i, j) = 255;
			else
				des.at<uchar>(i, j) = 0;
		}
}
void m_mid(Mat src, Mat& des)
{
	Mat temp(src.size(), src.type());
	for (int i = 0; i < src.rows; i++)
		for (int j = 0; j < src.cols; j++)
		{
			if (src.at<uchar>(i, j) == 0)
			{
				int count = 0;
				for (int x = -1; x <= 1; x++)
					for (int y = -1; y <= 1; y++)
					{
						if (i + x >= 0 && i + x < src.rows&&j + y >= 0 && j + y < src.cols)
							if (src.at<uchar>(i + x, j + y) == 0)
								count++;
					}
				if (count >= 3)
					temp.at<uchar>(i, j) = 0;
				else
					temp.at<uchar>(i, j) = 255;
			}
			else
				temp.at<uchar>(i, j) = 255;
		}
	temp.copyTo(des);
}
void m_black(Mat src, Mat& des)
{
	Mat result(src.size(), src.type());
	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{
			result.at<uchar>(i, j) = src.at<uchar>(i, j);
		}
	}
	bool sw(1);

	for (int i = 0; i < src.rows; i++)
	{
		for (int j = 0; j < src.cols; j++)
		{

			if ((int)src.at<uchar>(i, j) == 0)
			{
				for (int u = j + 2; u < j + EDGE && u < src.cols; u++)
				{
					if ((int)src.at<uchar>(i, u) == 0)
					{
						for (int v = u; v >= j; v--)
							result.at<uchar>(i, v) = 0;
						//result.at<uchar>(i, u) = 255;
						j = u + 1;
						break;
					}
					//optional
					//if (u == j + EDGE - 1 && i != 0 && j != 0 && i != src.rows - 1 && j != src.cols - 1)
						//  result.at<uchar>(i - 1, j - 1) = result.at<uchar>(i - 1, j) = result.at<uchar>(i - 1, j + 1) = result.at<uchar>(i , j - 1) 
						//= result.at<uchar>(i , j + 1) = result.at<uchar>(i + 1, j - 1) = result.at<uchar>(i + 1, j) = result.at<uchar>(i + 1, j + 1) = 0;
				}
			}
		}
	}
	sw = 1;

	for (int j = 0; j < src.cols; j++)
	{
		for (int i = 0; i < src.rows; i++)
		{

			if ((int)src.at<uchar>(i, j) == 0)
			{
				for (int u = i + 2; u < i + EDGE && u < src.rows; u++)
				{
					if ((int)src.at<uchar>(u, j) == 0)
					{
						for (int v = u; v >= i; v--)
							result.at<uchar>(v, j) = 0;
						//result.at<uchar>(u, j) = 255;
						i = u + 1;
						break;
					}
					//optional
					//if (u == i + EDGE - 1 && i != 0 && j != 0 && i != src.rows - 1 && j != src.cols - 1)
						//result.at<uchar>(i - 1, j - 1) = result.at<uchar>(i - 1, j) = result.at<uchar>(i - 1, j + 1) = result.at<uchar>(i, j - 1)
						//= result.at<uchar>(i, j + 1) = result.at<uchar>(i + 1, j - 1) = result.at<uchar>(i + 1, j) = result.at<uchar>(i + 1, j + 1) = 0;
				}
			}
		}
	}
	for (int i = 1; i < src.rows - 1; i++)
		for (int j = 1; j < src.cols - 1; j++)
		{
			if (((result.at<uchar>(i, j - 1) == 0) && (result.at<uchar>(i, j + 1) == 0)) || ((result.at<uchar>(i - 1, j) == 0) && (result.at<uchar>(i + 1, j) == 0)))
				result.at<uchar>(i, j) = 0;
		}
	result.copyTo(des);
}
void m_edge(float sig, Mat src, Mat& des, int level)
{
	Mat temp(src.size(),src.type());
	float sig0 = sig * pow(2.0, 1.0 / 3.0);

	if (level == 0)
	{
		Mat gauss;
		GaussianBlur(src, gauss, Size(), sig, sig);
		subtract(gauss, src, temp);
		m_inv(7, temp, temp);
		m_mid(temp, temp);
		temp.copyTo(des);
		return;
	}
	if (level == 1)
	{
		Mat gauss1, gauss2;
		float sigP = sig * pow(2.0, 1.0 / 3.0);
		GaussianBlur(src, gauss1, Size(), sig, sig);
		GaussianBlur(gauss1, gauss2, Size(), sigP, sigP);
		subtract(gauss2, gauss1, temp);
		m_inv(6, temp, temp);
		Mat element= getStructuringElement(MORPH_ELLIPSE, Size(2, 2));
		dilate(temp, temp, element);
		temp.copyTo(des);
		return;
	}
	return;
}
void m_dilate(Mat src, Mat& des)
{
	Mat temp(src.size(), src.type());
	Mat element1 = getStructuringElement(MORPH_ELLIPSE, Size(5, 5)); 
	Mat element2 = getStructuringElement(MORPH_ELLIPSE, Size(2, 2));

	erode(src, temp, element1);
	dilate(temp, temp, element2);
	dilate(temp, temp, element2);
	dilate(temp, temp, element2);
	dilate(temp, temp, element2);
	dilate(temp, temp, element2);
	temp.copyTo(des);
}