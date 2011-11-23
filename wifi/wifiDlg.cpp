// wifiDlg.cpp : 实现文件
//

#include "stdafx.h"
#include "wifi.h"
#include "wifiDlg.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


#include < mmsystem.h >          //这个头文件是干什么的记不得了，呵呵

#define WM_NOTIFYICON	WM_USER+5  //自定义消息的ID
#define IDI_ICON		0x0005     //图标ID



CString ip;
int doit() 
{
	ip="";
	char host_name[255]; 
	//获取本地主机名称 
	if (gethostname(host_name, sizeof(host_name)) == SOCKET_ERROR) { 
		return 1; 
	} 
	//从主机名数据库中得到对应的“主机” 
	struct hostent *phe = gethostbyname(host_name); 
	if (phe == 0) { 
		return 1; 
	} 
	//循环得出本地机器所有IP地址 
	for (int i = 0; phe->h_addr_list[i] != 0; ++i) { 
		struct in_addr addr; 
		memcpy(&addr, phe->h_addr_list[i], sizeof(struct in_addr)); 
		//printf("%s\n", inet_ntoa(addr)); 
		ip += inet_ntoa(addr);
		ip += "\r\n";
	}
	return 0;
} 
int back_ip() 
{ 
	WSAData wsaData; 
	if (WSAStartup(MAKEWORD(1, 1), &wsaData) != 0) 
	{
		return 255; 
	} 
	doit(); 
	WSACleanup(); 
} 

// 用于应用程序“关于”菜单项的 CAboutDlg 对话框

class CAboutDlg : public CDialog
{
public:
	CAboutDlg();

// 对话框数据
	enum { IDD = IDD_ABOUTBOX };

	protected:
	virtual void DoDataExchange(CDataExchange* pDX);    // DDX/DDV 支持

// 实现
protected:
	DECLARE_MESSAGE_MAP()
};

CAboutDlg::CAboutDlg() : CDialog(CAboutDlg::IDD)
{
}

void CAboutDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}

BEGIN_MESSAGE_MAP(CAboutDlg, CDialog)
END_MESSAGE_MAP()


// CwifiDlg 对话框




CwifiDlg::CwifiDlg(CWnd* pParent /*=NULL*/)
	: CDialog(CwifiDlg::IDD, pParent)
	, m_name(_T(""))
	, m_passwd(_T(""))
{
	m_hIcon = AfxGetApp()->LoadIcon(IDR_MAINFRAME);
}

void CwifiDlg::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
	DDX_Text(pDX, IDC_EDIT2, m_name);
	DDX_Text(pDX, IDC_EDIT1, m_passwd);
}

BEGIN_MESSAGE_MAP(CwifiDlg, CDialog)
	ON_WM_SYSCOMMAND()
	ON_WM_PAINT()
	ON_WM_QUERYDRAGICON()
	//}}AFX_MSG_MAP
	ON_BN_CLICKED(IDC_BUTTON1, &CwifiDlg::OnBnClickedButton1)
	ON_WM_NOTIFYFORMAT()
	ON_MESSAGE(WM_NOTIFYICON, &CwifiDlg::OnNotifyIcon)
	ON_EN_CHANGE(IDC_EDIT3, &CwifiDlg::OnEnChangeEdit3)
	ON_BN_CLICKED(IDC_BUTTON2, &CwifiDlg::OnBnClickedButton2)
END_MESSAGE_MAP()


// CwifiDlg 消息处理程序

BOOL CwifiDlg::OnInitDialog()
{
	CDialog::OnInitDialog();

	// 将“关于...”菜单项添加到系统菜单中。

	// IDM_ABOUTBOX 必须在系统命令范围内。
	ASSERT((IDM_ABOUTBOX & 0xFFF0) == IDM_ABOUTBOX);
	ASSERT(IDM_ABOUTBOX < 0xF000);

	CMenu* pSysMenu = GetSystemMenu(FALSE);
	if (pSysMenu != NULL)
	{
		CString strAboutMenu;
		strAboutMenu.LoadString(IDS_ABOUTBOX);
		if (!strAboutMenu.IsEmpty())
		{
			pSysMenu->AppendMenu(MF_SEPARATOR);
			pSysMenu->AppendMenu(MF_STRING, IDM_ABOUTBOX, strAboutMenu);
		}
	}

	// 设置此对话框的图标。当应用程序主窗口不是对话框时，框架将自动
	//  执行此操作
	SetIcon(m_hIcon, TRUE);			// 设置大图标
	SetIcon(m_hIcon, FALSE);		// 设置小图标

	ShowWindow(SW_NORMAL);

	// TODO: 在此添加额外的初始化代码
	back_ip();
	SetDlgItemText(IDC_EDIT3,ip);  //显示输出信息到编辑框,并刷新窗口
	UpdateWindow();

	NOTIFYICONDATA nd;
	nd.cbSize	= sizeof (NOTIFYICONDATA);
	nd.hWnd	= m_hWnd;
	nd.uID	= IDI_ICON;
	nd.uFlags	= NIF_ICON|NIF_MESSAGE|NIF_TIP;
	nd.uCallbackMessage= WM_NOTIFYICON;
	nd.hIcon	= m_hIcon;

	strcpy(nd.szTip, "wifi共享");

	Shell_NotifyIcon(NIM_ADD, &nd);

	UpdateData(TRUE);
	m_help="单击查看帮助";
	UpdateData(FALSE);

	return TRUE;  // 除非将焦点设置到控件，否则返回 TRUE
}

void CwifiDlg::OnSysCommand(UINT nID, LPARAM lParam)
{
	if ((nID & 0xFFF0) == IDM_ABOUTBOX)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	else  if(nID==SC_MINIMIZE)
	{
		ShowWindow(SW_HIDE);
		
	}
	else if(nID==SC_CLOSE)
	{
		if(MessageBox("确定要退出吗？","",MB_OKCANCEL|MB_ICONQUESTION)==IDOK)
		{
			NOTIFYICONDATA nd;
			nd.cbSize	= sizeof (NOTIFYICONDATA);
			nd.hWnd	= m_hWnd;

			nd.uID	= IDI_ICON;
			nd.uFlags	= NIF_ICON|NIF_MESSAGE|NIF_TIP;
			nd.uCallbackMessage	= WM_NOTIFYICON;
			nd.hIcon	= m_hIcon;

			Shell_NotifyIcon(NIM_DELETE, &nd);
			

			CString cmd_c;
			cmd_c="netsh wlan set hostednetwork mode=disallow";
			//MessageBox(cmd_c,"aa",0);
			//cmd_c="net user "+m_name+" /add";

			SECURITY_ATTRIBUTES sa;
			HANDLE hRead,hWrite;  
			sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
			sa.lpSecurityDescriptor = NULL;  //使用系统默认的安全描述符 
			sa.bInheritHandle = TRUE;  //创建的进程继承句柄

			if (!CreatePipe(&hRead,&hWrite,&sa,0))  //创建匿名管道
			{  
				MessageBox("CreatePipe Failed!","提示",MB_OK | MB_ICONWARNING);  
				return;
			}

			STARTUPINFO si; 
			PROCESS_INFORMATION pi;

			ZeroMemory(&si,sizeof(STARTUPINFO));
			si.cb = sizeof(STARTUPINFO); 
			GetStartupInfo(&si); 
			si.hStdError = hWrite; 
			si.hStdOutput = hWrite;  //新创建进程的标准输出连在写管道一端
			si.wShowWindow = SW_HIDE;  //隐藏窗口 
			si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;


			char cmdline[200]; 
			CString tmp;
			tmp.Format("cmd /C %s",cmd_c);
			sprintf(cmdline,"%s",tmp);
			if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))  //创建子进程
			{
				MessageBox("CreateProcess Failed!","提示",MB_OK | MB_ICONWARNING);  
				return;
			}
			CloseHandle(hWrite);  //关闭管道句柄

			//Sleep(1000);

			CDialog::OnSysCommand(nID, lParam);
		}
	}
	else
	{
		CDialog::OnSysCommand(nID, lParam);
	}
}

// 如果向对话框添加最小化按钮，则需要下面的代码
//  来绘制该图标。对于使用文档/视图模型的 MFC 应用程序，
//  这将由框架自动完成。

void CwifiDlg::OnPaint()
{
	if (IsIconic())
	{
		CPaintDC dc(this); // 用于绘制的设备上下文

		SendMessage(WM_ICONERASEBKGND, reinterpret_cast<WPARAM>(dc.GetSafeHdc()), 0);

		// 使图标在工作区矩形中居中
		int cxIcon = GetSystemMetrics(SM_CXICON);
		int cyIcon = GetSystemMetrics(SM_CYICON);
		CRect rect;
		GetClientRect(&rect);
		int x = (rect.Width() - cxIcon + 1) / 2;
		int y = (rect.Height() - cyIcon + 1) / 2;

		// 绘制图标
		dc.DrawIcon(x, y, m_hIcon);
	}
	else
	{
		CDialog::OnPaint();
	}
}

//当用户拖动最小化窗口时系统调用此函数取得光标
//显示。
HCURSOR CwifiDlg::OnQueryDragIcon()
{
	return static_cast<HCURSOR>(m_hIcon);
}


void CwifiDlg::OnBnClickedButton1()
{
	UpdateData(TRUE);
	// TODO: 在此添加控件通知处理程序代码
	CString cmd_c;
	cmd_c="netsh wlan set hostednetwork mode=allow ssid="+m_name+" key="+m_passwd+"&&netsh wlan start hostednetwork";
	//MessageBox(cmd_c,"aa",0);
	//cmd_c="net user "+m_name+" /add";

	SECURITY_ATTRIBUTES sa;
	HANDLE hRead,hWrite;  
	sa.nLength = sizeof(SECURITY_ATTRIBUTES); 
	sa.lpSecurityDescriptor = NULL;  //使用系统默认的安全描述符 
	sa.bInheritHandle = TRUE;  //创建的进程继承句柄

	if (!CreatePipe(&hRead,&hWrite,&sa,0))  //创建匿名管道
	{  
		MessageBox("CreatePipe Failed!","提示",MB_OK | MB_ICONWARNING);  
		return;
	}

	STARTUPINFO si; 
	PROCESS_INFORMATION pi;

	ZeroMemory(&si,sizeof(STARTUPINFO));
	si.cb = sizeof(STARTUPINFO); 
	GetStartupInfo(&si); 
	si.hStdError = hWrite; 
	si.hStdOutput = hWrite;  //新创建进程的标准输出连在写管道一端
	si.wShowWindow = SW_HIDE;  //隐藏窗口 
	si.dwFlags = STARTF_USESHOWWINDOW | STARTF_USESTDHANDLES;


	char cmdline[200]; 
	CString tmp;
	tmp.Format("cmd /C %s",cmd_c);
	sprintf(cmdline,"%s",tmp);
	

	if (!CreateProcess(NULL,cmdline,NULL,NULL,TRUE,NULL,NULL,NULL,&si,&pi))  //创建子进程
	{
		MessageBox("CreateProcess Failed!","提示",MB_OK | MB_ICONWARNING);  
		return;
	}
	CloseHandle(hWrite);  //关闭管道句柄
	Sleep(2000);
	

	char buffer[4096] = {0};
	DWORD bytesRead;
	

	MessageBox("虚拟wifi已建立，若您是在此机器上首次使用虚拟wifi，请找到“网络连接”右键单击已连接到Internet的网络连接，选择“属性”→“共享”，勾上“允许其他*****连接(N)”并选择“虚拟WiFi”","",0);
	Sleep(4000);
	back_ip();
	SetDlgItemText(IDC_EDIT3,ip);  //显示输出信息到编辑框,并刷新窗口
	UpdateWindow();
	CloseHandle(hRead);
}



UINT CwifiDlg::OnNotifyFormat(CWnd *pWnd, UINT nCommand)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值

	return CDialog::OnNotifyFormat(pWnd, nCommand);
}

LRESULT CwifiDlg::OnNotifyIcon(WPARAM wParam, LPARAM lParam)
{
	if ((wParam == IDI_ICON)&&(lParam == WM_LBUTTONDOWN))
		ShowWindow(SW_SHOWNORMAL);
	return NULL;
}
void CwifiDlg::OnEnChangeEdit3()
{
	// TODO:  如果该控件是 RICHEDIT 控件，则它将不会
	// 发送该通知，除非重写 CDialog::OnInitDialog()
	// 函数并调用 CRichEditCtrl().SetEventMask()，
	// 同时将 ENM_CHANGE 标志“或”运算到掩码中。

	// TODO:  在此添加控件通知处理程序代码
}

bool f=true;

void CwifiDlg::OnEnSetfocusEdit5()
{
	// TODO: 在此添加控件通知处理程序代码

	if(f)
	{
		CAboutDlg dlgAbout;
		dlgAbout.DoModal();
	}
	f=!f;

}

void CwifiDlg::OnBnClickedButton2()
{
	// TODO: 在此添加控件通知处理程序代码
	ShellExecute(NULL,"open","http://blog.qiaoy.net",NULL,NULL,SW_SHOWNORMAL);
}
