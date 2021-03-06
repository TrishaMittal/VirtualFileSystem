//VfsFileInfo.h
#ifndef VFSFILEINFO_H
#define VFSFILEINFO_H
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include"VfsNodeInfo.h"

class VfsFile;
class VfsFolderInfo;
using namespace std;
class VfsFileInfo : public VfsNodeInfo{
	int file_offset;
        long files_bytes;
	VfsFile * vfsfile;
	VfsFolderInfo * parent_folder;
public:
	
	VfsFileInfo(int, long, string, string);
	void getheader(HeaderRecord &headrec);	
	~VfsFileInfo();
	void save(fstream &ex, string reponame);
	void load(string reponame, fstream &ext);
	void setParentFolder(VfsFolderInfo *parent);
	string getpath();
	
};
#endif

