#include "stdafx.h"
#include <opencv2\opencv.hpp>

#define PI 3.1416
using namespace std;

int _tmain(int argc, _TCHAR* argv[])
{
	
	CvCapture *camera;//�J�����̌^�錾
	camera = cvCreateCameraCapture(0);//�ǂ̃J�������g�����̎w��
	IplImage *input,*hsv,*output,*mask;//�摜�̌^�錾
	cvNamedWindow("input",CV_WINDOW_AUTOSIZE);//�E�B���h�E�̐ݒ�
	cvNamedWindow("output",CV_WINDOW_AUTOSIZE);
	//cvNamedWindow("slider",CV_WINDOW_AUTOSIZE);
	cvNamedWindow("slider",400);//�X���C�_�[�����̃E�B���h�E��ݒ�
	cvNamedWindow("gray",CV_WINDOW_AUTOSIZE);

	IplImage *ryoiki;

	input=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);//���͉摜��RGB�Ŏ󂯎�邽��
	hsv=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);//���͉摜��hsv�ɕϊ����Ă���邽��
	output=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,3);//�o�͉摜���i�[���邽��
	mask=cvCreateImage(cvSize(640,480),IPL_DEPTH_8U,1);//�}�X�N�摜�̓O���[�X�P�[��
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

	int key;//�L�[���͂�ۑ����Ă����ϐ�

	
	//----------�~�`�x�����߂邽�߂ɁA�ǉ�---------------------
	CvMemStorage *storage; /* �̈揈���̍�ƃf�[�^�̊i�[�ꏊ�ւ̃|�C���^ */
	CvSeq *initial_contour; /* �֊s���W���̍ŏ��̗֊s���ւ̃|�C���^ */
	storage=cvCreateMemStorage(0); /* ��ƃf�[�^�̊i�[�ꏊ�̊m�� */
	//---------------------------------------------------------
	

	for(;;){//�����Ƃ��肩����
		cvSetZero(output);//�o�͉摜���N���A�i���ŏ㏑������j
		cvSetZero(ryoiki);
		min.val[0]=h_min;
		min.val[1]=s_min;
		min.val[2]=v_min;
		max.val[0]=h_max;
		max.val[1]=s_max;
		max.val[2]=v_max;
		input = cvQueryFrame(camera);//image�ɃJ�����̉摜������
		cvShowImage("input",input);//image�ɓ����Ă���摜���E�B���h�E�ɕ\��������

		cvCvtColor(input,hsv,CV_BGR2HSV);//input��hsv�ɕϊ��E�i�[

		cvInRangeS(hsv,cvScalar(h_min,s_min,v_min),cvScalar(h_max,s_max,v_max),mask);
		
		//closing
		cvDilate(mask,mask,NULL,close);
		cvErode(mask,mask,NULL,close);
		
		//opening
		cvErode(mask,mask,NULL,open);
		cvDilate(mask,mask,NULL,open);

		
		cvCopy(input,output,mask);//�}�X�N�摜�����ɂ���output�ɃR�s�[����
		cvShowImage("output",output);//output���o��
		cvShowImage("gray",mask);

		//S�L�[�����͂��ꂽ��for���𔲂���(�v���O�����I��)
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
				cout<<"�~�`�x:"<<enkei<<"("<<max_size<<","<<max_length<<")"<<endl;
				if(enkei<0.16){
					cout<<"�p�["<<endl;
				}
				else if(enkei<0.30){
					cout<<"�`���L"<<endl;
				}
				else{
					cout<<"�O�["<<endl;
				}
			}
		}
		if(key=='s'){
			break;
		}


	}
	
	//�g���Ă����J�������������
	cvReleaseCapture(&camera);

	//�v���O�����I��
	return 0;
}

