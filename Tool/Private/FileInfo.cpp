#include "stdafx.h"
#include "FileInfo.h"


CFileInfo::CFileInfo()
{
}


CFileInfo::~CFileInfo()
{
}

CString CFileInfo::ConvertRelativePath(CString strFullPath)
{
	// 상대 경로를 저장할 배열
	TCHAR		szRelativePath[MAX_PATH] = L"";

	// 현재 절대 경로를 저장하고 있는 배열
	TCHAR		szCurDirPath[MAX_PATH] = L"";

	GetCurrentDirectory(MAX_PATH, szCurDirPath);


	// 상대 경로로 만들어주는 함수
	// 단, 두 경로가 반드시 같은 드라이브에 있어야 함.
	PathRelativePathTo(szRelativePath, 
		szCurDirPath,
		FILE_ATTRIBUTE_DIRECTORY,
		strFullPath.GetString(), 
		FILE_ATTRIBUTE_DIRECTORY);

	return CString(szRelativePath);
}

void CFileInfo::DirInfoExtraction(const wstring & wstrPath, list<Engine::IMGPATH*>& rPathInfoList)
{
	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	// 주어진 경로에 파일의 유무를 확인하는 함수
	// 존재하지 않으면 false 리턴, 존재하면 true리턴
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	while (bContinue)
	{
		bContinue = Find.FindNextFile();

		if(Find.IsDots())
			continue;

		else if (Find.IsDirectory())
		{
			// GetFilePath : 현재 찾은 경로를 얻어오는 함수
			DirInfoExtraction(wstring(Find.GetFilePath()), rPathInfoList);
		}

		else // 파일을 찾은 상황
		{
			// 찾은게 시스템 파일인 경우 건너뛴다.
			if (Find.IsSystem())
				continue;

			Engine::IMGPATH*		pImgPath = new Engine::IMGPATH;
			TCHAR			szPath[MAX_PATH] = L"";

			lstrcpy(szPath, Find.GetFilePath().GetString());

			// 현재 경로 상태에서 파일명과 확장자 명을 잘라냄
			PathRemoveFileSpec(szPath);

			// 파일의 개수를 구해서 구조체 멤버 변수의 값으로 채워줌
			pImgPath->iCount = DirFileCount(szPath);

			// GetFileTitle : 파일명만 얻어오는 함수(확장자명 제외)
			// L"AKIHA_AKI01_000"
			wstring	wstrTextureName = Find.GetFileTitle().GetString();

			// substr(시작, 끝) : 시작에서 끝에 해당하는 문자열을 얻어오는 함수
			// L"AKIHA_AKI01_00%d.png"
			wstrTextureName = wstrTextureName.substr(0, wstrTextureName.size() - 1) + L"%d.png";

			TCHAR	szBuf[MAX_STR] = L"";
			lstrcpy(szBuf, Find.GetFilePath().GetString());
			// D:\유준환\124기\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_000.png

			PathRemoveFileSpec(szBuf);
			// D:\유준환\124기\Frame124\Texture\Stage\Player\Attack\

			// 두 문자열을 결합하여 완성된 경로를 첫 번째 인자값에 저장
			PathCombine(szBuf, szBuf, wstrTextureName.c_str());
			// D:\유준환\124기\Frame124\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png

			pImgPath->wstrPath = ConvertRelativePath(szBuf);
			// ..\Texture\Stage\Player\Attack\AKIHA_AKI01_00%d.png

			PathRemoveFileSpec(szBuf);
			// ..\Texture\Stage\Player\Attack\

			// PathFindFileName : 파일명을 찾거나 가장 마지막 경로를 찾아내는 함수
			pImgPath->wstrStateKey = PathFindFileName(szBuf);

			PathRemoveFileSpec(szBuf);
			// ..\Texture\Stage\Player\

			// PathFindFileName : 파일명을 찾거나 가장 마지막 경로를 찾아내는 함수
			pImgPath->wstrObjKey = PathFindFileName(szBuf);		

			rPathInfoList.push_back(pImgPath);

			Find.FindFile(Find.GetFilePath());
			Find.FindNextFile();
		}
		
	}

}

int CFileInfo::DirFileCount(const wstring & wstrPath)
{
	wstring	wstrFilePath = wstrPath + L"\\*.*";

	// mfc에서 제공하는 파일 및 경로 제어 관련 클래스
	CFileFind		Find;

	// 주어진 경로에 파일의 유무를 확인하는 함수
	// 존재하지 않으면 false 리턴, 존재하면 true리턴
	BOOL	bContinue = Find.FindFile(wstrFilePath.c_str());

	int	iFileCnt = 0;

	while (bContinue)
	{
		// 파일의 유무를 확인하고 실제 그 파일에 접근하고 싶을 경우
		// FindNextFile함수를 무조건 호출해야 한다. *.* 붙은 경우 해당 경로 하위 모든 파일을 찾는 것으로 해석
		// FindNextFile 가장 첫 번째 파일에 접근하게 된다.

		// FindNextFile : 동일 경로 안에서 다음 파일을 찾는 함수
		// 더이상 찾을게 없거나 마지막 대상인 경우 0을 리턴
		bContinue = Find.FindNextFile();

		if (Find.IsDots())
			continue;
		
		// 찾은게 시스템 파일인 경우 건너뛴다.
		if(Find.IsSystem())
			continue;

		++iFileCnt;
	}

	return iFileCnt;
}
