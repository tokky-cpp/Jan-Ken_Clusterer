#include "stdafx.h"
#include <opencv2\opencv.hpp>

#define PI 3.1416
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	CvCapture *camera;//カメラの型宣言
	camera = cvCreateCameraCapture(0);//どのカメラを使うかの指定
	IplImage *input,*hsv,*output,*mask;//画像の型宣言
	cvNamedWindow("input",CV_WINDOW_AUTOSIZE);//ウィンドウの設定
	cvNamedWindow("output",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("slider",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("slider",400);//スライダーだけのウィンドウを設定
	cvNamedWindow("gray",CV_WINDOW_AUTOSIZE);

	IplImage *ryoiki;

	input=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);//入力画像をRGBで受け取るため
	hsv=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);//入力画像をhsvに変換していれるため
	output=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);//出力画像を格納するため
	mask=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);//マスク画像はグレースケール
	ryoiki=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);

	int h_min=0,h_max=25,s_min=0,s_max=256,v_min=0,v_max=256,open=5,close=1;
	CvScalar min,max;
	cvCreateTrackbar("H_min","slider",&h_min,181);
	cvCreateTrackbar("H_max","slider",&h_max,181);
	cvCreateTrackbar("S_min","slider",&s_min,256);
	cvCreateTrackbar("S_max","slider",&s_max,256);
	cvCreateTrackbar("V_min","slider",&v_min,256);
	cvCreateTrackbar("V_max","slider",&v_max,256);
	cvCreateTrackbar("Closing","slider",&close,10);
	cvCreateTrackbar("Opening","slider",&open,10);

	int key;//キー入力を保存しておく変数

	
	//----------円形度を求めるために、追加---------------------
	CvMemStorage *storage; /* 領域処理の作業データの格納場所へのポインタ */
	CvSeq *initial_contour; /* 輪郭線集合の最初の輪郭線へのポインタ */
	storage=cvCreateMemStorage(0); /* 作業データの格納場所の確保 */
	//---------------------------------------------------------
	

	for(;;){//ずっとくりかえし
		cvSetZero(output);//出力画像をクリア（黒で上書きする）
		cvSetZero(ryoiki);
		min.val[0]=h_min;
		min.val[1]=s_min;
		min.val[2]=v_min;
		max.val[0]=h_max;
		max.val[1]=s_max;
		max.val[2]=v_max;
		input = cvQueryFrame(camera);//imageにカメラの画像を入れる
		cvShowImage("input",input);//imageに入っている画像をウィンドウに表示させる

		cvCvtColor(input,hsv,CV_BGR2HSV);//input→hsvに変換・格納

		cvInRangeS(hsv,cvScalar(h_min,s_min,v_min),cvScalar(h_max,s_max,v_max),mask);
		
		//closing
		cvDilate(mask,mask,NULL,close);
		cvErode(mask,mask,NULL,close);
		
		//opening
		cvErode(mask,mask,NULL,open);
		cvDilate(mask,mask,NULL,open);

		
		cvCopy(input,output,mask);//マスク画像を元にしてoutputにコピーする
		cvShowImage("output",output);//outputを出力
		cvShowImage("gray",mask);

		//Sキーが入力されたらfor文を抜ける(プログラム終了)
		key=cvWaitKey(1);
		int a;
		if(key=='g'){

			a=cvFindContours(mask,storage,&initial_contour,sizeof(CvContour),CV_RETR_EXTERNAL ,CV_CHAIN_APPROX_SIMPLE,cvPoint(0,0));
			double max_size=0;
			double max_length=0;
			CvSeq* max_ryoiki;
			if(a!=NULL){
				for(CvSeq *n=initial_contour;n!=NULL;n=n->h_next){
					double length = cvArcLength (n, CV_WHOLE_SEQ, 1);  

					double size = cvContourArea (n); 
					if(max_size<size){
						max_size=size;
						max_length=length;
						max_ryoiki=n;
					}

					cvDrawContours(ryoiki, n, cvScalar(255,0,0), cvScalar(0,255,0), 0, 1, 8, cvPoint(0,0));
					cvNamedWindow("ryoiki",CV_WINDOW_AUTOSIZE);
					cvShowImage("ryoiki",ryoiki);
				}
				double enkei=4*PI*max_size/(max_length*max_length);
				cout<<"円形度:"<<enkei<<"("<<max_size<<","<<max_length<<")"<<endl;
				if(enkei<0.16){
					cout<<"パー"<<endl;
				}
				else if(enkei<0.30){
					cout<<"チョキ"<<endl;
				}
				else{
					cout<<"グー"<<endl;
				}
			}
		}
		if(key=='s'){
			break;
		}


	}
	
	//使っていたカメラを解放して
	cvReleaseCapture(&camera);

	//プログラム終了
	return 0;
}

