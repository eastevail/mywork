/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: desktop.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#ifndef __DESKTOP_H__
#define __DESKTOP_H__

#include "abspage.h"
#include "common.h"
#include "type_common.h"
#include "msg_common.h"
#include "sys_msg_drv.h"
#include "CNotifictions.h"
#include "chargeManager.h"

#define  COUNT_MAX     3

class CDesktop
{
public:
    ~CDesktop();

    static CDesktop* GetDesktop(void);

	static void deskKeyLock(BOOL lock);

    /* ����DirectFB�Ĵ��¼�ѭ�� */
    void DoLoop(void);

    /* ��ȡLiTE����Ĵ��� */
    LiteWindow *GetLiTEWindow();

    /* ���ݶ����page��ö��new�µ�page������OnPageInit */
    void ShowPageByID(Page_E pageID);

    /* �����������Ͱ�����Ϣ */
    void PostKeyMessage(int iMsg);

    /* �����Ӷ�����Ϣ */
    void PostWindowMessage(Window_Msg_S *winMsg);

    /* �Ƿ��д洢�豸 */
    BOOL HasStorage(void);

	int  IsSDCardFree(void);

    /* ��ȡ�Ȳ���¼�������Ŀ¼, �ٶ�ֻ��SD��*/
    char *GetDevPath(void);

    /**/
    int ComSetParity(int fd, int databits, int stopbits, int parity);

    /**/
    void ComSetSpeed(int fd, int speed);

    /*  */
    int CtrlLCDBright(TP_BRIGHTNESS_E brightLevel);

    /*  */
    void StartCheckUSB(void);

    /* ��ȡҡ��LED������ */
    int GetRockBrightValue(void);

	Page_E getCurrentPage(){return m_curPage;};

	void setMic(int f){ if(pChargeManager)pChargeManager->GIO60_Ctrl(f);};

	int GetJPEGFileSize(char *filePath,int *width ,int *height );

	void delayKeyOperate(unsigned int utime);

	void waitingOperate(unsigned int utime, UI_FUNCTIONCB funCb,void *data);

	static DFBResult delayKeyTimeCB(void *data);

	void delayKeyTimeout();

	void SDCardUnMount(){ memset(&m_devPath, 0, MAX_DEVPATH_LEN);	};

	int videoLostDetected();

	BOOL isvideoSavePage();

	void SDCardDetected();
	int CheckCanRecordCapture();

private:
    CDesktop();

private:

    /* ��ʼ������ */
    BOOL InitDesktop();

    /* �������� */
    BOOL UnInitDesktop();

    /* ��ʼ��LiTE���󻯵Ĵ��� */
    DFBResult InitLiteWindow();

    /* ����LiTE���󻯵Ĵ��� */
    DFBResult DestoryLiteWindow();

    /* ��ʼ����ʱ���¼� */
    BOOL InitTimer();

    /* ���ٶ�ʱ�� */
    BOOL DestoryTimer();

    /* ��ʼ������ʾ�ϱ����� */
    DFBResult InitTopLabel();

    /* �����ϱ�������label */
    DFBResult DestoryTopLabel();

    /* ��ȡ��ǰ��ʾҳ���ָ�� */
    CAbsPage* GetPagePtr();

    /* ��ʱ������ʾ���ӵ�ص����Ļص����� */
    static DFBResult BoardCellCB(void *data);

    /* ��ʾ���ӵ�ص��� */
    void ShowBoardCell();

    /* LiTE�Ĵ��ڰ����ص����� */
    static DFBResult OnDesktopKeyboardEvent(DFBWindowEvent *dfbEvent, void *data);

    /* ������Ϣ���� */
    DFBResult DesktopKeyBoardEvent(DFBWindowEvent *dfbEvent);

    /* ��Ϣ�����߳� */
    static void *MsgThreadCB(void *arg);

    /* ��Ϣ���� */
    void DoMsgThread(void);

    /**/
    int ProcMsg(MSG_BUF_S *pDst, MSG_BUF_S *pSrc, BOOL *bReply);

    /* �����Ȳ���¼� */
    void DealDevChgMsg(MSG_EVENT_CHNG_DEV_S *pReq);

    /*  */
    void InitStorageMount();

    /*  */
    BOOL StorageMountCheck(char *devName);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);

    /* ��ȡ���ڽ��� */
    static void *SerialThreadCB(void *arg);

    /*  */
    void SerialThread(void);

    /*  */
    int ParseNECData(char *szBuffer, int iLen);

    /*  */
    int InitLCD(void);

    /*  */
    void UninitLCD(void);

    /* ��ʱ����Ƿ�ͨ��USB���ӵ�PC�� */
    static DFBResult CheckUSBCB(void *data);

    void DoCheckUSB(void);

	inline int isSmooth(char *array,int* count,char value);

//--------------------------------------------------------------
	void HandlePageVideoKeyEvent(DFBWindowEvent *dfbEvent);
	void HandlePageRecordKeyEvent(DFBWindowEvent *dfbEvent);
	void HandlePageSettingKeyEvent(DFBWindowEvent *dfbEvent);
	void HandlePageReviewKeyEvent(DFBWindowEvent *dfbEvent);
	void HandlePageConnectToPCKeyEvent(DFBWindowEvent *dfbEvent);

private:
    static BOOL g_keyLock;
    static CDesktop *g_desktop;

private:

    /* LiTE����ָ�� */
    LiteWindow*                         m_liteWin;

    /* �����page������������ҳ��ĳ���, ����ֻ����ʾһ��ҳ��, ֻ�Ǳ������ */
    CAbsPage*                           m_pagePtr;

    /* ���ӵ�����ʱ��ID */
    int                                 m_boardCellTimeID;

    /* ��Ϣ�����߳� ID */
    pthread_t                           m_msgThread;

    /* �ϱ����� */
    LiteLabel*                          m_topLabel;

    char                                m_devPath[MAX_DEVPATH_LEN];

	char                                m_BatsValueQQ[COUNT_MAX];
	int                                 m_QQCount;

    /* LED������ */
    int                                 m_ledBrightValue;
	char                                m_ledBrightRR[COUNT_MAX];
	int                                 m_RRCount;

	char                                m_chargeMM[COUNT_MAX];
	int                                 m_MMCount;


    /* �������ݶ�ȡ�����߳� ID */
    pthread_t                           m_serialThread;

    /* ����FD */
    int                                 m_lcdDevFd;

    /* ����豸�Ƿ�鵽������ */
    int                                 m_checkUsbTimer;

    /* �Ƿ����ӵ������ϵı�ʾ */
    bool                                m_bConnectToPC;

	bool                                m_bUSBCharge;

	ChargeManager *pChargeManager;

	LiteLabel*							m_waitingLabel;
	LiteProgressBar*                    m_waitingBar;
	UI_FUNCTIONCB                       m_waitfunCb;
	void*                               m_waitfunData;
	unsigned int                        m_waitpassTime;
	unsigned int                        m_waitsetTime;

	int  m_delayKeyTimeID;

	BOOL m_SDCardExsit;

	int m_vediofd;
	int m_vedioStatus;

//-------------------------------------------------------
private:
	void InitRunParam();
	void InitBatProp();

	void InitLcdBatPos();
	void DrawLcdBat();
	void FillLcdBat();
	void DispLcdBatBar(IDirectFBSurface *surface, int bar);

	void InitHandleBatPos();
	void DrawHandleBat();
	void FillHandleBat();
	void DispHandleBatBar(IDirectFBSurface *surface, int bar);

	void CheckBatShow();

	static void AfterDrawLabelBatCB(void *data);
	void DrawLabelBat();

	void CheckNotification();
	void CheckRunError();
	void ProcNotification();
	void CheckUsbConnect();
	void DoHeartbeat();

	void StartDateTimeCheck();
	void InitShutDownUI();
	void InitTimeStampUI();

	void UpdateBacklight();

//-------------------------------------------------------
	// Battery area
	DFBRectangle m_rcTwoBat;
	DFBColor m_clrBatNormal;
	DFBColor m_clrBatLow;
	DFBColor m_clrBatBk;
	// Handle battery
	DFBRectangle m_rcLcdBat;
	DFBRectangle m_rcLcdBat2;
	DFBRectangle m_rcLcdBat3;
	int m_blockWidthLcdBat;
	int m_spaceLcdBat;
	// LCD battery
	DFBRectangle m_rcHandleBat;
	DFBRectangle m_rcHandleBat2;
	DFBRectangle m_rcHandleBat3;
	int m_blockWidthHandleBat;
	int m_spaceHandleBat;

	LiteLabel *m_lblBat;

	// Shut Down
	LiteLabel *m_lblShutDown;
	LiteImage *m_imgLogo;

	// Time stamp
	LiteImage *m_imgStamp;
	LiteLabel *m_lblStampTime;
	LiteLabel *m_lblStampWeek;
	LiteLabel *m_lblStampDate;

public:
	CNotifictions *m_pNotifictionlMng;
	Page_E m_curPage;

public:
	BOOL isSDCardExsit();

	static void CheckTriggerUpdateCb(void *param);
	void CheckTriggerUpdate();
};

#endif

