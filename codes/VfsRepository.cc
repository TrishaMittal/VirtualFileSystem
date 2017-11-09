//VfsRepository.cc

#include <iostream>
#include <string>
#include <fstream>
#include "VfsRepository.h"
#include <cstring>
#include <sys/stat.h>
#include <map>
#include "vfs_errors.h"
#include "VfsFolderInfo.h"
#include "VfsNodeInfo.h"
#include "VfsFileInfo.h"
#include "PathString.h"
#include <cctype>
#include <iterator>

VfsRepository::VfsRepository()
{
        repo_name="";
        repo_status =1;
	repo_file_path="";
}
VfsRepository::VfsRepository(string name, string external_file) 
{
        repo_name=name;
        repo_status =1;
	repo_file_path = external_file;

}


void VfsRepository::create(string fname, string fpath)
{
	char buffer[102400];
	HeaderRecord headrec;
	repo_name=fname;
	repo_file_path=fpath;
	fstream fptr;
	fptr.open(repo_file_path+"/"+fname, ios::out | ios::binary);
	fptr.close();
	fptr.open(repo_file_path+"/"+fname, ios::in| ios::out | ios::binary);
	if (!fptr.good())
		throw VFS_CREATE_01;
	num_nodes=0;
	fptr.write((char * ) &num_nodes, sizeof(int));
	for (int i=0;i<MAX_NODES;i++)
	{	
		fptr.write((char *)&headrec, sizeof (HeaderRecord));
	}
	fptr.close();
}


void VfsRepository::open(string fname, string fpath)
{
	int num;
	fstream constr;
	repo_file_path=fpath;
	repo_status =0;
	repo_name=fname;
	constr.open(repo_file_path+"/"+repo_name,ios::in|ios::out|ios::binary);
	if( !constr.good() )
		throw VFS_OPEN_01;	
	constr.read((char *)&num, sizeof(int));
	for (int i=0;i<num;i++)
		{
			HeaderRecord headrec;
			constr.read((char *)&headrec, sizeof(HeaderRecord));
			VfsNodeInfo * nd;
			if (headrec.node_type==1)
				nd = new VfsFolderInfo(headrec.folder_path, headrec.node_name);
			else if (headrec.node_type==0)
				nd = new VfsFileInfo(headrec.offset, headrec.size,headrec.folder_path, headrec.node_name);
			string full_name(headrec.folder_path);
			string node_name(headrec.node_name);
			if (full_name=="" && node_name == "")
				full_name="/";
			else
				full_name=full_name+"/"+node_name;
			nodes[full_name]= nd;
		}
	constr.close();
	map<string, VfsNodeInfo *>::iterator it;
	for (it=nodes.begin();it!=nodes.end();it++)
	{
		PathString pstr(it->first,"/");
		if (it->second->getNodeType()==1)
		{		
			if (it->first =="/")
			{
				VfsFolderInfo *current = dynamic_cast<VfsFolderInfo *>(it->second);
				current->setParentFolder(nullptr);
			}
			else if(pstr.excludeLast()=="")
			{
				VfsFolderInfo *current = dynamic_cast<VfsFolderInfo *>(it->second);
				VfsFolderInfo *parent = dynamic_cast<VfsFolderInfo *>(nodes["/"]);
				current->setParentFolder(parent);
				parent->addChildFolder(current);
			}
			else
			{
				VfsFolderInfo *current = dynamic_cast<VfsFolderInfo *>(it->second);
				VfsFolderInfo * parent = dynamic_cast<VfsFolderInfo *>(nodes[pstr.excludeLast()]);
				current->setParentFolder(parent);
				parent->addChildFolder(current);
			}
		}
		else if (it->second->getNodeType()==0)
		{
			VfsFileInfo *current = dynamic_cast<VfsFileInfo *>(it->second);
			VfsFolderInfo * parent = dynamic_cast<VfsFolderInfo *>(nodes[pstr.excludeLast()]);
			current->setParentFolder(parent);
			parent->addSubFile(current);
		}
	}
}


void VfsRepository::close()
{	
	fstream constr;
	if (repo_status== 1)
		throw VFS_GEN_01;
	constr.open(repo_file_path+"/"+repo_name,ios::in|ios::out|ios::binary);
	if (!constr.good())
		throw VFS_CLOSE_01;
	constr.write((char *)&num_nodes, sizeof(int));
	map<string, VfsNodeInfo *>::iterator i;
	for (i=nodes.begin();i!=nodes.end();i++)
	{
		HeaderRecord headrec;
		i->second->getheader(headrec);
		constr.write((char *)&headrec, sizeof(HeaderRecord));
		delete i->second;
	}
	nodes.clear();
	constr.close();
}


int VfsRepository::makeDir( string fpath, string fname )
{
	if (repo_status== 1)
		throw VFS_GEN_01;
	map <string, VfsNodeInfo *>::iterator it;
	if (nodes.empty())
	{
		
		if( (fpath =="") &&( fpath ==""))
			{

			VfsFolderInfo *f= new VfsFolderInfo(fpath, fname);
			f->setParentFolder(nullptr);
			nodes["/"]=f;
			num_nodes++;
                        }
		else
		
			throw VFS_MAKEDIR_01;
	}
	else if (nodes.find(fpath+"/"+fname) != nodes.end() )
	{
		throw VFS_MAKEDIR_02;
	}
	else if (nodes.find(fpath) != nodes.end())
	{
		VfsFolderInfo *f = new VfsFolderInfo(fpath, fname);
		nodes[(fpath+"/"+fname)]=f;
		VfsFolderInfo *fptr = dynamic_cast<VfsFolderInfo*>(nodes[fpath]);
		fptr->addChildFolder(f);
		f->setParentFolder(dynamic_cast<VfsFolderInfo*>(nodes[fpath]));
		num_nodes+=1;	
	}
	else if(fpath=="" && nodes.find("/") != nodes.end())
	{
		VfsFolderInfo *f = new VfsFolderInfo(fpath, fname);
		nodes[(fpath+"/"+fname)]=f;
		VfsFolderInfo *fptr = dynamic_cast<VfsFolderInfo*>(nodes["/"]);
		fptr->addChildFolder(f);
		f->setParentFolder(dynamic_cast<VfsFolderInfo*>(nodes["/"]));
		num_nodes+=1;	
	}
	else
		throw VFS_MAKEDIR_01;
}	


void VfsRepository::list(string path, vector <string> &content)
{
	if(repo_status==1)
		throw VFS_GEN_01;
	else if (nodes.find(path) == nodes.end())
		throw VFS_LISTDIR_01;
	VfsFolderInfo *fptr = dynamic_cast<VfsFolderInfo*>(nodes[path]);
	fptr->getContents(content,path);	
}


void VfsRepository::copyIn(string external_file_path, string internal_file_path)
{
	fstream ex; 
	ex.open(external_file_path, ios::in | ios::binary);
	if(repo_status==1)
		throw VFS_GEN_01;
	VfsFolderInfo *parent;
	PathString obj(internal_file_path,"/");
	string childpath = obj.getLast();
	string parentpath = obj.excludeLast();
	if (parentpath=="")
		parentpath="/";
	parent = dynamic_cast <VfsFolderInfo *> (nodes[parentpath]);
	if(nodes.find(parentpath)==nodes.end())
		throw VFS_COPYIN_01;
	if(nodes.find(internal_file_path) != nodes.end())
		throw VFS_COPYIN_02;
	if( !ex.good() )
		throw VFS_COPYIN_03; // If the stream is not good
	else
	{
		VfsFileInfo *file_info = new VfsFileInfo(0,0, parentpath,childpath);
		file_info->save(ex,repo_file_path + "/" + repo_name);
		parent->addSubFile(file_info); 
		nodes[internal_file_path]=file_info;
		num_nodes++;	
	}
	ex.close();	
}


void VfsRepository::copyOut( string internal_file_path, string external_file_path)
{
	fstream ex; 
	ex.open(external_file_path, ios::out | ios::binary);
	ex.close();
	ex.open(external_file_path, ios::in |ios::out | ios::binary);
	PathString obj(internal_file_path,"/");
	string childpath = obj.getLast();
	string parentpath = obj.excludeLast();
	if(repo_status==1)
		throw VFS_GEN_01;
	if(nodes.find(internal_file_path) == nodes.end())
		throw VFS_COPYOUT_01;
	if( !ex.good() )
		throw VFS_COPYOUT_02; // If the stream is not good
	else
	{
		VfsFileInfo *file_info;
		file_info = dynamic_cast <VfsFileInfo *> (nodes[internal_file_path]); 
		file_info->load(repo_file_path+"/"+repo_name, ex);
	}
	ex.close();
}
