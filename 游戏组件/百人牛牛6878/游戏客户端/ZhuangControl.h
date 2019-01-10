#pragma once

class CZhuangControl
{
public:
	CZhuangControl(void);
	~CZhuangControl(void);

	//��Դ
public:
	CBitmap							m_ImageZhuang;				//ͼƬ��Դ
	//����
private:
	int								m_CurrentIndex;				//��ǰ֡
	CPoint							m_pointBase;				//��׼λ��
	CPoint							m_pointOrigin;				//��ԭ��
	bool							m_bVisbale;					//�Ƿ�ɼ�
	CSize							m_sizeRECT;					//��ʾ��Χ
	CSize							m_FrameSize;				//֡��С
	static const int				ZHUANG_FLAME_COUNT = 41;	///��֡��
	CDC								memDC;
	int								m_nBeginIndex;				///�ȷ���˭��  2 ��   3 ��   4 ��  5 ��  1 ׯ
	int								m_nPerFrameCount;			///ÿ������8֡

	//����
public:
	///���÷�����ʼ
	void SetBeginIndex(int nIndex){m_nBeginIndex = nIndex;};
	///�õ�������ʼ
	int GetBeginIndex(){return m_nBeginIndex;};
	//���õ���
	void SetSiceValue(BYTE nValue);

	//����֡����
	void SetFrameIndex(int nIndex);
	//��ȡ֡����
	int GetFrameIndex();
	///һ������8֡�ļ�����
	void AddFrame(){m_nPerFrameCount++;};
	///�ж�һ�������Ƿ񲥷����
	BOOL isPerFrameFinish(){if(m_nPerFrameCount%4==0)
								return TRUE;
							else
								return FALSE;};
	///����ÿ������������
	void ResetPerFrame(){m_nPerFrameCount = 1;};
	//���û�׼λ��
	void SetPointBase(int x, int y);
	//���û�׼λ��
	void SetPointBase(CPoint pos);

	//��ʾ���
	bool IsFinish();

	//������ʾ��Χ
	void SetSize(int w, int h);
	//������ʾ��Χ
	void SetSize(CSize size);

	//�Ƿ���ʾ
	bool IsVisable()
	{
		return m_bVisbale;
	}
	//�Ƿ���ʾ
	void SetVisable(bool bVisbale)
	{
		m_bVisbale = bVisbale;
	}

	//�滭�˿�
	VOID DrawCardControl(CDC * pDC);
};
