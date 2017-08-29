//VfsFIleInfo.cc

#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include "VfsFileInfo.h"
#include "VfsFile.h"
using namespace std;

VfsFileInfo::VfsFileInfo(int offset, long byte, string path, string name):VfsNodeInfo(name,0,path)
{	
	file_offset=offset;
	files_bytes=byte;
}
void VfsFileInfo::getheader(HeaderRecord &headrec)
{	
	//cout << "getheader..."<<node_name<<","<<node_path<<","<<endl;
	strcpy(headrec.node_name,node_name.c_str());
	strcpy(headrec.folder_path,node_pa.c_str());
	headrec.size=files_bytes;
 	headrec.offset=file_offset;
        headrec.node_type=0;
}
VfsFileInfo::~VfsFileInfo()
{
}

string VfsFileInfo::getpath()
{
	return node_pa;
}


void VfsFileInfo::setParentFolder(VfsFolderInfo *parent)
{
	parent_folder=parent;
}

void VfsFileInfo::save(fstream &ex, string reponame)
{
	//fstream ext;
	//cout<<"1.5 ...."<<reponame<<endl;
	//string external = external_file_path;
	///ext.open(external,ios::in| ios::out | ios::binary);
	VfsFile *vfs_file = new VfsFile();
	vfs_file->SaveInVfs(reponame, ex, file_offset, files_bytes);
	//ext.close();
}

void VfsFileInfo::load(string reponame, fstream &ext)
{ 
	//cout<<"3...."<<endl;
	VfsFile *vfs_file = new VfsFile();
	vfs_file->exportfile(reponame, ext, file_offset, files_bytes);
	//cout << "file_offset"<<file_offset<<endl;
	delete vfs_file;
}
																																																		    
