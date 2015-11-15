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
*/