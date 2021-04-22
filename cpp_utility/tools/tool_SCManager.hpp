////////////////////////////////////////////////////////////////////////////////
//
// Class Name:  tool_SCManager
// Description: windows服务管理工具类
// Class URI:   https://github.com/Shiver/cpp-utility
// Author:      Shiver
// Author URI:  https://www.shiver.fun/
// License:     此文件单独授权 以MIT方式开源共享
// Last Update: Feb 19, 2020
//
////////////////////////////////////////////////////////////////////////////////

#ifndef _TOOL_SCMANAGER_CPP
#define _TOOL_SCMANAGER_CPP

#include <Windows.h>
#include <string>

namespace SCManager {
	void startService(const std::string& strServiceName);
	bool Stop_Server(const std::string& strServiceName);
	DWORD Query_Server_Status(const std::string& strServiceName);
}

void SCManager::startService(const std::string& strServiceName)
{
	auto scmHandle = OpenSCManagerW(NULL, NULL, SC_MANAGER_ALL_ACCESS);
	//SAFE_CALL(scmHandle, NULL);

	auto serviceHandle = OpenServiceA(scmHandle,
		strServiceName.c_str(),
		SERVICE_ALL_ACCESS);
	//SAFE_CALL(serviceHandle, NULL);

	SERVICE_STATUS serviceStatus;
	//SAFE_CALL(QueryServiceStatus(serviceHandle, &serviceStatus), 0);
	QueryServiceStatus(serviceHandle, &serviceStatus);
	if (serviceStatus.dwCurrentState == SERVICE_START &&
		serviceStatus.dwCurrentState != SERVICE_START_PENDING)
		return;

	//SAFE_CALL(StartServiceW(serviceHandle, 0, NULL), FALSE);
	StartServiceW(serviceHandle, 0, NULL);

	CloseServiceHandle(scmHandle);
	CloseServiceHandle(serviceHandle);
}

bool SCManager::Stop_Server(const std::string& strServiceName)
{
	bool bResult = false;
	if (strServiceName.empty())
	{
		return bResult;
	}
	SC_HANDLE sc_Manager = ::OpenSCManagerA(NULL, NULL, GENERIC_EXECUTE);
	if (sc_Manager)
	{
		SC_HANDLE sc_service = ::OpenServiceA(sc_Manager, strServiceName.c_str(), SERVICE_ALL_ACCESS);
		if (sc_service)
		{
			SERVICE_STATUS_PROCESS service_status;
			ZeroMemory(&service_status, sizeof(SERVICE_STATUS_PROCESS));
			DWORD dwpcbBytesNeeded = sizeof(SERVICE_STATUS_PROCESS);
			if (::QueryServiceStatusEx(sc_service, SC_STATUS_PROCESS_INFO,
				(LPBYTE)&service_status,
				dwpcbBytesNeeded,
				&dwpcbBytesNeeded))
			{
				SERVICE_CONTROL_STATUS_REASON_PARAMSA service_control_status;
				DWORD dwerror = NULL;
				ZeroMemory(&service_control_status, sizeof(SERVICE_CONTROL_STATUS_REASON_PARAMSA));
				if (service_status.dwCurrentState == SERVICE_RUNNING)
				{
					service_control_status.dwReason = SERVICE_STOP_REASON_FLAG_PLANNED | SERVICE_STOP_REASON_MAJOR_APPLICATION | SERVICE_STOP_REASON_MINOR_NONE;;
					if (!::ControlServiceExA(sc_service, SERVICE_CONTROL_STOP, SERVICE_CONTROL_STATUS_REASON_INFO, &service_control_status))
					{
						dwerror = ::GetLastError();
						::CloseServiceHandle(sc_service);
						::CloseServiceHandle(sc_Manager);
						return bResult;
					}
					while (::QueryServiceStatusEx(sc_service, SC_STATUS_PROCESS_INFO,
						(LPBYTE)&service_status,
						dwpcbBytesNeeded,
						&dwpcbBytesNeeded))
					{
						Sleep(service_status.dwWaitHint);
						if (service_status.dwCurrentState == SERVICE_STOPPED)
						{

							bResult = true;
							break;
						}
					}
				}
			}
			::CloseServiceHandle(sc_service);
		}
		::CloseServiceHandle(sc_Manager);
	}
	return bResult;
}

DWORD SCManager::Query_Server_Status(const std::string& strServiceName)
{
	DWORD nResult = 0;
	if (strServiceName.empty())
	{
		return nResult;
	}
	SC_HANDLE sc_Manager = ::OpenSCManagerA(NULL, NULL, GENERIC_EXECUTE);
	if (sc_Manager)
	{
		SC_HANDLE sc_service = ::OpenServiceA(sc_Manager, strServiceName.c_str(), SERVICE_ALL_ACCESS);
		if (sc_service)
		{
			SERVICE_STATUS_PROCESS service_status;
			ZeroMemory(&service_status, sizeof(SERVICE_STATUS_PROCESS));
			DWORD dwpcbBytesNeeded = sizeof(SERVICE_STATUS_PROCESS);
			if (::QueryServiceStatusEx(sc_service, SC_STATUS_PROCESS_INFO,
				(LPBYTE)&service_status,
				dwpcbBytesNeeded,
				&dwpcbBytesNeeded))
			{
				nResult = service_status.dwCurrentState;
			}
			::CloseServiceHandle(sc_service);
		}
		::CloseServiceHandle(sc_Manager);
	}
	return nResult;
}


int test() {
	SCManager::startService("Bonjour Service");//必须管理员权限运行
	SCManager::Stop_Server("Bonjour Service");
	int ret = SCManager::Query_Server_Status("Bonjour Service");

	printf("status :%d", ret);

	system("pause");
	return 0;
}
#endif