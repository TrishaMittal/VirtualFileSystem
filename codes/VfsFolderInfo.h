//VfsFolderInfo.h

#ifndef VFSFOLDERINFO_H
#define VFSFOLDERINFO_H
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include "VfsFileInfo.h"
#include "VfsNodeInfo.h"
using namespace std;

class VfsFolderInfo : public VfsNodeInfo
	{
	string node_path;
	VfsFolderInfo * parent;
	vector <VfsFolderInfo *> subfolders;
	vector <VfsFileInfo *> vfsfileinfo;
public:
	void setParentFolder (VfsFolderInfo *);
	void addChildFolder (VfsFolderInfo*);
	VfsFolderInfo(string , string);
	void addSubFile(VfsFileInfo *); 
	void getheader(HeaderRecord &headrec);
	~VfsFolderInfo();
	void getContents(vector <string>& , string path);

	};
#endif

