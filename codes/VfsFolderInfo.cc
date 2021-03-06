//VfsFolderInfo.cc
#include "VfsFolderInfo.h"
#include<iostream>
#include<string>
#include<cstring>
#include<vector>
#include"PathString.h"
#include"VfsFileInfo.h"
using namespace std;

VfsFolderInfo::VfsFolderInfo(string fpath, string fname):VfsNodeInfo(fname,1,fpath)
{
	node_path=fpath;
}

void VfsFolderInfo::setParentFolder (VfsFolderInfo * par)
{
	parent = par;
}

void VfsFolderInfo::addChildFolder (VfsFolderInfo* child)
{
	subfolders.push_back(child);
}

void VfsFolderInfo::getContents(vector <string> &content, string path)
{
	for (auto i : subfolders)
	{
		string ssec= i->getname();
		string add= (path+ "/"+ssec+"/") ;
		if (path=="/")
			content.push_back("/"+ssec+"/");
		else
			content.push_back(add);
	}
	for (auto i : vfsfileinfo)
	{
		string first = i->getpath();
		string second = i->getname();		
		content.push_back(first +"/"+second );
	}
}

void VfsFolderInfo::addSubFile(VfsFileInfo * subfile) 
{
	vfsfileinfo.push_back(subfile);
}

void VfsFolderInfo::getheader(HeaderRecord &headrec)
{
	strcpy(headrec.node_name,node_name.c_str());
	strcpy(headrec.folder_path, node_path.c_str());
	headrec.node_type=1;
}

VfsFolderInfo::~VfsFolderInfo()
{
}
