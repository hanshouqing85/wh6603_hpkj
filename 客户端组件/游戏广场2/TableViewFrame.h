#ifndef TABLE_VIEW_FRAME_HEAD_FILE
#define TABLE_VIEW_FRAME_HEAD_FILE

#pragma once

#include "Stdafx.h"
#include "Resource.h"
#include "GamePlazaHead.h"

//////////////////////////////////////////////////////////////////////////////////

//��ť��ʶ
#define BT_ENTER_GAME				1									//������Ϸ
#define BT_CLOSE_SERVER				2									//�رշ���

//��������
struct tagTableAttribute
{
	//����״̬
	WORD							wWatchCount;						//�Թ���Ŀ
	DWORD							dwTableOwnerID;						//��������

	//���Ա���
	WORD							wTableID;							//���Ӻ���
	WORD							wChairCount;						//������Ŀ
	IClientUserItem *				pIClientUserItem[MAX_CHAIR];		//�û���Ϣ
};

//////////////////////////////////////////////////////////////////////////////////

//������ͼ
class CTableView : public ITableView
{
	//״̬����
protected:
	tagTableAttribute				m_TableAttribute;					//��������

	//����ӿ�
protected:
	ITableViewFrame *				m_pITableViewFrame;					//���ӽӿ�

	//��������
public:
	//���캯��
	CTableView();
	//��������
	virtual ~CTableView();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { delete this; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ܽӿ�
public:
	//��������
	virtual WORD GetNullChairCount(WORD & wNullChairID);
	//���ú���
	virtual VOID InitTableView(WORD wTableID, WORD wChairCount, ITableViewFrame * pITableViewFrame);

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wChairID, IClientUserItem * pIClientUserItem);
};

//////////////////////////////////////////////////////////////////////////////////

//���鶨��
typedef CWHArray<CTableView *>		CTableViewArray;					//��������

//��Ϸ����
class CTableViewFrame : public CWnd, public ITableViewFrame
{
	//��Ԫ����
	friend class CTableView;

	//���Ա���
protected:
	WORD							m_wTableCount;						//��Ϸ����
	WORD							m_wChairCount;						//������Ŀ

	//��ť����
protected:
	bool							m_bHovering;						//������־
	BYTE							m_cbButtonDown;						//��ť����
	BYTE							m_cbButtonHover;					//��ť����

	//�ؼ�����
protected:
	CTableViewArray					m_TableViewArray;					//��������

	//����ӿ�
protected:
	ITableViewFrameSink *			m_pITableViewFrameSink;				//֪ͨ�ӿ�

	//��������
public:
	//���캯��
	CTableViewFrame();
	//��������
	virtual ~CTableViewFrame();

	//�����ӿ�
public:
	//�ͷŶ���
	virtual VOID Release() { return; }
	//�ӿڲ�ѯ
	virtual VOID * QueryInterface(REFGUID Guid, DWORD dwQueryVer);

	//���ýӿ�
public:
	//��������
	virtual bool ConfigTableFrame(WORD wTableCount, WORD wChairCount);
	//��������
	virtual bool CreateTableFrame(CWnd * pParentWnd, UINT uWndID, IUnknownEx * pIUnknownEx);

	//��Ϣ�ӿ�
public:
	//������Ŀ
	virtual WORD GetTableCount() { return m_wTableCount; }
	//������Ŀ
	virtual WORD GetChairCount() { return m_wChairCount; }

	//�û��ӿ�
public:
	//��ȡ�û�
	virtual IClientUserItem * GetClientUserItem(WORD wTableID, WORD wChairID);
	//������Ϣ
	virtual bool SetClientUserItem(WORD wTableID, WORD wChairID, IClientUserItem * pIClientUserItem);

	//���ܽӿ�
public:
	//��ȡ����
	virtual ITableView * GetTableViewItem(WORD wTableID);
	//��������
	virtual WORD GetNullChairCount(WORD wTableID, WORD & wNullChairID);

	//�ڲ�����
protected:
	//��ť����
	BYTE GetHoverButton(CPoint MousePoint);

	//��Ϣ����
protected:
	//�ػ���Ϣ
	VOID OnPaint();
	//����ƶ�
	VOID OnMouseMove(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonUp(UINT nFlags, CPoint Point);
	//�����Ϣ
	VOID OnLButtonDown(UINT nFlags, CPoint Point);
	//����뿪
	LRESULT OnMouseLeave(WPARAM wParam, LPARAM lParam);

	DECLARE_MESSAGE_MAP()
};

//////////////////////////////////////////////////////////////////////////////////

#endif