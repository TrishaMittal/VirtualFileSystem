//VfsFile.h

#ifndef VFSFILE_H
#define VFSFILE_H
#include<string>
#include<iostream>
#include<map>
#include<vector>
class VfsFileInfo;
using namespace std;

class VfsFile
{
	char * data_bytes;
	VfsFileInfo * vfsfileinfo;
public:
	VfsFile();
	void exportfile(string reponame, fstream &ext, int offset, long size);
	void SaveInVfs(string reponame, fstream &ext, int &offset , long &size);
	~VfsFile();

};
#endif
