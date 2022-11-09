#pragma once
#include"CmLib.h"
#include"stack.h"

#define UP 80
#define DOWN 20

#define EDGE 6

#define UP_PERCENT 0.25
#define DOWN_PERCENT 0.05

#define GRAY_DOWN 10
#define GRAY_UP 200

using namespace std;

void m_edge(float sig, Mat src, Mat& des, int level);
void m_inv(int the, Mat src, Mat& des);

void m_mid(Mat src, Mat& des);

void m_black(Mat src, Mat& des);
void m_dilate(Mat src, Mat& des);


void m_pickArea(int the, Mat src, Mat& des);
void m_getCorrectColor(Mat src, Mat& areaSrc);

void getColor(stack* s, uchar color, uchar base, int the, int x, int y, Mat src, Mat* des, Mat* check);
void changeColor(stack* s, Mat* areaSrc, Mat* check, int recent, int x, int y, uchar value);
void getRealColor(stack* s, Mat areaSrc, Mat src, Mat* check, int recent, int x, int y, double* count, double* sum);

void m_addShadow(Mat areaSrc, Mat edgeSrc, Mat originSrc, Mat& des);

void m_shadow(Mat& des, uchar strength, double direction);




