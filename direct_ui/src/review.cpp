/******************************************************************************
	Copyright (C), 2008-2018, CW ELECTRONIC Tech. Co., Ltd

	File Name   	: review.cpp
	Version		: 1.0
	Author		: CW Video Server Software Group
	Created		: 2014.12.02
	Description	:
	History		:
					Create by zyfan.2014.12.18
******************************************************************************/

#include <sys/types.h>
#include <dirent.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <unistd.h>

#include "review.h"
#include "desktop.h"
#include "play_page.h"
#include "edit_page.h"

#include "avilib.h"

//-------------------------------------------------------
#define MSG_NO_CARD		0
#define MSG_NO_FILE		1

//-------------------------------------------------------
#define TOP_HEIGHT				30
#define BOTTOM_HEIGHT			30
#define PRE_NEXT_IMAGE_WIDTH	30

//-------------------------------------------------------
CReviewPage::CReviewPage(CAbsPage *parentPage)
	: CAbsPage(parentPage)
{
	m_leftBar = NULL;
	m_rightBar = NULL;
	m_topLabel = NULL;
	m_showImage = NULL;
	m_filelist = NULL;

	m_fileNum = 0;
	m_curKeyIndex = 1;

	m_noStorageMsg = NULL;
	m_lblStorageMsg = NULL;

	m_bottomLabel = NULL;
	m_fileNameLabel = NULL;
	m_fileTypeImage = NULL;
	m_fileFlagImage = NULL;
	m_listBox = NULL;
	m_subPagePtr = NULL;
	m_recdration = NULL;
	m_showImage90 = NULL;
	m_curFiledration = 0.0;
	m_ReviewImage= NULL;

	m_modalDialog = NULL;

	m_showPreImage = NULL;
	m_showNextImage = NULL;

	m_afterFileDuration = 0;
}

CReviewPage::~CReviewPage()
{
	if (m_subPagePtr != NULL)
	{
		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;
	}

	delFileList();
}

void CReviewPage::OnPageInit()
{
	CDesktop 	*pDesktop 	= CDesktop::GetDesktop();
	File_Attr_S	fileAttr;

	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_IDLE;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	if( pDesktop->isSDCardExsit() == FALSE )
	{
		ShowNoStorageMsg(MSG_NO_CARD);
		return;
	}

	if( SearchFiles() <= 0 )
	{
		ShowNoStorageMsg(MSG_NO_FILE);
		return;
	}

	// Prev image
	int nIndex = (m_curKeyIndex-1 < 1) ? m_fileNum : (m_curKeyIndex-1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));
	DBG("ShowPreFileBG, nIndex: %d\n", nIndex);
	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowPreFileBG(&fileAttr);
	}

	// Next image
	nIndex = (m_curKeyIndex+1 > m_fileNum ) ? 1 : (m_curKeyIndex+1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));
	DBG("ShowNextFileBG, nIndex: %d\n", nIndex);
	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowNextFileBG(&fileAttr);
	}

	// Current image
	memset(&fileAttr, 0, sizeof(File_Attr_S));
	DBG("ShowFileBG, nIndex: %d\n", m_curKeyIndex);
	if (GetFileByKey(m_curKeyIndex, &fileAttr))
	{
		ShowFileBG(&fileAttr);
	}

	InitFrame();

	lite_update_box(LITE_BOX(m_bottomLabel), NULL);
}

void CReviewPage::OnPageExit()
{
	DBG("OnPageExit, m_noStorageMsg: %p\n", m_noStorageMsg);

	DestoryFrame();

	SAFE_DEL_LITE(m_showImage90);

	SAFE_DEL_LITE(m_noStorageMsg);
	SAFE_DEL_LITE(m_lblStorageMsg);

	SAFE_DEL_LITE(m_showImage);
	SAFE_DEL_LITE(m_showPreImage);
	SAFE_DEL_LITE(m_showNextImage);

	SAFE_DEL_LITE(m_listBox);

	if(m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}
}

void CReviewPage::delFileList()
{
	if( m_filelist != NULL )
	{
		while(m_fileNum--)
		{
			//printf("%s\n", m_filelist[n]->d_name);
			free(m_filelist[m_fileNum]);
		}

		free(m_filelist);
	}

	m_fileNum = 0;
}

void CReviewPage::OnKeyEvent(DFBWindowEvent *dfbEvent)
{
	DBG("[DirectFBUI-CReviewPage] key_code = %X\n", dfbEvent->key_code);

	if( m_modalDialog != NULL )
	{
		DBG("OnKeyEvent, m_modalDialog: %p\n", m_modalDialog);
		m_modalDialog->OnKeyEvent(dfbEvent);
		return ;
	}

	if( m_subPagePtr != NULL )
	{
		/*if( dfbEvent->type == DWET_KEYDOWN && \
			dfbEvent->key_code == REPLAY_BTN )
		{
			m_subPagePtr->OnKeyEvent(dfbEvent);
			Recover();
		}
		else*/
		{
			m_subPagePtr->OnKeyEvent(dfbEvent);
		}
	}
	else
	{
		if( dfbEvent->type == DWET_KEYDOWN )
		{
			if( m_noStorageMsg || m_lblStorageMsg )
			{
				DBG("OnKeyEvent, m_noStorageMsg: %p\n", m_noStorageMsg);
				OnReplayKeyEvent();
			}
			else
			{
				switch( dfbEvent->key_code )
				{
					case LEFT_BTN:
					{
						//DBG("LEFT_BTN, g_RunParam.sdDetect: %d\n", g_RunParam.sdDetect);
						//if( g_RunParam.sdDetect == 0 )
							OnLeftKeyEvent();
						g_RunParam.dlyKey = 1;
						break;
					}

					case RIGHT_BTN:
					{
						//DBG("RIGHT_BTN, g_RunParam.sdDetect: %d\n", g_RunParam.sdDetect);
						//if( g_RunParam.sdDetect == 0 )
							OnRightKeyEvent();
						g_RunParam.dlyKey = 1;
						break;
					}

					case UP_BTN:
					{
						OnUpKeyEvent();
						break;
					}

					case DOWN_BTN:
					{
						OnDownKeyEvent();
						break;
					}

					case OK_BTN:
					{
						//DBG("OK_BTN, g_RunParam.sdDetect: %d\n", g_RunParam.sdDetect);
						//if( g_RunParam.sdDetect == 0 )
							OnOKKeyEvent();
						break;
					}

					case SETTING_BTN:
					{
						OnSettingKeyEvent();
						break;
					}

					case REPLAY_BTN:
					{
						OnReplayKeyEvent();
						g_RunParam.dlyKey = 1;
						break;
					}

					case VIDEO_CAP_BTN:
					{
						OnVideoCaptureKeyEvent();
						break;
					}

					default:
					{
						DBG("[DirectFBUI-VideoPage] not support this key event, key_code = %d\n", dfbEvent->key_code);
						break;
					}
				}
			}
		}
	}
}

void CReviewPage::OnWindowEvent(Window_Msg_S *winMsg)
{
	if (m_subPagePtr != NULL)
	{
		m_subPagePtr->OnWindowEvent(winMsg);
	}
}

BOOL CReviewPage::GetCurPlayFile(char *szFile, int iLen)
{
	File_Attr_S					fileAttr;

	memset(&fileAttr, 0, sizeof(File_Attr_S));

	DBG("m_curKeyIndex = %d\n",m_curKeyIndex);

	GetFileByKey(m_curKeyIndex, &fileAttr);

	strncpy(szFile, fileAttr.szFilePath, iLen - 1);

	return 0;
}

void CReviewPage::Recover()
{
	File_Attr_S		fileAttr;

	if( m_subPagePtr != NULL )
	{
		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;
	}

	DECODE_MP4_S mp4Info;

	int nIndex = (m_curKeyIndex-1 < 1) ? m_fileNum : (m_curKeyIndex-1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));

	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowPreFileBG(&fileAttr);
	}

	nIndex = (m_curKeyIndex+1 > m_fileNum ) ? 1 : (m_curKeyIndex+1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));

	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowNextFileBG(&fileAttr);
	}

	memset(&fileAttr, 0, sizeof(File_Attr_S));
	if (GetFileByKey(m_curKeyIndex, &fileAttr))
	{
		ShowFileBG(&fileAttr);
		InitFrame();
	}

	mp4Info.playState = DECODE_STATE_IDLE;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	lite_update_box(LITE_BOX(m_bottomLabel), NULL);

	if(m_showPreImage)
		lite_update_box(LITE_BOX(m_showPreImage), NULL);

	if(m_showImage90)
		lite_update_box(LITE_BOX(m_showImage90), NULL);

	if(m_showImage)
		lite_update_box(LITE_BOX(m_showImage), NULL);

	if(m_showImage)
		lite_update_box(LITE_BOX(m_showNextImage), NULL);
}

BOOL CReviewPage::InitFrame()
{
#if 1 // xjf
	DFBRectangle				dfbRect;
	CDesktop*					pDesktop 		= NULL;
	LiteWindow*					liteWin 		= NULL;
	int							screenWidth 	= 0;
	int							screenHeight	= 0;
	DFBColor					bgColor;
	DFBColor					fontColor;
	int							width			= 0;
	int							height			= 0;
	File_Attr_S					fileAttr;
	char						szPrefix[16];
	char						szExten[8];
	char						*ptr			= NULL;
	char						szfilenameTxt[16];

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	GetFileByKey(m_curKeyIndex, &fileAttr);

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-ReviewPage] lite window must init first\n");
		return FALSE;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	// 上Label
	dfbRect.x				= 0;
	dfbRect.y				= 0;
	dfbRect.w				= screenWidth - SETTING_TXT_W/2+30;
	dfbRect.h				= REVIEW_TR_H;

	bgColor.a				= TOP_LABEL_COLOR_A;
	bgColor.r				= TOP_LABEL_COLOR_R;
	bgColor.g				= TOP_LABEL_COLOR_G;
	bgColor.b				= TOP_LABEL_COLOR_B;

	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 16, NULL, &m_topLabel))
	{
		DBG("[DirectFBUI-ReviewPage] create top label failure\n");
		return FALSE;
	}

	lite_set_label_draw_after(m_topLabel, AfterDrawTopLabelCB, this);

#if 1
	// 底部label
	dfbRect.x				= 0;
	dfbRect.y				= screenHeight - REVIEW_POST_H;
	dfbRect.w				= screenWidth;
	dfbRect.h				= REVIEW_POST_H;

	bgColor.a				= REVIEW_TOP_BG_COLOR_A;
	bgColor.r				= REVIEW_TOP_BG_COLOR_R;
	bgColor.g				= REVIEW_TOP_BG_COLOR_G;
	bgColor.b				= REVIEW_TOP_BG_COLOR_B;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 16, NULL, &m_bottomLabel))
	{
		DBG("[DirectFBUI-ReviewPage] create top label failure\n");
		return FALSE;
	}

	// 显示竖条  防止覆盖, 放到底部label的初始化之后
	dfbRect.x		= 25;
	dfbRect.y		= TOP_LABEL_H;
	dfbRect.w		= 20;
	dfbRect.h		= screenHeight - TOP_LABEL_H;
	m_leftBar		= new CLabelBar(&dfbRect);

	dfbRect.x		= screenWidth - 25 - 20;
	dfbRect.y		= TOP_LABEL_H;
	dfbRect.w		= 20;
	dfbRect.h		= screenHeight - TOP_LABEL_H;
	m_rightBar		= new CLabelBar(&dfbRect);

	// 文件名
	width					= 120;
	height					= REVIEW_POST_H;
	dfbRect.x				= (screenWidth - width - REVIEW_FILE_TYPE_IMAGE_W - 2) / 2 - 22 + REVIEW_FILE_TYPE_IMAGE_W;
	dfbRect.y				= screenHeight - REVIEW_POST_H;
	dfbRect.w				= width;
	dfbRect.h				= height;

	bgColor.a				= 0xFF;
	bgColor.r				= 0x3f;
	bgColor.g				= 0x3f;
	bgColor.b				= 0x3f;
	fontColor.a				= 0xFF;
	fontColor.r				= 0xFF;
	fontColor.g				= 0xFF;
	fontColor.b				= 0xFF;
	if (DFB_OK != CreateLiTELabel(&dfbRect, &bgColor, 20, &fontColor, &m_fileNameLabel))
	{
		DBG("[DirectFBUI-ReviewPage] create time label failure\n");
		return FALSE;
	}
	lite_set_label_alignment(m_fileNameLabel, LITE_LABEL_RIGHT);
	ptr = strrchr(fileAttr.szFilePath, '/');
	if (ptr != NULL)
	{

		memset(szPrefix, 0, sizeof(szPrefix));
		memset(szExten, 0, sizeof(szExten));
		memset(szfilenameTxt, 0, sizeof(szfilenameTxt));

		sscanf(ptr + 1, "%[^.].%s", szPrefix, szExten);

		DBG("szPrefix = %s , szExten = %s\n",szPrefix, szExten);

		int k = 0, j=0;
		while(k <strlen(szPrefix))
		{
			if(szPrefix[k] >= 'a' && szPrefix[k]<='z')
				szfilenameTxt[j++] = toupper(szPrefix[k++]);
			else if((szPrefix[k] >= '0' && szPrefix[k]<= '9')||(szPrefix[k] >= 'A' && szPrefix[k]<='Z'))
				szfilenameTxt[j++] = szPrefix[k++];
			else
				k++;
		}

		DBG("szfilenameTxt = %s\n",szfilenameTxt);
		lite_set_label_text(m_fileNameLabel, szfilenameTxt);
		lite_set_label_set_visible(m_fileNameLabel, TRUE);
	}

	// 文件类型标示
	dfbRect.x 			= (screenWidth - 120 - 14 - REVIEW_FILE_TYPE_IMAGE_W) / 2;
	dfbRect.y 			= screenHeight - (REVIEW_POST_H + REVIEW_FILE_TYPE_IMAGE_H +4) / 2;
	dfbRect.w 			= REVIEW_FILE_TYPE_IMAGE_W;
	dfbRect.h 			= REVIEW_FILE_TYPE_IMAGE_H;
	if (fileAttr.fileType & FILE_IMG)
	{
		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"REVIEW_IMG_FILE, &m_fileTypeImage))
		{
			return FALSE;
		}
	}
	else if (fileAttr.fileType & FILE_REC)
	{
		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"REVIEW_REC_FILE, &m_fileTypeImage))
		{
			return FALSE;
		}
	}

	// 文件FLAG
	dfbRect.x			= screenWidth - REVIEW_POST_LEFT - REVIEW_OUT_POST_W - REVIEW_FLAG_IMAGE_W - 15;
	dfbRect.y			= screenHeight - (REVIEW_POST_H + REVIEW_FLAG_IMAGE_W) / 2;
	dfbRect.w			= REVIEW_FLAG_IMAGE_W;
	dfbRect.h			= REVIEW_FLAG_IMAGE_H;

	if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"REVIEW_FLAG_FILE, &m_fileFlagImage))
	{
		return FALSE;
	}
	lite_set_image_visible(m_fileFlagImage, false);

	if (fileAttr.fileType & FILE_FLAG)
	{
		lite_set_image_visible(m_fileFlagImage, true);
	}
#endif
#endif
	return TRUE;
}

void CReviewPage::DestoryFrame()
{
	SAFE_DEL_LITE(m_topLabel);
	SAFE_DEL_LITE(m_ReviewImage);
	SAFE_DEL_LITE(m_bottomLabel);
	SAFE_DEL_LITE(m_fileNameLabel);
	SAFE_DEL_LITE(m_fileTypeImage);
	SAFE_DEL_LITE(m_fileFlagImage);
	SAFE_DEL_LITE(m_recdration);

	if (m_leftBar != NULL)
	{
		delete m_leftBar;
		m_leftBar = NULL;
	}
	if (m_rightBar != NULL)
	{
		delete m_rightBar;
		m_rightBar = NULL;
	}
}

//-------------------------------------------------------
int CheckRecFile(const char *pFilePath)
{
	int ret = FALSE;
	avi_t *g_avi_hndl = NULL ;

	if( pFilePath == NULL )
		goto ERR_EXIT;

	if( (g_avi_hndl = AVI_open_input_file(pFilePath, 1)) != NULL )
	{
		//long duration = (g_avi_hndl->video_frames )*1.0 / g_avi_hndl->fps;
		AVI_close(g_avi_hndl);
		ret = TRUE;
	}

ERR_EXIT:
	return ret;
}

int dir_file_filter(const struct dirent *entry)
{
	char szFilePath[64];
	char szFileName[32] = {0};
	char szPrefix[16] = {0};
	char szExten[8] = {0};
	BOOL bRet = TRUE;
	int width = 0, height = 0;
	CDesktop *pDesktop = CDesktop::GetDesktop();
	char *pDevPath = pDesktop->GetDevPath();

	strcpy(szFileName, entry->d_name);
	//DBG("szFileName: %s\n", szFileName);
	// IMGxxx.jpg, RECxxx.avi; FIMGxxx.jpg, FRECxxx.avi
	if( !(strlen(szFileName) == 10 || strlen(szFileName) == 11) )
	{
		return FALSE;
	}

	//DBG("dir_file_filter, szFileName: %s\n", szFileName);

	// [REC/IMG]_0000.[JPG/jpg/AVI/avi], REC打头的是录像文件, IMG打头的是图片
	// FLAG0000.[JPG/AVI/avi/jpg] FLAG打头的是标记文件
	//memset(szPrefix, 0, sizeof(szPrefix));
	//memset(szExten, 0, sizeof(szExten));
	sscanf(szFileName, "%[^.].%s", szPrefix, szExten);
	//DBG("szPrefix: %s, szExten: %s\n", szPrefix, szExten);

#if 1 // xjf
	if( strlen(szExten) != 3 )
	{
		bRet = FALSE;
		goto ERR_EXIT;
	}
#endif

#if 1
	if( strncasecmp(szExten, "AVI", 3) && \
		strncasecmp(szExten, "JPG", 3) )
	{
		bRet = FALSE;
		goto ERR_EXIT;
	}
#endif

#if 0 // xjf
	sprintf(szFilePath, "%s/DCIM/%s", pDevPath, szFileName);

	if( strncasecmp(szExten, "JPG", 3) == 0 && \
		CDesktop::GetDesktop()->GetJPEGFileSize(szFilePath, &width, &height) == FALSE )
	{
		bRet = FALSE;
		goto ERR_EXIT;
	}
#endif

#if 0 // xjf
	sprintf(szFilePath, "%s/DCIM/%s", pDevPath, szFileName);

	if( strncasecmp(szExten, "AVI", 3) == 0 && \
		CheckRecFile(szFilePath) == FALSE )
	{
		bRet = FALSE;
		goto ERR_EXIT;
	}
#endif

	if( strncasecmp(szPrefix, "REC", 3) && \
		strncasecmp(szPrefix, "IMG", 3) && \
		strncasecmp(szPrefix, "FREC", 4) && \
		strncasecmp(szPrefix, "FIMG", 4) )
	{
		bRet = FALSE;
	}

ERR_EXIT:
#if REMOVE_BAD_FILE
	if( bRet == FALSE && g_RunParam.fileRemove.bAdd == TRUE )
	{
		g_RunParam.fileRemove.lstFiles.push_back(szFileName);
		DBG("fileRemove, szFileName: %s\n", szFileName);
	}
#endif

	return bRet;
}

int dir_file_sort(const struct dirent **a,const struct dirent **b)
{
	char szVal[5];
	int index_a = 0;
	int index_b = 0;

	memset(szVal,0,sizeof(szVal));
	sscanf( (*a)->d_name , "%*[^0-9]%[0-9]" ,szVal);
	index_a = atoi(szVal);

	memset(szVal,0,sizeof(szVal));
	sscanf( (*b)->d_name , "%*[^0-9]%[0-9]" ,szVal);
	index_b = atoi(szVal);

	return (index_a > index_b) ? FALSE : TRUE;
}

int CReviewPage::SearchFiles()
{
	CDesktop 	*pDesktop 	= CDesktop::GetDesktop();
	const char	*pDevPath	= NULL;
	char		szSearchPath[128];

	pDevPath = pDesktop->GetDevPath();
	memset(szSearchPath, 0, sizeof(szSearchPath));
	sprintf(szSearchPath, "%s/DCIM", pDevPath);

	DBG("[DirectFBUI-CReviewPage] search Files  : %s\n", szSearchPath);

#if REMOVE_BAD_FILE
	int cntFile = g_RunParam.fileRemove.lstFiles.size();
	if( cntFile <= 0 )
		g_RunParam.fileRemove.bAdd = TRUE;
	else
		g_RunParam.fileRemove.bAdd = FALSE;
#endif

	m_fileNum = scandir(szSearchPath, &m_filelist, dir_file_filter, dir_file_sort);

	DBG("[DirectFBUI-CReviewPage] search num  : %d\n", m_fileNum);

#if 0
	for( int i = 0; i < m_fileNum; i++ )
		DBG("m_filelist[%d]->d_name: %s\n", i, m_filelist[i]->d_name);
#endif

	g_RunParam.fileRemove.bAdd = FALSE;

	return m_fileNum;
}

void CReviewPage::ShowNoStorageMsg(int type)
{
	DFBRectangle			dfbRect;
	DFBColor fontColor;
	CDesktop*				pDesktop		= NULL;
	LiteWindow* 			liteWin 		= NULL;
	int  					screenWidth 	= 0;
	int 					screenHeight	= 0;
	tsLangNotiMsg *pNotiInfo;

	DBG("ShowNoStorageMsg: %d\n", type);

	if( type == MSG_NO_CARD )
	{
		// No Card
		pNotiInfo = &g_CurLangDesc.NotiMsg[Notifiction_NoSDCard];
	}
	else
	{
		// No File
		pNotiInfo = &g_CurLangDesc.NotiMsg[Notifiction_NoFile];
	}

	if( m_noStorageMsg == NULL )
	{
		pDesktop = CDesktop::GetDesktop();
		liteWin = pDesktop->GetLiTEWindow();

		lite_get_window_size(liteWin, &screenWidth, &screenHeight);

		dfbRect.x = 0;
		dfbRect.y = 0;
		dfbRect.w = screenWidth;
		dfbRect.h = screenHeight;

		if( DFB_OK == lite_new_image(LITE_BOX(liteWin), &dfbRect, liteNoImageTheme, &m_noStorageMsg) )
		{
			lite_load_image(m_noStorageMsg, pNotiInfo->imgPath.c_str());
			lite_set_image_visible(m_noStorageMsg, true);
			lite_update_box(LITE_BOX(m_noStorageMsg), NULL);
		}

		// text
		fontColor.a			= RECORD_TIME_COLOR_A;
		fontColor.r			= RECORD_TIME_COLOR_R;
		fontColor.g			= RECORD_TIME_COLOR_G;
		fontColor.b			= RECORD_TIME_COLOR_B;

		// main text 1
		dfbRect.x 			= 0;
		dfbRect.y 			= MAIN_TEXT1_POS;
		dfbRect.w 			= screenWidth;
		dfbRect.h 			= MAIN_TEXT_HEIGHT;
		if (DFB_OK == lite_new_label(LITE_BOX(liteWin), &dfbRect, NULL, MAIN_FONT_SIZE, &fontColor, &m_lblStorageMsg))
		{
			lite_set_label_alignment(m_lblStorageMsg, LITE_LABEL_CENTER);
			lite_set_label_set_visible(m_lblStorageMsg, true);
			lite_set_label_text(m_lblStorageMsg, pNotiInfo->vecMainText[0].c_str());
		}
	}
	else
	{
		lite_load_image(m_noStorageMsg, pNotiInfo->imgPath.c_str());
		lite_set_image_visible(m_noStorageMsg, true);
		lite_update_box(LITE_BOX(m_noStorageMsg), NULL);

		lite_set_label_alignment(m_lblStorageMsg, LITE_LABEL_CENTER);
		lite_set_label_set_visible(m_lblStorageMsg, true);
		lite_set_label_text(m_lblStorageMsg, pNotiInfo->vecMainText[0].c_str());
	}
}

int CReviewPage::GetTotalFileCount(void)
{
	return m_fileNum;
}

int CReviewPage::GetJPEGFileSize(char *filePath,int *width ,int *height )
{
	return CDesktop::GetDesktop()->GetJPEGFileSize(filePath,width,height);
}

BOOL CReviewPage::FilterFiles(const char *szFileName)
{
	if (szFileName == NULL)
	{
		return FALSE;
	}

	char szPrefix[16];
	char szExten[8];

	// 分析拓邦给的样机, 过滤规则如下
	// (1) 符合8.3文件命名规则
	// (2) [REC/IMG]_0000.[JPG/jpg/AVI/avi], REC打头的是录像文件, IMG打头的是图片
	// (3) FLAG0000.[JPG/AVI/avi/jpg] FLAG打头的是标记文件
	memset(szPrefix, 0, sizeof(szPrefix));
	memset(szExten, 0, sizeof(szExten));
	sscanf(szFileName, "%[^.].%s", szPrefix, szExten);

	if (strlen(szPrefix) != 8 || strlen(szExten) != 3)
	{
		return FALSE;
	}


	if (strncasecmp(szExten, "AVI", 3)
		&& strncasecmp(szExten, "JPG", 3))
	{
		return FALSE;
	}

	if (strncasecmp(szPrefix, "REC", 3)
		&& strncasecmp(szPrefix, "IMG", 3)
		&& strncasecmp(szPrefix, "FLAG", 4))
	{
		return FALSE;
	}

	return TRUE;
}

BOOL CReviewPage::GetFileByKey(int keyIndex, File_Attr_S *fileAttr)
{
	char szFileName[64] = {0};
	const char *pDevPath = NULL;
	char szfilePath[64];
	CDesktop *pDesktop 	= CDesktop::GetDesktop();
	int	maxCount = GetTotalFileCount();
	struct stat st;
	int fileType = 0;

	if( keyIndex < 1 || keyIndex > maxCount || fileAttr == NULL )
	{
		return FALSE;
	}

	pDevPath = pDesktop->GetDevPath();
	memset(szfilePath, 0, sizeof(szfilePath));
	sprintf(szfilePath, "%s/DCIM", pDevPath);

	memset(szFileName, 0, sizeof(szFileName));
	strcpy(szFileName, m_filelist[keyIndex-1]->d_name); // m_filelist from 0 to n-1

	sprintf(fileAttr->szFilePath, "%s/%s", szfilePath, szFileName);

	if( -1 == stat(fileAttr->szFilePath, &st) )
	{
		DBG("%s NO Atrrb\n", fileAttr->szFilePath);
		return FALSE;
	}

	fileAttr->lastAccesstime = st.st_mtime;

	char szPrefix[16];
	char szExten[8];

	memset(szPrefix, 0, sizeof(szPrefix));
	memset(szExten, 0, sizeof(szExten));
	sscanf(szFileName, "%[^.].%s", szPrefix, szExten);

	if( !strncasecmp(szExten, "AVI", 3) )
	{
		fileType |= FILE_REC;
	}
	else if( !strncasecmp(szExten, "JPG", 3) )
	{
		fileType |= FILE_IMG;
	}

	if( !strncasecmp(szPrefix, "FIMG", 4) || !strncasecmp(szPrefix, "FREC", 4) )
	{
		fileType |= FILE_FLAG;
	}

	fileAttr->fileType 	= fileType;

	DBG("GetFileByKey, keyIndex: %d, name: %s, type: %d, time: %u\n", \
		keyIndex, fileAttr->szFilePath, fileAttr->fileType, \
		fileAttr->lastAccesstime);

	return TRUE;
}

int CReviewPage::GetRecFileDuration(char *pFilePath,char *pTimeText)
{
	int ret = FAILURE;
	avi_t *  g_avi_hndl = NULL ;
	long duration = 0;
	unsigned int totalTime = 0;
	int min = 0;
	int sec = 0;

	if(pFilePath == NULL ||pTimeText == NULL)
		return ret;

	if ((g_avi_hndl = AVI_open_input_file(pFilePath, 1)) != NULL)
	{
		m_curFiledration = (g_avi_hndl->video_frames )*1.0f / g_avi_hndl->fps;
		duration = (long)(m_curFiledration + 0.5f);
		m_afterFileDuration = duration;
		DBG("m_afterFileDuration: %ld, m_curFiledration: %f\n", m_afterFileDuration, m_curFiledration);

		sec = (int)duration%60;
		min = (int)duration/60;

		//DBG("duration = %ld, video_frames = %ld, fps = %f\n",duration,g_avi_hndl->video_frames,g_avi_hndl->fps);

		sprintf(pTimeText,"%02d:%02d",min,sec);

		AVI_close(g_avi_hndl);

		ret = SUCCESS;
	}
	else{
		DBG("file %s Open Fail\n",pFilePath);
	}

	return ret;
}

void CReviewPage::ShowNextFileBG(File_Attr_S * fileAttr)
{
	DFBRectangle				dfbRect;
	CDesktop*					pDesktop		= NULL;
	LiteWindow* 				liteWin 		= NULL;
	int 						screenWidth 	= 0;
	int 						screenHeight	= 0;
	char						szFilePath[MAX_FILE_PATH] = {0, };
	DFBColor   bgColor;
	DFBColor   fontColor;
	int width,height;

#if 1
	pDesktop			= CDesktop::GetDesktop();
	liteWin 			= pDesktop->GetLiTEWindow();

	if (liteWin == NULL || m_fileNum <= 1)
	{
		DBG("[DirectFBUI-ReviewPage] lite window must init first\n");
		SAFE_DEL_LITE(m_showNextImage);
		return;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	dfbRect.x				= screenWidth - PRE_NEXT_IMAGE_WIDTH;
	dfbRect.y				= TOP_HEIGHT;
	dfbRect.w				= screenWidth ;
	dfbRect.h				= screenHeight - TOP_HEIGHT - BOTTOM_HEIGHT;

	memset(szFilePath, 0, MAX_FILE_PATH);

	if (fileAttr->fileType & FILE_IMG)
	{
		strncpy(szFilePath, fileAttr->szFilePath, MAX_FILE_PATH);
	}
	else if (fileAttr->fileType & FILE_REC)
	{
		// 根据规则查找JPG文件
		int index = 0;
		char *ptr = NULL;
		char *pDevPath = NULL;

		char szVal[5];
		ptr = strrchr(fileAttr->szFilePath, '/');

		if (ptr != NULL)
		{
			ptr ++;

			memset(szVal, 0, sizeof(szVal));

			sscanf(ptr, "%*[^0-9]%[0-9]", szVal) ;

			index = atoi(szVal);

			pDevPath = pDesktop->GetDevPath();
			sprintf(szFilePath, "%s/.thumbs/%03d.db", pDevPath, index);
		}

		if (access(szFilePath, F_OK) != 0)
		{
			memset(szFilePath, 0, sizeof(szFilePath));
			sprintf(szFilePath, PIC_PATH"/"NO_PREVIEW_FILE);
		}
	}

	if(GetJPEGFileSize(szFilePath,&width,&height) == FALSE)
	{
		width = 320;
		height = 240;
	}

	if(width < height)
	{
		SAFE_DEL_LITE(m_showNextImage);
		return ;
	}

	//DBG("szFilePath = %s  [width: %d x height:%d]\n",szFilePath,m_showNextImage->width,m_showNextImage->height);
	if (m_showNextImage == NULL)
	{
		DBG("szFilePath = %s  [width: %d x height:%d]\n",szFilePath,width,height);
		CreateLiTEImage(&dfbRect, szFilePath, &m_showNextImage);
	}
	else
	{
		lite_load_image(m_showNextImage, szFilePath);
	}
#endif
}

void CReviewPage::ShowPreFileBG(File_Attr_S *fileAttr)
{
	DFBRectangle				dfbRect;
	CDesktop*					pDesktop		= NULL;
	LiteWindow* 				liteWin 		= NULL;
	int 						screenWidth 	= 0;
	int 						screenHeight	= 0;
	char						szFilePath[MAX_FILE_PATH] = {0, };
	DFBColor   bgColor;
	DFBColor   fontColor;
	int width,height;

#if 1
	pDesktop			= CDesktop::GetDesktop();
	liteWin 			= pDesktop->GetLiTEWindow();

	if (liteWin == NULL || m_fileNum <= 1)
	{
		DBG("[DirectFBUI-ReviewPage] lite window must init first\n");
		SAFE_DEL_LITE(m_showPreImage);
		return;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	dfbRect.x				= 0;
	dfbRect.y				= TOP_HEIGHT;
	dfbRect.w				= 40 ;
	dfbRect.h				= screenHeight - TOP_HEIGHT - BOTTOM_HEIGHT;

	memset(szFilePath, 0, MAX_FILE_PATH);

	if (fileAttr->fileType & FILE_IMG)
	{
		strncpy(szFilePath, fileAttr->szFilePath, MAX_FILE_PATH);
	}
	else if (fileAttr->fileType & FILE_REC)
	{
		// 根据规则查找JPG文件
		int index = 0;
		char *ptr = NULL;
		char *pDevPath = NULL;

		char szVal[5];

		ptr = strrchr(fileAttr->szFilePath, '/');

		if (ptr != NULL)
		{
			ptr ++;

			memset(szVal,0,sizeof(szVal));

			sscanf( ptr , "%*[^0-9]%[0-9]" ,szVal) ;

			index = atoi(szVal);

			pDevPath = pDesktop->GetDevPath();
			sprintf(szFilePath, "%s/.thumbs/%03d.db", pDevPath, index);
		}

		if (access(szFilePath, F_OK) != 0)
		{
			memset(szFilePath, 0, sizeof(szFilePath));
			sprintf(szFilePath, PIC_PATH"/"NO_PREVIEW_FILE);
		}
	}

	if(GetJPEGFileSize(szFilePath,&width,&height) == FALSE)
	{
		width = 320;
		height = 240;
	}

	if(width < height)
	{
		SAFE_DEL_LITE(m_showPreImage);
		return ;
	}

	DFBRectangle fbRect;

	fbRect.x				= 640 - 100;
	fbRect.y				= 0;
	fbRect.w				= 100;
	fbRect.h				= 480;

	if (m_showPreImage == NULL)
	{
		DBG("ShowPreFileBG, szFilePath = %s  [width: %d x height: %d]\n", \
			szFilePath, dfbRect.w, dfbRect.h);
		CreateLiTEImage(&dfbRect, szFilePath, &m_showPreImage);
		lite_set_image_clipping(m_showPreImage, &fbRect);
	}
	else
	{
		lite_get_image_size(m_showPreImage, &width, &height);
		DBG("ShowPreFileBG, szFilePath = %s  [width: %d x height: %d]\n", \
			szFilePath, width, height);
		lite_load_image(m_showPreImage, szFilePath);
		lite_set_image_clipping(m_showPreImage, &fbRect);
	}
#endif
}

void CReviewPage::ShowFileBG(File_Attr_S *fileAttr)
{
	DFBRectangle				dfbRect;
	CDesktop*					pDesktop 		= NULL;
	LiteWindow*					liteWin 		= NULL;
	int							screenWidth 	= 0;
	int							screenHeight	= 0;
	char						szFilePath[MAX_FILE_PATH] = {0, };
	DFBColor   bgColor;
	DFBColor   fontColor;
	int width,height;

#if 1
	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-ReviewPage] lite window must init first\n");
		return;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	memset(szFilePath, 0, MAX_FILE_PATH);

	if (fileAttr->fileType & FILE_IMG)
	{
		strncpy(szFilePath, fileAttr->szFilePath, MAX_FILE_PATH);

		if(m_recdration != NULL)
		{
			lite_set_label_set_visible(m_recdration,FALSE);
		}
	}
	else if (fileAttr->fileType & FILE_REC)
	{
		// 根据规则查找JPG文件
		int index = 0;
		char *ptr = NULL;
		char *pDevPath = NULL;
		char szVal[5];

		//DBG("szFilePath = %s\n",fileAttr->szFilePath);

		ptr = strrchr(fileAttr->szFilePath, '/');
		if (ptr != NULL)
		{
			ptr ++;

			memset(szVal,0,sizeof(szVal));

			sscanf( ptr , "%*[^0-9]%[0-9]" ,szVal) ;

			index = atoi(szVal);

			DBG("index = %d\n",index);

			pDevPath = pDesktop->GetDevPath();
			sprintf(szFilePath, "%s/.thumbs/%03d.db", pDevPath, index);
		}

		if (access(szFilePath, F_OK) != 0)
		{
			memset(szFilePath, 0, sizeof(szFilePath));
			sprintf(szFilePath, PIC_PATH"/"NO_PREVIEW_FILE);
		}
	}

	if(GetJPEGFileSize(szFilePath,&width,&height) == FALSE)
	{
		width = 320;
		height = 240;
	}

	g_RunParam.pictProp.pic_width = width;
	g_RunParam.pictProp.pic_height = height;
	sprintf(g_RunParam.pictProp.pic_name, "%s", szFilePath);

	if( width >= height )
	{
		SAFE_DEL_LITE(m_showImage90);

		dfbRect.x				= PRE_NEXT_IMAGE_WIDTH;
		dfbRect.y				= TOP_HEIGHT;
		dfbRect.w				= screenWidth- 30*2 ;
		dfbRect.h				= screenHeight- TOP_HEIGHT - BOTTOM_HEIGHT;

		if(m_showImage == NULL)
		{
			DBG("szFilePath = %s  [width: %d x height:%d]\n",szFilePath,width,height);
			CreateLiTEImage(&dfbRect, szFilePath, &m_showImage);
			lite_set_image_draw_after(m_showImage, AfterDrawShowBGCB, this);
		}
		else
		{
			DBG("ShowFileBG, szFilePath = %s  [width: %d x height:%d]\n", \
				szFilePath, width, height);
			lite_load_image(m_showImage, szFilePath);
		}
	}
	else
	{
		SAFE_DEL_LITE(m_showImage);

		dfbRect.h				= screenHeight- TOP_HEIGHT - BOTTOM_HEIGHT;
		dfbRect.w				= (int)(dfbRect.h*(width*1.0/ height));
		dfbRect.x				= (screenWidth - dfbRect.w)/2;
		dfbRect.y				= TOP_HEIGHT;

		if(m_showImage90 == NULL)
		{
			DBG("szFilePath = %s  [width: %d x height:%d]\n",szFilePath,width,height);
			CreateLiTEImage(&dfbRect, szFilePath, &m_showImage90);
			lite_set_image_draw_after(m_showImage90, AfterDrawShowBGCB, this);
		}
		else
		{
			DBG("ShowFileBG, szFilePath = %s  [width: %d x height:%d]\n", \
				szFilePath, width, height);
			lite_load_image(m_showImage90, szFilePath);
		}
	}

	//DBG("x:%d y:%d [width: %d  height:%d]\n",dfbRect.x,dfbRect.y,dfbRect.w,dfbRect.h);
	//DBG("[DirectFBUI-ReviewPage] show image %s success\n", szFilePath);
#endif
}

void CReviewPage::AfterDrawTopLabelCB(void *data)
{
	CReviewPage *pPage = (CReviewPage *)data;

	if (pPage)
	{
		pPage->AfterDrawTopLabel();
	}
}

void CReviewPage::AfterDrawTopLabel(void)
{
	if (m_topLabel == NULL)
	{
		return;
	}

	DFBRectangle dfbRect;

	LiteBox*			liteBox			= LITE_BOX(m_topLabel);
	IDirectFBSurface*	surface			= liteBox->surface;
	int					height			= 16; 	// pixel
	int					left			= 2;	// pixel
	int					width			= 10;	// pixel
	DFBPoint			p1, p2, p3;
	int					space			= 100;	// pixel
	char 				szBuf[MAX_LANG_LEN];
	IDirectFBFont*		fontInterface 	= NULL;
	int					fontWidth 		= 0;
	int					fontHeight		= 0;
	DFBSurfaceTextFlags flags 			= DSTF_TOP;

	if(m_ReviewImage == NULL)
	{
		dfbRect.x			= PLAY_REVIEW_IMG_X;
		dfbRect.y			= PLAY_REVIEW_IMG_Y;
		dfbRect.w			= PLAY_REVIEW_IMG_W;
		dfbRect.h			= PLAY_REVIEW_IMG_H;

		if (DFB_OK != CreateLiTEImage(&dfbRect, PIC_PATH"/"PLAY_REVIEW_FILE, &m_ReviewImage))
		{
			return FALSE;
		}
	}

	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

	// fill triangles
	height							= 16;
	width							= 10;
	p1.x							= liteBox->rect.w - 120 + SETTING_TXT_W/2 -20;
	p1.y							= liteBox->rect.h / 2;
	p2.x							= liteBox->rect.w - width - 120 + SETTING_TXT_W/2-20;
	p2.y							= (liteBox->rect.h - height) / 2;
	p3.x							= liteBox->rect.w - width - 120 + SETTING_TXT_W/2-20;
	p3.y							= (liteBox->rect.h + height) / 2;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	p1.x							= liteBox->rect.w - space - 120 + SETTING_TXT_W/2-20;
	p1.y							= liteBox->rect.h / 2;
	p2.x							= liteBox->rect.w - space + width - 120 + SETTING_TXT_W/2-20;
	p2.y							= (liteBox->rect.h - height) / 2;
	p3.x							= liteBox->rect.w - space + width - 120 + SETTING_TXT_W/2-20;
	p3.y							= (liteBox->rect.h + height) / 2;
	surface->FillTriangle(surface, p1.x, p1.y, p2.x, p2.y, p3.x, p3.y);

	//memset(szBuf, 0, MAX_LANG_LEN);
	//sprintf(szBuf, "%d / %d", m_curKeyIndex, GetTotalFileCount());
	sprintf(szBuf, "%d / %d", GetTotalFileCount()-(m_curKeyIndex-1), GetTotalFileCount());
	surface->GetFont(surface, &fontInterface);

	fontInterface->GetHeight(fontInterface, &fontHeight);
	fontInterface->GetStringWidth(fontInterface, szBuf, strlen(szBuf), &fontWidth);
	surface->DrawString(surface, szBuf, -1, (space - fontWidth) / 2 + (liteBox->rect.w - space + SETTING_TXT_W/2) - 120-20, (liteBox->rect.h - fontHeight) / 2, flags);

	lite_update_box(LITE_BOX(m_ReviewImage), NULL);
}

void CReviewPage::AfterDrawShowBGCB(void *data)
{
	CReviewPage *pPage = (CReviewPage *)data;

	if (pPage)
	{
		pPage->AfterDrawShowBG();
	}
}

void CReviewPage::AfterDrawShowBG(void)
{
	LiteBox*			liteBox			= NULL;
	IDirectFBSurface*	surface			= NULL;
	DFBSurfaceTextFlags flags 			= DSTF_TOP;
	File_Attr_S			fileAttr;
	struct tm*          fileLocalTime	= NULL;
	char				szBuf[MAX_LANG_LEN];
	int					iHour			= 0;
	int					iLen			= 0;
	CDesktop*			pDesktop 		= NULL;
	LiteWindow*			liteWin 		= NULL;
	int 				screenWidth		= 0;
	int					screenHeight	= 0;

	DBG("AfterDrawShowBG\n");

	if(m_showImage)
		liteBox = LITE_BOX(m_showImage);
	else if(m_showImage90)
		liteBox = LITE_BOX(m_showImage90);
	else
		return;

	surface			= liteBox->surface;

	GetFileByKey(m_curKeyIndex, &fileAttr);
	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();
	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	// 显示时间
#if 0
	fileLocalTime = localtime(&fileAttr.lastAccesstime);
	memset(szBuf, 0, MAX_LANG_LEN);
	iHour = (fileLocalTime->tm_hour >= 12) ? (fileLocalTime->tm_hour - 12) : fileLocalTime->tm_hour;
	if (iHour < 10)
	{
		iLen += sprintf(szBuf + iLen, "0%d:", iHour);
	}
	else
	{
		iLen += sprintf(szBuf + iLen, "%d:", iHour);
	}

	if (fileLocalTime->tm_min < 10)
	{
		iLen += sprintf(szBuf + iLen, "0%d ", fileLocalTime->tm_min);
	}
	else
	{
		iLen += sprintf(szBuf + iLen, "%d ", fileLocalTime->tm_min);
	}
	if (fileLocalTime->tm_hour < 12)
	{
		iLen += sprintf(szBuf + iLen, "%s", "AM");
	}
	else
	{
		iLen += sprintf(szBuf + iLen, "%s", "PM");
	}
#else
	// xjf
	fileLocalTime = localtime(&fileAttr.lastAccesstime);

	if( g_RunParam.tuobangPara.TimeFmt == TIME_FMT_12HR )
	{
		if( fileLocalTime->tm_hour >= 12 )
		{
			if( fileLocalTime->tm_hour == 12 )
				sprintf(szBuf, "%02d:%02d %s", fileLocalTime->tm_hour, fileLocalTime->tm_min, g_CurLangDesc.Public.PM.c_str());
			else
				sprintf(szBuf, "%02d:%02d %s", (fileLocalTime->tm_hour-12), fileLocalTime->tm_min, g_CurLangDesc.Public.PM.c_str());
		}
		else
		{
			if( fileLocalTime->tm_hour == 0 )
				sprintf(szBuf, "%02d:%02d %s", (fileLocalTime->tm_hour+12), fileLocalTime->tm_min, g_CurLangDesc.Public.AM.c_str());
			else
				sprintf(szBuf, "%02d:%02d %s", fileLocalTime->tm_hour, fileLocalTime->tm_min, g_CurLangDesc.Public.AM.c_str());
		}
	}
	else
	{
		sprintf(szBuf, "%02d:%02d", fileLocalTime->tm_hour, fileLocalTime->tm_min);
	}
#endif

	if(m_showImage90)
		surface->DrawString(surface, szBuf, -1, REVIEW_OUT_POST_W/2 - 2, 3, flags);
	else
		surface->DrawString(surface, szBuf, -1, REVIEW_OUT_POST_W, 3, flags);

	// 显示文件日期
	memset(szBuf, 0, MAX_LANG_LEN);
	iLen = 0;

	if (fileLocalTime->tm_mon+1 < 10)
	{
		iLen += sprintf(szBuf + iLen, "0%d/", fileLocalTime->tm_mon+1);
	}
	else
	{
		iLen += sprintf(szBuf + iLen, "%d/", fileLocalTime->tm_mon+1);
	}


	if (fileLocalTime->tm_mday < 10)
	{
		iLen += sprintf(szBuf + iLen, "0%d/", fileLocalTime->tm_mday);
	}
	else
	{
		iLen += sprintf(szBuf + iLen, "%d/", fileLocalTime->tm_mday);
	}

	iLen += sprintf(szBuf + iLen, "%d", fileLocalTime->tm_year + 1900);

	if(m_showImage90)
		surface->DrawString(surface, szBuf, -1,  REVIEW_OUT_POST_W/2 - 2, 15, flags);
	else
		surface->DrawString(surface, szBuf, -1, screenWidth- 30*2 - 90, 3, flags);

#if 1
	if (fileAttr.fileType & FILE_REC)
	{
		DFBColor   bgColor;
		DFBColor   fontColor;
		DFBRectangle dfbRect;

		char szTimeText[32] = {0};

		if(GetRecFileDuration(fileAttr.szFilePath,szTimeText) == SUCCESS)
		{

			dfbRect.x				= 50;
			dfbRect.y				= 35;
			dfbRect.w				= 100;
			dfbRect.h				= 30;

			bgColor.a				= TOP_LABEL_COLOR_A;
			bgColor.r				= 0;
			bgColor.g				= 0;
			bgColor.b				= 0;

			fontColor.a 			= 0xFF;
			fontColor.r 			= 0xFF;
			fontColor.g 			= 0xFF;
			fontColor.b 			= 0xFF;

			if(m_showImage90)
				surface->DrawString(surface, szTimeText, -1,  REVIEW_OUT_POST_W/2 -2, 160, flags);
			else
				surface->DrawString(surface, szTimeText, -1, REVIEW_OUT_POST_W , 160, flags);
		}
	}
#endif
}

void CReviewPage::OnLeftKeyEvent(void)
{
	int 			totalCount = GetTotalFileCount();
	File_Attr_S		fileAttr;

	if (m_listBox != NULL)
	{
		return;
	}

	m_curKeyIndex 	-= 1;
	m_curKeyIndex	= (m_curKeyIndex <= 0) ? totalCount : m_curKeyIndex;

	int nIndex = (m_curKeyIndex-1 < 1) ? m_fileNum : (m_curKeyIndex-1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));

	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowPreFileBG(&fileAttr);
	}

	nIndex = (m_curKeyIndex+1 > m_fileNum ) ? 1 : (m_curKeyIndex+1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));

	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowNextFileBG(&fileAttr);
	}

	memset(&fileAttr, 0, sizeof(File_Attr_S));
	if (GetFileByKey(m_curKeyIndex, &fileAttr))
	{
		DestoryFrame();
		ShowFileBG(&fileAttr);
		InitFrame();
	}
	else
	{
		OnLeftKeyEvent();
	}
}

void CReviewPage::OnRightKeyEvent(void)
{
	int 			totalCount = GetTotalFileCount();
	File_Attr_S		fileAttr;

	if (m_listBox != NULL)
	{
		return;
	}

	m_curKeyIndex 	+= 1;
	m_curKeyIndex	= (m_curKeyIndex > totalCount) ? 1 : m_curKeyIndex;

	int nIndex = (m_curKeyIndex-1 < 1) ? m_fileNum : (m_curKeyIndex-1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));

	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowPreFileBG(&fileAttr);
	}

	nIndex = (m_curKeyIndex+1 > m_fileNum ) ? 1 : (m_curKeyIndex+1);
	memset(&fileAttr, 0, sizeof(File_Attr_S));

	if (GetFileByKey(nIndex, &fileAttr))
	{
		ShowNextFileBG(&fileAttr);
	}

	memset(&fileAttr, 0, sizeof(File_Attr_S));
	if (GetFileByKey(m_curKeyIndex, &fileAttr))
	{
		DestoryFrame();
		ShowFileBG(&fileAttr);
		InitFrame();
	}
	else
	{
		OnRightKeyEvent();
	}
}

void CReviewPage::OnUpKeyEvent(void)
{
	if (m_listBox == NULL)
	{
		return;
	}

	int curSelIndex = 0;
	int totalCount = 0;

	if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
	{
		DBG("[DirectFBUI-Review] get list index failure\n");
		return;
	}

	if (DFB_OK != lite_list_get_item_count(m_listBox, &totalCount))
	{
		DBG("[DirectFBUI-Review] get list count failure\n");
		return;
	}

	curSelIndex -= 1;
	curSelIndex = (curSelIndex < 0) ? (totalCount - 1) : curSelIndex;

	lite_list_set_selected_item_index(m_listBox, curSelIndex);
}

void CReviewPage::OnDownKeyEvent(void)
{
	if (m_listBox == NULL)
	{
		return;
	}

	int curSelIndex = 0;
	int totalCount = 0;

	if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
	{
		DBG("[DirectFBUI-Review] get list index failure\n");
		return;
	}

	if (DFB_OK != lite_list_get_item_count(m_listBox, &totalCount))
	{
		DBG("[DirectFBUI-Review] get list count failure\n");
		return;
	}

	curSelIndex += 1;
	curSelIndex = (curSelIndex >= totalCount) ? 0 : curSelIndex;

	lite_list_set_selected_item_index(m_listBox, curSelIndex);
}

void CReviewPage::OnOKKeyEvent(void)
{
	if( m_listBox == NULL )
	{
		// 执行初始化
		InitListBox();
		g_RunParam.dlyKey = 1;
	}
	else
	{
		// 获取listBox命令
		int curSelIndex = 0;
		List_Cmd_E listCmd = List_Begin;
		unsigned long temp = 0;

		if (DFB_OK != lite_list_get_selected_item_index(m_listBox, &curSelIndex))
		{
			DBG("[DirectFBUI-Review] get list index failure\n");
			return;
		}

		if (DFB_OK != lite_list_get_item(m_listBox, curSelIndex, (LiteListItemData *)&temp))
		{
			DBG("lite_list_get_item error\n");
			return;
		}

		listCmd = (List_Cmd_E)temp;
		switch (listCmd)
		{
			case List_Play:
			{
				DoCmdPlayFile();
				g_RunParam.dlyKey = 2;
				break;
			}

			case List_Edit:
			{
				DoCmdEditFile();
				g_RunParam.dlyKey = 1;
				break;
			}

			case List_Delete:
			{
				DoCmdDeleteFile();
				g_RunParam.dlyKey = 1;
				break;
			}

			case List_DoFlag:
			case List_UnDoFlag:
			{
				DoCmdFlagFile();
				DoCmdDone();
				g_RunParam.dlyKey = 1;
				break;
			}

			case List_Done:
			{
				DoCmdDone();
				g_RunParam.dlyKey = 1;
				break;
			}

			default:
			{
				break;
			}
		}
	}
}

void CReviewPage::OnSettingKeyEvent(void)
{

}

void CReviewPage::OnReplayKeyEvent(void)
{
	CDesktop *pDesktop = CDesktop::GetDesktop();
	pDesktop->ShowPageByID(DEFAULT_PAGE);
}

void CReviewPage::OnVideoCaptureKeyEvent(void)
{

}

void CReviewPage::InitListBox(void)
{
	CDesktop*			pDesktop 		= NULL;
	LiteWindow*			liteWin 		= NULL;
	DFBRectangle		dfbRect;
	int 				screenWidth		= 0;
	int					screenHeight 	= 0;
	int					count			= 0;
	File_Attr_S			fileAttr;
	int					listCmd			= 0;

	pDesktop			= CDesktop::GetDesktop();
	liteWin				= pDesktop->GetLiTEWindow();

	if (liteWin == NULL)
	{
		DBG("[DirectFBUI-review] lite window must init first\n");
		return;
	}

	lite_get_window_size(liteWin, &screenWidth, &screenHeight);

	DBG("liteDefaultListTheme = %p\n", liteDefaultListTheme);

	if (liteDefaultListTheme == NULL)
	{
		if (DFB_OK != lite_new_list_theme(PIC_PATH"/"SCROLL_BAR_FILE, 3, &liteDefaultListTheme))
		{
			DBG("DirectFB_UI-review: Load list theme fail\n");
			return FALSE;
		}
	}

	dfbRect.x 			= 20 + 20 + 1;
	dfbRect.y 			= TOP_LABEL_H + 1;
	dfbRect.w 			= screenWidth - 2 * (20 + 20 + 1);
	dfbRect.h 			= 160;

	if (DFB_OK != lite_new_list(LITE_BOX(liteWin), &dfbRect, liteDefaultListTheme, 18, &m_listBox))
	{
		DBG("DirectFB_UI-review: Create list fail\n");
		return;
	}

	if (DFB_OK != lite_list_set_row_height(m_listBox, dfbRect.h/4))
	{
		DBG("DirectFB_UI-settingPage: set list row height failure\n");
		return FALSE;
	}

	memset(&fileAttr, 0, sizeof(File_Attr_S));
	GetFileByKey(m_curKeyIndex, &fileAttr);

	lite_list_get_item_count(m_listBox, &count);
	if (fileAttr.fileType & FILE_REC)
	{
		lite_list_insert_item(m_listBox, count, (LiteListItemData)List_Play);
	}
	else if (fileAttr.fileType & FILE_IMG)
	{
		lite_list_insert_item(m_listBox, count, (LiteListItemData)List_Edit);
	}

	// 添加delete
	lite_list_get_item_count(m_listBox, &count);
	lite_list_insert_item(m_listBox, count, (LiteListItemData)List_Delete);

	// do/undo flag
	lite_list_get_item_count(m_listBox, &count);
	if (fileAttr.fileType & FILE_FLAG)
	{
		lite_list_insert_item(m_listBox, count, (LiteListItemData)List_UnDoFlag);
	}
	else
	{
		lite_list_insert_item(m_listBox, count, (LiteListItemData)List_DoFlag);
	}

	// done
	lite_list_get_item_count(m_listBox, &count);
	lite_list_insert_item(m_listBox, count, (LiteListItemData)List_Done);

	lite_list_on_draw_item(m_listBox, OnListDrawItem, this);

	lite_list_set_selected_item_index(m_listBox, 0);
}

void CReviewPage::OnListDrawItem(LiteList *l, ListDrawItem *draw_item, void *ctx)
{
	CReviewPage *pPage = (CReviewPage *)ctx;

	if (pPage)
	{
		pPage->DrawListItem(l, draw_item);
	}
}

void CReviewPage::DestoryListBox(void)
{
	SAFE_DEL_LITE(m_listBox);
}

void CReviewPage::DrawListItem(LiteList *l, ListDrawItem *draw_item)
{
	IDirectFBSurface*		surface 					= draw_item->surface;
	List_Cmd_E				listCmd						= List_Begin;
	IDirectFBFont*			fontInterface 				= NULL;
	int 					fontHeight 						= 0;
	int						fontWidth						= 0;

	if (surface == NULL)
	{
		return;
	}

	listCmd = (List_Cmd_E)draw_item->item_data;
	if (draw_item->selected)
	{
		surface->SetColor(surface, LIST_BG_ACTIVE_COLOR_R, LIST_BG_ACTIVE_COLOR_G,
							LIST_BG_ACTIVE_COLOR_B, LIST_BG_ACTIVE_COLOR_A);
	}
	else
	{
		surface->SetColor(surface, LIST_BG_COLOR_R, LIST_BG_COLOR_G, LIST_BG_COLOR_B, LIST_BG_COLOR_A);
	}

	surface->FillRectangle(surface, draw_item->rc_item.x, draw_item->rc_item.y, draw_item->rc_item.w, draw_item->rc_item.h);

	char *pItem = NULL;

	switch (listCmd)
	{
		case List_Play:
		{
			pItem = g_CurLangDesc.Review.Play.c_str();
			break;
		}

		case List_Edit:
		{
			pItem = g_CurLangDesc.Review.Edit.c_str();
			break;
		}

		case List_Delete:
		{
			pItem = g_CurLangDesc.Review.Delete.c_str();
			break;
		}

		case List_DoFlag:
		{
			pItem = g_CurLangDesc.Review.Flag.c_str();
			break;
		}

		case List_UnDoFlag:
		{
			pItem = g_CurLangDesc.Review.RemoveFlag.c_str();
			break;
		}

		case List_Done:
		{
			pItem = g_CurLangDesc.Review.Done.c_str();
			break;
		}

		default:
		{
			DBG("DirectFB_UI-review: do not support %d\n", listCmd);
			break;
		}
	}

	surface->GetFont(surface, &fontInterface);

	fontInterface->GetHeight(fontInterface, &fontHeight);
	fontInterface->GetStringWidth(fontInterface, pItem, strlen(pItem), &fontWidth);

	surface->SetColor(surface, 0xFF, 0xFF, 0xFF, 0xFF);
	surface->DrawString(surface, pItem, -1, draw_item->rc_item.x + (draw_item->rc_item.w - fontWidth) / 2,
						draw_item->rc_item.y + (draw_item->rc_item.h - fontHeight) / 2, DSTF_TOP);
}

void CReviewPage::DoCmdPlayFile(void)
{
	if (m_subPagePtr != NULL)
	{
		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;
	}

	OnPageExit();

	m_subPagePtr = new CPlayPage(this);
	m_subPagePtr->OnPageInit();
}

void CReviewPage::DoCmdEditFile(void)
{
	if (m_subPagePtr != NULL)
	{
		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;
	}

	OnPageExit();

	m_subPagePtr = new CEditPage(this);
	m_subPagePtr->OnPageInit();
}

void CReviewPage::OnDeleteOKCB(void *ctx)
{
	CReviewPage *pPage = (CReviewPage *)ctx;

	if (pPage)
	{
		pPage->OnDeleteOK();
	}
}

void CReviewPage::OnDeleteOK()
{
	File_Attr_S 	fileAttr;
	char			szCmd[64];

	DBG("[DirectFBUI-CReviewPage] OnDeleteOK\n");

	memset(&fileAttr, 0, sizeof(File_Attr_S));
	memset(szCmd, 0, sizeof(szCmd));

	if( GetFileByKey(m_curKeyIndex, &fileAttr) )
	{
		int ra = remove(fileAttr.szFilePath);
		DBG("remove, ret: %d, %s\n", ra, fileAttr.szFilePath);

		//sprintf(szCmd, "rm -fr %s", fileAttr.szFilePath);
		//system(szCmd);
		//DBG("DirectFB_UI-review: szCmd : remove ret = %d : %s\n", ra,fileAttr.szFilePath);

		if( m_curKeyIndex >= m_fileNum )
		{
			//rcrd_msg_event_req_strginfo();
			usleep(200000);

			m_curKeyIndex = m_fileNum - 1;
		}
	}

	if( m_modalDialog )
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}

	DoCmdDone();
}

void CReviewPage::OnCancelCB(void *ctx)
{
	CReviewPage *pPage = (CReviewPage *)ctx;

	if (pPage)
	{
		pPage->OnCancel();
	}
}

void CReviewPage::OnCancel()
{
	DBG("[DirectFBUI-CReviewPage] OnCancel\n");

	if (m_modalDialog)
	{
		delete m_modalDialog;
		m_modalDialog = NULL;
	}

	DoCmdDone();
}

void CReviewPage::DoCmdDeleteFile(void)
{
	DFBRectangle		dfbRect;
	char				szTextBuf[MAX_LANG_LEN];
	char				szCurFile[MAX_LANG_LEN];
	char				szCurFileName[MAX_LANG_LEN];
	char                *ptr;

	dfbRect.x			= 0;
	dfbRect.y			= TOP_LABEL_H-2;
	dfbRect.w			= 320;
	dfbRect.h			= 240 ;

	DBG("[DirectFBUI-VideoPage] create dialog\n");
	m_modalDialog		= new CModalDialog(dfbRect);

	m_modalDialog->SetOKCB(g_CurLangDesc.Public.Cancel.c_str(), OnCancelCB, this);
	m_modalDialog->SetCancelCB(g_CurLangDesc.Public.Delete.c_str(), OnDeleteOKCB, this);

	GetCurPlayFile(szCurFile, MAX_FILE_LEN);
	if (NULL == (ptr = strrchr(szCurFile, '/')))
	{
		delete m_modalDialog;
		return;
	}

	memset(szCurFileName, 0, MAX_LANG_LEN);

	strncpy(szCurFileName, ptr + 1, sizeof(szCurFileName) - 1);

	int k = 0;
	while( k <strlen(szCurFileName) )
	{
		if( szCurFileName[k] >= 'a' && szCurFileName[k]<='z' )
			szCurFileName[k] = toupper(szCurFileName[k]);
		k++;
	}

	DBG("delete %s?\n", szCurFileName);

	LoadDelMsg(szCurFileName, szTextBuf);
	m_modalDialog->setDailogText(szTextBuf, NULL);

	m_modalDialog->ShowDialog();
}

void CReviewPage::DoCmdFlagFile()
{
	File_Attr_S		fileAttr;
	File_Attr_S		new_fileAttr;
	char 			szFileName[64] = {0, };
	char 			szPrefix[16] = {0, };
	char 			szExten[8] = {0, };
	char			szTemp[128] = {0, };
	int				index = 0;
	char			*ptr = NULL;
	int				fileType 		= 0;
	struct stat 	st;

	memset(&fileAttr, 0, sizeof(File_Attr_S));
	memset(&new_fileAttr, 0, sizeof(File_Attr_S));
	DBG("m_curKeyIndex = %d\n", m_curKeyIndex);

	if( GetFileByKey(m_curKeyIndex, &fileAttr) )
	{
		DBG("szFilePath = %s\n", fileAttr.szFilePath);

		if (-1 == stat(fileAttr.szFilePath, &st))
		{
			return;
		}

		if (NULL == (ptr = strrchr(fileAttr.szFilePath, '/')))
		{
			return;
		}

		strncpy(szTemp, fileAttr.szFilePath, ptr - fileAttr.szFilePath);
		strncpy(szFileName, ptr + 1, sizeof(szFileName) - 1);

		sscanf(szFileName, "%[^.].%s", szPrefix, szExten);

		DBG("++++++++++++ szPrefix = %s szExten = %s\n",szPrefix,szExten);

		int k = 0;
		while( k < strlen(szExten) )
		{
			if( szExten[k] >= 'a' && szExten[k] <= 'z' )
				szExten[k] = toupper(szExten[k]);
			k++;
		}

		if( !strncasecmp(szPrefix, "FIMG", 4) || !strncasecmp(szPrefix, "FREC", 4) )
		{
			fileType = FILE_FLAG;
		}

		//index = atoi(&szPrefix[4]);

		DBG("++++++++++++ szPrefix = %s\n",szPrefix);

		sscanf(szPrefix, "%*[^0-9]%d",&index);

		DBG("++++++++++++ index = %d\n",index);

		memset(szFileName, 0, sizeof(szFileName));

		if( fileType == FILE_FLAG )
		{
			// 变成非标记文件
			lite_set_image_visible(m_fileFlagImage, FALSE);

			if( fileAttr.fileType & FILE_IMG )
			{
				sprintf(szFileName, "IMG%03d.%s", index, szExten);
				new_fileAttr.fileType |= FILE_IMG;
			}
			else if( fileAttr.fileType & FILE_REC )
			{
				sprintf(szFileName, "REC%03d.%s", index, szExten);
				new_fileAttr.fileType |= FILE_REC;
			}
		}
		else
		{
			// 变成标记文件
			lite_load_image(m_fileFlagImage, PIC_PATH"/"REVIEW_FLAG_FILE);
			lite_set_image_visible(m_fileFlagImage, TRUE);

			if( fileAttr.fileType & FILE_IMG )
			{
				sprintf(szFileName, "FIMG%03d.%s", index, szExten);
			}
			else if( fileAttr.fileType & FILE_REC )
			{
				sprintf(szFileName, "FREC%03d.%s", index, szExten);
			}

			new_fileAttr.fileType |= FILE_FLAG;
		}

		new_fileAttr.lastAccesstime	= fileAttr.lastAccesstime;

		sprintf(new_fileAttr.szFilePath, "%s/%s", szTemp, szFileName);

		DBG("new_fileAttr: %s, index = %d\n", new_fileAttr.szFilePath, index);

		rename(fileAttr.szFilePath, new_fileAttr.szFilePath);
	}
}

void CReviewPage::DoCmdDone(void)
{
	File_Attr_S		fileAttr;

	DestoryListBox();
	delFileList();

	DECODE_MP4_S mp4Info;
	mp4Info.playState = DECODE_STATE_IDLE;
	sys_msg_cmd_playmp4_chg(&mp4Info, SYS_MSG_CMD_DECODE_PLAY_STATE_CHG, MSG_TYPE_ENCODE_SVR);

	if( SearchFiles() > 0 )
	{
		int nIndex = (m_curKeyIndex-1 < 1) ? m_fileNum : (m_curKeyIndex-1);
		memset(&fileAttr, 0, sizeof(File_Attr_S));
		if (GetFileByKey(nIndex, &fileAttr))
		{
			ShowPreFileBG(&fileAttr);
		}

		nIndex = (m_curKeyIndex+1 > m_fileNum ) ? 1 : (m_curKeyIndex+1);
		memset(&fileAttr, 0, sizeof(File_Attr_S));
		if (GetFileByKey(nIndex, &fileAttr))
		{
			ShowNextFileBG(&fileAttr);
		}

		memset(&fileAttr, 0, sizeof(File_Attr_S));
		if (GetFileByKey(m_curKeyIndex, &fileAttr))
		{
			DestoryFrame();
			ShowFileBG(&fileAttr);
			InitFrame();
		}
	}
	else
	{
		ShowNoStorageMsg(MSG_NO_FILE);
	}
}

void CReviewPage::DeleteSubPage()
{
	if (m_subPagePtr != NULL)
	{
		DBG("DeleteSubPage, m_subPagePtr: %p\n", m_subPagePtr);

		m_subPagePtr->OnPageExit();
		delete m_subPagePtr;
		m_subPagePtr = NULL;
	}
}

void CReviewPage::UpdateMsg()
{
	DBG("UpdateMsg\n");

	if( m_noStorageMsg != NULL )
	{
		SAFE_DEL_LITE(m_noStorageMsg);
		SAFE_DEL_LITE(m_lblStorageMsg);

		CDesktop *pDesktop = CDesktop::GetDesktop();
		pDesktop->ShowPageByID(DEFAULT_PAGE);
	}
}

void CReviewPage::OnUpdateArrow()
{
	if( m_subPagePtr )
		m_subPagePtr->OnUpdateArrow();
}




