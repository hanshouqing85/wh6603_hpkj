//���ֽṹ�嶨��
//�ӵ��ṹ
#include "..\��Ϣ����\CMD_Game.h"
struct FilesParmesA
{
    int                             m_PngSize;                              //�ļ���С
};
//�齱ת��
struct ChoujiangStruct
{
      bool ishave; //�Ƿ��г齱
	  float  quanshu;//��ǰת�˶���Ȧ��
	  float  index;  //��Ľ����ĸ�
	  float  nowindex;//��ǰת���ǲ���
	  float  roloation ;
	  float  score;
};
//�ɶ�
struct FireStruct
{
      
	  int m_ptx;
	 //
	  int m_pty;
	  //
	  int m_rontion;
};
struct FireStructAll
{
	  //
	  int m_index;
      int m_Have;
	  float m_scale;
	  FireStruct smfire[40];
};
//������ʾЧ��
struct FontStruct
{
	   //
	  TCHAR                   m_Infomation[500];
	   //
      int m_Have;
	  //�˳�����
      int m_Curr;
};
struct Bullet
{
	 //�ӵ��Ƿ����
	 bool  m_Have;
	 //�ӵ��ٶ�
	 int   m_Speed;
	 //�ӵ��Ƕ�
	 float m_Roation;
	 //�ӵ�����
	 POINT m_Point;
	 //�������
	 int m_Senduser;
	 //�ӵ�״̬
	 bool m_Stop;
	 //��״����
	 int m_Netindex;
	 //
	 float m_ptx;
	 //
	 float m_pty;
	 //�Ƿ񳬼��ӵ�
	 bool m_issuper;
	 //�ӵ�3��
	 int m_isThree;
	 //�Ƿ�����˷����
	 bool m_isAndroid;
	 //������ʵ
	 int  m_RealUser;
	 //�ӵ�ID
	 int  m_ZiDanID;
	 //
	 int  m_Bilv;
};
//�������
struct Fish
{
     //�������
	 int m_index;
	 //���ID
	 int m_fishID;
	 //��ǰ��������
	 int m_actionindex;
	 //��ǰ�Ƿ񱻲���
	 bool m_isHit;
	 //�Ƿ����
	 bool m_Have;
	 //��ǰ����
	 POINT m_Point;
	 //��ǰ�Ƕ�
	 float m_Roation;
	 //�������,���������ʱ���ɷ���˷��͹�����
	 int m_DeadMoney;
	 //�������,�����������ң��ɷ��������͹���
	 int m_HitUser;
	 //˽��
	 int m_Nowindex;
	 //
	 int m_PtIndex;
	 //
	 int m_Currtime;
	 //
	 int m_speed;
	 //
	 float m_ptx;
	 //
	 float m_pty;
	 //����С��
	 int  m_smallFish;
	 //
	 int  m_traceIndex;
	 //
	 int  m_creatTime;
	 //
	 int  m_RoationArr[5];
	 //
	 int  m_LiKuiScore;


};
//Ʈ����ͼƬ
struct Gold
{
	 //Ʈ�����
	 int m_Player;
	 //�Ƿ����
	 bool m_Have;
	 //��ǰ����
	 POINT m_Point;
	 //��ǰ�Ƕ�
	 float m_Roation;
	 //�������,���������ʱ���ɷ���˷��͹�����
	 int m_DeadMoney;
	  //��ǰ��������
	 int m_actionindex;
	 //
	 float m_ptx;
	 //
	 float m_pty;
     //
	 int  m_speed;
	 //
	 int  m_goldtype;

};
//��Ǯ����
struct MuchMoneyaction
{   
	 //�Ƿ����
     bool m_have;
	 //����X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	 //������Ǯ
	 int m_DiuMoney;
	 //ת���Ƕ�
	 int m_Roation;


};
//��ס����
struct Dingaction
{   
	 //�Ƿ����
     bool m_have;
	 //����X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	
};
//��ס����
struct Zhaaction
{   
	 //�Ƿ����
     bool m_have;
	 //����X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	 //������Ǯ
	 int m_DiuMoney;
	 //ת���Ƕ�
	 int m_Roation;
	
};
//���Ǯ������,ÿ�����Ӧ����3�����������ݽṹ
struct MoneyDui
{
	 int m_Have;
	 //
	 int m_Money;
	 //
	 int m_Time;
	 //
	 //
	 int m_FishScore;
	 //
	 int m_Nowscore;
	 //
	 int m_colorindex;
    
};
//�˳��л�����
struct WaterWavestruct
{
	  //�˳�����
      int m_ptx;
	  //
	  int m_pty;
	  //ʱ������
	  int m_Time;
	  //��ǰͼƬ
	  int m_currImgIndex;
}
;
//���������ʾ����
struct Numstruct
{
	  int m_Have;
	  //�˳�����
      int m_ptx;
	  //
	  int m_pty;
	  //ʱ������
	  int m_Time;
	  //��ǰͼƬ
	  int m_currImgIndex;
	  //
	  int m_Numcount;
	  //
	  int  m_HitUser;
	  //
	  float m_beishu;

}
;
//���������ʾ����
struct Soundmstruct
{
	  int m_Have;
	  int m_Time;
      LPCTSTR m_path;
	 

}
;
struct CursorStruct
{
      int m_ptx;
	  int m_pty;
	  int m_have;
};
struct DoudongStruct
{
      int m_have;
	  int m_width;
	  int m_height;
	  int m_doucount;
};
struct SuoStruct
{
      int m_ptx;
	  int m_pty;
	  int m_have;
};
