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

    /* 进入DirectFB的大事件循环 */
    void DoLoop(void);

    /* 获取LiTE抽象的窗口 */
    LiteWindow *GetLiTEWindow();

    /* 根据定义的page的枚举new新的page并调用OnPageInit */
    void ShowPageByID(Page_E pageID);

    /* 用于主动发送按键消息 */
    void PostKeyMessage(int iMsg);

    /* 发送子定义消息 */
    void PostWindowMessage(Window_Msg_S *winMsg);

    /* 是否有存储设备 */
    BOOL HasStorage(void);

	int  IsSDCardFree(void);

    /* 获取热插拔事件产生的目录, 假定只有SD卡*/
    char *GetDevPath(void);

    /**/
    int ComSetParity(int fd, int databits, int stopbits, int parity);

    /**/
    void ComSetSpeed(int fd, int speed);

    /*  */
    int CtrlLCDBright(TP_BRIGHTNESS_E brightLevel);

    /*  */
    void StartCheckUSB(void);

    /* 获取摇杆LED灯亮度 */
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

    /* 初始化桌面 */
    BOOL InitDesktop();

    /* 销毁桌面 */
    BOOL UnInitDesktop();

    /* 初始化LiTE抽象化的窗口 */
    DFBResult InitLiteWindow();

    /* 销毁LiTE抽象化的窗口 */
    DFBResult DestoryLiteWindow();

    /* 初始化定时器事件 */
    BOOL InitTimer();

    /* 销毁定时器 */
    BOOL DestoryTimer();

    /* 初始化并显示上背景条 */
    DFBResult InitTopLabel();

    /* 销毁上背景条的label */
    DFBResult DestoryTopLabel();

    /* 获取当前显示页面的指针 */
    CAbsPage* GetPagePtr();

    /* 定时更新显示板子电池电量的回调函数 */
    static DFBResult BoardCellCB(void *data);

    /* 显示板子电池电量 */
    void ShowBoardCell();

    /* LiTE的窗口按键回调函数 */
    static DFBResult OnDesktopKeyboardEvent(DFBWindowEvent *dfbEvent, void *data);

    /* 按键消息处理 */
    DFBResult DesktopKeyBoardEvent(DFBWindowEvent *dfbEvent);

    /* 消息处理线程 */
    static void *MsgThreadCB(void *arg);

    /* 消息处理 */
    void DoMsgThread(void);

    /**/
    int ProcMsg(MSG_BUF_S *pDst, MSG_BUF_S *pSrc, BOOL *bReply);

    /* 处理热插拔事件 */
    void DealDevChgMsg(MSG_EVENT_CHNG_DEV_S *pReq);

    /*  */
    void InitStorageMount();

    /*  */
    BOOL StorageMountCheck(char *devName);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);

    /* 读取串口进程 */
    static void *SerialThreadCB(void *arg);

    /*  */
    void SerialThread(void);

    /*  */
    int ParseNECData(char *szBuffer, int iLen);

    /*  */
    int InitLCD(void);

    /*  */
    void UninitLCD(void);

    /* 定时检测是否通过USB连接到PC上 */
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

    /* LiTE窗口指针 */
    LiteWindow*                         m_liteWin;

    /* 这里的page并不是真正的页面的抽象, 所以只能显示一个页面, 只是便于理解 */
    CAbsPage*                           m_pagePtr;

    /* 板子电量定时器ID */
    int                                 m_boardCellTimeID;

    /* 消息处理线程 ID */
    pthread_t                           m_msgThread;

    /* 上背景条 */
    LiteLabel*                          m_topLabel;

    char                                m_devPath[MAX_DEVPATH_LEN];

	char                                m_BatsValueQQ[COUNT_MAX];
	int                                 m_QQCount;

    /* LED灯亮度 */
    int                                 m_ledBrightValue;
	char                                m_ledBrightRR[COUNT_MAX];
	int                                 m_RRCount;

	char                                m_chargeMM[COUNT_MAX];
	int                                 m_MMCount;


    /* 串口数据读取处理线程 ID */
    pthread_t                           m_serialThread;

    /* 屏的FD */
    int                                 m_lcdDevFd;

    /* 检测设备是否查到电脑上 */
    int                                 m_checkUsbTimer;

    /* 是否连接到电脑上的标示 */
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

