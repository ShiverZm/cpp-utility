﻿////////////////////////////////////////////////////////////////////////////////
//
// Class Name:  tool_Priv
// Description: 权限工具类
// Class URI:   https://github.com/fawdlstty/NetToolbox
// Author:      Fawdlstty
// Author URI:  https://www.fawdlstty.com/
// License:     此文件单独授权 以MIT方式开源共享
// Last Update: Jan 05, 2019
//
////////////////////////////////////////////////////////////////////////////////

#ifndef __TOOL_PRIV_HPP__
#define __TOOL_PRIV_HPP__

#include <Windows.h>
#include <ShlObj.h>

#include "tool_String.hpp"



class tool_Priv {
public:
	// 判断是否是管理员权限
	static bool is_admin () {
		return !!::IsUserAnAdmin ();
	}

	// 重启并以管理员方式运行。假设成功，即需要退出自身
	static bool adjust_restart (size_t sel1, size_t sel2) {
		if (IDOK != ::MessageBox (NULL, International::translate (_T ("Insufficient permissions. Do you want to reopen the program with administrator permissions?")).data (), International::translate (_T ("Info")).data (), MB_ICONQUESTION | MB_OKCANCEL))
			return false;
		faw::String str_param = faw::String::format (_T ("-jump %d,%d"), sel1, sel2);
		return (size_t) ::ShellExecute (NULL, _T ("runas"), faw::Directory::get_current_file ().c_str (), str_param.c_str (), _T (""), SW_SHOWNORMAL) > 32;
	}

	// 提升调试模式权限
	static bool adjust_debug () {
		HANDLE hToken = NULL;
		if (!::OpenProcessToken (::GetCurrentProcess (), TOKEN_ADJUST_PRIVILEGES, &hToken))
			return false;
		TOKEN_PRIVILEGES tp { 1 };
		tp.Privileges[0].Attributes = SE_PRIVILEGE_ENABLED;
		bool bRet = false;
		if (::LookupPrivilegeValue (nullptr, SE_DEBUG_NAME, &tp.Privileges[0].Luid))
			bRet = !!::AdjustTokenPrivileges (hToken, FALSE, &tp, sizeof (TOKEN_PRIVILEGES), nullptr, nullptr);
		::CloseHandle (hToken);
		return bRet;
	}
};

#endif //__TOOL_PRIV_HPP__
