#pragma once
namespace OS
{


	class utility
	{
	public:
		static CString GetExecutableDirectory();
		static void WriteHeader(CStdioFile* file);
		static CStdioFile* CreateFile(const CString& folderPath);
		static std::string utility::ComputerName();
		static std::string utility::UserName();
	};
};
