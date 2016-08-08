/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: review.h
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.26
******************************************************************************/

#ifndef __REVIEW_H__
#define __REVIEW_H__

#include <map>

#include "abspage.h"
#include "label_bar.h"
#include "modaldialog.h"


#define MAX_FILE                2000            // 最多支持搜索的文件个数
#define MAX_FILE_PATH           128

#define FILE_IMG                0x0001      // 图片文件
#define FILE_REC                0x0002      // 录像文件
#define FILE_FLAG               0x0004      // 标记文件

typedef struct
{
    int                 fileType;
    time_t              lastAccesstime;           // time of last modification
    char                szFilePath[MAX_FILE_PATH];
	int					width;
	int 				height;
} File_Attr_S;

typedef enum
{
    List_Begin = -1,
    List_Play,
    List_Edit,
    List_Delete,
    List_DoFlag,
    List_UnDoFlag,
    List_Done,
    List_End,
} List_Cmd_E;

class CReviewPage : public CAbsPage
{
public:

    CReviewPage(CAbsPage *parentPage = NULL);

    ~CReviewPage();

    /*进入页面的初始化事件*/
    virtual void OnPageInit();

    /*退出页面事件*/
    virtual void OnPageExit();

    /*页面按键事件*/
    virtual void OnKeyEvent(DFBWindowEvent *dfbEvent);

    /* 窗口自定义事件 */
    virtual void OnWindowEvent(Window_Msg_S *winMsg);

    /* 获取当前选中播放的文件 */
    BOOL GetCurPlayFile(char *szFile, int iLen);

    /*  */
    void Recover();


	int GetJPEGFileSize(char *filePath,int *width ,int *height );


	static void OnDeleteOKCB(void *ctx);

	/* 保存函数 */
	void OnDeleteOK();

	/* 去掉按钮回调函数 */
	static void OnCancelCB(void *ctx);

	/* 取消函数 */
	void OnCancel();

	double GetCurFileduration(){return m_curFiledration;}
	long GetAfterFileduration(){return m_afterFileDuration;}

	void DeleteSubPage();
	void UpdateMsg();
	virtual void OnUpdateArrow();

private:

    /*  */
    BOOL InitFrame();

    /*  */
    void DestoryFrame();

    /* 搜索文件, 不递归搜索目录 */
   // int SearchFiles(const char *szPath = "/media/mmcblk0p1/DCIM001");
	int SearchFiles();

	void delFileList();

    /* 无存储器件提示信息 */
    void ShowNoStorageMsg(int type);

    /* 获取文件总的个数 */
    int GetTotalFileCount(void);

	int GetRecFileDuration(char *pFilePath,char *pTimeText);


    /* 根据规则过滤搜索到的文件名 */
    BOOL FilterFiles(const char *szFileName);

    /* 根据key从map中查找文件 */
    BOOL GetFileByKey(int keyIndex, File_Attr_S *fileAttr);

	void ShowPreFileBG(File_Attr_S *fileAttr);

	void ShowNextFileBG(File_Attr_S *fileAttr);

    void ShowFileBG(File_Attr_S *fileAttr);

    /*  */
    static void AfterDrawTopLabelCB(void *data);

    /*  */
    void AfterDrawTopLabel(void);

    /*  */
    static void AfterDrawShowBGCB(void *data);

    /*  */
    void AfterDrawShowBG(void);

    /* 左键按下事件 */
    void OnLeftKeyEvent(void);

    /* 右键按下事件 */
    void OnRightKeyEvent(void);

    /* 上键按下事件 */
    void OnUpKeyEvent(void);

    /* 下键按下事件 */
    void OnDownKeyEvent(void);

    /* OK键按下事件 */
    void OnOKKeyEvent(void);

    /* SETTING键按下事件 */
    void OnSettingKeyEvent(void);

    /* REPLAY键按下事件 */
    void OnReplayKeyEvent(void);

    /* video/capture键按下事件 */
    void OnVideoCaptureKeyEvent(void);

    /* 初始化下拉框 */
    void InitListBox(void);

    /* list 框的回调函数 */
    static void OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx);

    /*  */
    void DestoryListBox(void);

    /* */
    void DrawListItem(LiteList *l, ListDrawItem *draw_item);

    /* */
    void DoCmdPlayFile(void);

    /* */
    void DoCmdEditFile(void);

    /*  */
    void DoCmdDeleteFile(void);

    /*  */
    void DoCmdFlagFile();

    /*  */
    void DoCmdDone(void);

private:

    /*  */
    CLabelBar*                          m_leftBar;

    /*  */
    CLabelBar*                          m_rightBar;

    /*  */
    LiteLabel*                          m_topLabel;

    /*  */
	LiteImage*                          m_showImage90;
    LiteImage*                          m_showImage;
	LiteImage*                          m_showPreImage;
	LiteImage*                          m_showNextImage;

	LiteImage*                          m_ReviewImage;

    /* 文件map */
  //  std::map<int,File_Attr_S>           m_fileMap;

	struct dirent **                    m_filelist;
	int                                 m_fileNum;

    /* 当前显示文件在map中的index */
    int                                 m_curKeyIndex;

    /*  */
    LiteImage*                          m_noStorageMsg;
	LiteLabel*							m_lblStorageMsg;

    LiteLabel*                          m_bottomLabel;

    LiteLabel*                          m_fileNameLabel;

    LiteImage*                          m_fileTypeImage;

    LiteImage*                          m_fileFlagImage;

    LiteList*                           m_listBox;

	LiteLabel*							m_recdration;

	CModalDialog*                       m_modalDialog;

    /*  */
    CAbsPage*                           m_subPagePtr;

	double                              m_curFiledration;

	long m_afterFileDuration;
};

#endif

