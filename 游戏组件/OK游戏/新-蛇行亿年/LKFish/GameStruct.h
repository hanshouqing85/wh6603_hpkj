//各种结构体定义
//子弹结构
#include "..\消息定义\CMD_Game.h"
struct FilesParmesA
{
    int                             m_PngSize;                              //文件大小
};
//抽奖转动
struct ChoujiangStruct
{
      bool ishave; //是否有抽奖
	  float  quanshu;//当前转了多少圈了
	  float  index;  //抽的奖是哪个
	  float  nowindex;//当前转到那部分
	  float  roloation ;
	  float  score;
};
//飞动
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
//文字显示效果
struct FontStruct
{
	   //
	  TCHAR                   m_Infomation[500];
	   //
      int m_Have;
	  //浪潮坐标
      int m_Curr;
};
struct Bullet
{
	 //子弹是否存在
	 bool  m_Have;
	 //子弹速度
	 int   m_Speed;
	 //子弹角度
	 float m_Roation;
	 //子弹坐标
	 POINT m_Point;
	 //发射玩家
	 int m_Senduser;
	 //子弹状态
	 bool m_Stop;
	 //网状序列
	 int m_Netindex;
	 //
	 float m_ptx;
	 //
	 float m_pty;
	 //是否超级子弹
	 bool m_issuper;
	 //子弹3可
	 int m_isThree;
	 //是否机器人发射的
	 bool m_isAndroid;
	 //发射真实
	 int  m_RealUser;
	 //子弹ID
	 int  m_ZiDanID;
	 //
	 int  m_Bilv;
};
//鱼儿数据
struct Fish
{
     //鱼儿序列
	 int m_index;
	 //鱼儿ID
	 int m_fishID;
	 //当前动作序列
	 int m_actionindex;
	 //当前是否被捕获
	 bool m_isHit;
	 //是否存在
	 bool m_Have;
	 //当前坐标
	 POINT m_Point;
	 //当前角度
	 float m_Roation;
	 //死亡金币,在鱼儿死亡时间由服务端发送过来的
	 int m_DeadMoney;
	 //捕获玩家,捕获这鱼的玩家，由服务器发送过来
	 int m_HitUser;
	 //私人
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
	 //附带小鱼
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
//飘落金币图片
struct Gold
{
	 //飘落玩家
	 int m_Player;
	 //是否存在
	 bool m_Have;
	 //当前坐标
	 POINT m_Point;
	 //当前角度
	 float m_Roation;
	 //死亡金币,在鱼儿死亡时间由服务端发送过来的
	 int m_DeadMoney;
	  //当前动作序列
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
//撒钱动画
struct MuchMoneyaction
{   
	 //是否存在
     bool m_have;
	 //坐标X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	 //撒多少钱
	 int m_DiuMoney;
	 //转换角度
	 int m_Roation;


};
//定住动画
struct Dingaction
{   
	 //是否存在
     bool m_have;
	 //坐标X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	
};
//定住动画
struct Zhaaction
{   
	 //是否存在
     bool m_have;
	 //坐标X
	 int m_ptx;
	 //
	 int m_pty;
	 //
	 int m_nowindex;
	 //撒多少钱
	 int m_DiuMoney;
	 //转换角度
	 int m_Roation;
	
};
//玩家钱堆数组,每个玩家应该有3个这样的数据结构
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
//浪潮切换数据
struct WaterWavestruct
{
	  //浪潮坐标
      int m_ptx;
	  //
	  int m_pty;
	  //时间索引
	  int m_Time;
	  //当前图片
	  int m_currImgIndex;
}
;
//打死鱼后显示数字
struct Numstruct
{
	  int m_Have;
	  //浪潮坐标
      int m_ptx;
	  //
	  int m_pty;
	  //时间索引
	  int m_Time;
	  //当前图片
	  int m_currImgIndex;
	  //
	  int m_Numcount;
	  //
	  int  m_HitUser;
	  //
	  float m_beishu;

}
;
//打死鱼后显示数字
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
