/*
 * 功能简介：detect faces in a picture
 * 作    者：Ethan
 * 编辑时间：2014年10月22日

 * v3.0 增加了一个人脸检测函数sclae参数选择
       功能，对一个图像如果检测到的人脸数不为1
       则改变scale的值，知道四个参数全部用完，
       直接返回最后一个参数检测到的人脸

 * v4.0 增加了成员函数initFaceDetector和
        releaseFaceDetector，将xml文件的读取
        和成员变量的初始化分开，可以将xml文件
        内容的读取放在循环外，减小了时间开销

 * v5.0 增加了成员函数getBigestFacePos、
        drawDetectedFaces、getBigestFaceIdx引
		入多人脸检测，标识所有检测到的人脸，
		同时，获取面积最大人脸的位置
 * v5.1 修改了新加入的drawDetectedFaces函数中，
        标出人脸位置的图像
*/
#pragma once
#ifndef _DETECT_FACE_H_
#define _DETECT_FACE_H_

#include "header.h"

#define MAX_FACE_COUNT_D_USTC  100 //一帧中最多的人脸数

struct _Rect //人脸位置
{
	int left;
	int top;
	int right;
	int bottom;
};

class FaceDetector//人脸检测器
{
public:
	FaceDetector(char* cascade_name_faces = "face.xml");
	~FaceDetector();

	bool initFaceDetector(IplImage* image);
	void releaseFaceDetector();

	int runFaceDetector();
	int getDetectFaceCount();
	int getDetectFacePos(int faceIndex, int pos[4]);
	void getBigestFacePos(int pos[4]);  /* 获取面积最大的人脸的位置*/
	IplImage* getImageGray();
	void drawDetectedFaces( IplImage* image_draw , std::string showMsg = "" ,CvScalar color=CV_RGB(0, 0, 255));/* 用矩形标出人脸位置，并在最大人脸位置输出信息*/

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
