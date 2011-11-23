// wifiDlg.h : 头文件
//

#pragma once


// CwifiDlg 对话框
class CwifiDlg : public CDialog
{
// 构造
public:
	CwifiDlg(CWnd* pParent = NULL);	// 标准构造函数

// 对话框数据
	enum { IDD = IDD_WIFI_DIALOG };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);	// DDX/DDV 支持


// 实现
protected:
	HICON m_hIcon;

	// 生成的消息映射函数
	virtual BOOL OnInitDialog();
	afx_msg void OnSysCommand(UINT nID, LPARAM lParam);
	afx_msg void OnPaint();
	afx_msg HCURSOR OnQueryDragIcon();
	DECLARE_MESSAGE_MAP()
public:
	CString m_name;
	CString m_passwd;
	afx_msg void OnBnClickedButton1();
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg UINT OnNotifyFormat(CWnd *pWnd, UINT nCommand);
	afx_msg LRESULT OnNotifyIcon(WPARAM wParam, LPARAM lParam);
	afx_msg void OnEnChangeEdit3();
	CString m_help;
	afx_msg void OnEnSetfocusEdit5();
	afx_msg void OnBnClickedCommand1();
	afx_msg void OnBnClickedButton2();
};
