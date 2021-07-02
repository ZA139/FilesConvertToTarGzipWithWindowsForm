#include<windows.h>
#include<Commdlg.h>
#include<cstdio>
#include<iostream>
#include<fstream>
#include<cstring>
#include<shlobj.h>
#include <cstring>
#include <string>
using namespace std;
bool savePathDialog(char* szFilePath) {
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szFilePath;
	bi.lpszTitle = ("选择要保存的路径:");
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return false;
	}
	SHGetPathFromIDList(idl, szFilePath);
	puts(szFilePath);
	if (strcmp(szFilePath, "") == 0)
	{
		// 检验是否获取成功
		return false;
	}
	return true;
}

bool openPathDialog(char* szFilePath) {
	BROWSEINFO bi;
	ZeroMemory(&bi, sizeof(BROWSEINFO));
	bi.hwndOwner = NULL;
	bi.pszDisplayName = szFilePath;
	bi.lpszTitle = ("选择要压缩的文件或者文件夹:");
	bi.ulFlags = BIF_BROWSEINCLUDEFILES;
	LPITEMIDLIST idl = SHBrowseForFolder(&bi);
	if (NULL == idl)
	{
		return false;
	}
	SHGetPathFromIDList(idl, szFilePath);
	puts(szFilePath);
	if (strcmp(szFilePath, "") == 0)
	{
		// 检验是否获取成功
		return false;
	}
	return true;
}
void strMyCopy(char const *src, char *des) {
	int i = 0;
	while (src[i]) {
		if (src[i] == '.') break;
		des[i] = src[i];
		i++;
	}
	return;
}
bool GetFileName(char const * Path, char* filename) {
	int pathlen = strlen(Path);
	bool bIsDirectory = true;
	for (int i = pathlen - 1; i >= 0; i--) {
		if (Path[i] == '\\') {
			strMyCopy(Path + i, filename);
			break;
		}
		if (Path[i] == '.') bIsDirectory = false;
	}
	return bIsDirectory;
}
void CompressToTar(char const * openPath, char * savePath) {
	//Surround with quote in target to support the path which inlcude space
	strcat(savePath, ".tar");
	string commendline = "7za a \""+ (string)savePath+"\" \""+openPath+"\"";
	system(commendline.c_str());
	return;
}
void CompressToGzip(char const * openPath, char * savePath) {
	strcat(savePath, ".gz");
	string commendline = "7za a \"" + (string)savePath + "\" \"" + openPath + "\"";
	system(commendline.c_str());
	return;
}
void ClearTarFile(char const *FilePath) {
	string commendline = "del " + (string)FilePath;
	system(commendline.c_str());
	return;
}
int main(void){
	char openPath[MAX_PATH] = { 0 };
	char savePath[MAX_PATH] = { 0 };
	if (openPathDialog(openPath) && savePathDialog(savePath))
	{
		char filename[MAX_PATH] = { 0 };
		bool bIsDirectory = GetFileName(openPath, filename);
		strcat(savePath, filename);
		CompressToTar(openPath, savePath);
		strcpy(openPath, savePath);
		CompressToGzip(openPath, savePath);
		ClearTarFile(openPath);
	}
	else {
		puts("UnknowError!\n");
		getchar();
	}
	return 0;
}
//@Todo: system  winform+shell