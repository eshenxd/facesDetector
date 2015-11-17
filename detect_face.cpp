/*****************************************
功能简介：detect faces in a picture
作    者：Ethan
编辑时间：2014年10月22日
*****************************************/

#include "detect_face.h"

using namespace std;

//char* cascade_name_faces="face.xml";

FaceDetector::FaceDetector(char* cascade_name_faces)
{
	faceModelFile = cascade_name_faces;
	
	storage_faces=cvCreateMemStorage(0);

	cascade_faces = (CvHaarClassifierCascade*)cvLoad( cascade_name_faces, 0, 0, 0 );

	faceCount=0;

	facePos=new _Rect[MAX_FACE_COUNT_D_USTC];

}

FaceDetector::~FaceDetector()
{
	if(facePos)
		delete[] facePos;

	cvReleaseMemStorage(&storage_faces);
	cvReleaseHaarClassifierCascade(&cascade_faces);
}


bool FaceDetector::initFaceDetector(IplImage* imageIn){

	width=imageIn->width;
	height=imageIn->height;

	scale=2;

	Frame=cvCreateImage(cvSize(width,height),imageIn->depth,imageIn->nChannels);
	cvCopy(imageIn,Frame);

	grayFrame=cvCreateImage(cvSize(width,height),8,1);
	small_img=cvCreateImage(cvSize(cvRound(width/scale),cvRound(height/scale)),8,1);
	
	return true;

}

void FaceDetector::releaseFaceDetector()
{
	if (Frame)
		cvReleaseImage(&Frame);

	if (grayFrame)
		cvReleaseImage(&grayFrame);

	if (small_img)
		cvReleaseImage(&small_img);
	
}

IplImage* FaceDetector::getImageGray(){

	return grayFrame;
}

int FaceDetector::runFaceDetector()
{
	if(Frame->nChannels==3)
	{
		cvCvtColor(Frame,grayFrame,CV_BGR2GRAY);
	}
	
	else
		cvCopy(Frame,grayFrame);

	if(!cascade_faces)
	{
		cout<<"ERROR: Could not load classifier cascade"<<endl;

		system("pause");
	}
	
	CvSeq* faces = NULL;

	cvClearMemStorage( storage_faces );

	cvResize( grayFrame , small_img , CV_INTER_LINEAR );

	//对图像进行直方图均衡化，也即增强图像的亮度和对比度
	cvEqualizeHist( small_img, small_img);
	
	int i = 0;//初始化迭代次数

	double param[4] = { 1.2, 1.6, 2.0, 3.0 };

	faces = cvHaarDetectObjects(small_img, cascade_faces, storage_faces,
		1.2, 3, 0/*CV_HAAR_DO_CANNY_PRUNING*/,
		cvSize(20, 20));

	faceCount=faces->total;

	for(int i = 0; i < (faces ? faces->total : 0); i++ )
	{
		CvRect* r = (CvRect*)cvGetSeqElem( faces, i );

		//保存人脸的位置
		facePos[i].left=cvRound(r->x*scale);
		facePos[i].top=cvRound(r->y*scale);
		facePos[i].right=facePos[i].left+cvRound(r->width*scale);
		facePos[i].bottom=facePos[i].top+cvRound(r->height*scale);
	}

	return 0;
}

int FaceDetector::getDetectFaceCount()
{
	return faceCount;
}

/* 获取检测到的所有人脸的位置，
 * faceIdx为人脸序列号，pos存
 *储人脸矩形的坐标
 */
int FaceDetector::getDetectFacePos(int faceIdx, int pos[4])
{
	if(faceIdx>=getDetectFaceCount())
	{
		cout<<"There's no enough face!"<<endl;

		return -1;
	}

	pos[0] = facePos[faceIdx].left;
	pos[1] = facePos[faceIdx].top;
	pos[2] = facePos[faceIdx].right;
	pos[3] = facePos[faceIdx].bottom;

	return 0;
}

int FaceDetector::getBigestFaceIdx()
{
	int area = 0;
	int index = 0;
	for(int idx = 0 ;idx<faceCount ;++idx){
		int width = facePos[idx].right - facePos[idx].left;
		int height = facePos[idx].top - facePos[idx].bottom;
		if((width*height)>area){
			area = width*height;
			index = idx;
		}
	}
	return index;
}

void FaceDetector::getBigestFacePos(int* pos)
{
	int index = getBigestFaceIdx();

	pos[0] = facePos[index].left;
	pos[1] = facePos[index].top;
	pos[2] = facePos[index].right;
	pos[3] = facePos[index].bottom;

}

void FaceDetector::drawDetectedFaces(IplImage* image_draw ,string showMsg ,CvScalar color){
	
	CvFont font;
	double hScale = 1;
	double vScale = 1;
	int lineWidth = 2;// 相当于写字的线条

	CvPoint p1,p2 ,p3;
	int index = getBigestFaceIdx();
	
	for(int idx = 0; idx < faceCount; idx++)
	{
		p1 = cvPoint(facePos[idx].left,facePos[idx].top);
		p2 = cvPoint(facePos[idx].right,facePos[idx].bottom);
		drawRect(image_draw,p1,p2);
	}

	p3 = cvPoint(facePos[index].left,(facePos[index].top - 10));
	   
	cvInitFont(&font, CV_FONT_HERSHEY_SIMPLEX | CV_FONT_ITALIC, hScale, vScale, 0, lineWidth);

	cvPutText(image_draw, showMsg.c_str(), p3, &font, color);//在图片中输出字符
}

void FaceDetector::drawRect(IplImage* img ,CvPoint p1 ,CvPoint p2){

	int dis_long_y = (p2.y - p1.y)/10;
	int dis_long_x = (p2.x - p1.x)/10;
	CvPoint top_left = p1;
	CvPoint bottom_right = p2;
	CvPoint top_right = cvPoint(p2.x,p1.y);
	CvPoint bottom_left = cvPoint(p1.x,p2.y);

	cvLine(img,top_left,cvPoint(top_left.x+dis_long_x,top_left.y),cvScalar(255,255,0),1,8,0);
	cvLine(img,top_left,cvPoint(top_left.x,top_left.y+dis_long_y),cvScalar(255,255,0),1,8,0);
	cvLine(img,top_right,cvPoint(top_right.x,top_right.y+dis_long_y),cvScalar(255,255,0),1,8,0);
	cvLine(img,top_right,cvPoint(top_right.x - dis_long_x,top_right.y),cvScalar(255,255,0),1,8,0);

	cvLine(img,bottom_left,cvPoint(bottom_left.x + dis_long_x,bottom_left.y),cvScalar(255,255,0),1,8,0);
	cvLine(img,bottom_left,cvPoint(bottom_left.x,bottom_left.y-dis_long_y),cvScalar(255,255,0),1,8,0);
	cvLine(img,bottom_right,cvPoint(bottom_right.x,bottom_right.y-dis_long_y),cvScalar(255,255,0),1,8,0);
	cvLine(img,bottom_right,cvPoint(bottom_right.x - dis_long_x,bottom_right.y),cvScalar(255,255,0),1,8,0);

}
