#include "pch.h"
#include <string>
#include <stdexcept>
#include <iostream>
#include <sstream>
#include <list>
#include <chrono>
#include <iomanip>
#include <ctime>
#include <windows.h>
#include "WinMTRGlobal.h"
#include "utility.h"

namespace OS
{
	std::string utility::ComputerName()
	{
		TCHAR computerName[MAX_COMPUTERNAME_LENGTH + 1];
		DWORD size = sizeof(computerName) / sizeof(computerName[0]);
		if (GetComputerName(computerName, &size))
		{
			return std::string(computerName, computerName + size);
		}
		else
		{
			return std::string();
		}
	}

	std::string utility::UserName()
	{
		TCHAR userName[UNLEN + 1];
		DWORD size = sizeof(userName) / sizeof(userName[0]);
		if (GetUserName(userName, &size))
		{
			return std::string(userName, userName + size - 1); // Exclude the null terminator
		}
		else
		{
			return std::string();
		}
	}

	CString utility::GetExecutableDirectory()
	{
		TCHAR exePath[MAX_PATH];
		if (GetModuleFileName(NULL, exePath, MAX_PATH) == 0)
		{
			// Handle error
			DWORD errorMessageID = ::GetLastError();
			LPSTR messageBuffer = nullptr;
			size_t size = FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM | FORMAT_MESSAGE_IGNORE_INSERTS,nullptr, errorMessageID, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPSTR)&messageBuffer, 0, NULL);
			CString errorMessage(messageBuffer, (int) size);
			LocalFree(messageBuffer);
			// return empty string
			return CString();
		}
		CString exeDir(exePath);
		int pos = exeDir.ReverseFind(_T('\\'));
		if (pos != -1)
		{
			exeDir = exeDir.Left(pos);
		}
		return exeDir;
	}

	void utility::WriteHeader(CStdioFile* file)
	{
		std::string header;

		for (int j = 0; j < MAXHOOP; j++)
		{
			if (j == 0) {
				header += "Date (UTC)" + FIELD_SEPARATOR + "ComputerName" + FIELD_SEPARATOR + "UserName" + FIELD_SEPARATOR ;
			}
			for (int i = 0; i < MTR_NR_COLS; ++i)
			{
				header += MTR_COLS[i];
				if (i < MTR_NR_COLS - 1)
				{
					header += FIELD_SEPARATOR;
				}
			}
			header += FIELD_SEPARATOR;
		}
		header += "\n";
		file->WriteString(header.c_str());
	}

	CStdioFile* utility::CreateFile(const CString& folderPath)
	{

		// Get current time in UTC
		auto now = std::chrono::system_clock::now();
		std::time_t now_c = std::chrono::system_clock::to_time_t(now);
		std::tm now_tm;
		gmtime_s(&now_tm, &now_c);

		// Format the time as a string
		std::ostringstream timeStream;
		timeStream << std::put_time(&now_tm, "%Y_%m_%d_%H_%M");
		CString currentTime(timeStream.str().c_str());

		// Create a base file name with date and time
		CString filePath;
		filePath.Format(_T("%s\\datalist_%s.txt"), folderPath, currentTime);

		// Ensure the directory exists
		if (!PathFileExists(folderPath))
		{
			if (!CreateDirectory(folderPath, NULL) && GetLastError() != ERROR_ALREADY_EXISTS)
			{
				TRACE_MSG("Failed to create directory: " << folderPath);
				return nullptr;
			}
		}

		// Open the file
		CStdioFile* file = new CStdioFile();
		CFileException fileException;
		if (!file->Open(filePath, CFile::modeCreate | CFile::modeWrite | CFile::typeText, &fileException))
		{
			CString errorMessage;
			errorMessage.Format(_T("Failed to open file: %s"), filePath);
			TRACE_MSG(errorMessage.GetBuffer());
			delete file;
			return nullptr;
		}
		return file;
	}
};
