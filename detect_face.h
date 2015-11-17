/*
 * ���ܼ�飺detect faces in a picture
 * ��    �ߣ�Ethan
 * �༭ʱ�䣺2014��10��22��

 * v3.0 ������һ��������⺯��sclae����ѡ��
       ���ܣ���һ��ͼ�������⵽����������Ϊ1
       ��ı�scale��ֵ��֪���ĸ�����ȫ�����꣬
       ֱ�ӷ������һ��������⵽������

 * v4.0 �����˳�Ա����initFaceDetector��
        releaseFaceDetector����xml�ļ��Ķ�ȡ
        �ͳ�Ա�����ĳ�ʼ���ֿ������Խ�xml�ļ�
        ���ݵĶ�ȡ����ѭ���⣬��С��ʱ�俪��

 * v5.0 �����˳�Ա����getBigestFacePos��
        drawDetectedFaces��getBigestFaceIdx��
		���������⣬��ʶ���м�⵽��������
		ͬʱ����ȡ������������λ��
 * v5.1 �޸����¼����drawDetectedFaces�����У�
        �������λ�õ�ͼ��
*/
#pragma once
#ifndef _DETECT_FACE_H_
#define _DETECT_FACE_H_

#include "header.h"

#define MAX_FACE_COUNT_D_USTC  100 //һ֡������������

struct _Rect //����λ��
{
	int left;
	int top;
	int right;
	int bottom;
};

class FaceDetector//���������
{
public:
	FaceDetector(char* cascade_name_faces = "face.xml");
	~FaceDetector();

	bool initFaceDetector(IplImage* image);
	void releaseFaceDetector();

	int runFaceDetector();
	int getDetectFaceCount();
	int getDetectFacePos(int faceIndex, int pos[4]);
	void getBigestFacePos(int pos[4]);  /* ��ȡ�������������λ��*/
	IplImage* getImageGray();
	void drawDetectedFaces( IplImage* image_draw , std::string showMsg = "" ,CvScalar color=CV_RGB(0, 0, 255));/* �þ��α������λ�ã������������λ�������Ϣ*/

private:
	int width;
	int height;

	IplImage* Frame;
	IplImage* grayFrame;
	IplImage* small_img;

	_Rect* facePos;

	int faceCount;
	double scale;

	char* faceModelFile;
	CvMemStorage* storage_faces;
	CvHaarClassifierCascade* cascade_faces;

	int getBigestFaceIdx();
	void drawRect(IplImage* img , CvPoint p1 ,CvPoint p2);
};

#endif
