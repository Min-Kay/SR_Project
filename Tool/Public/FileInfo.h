#pragma once

#include "Tool_Include.h"
#include "Engine_Struct.h"

class CFileInfo
{
public:
	CFileInfo();
	~CFileInfo();

public:
	static CString			ConvertRelativePath(CString strFullPath);

	// ������ ��� ������ �����ϴ� �Լ�
	static void				DirInfoExtraction(const wstring& wstrPath, list<Engine::IMGPATH*>& rPathInfoList);
	static	int				DirFileCount(const wstring& wstrPath);

};

