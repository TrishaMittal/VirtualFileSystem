//VfsNodeInfo.h
#ifndef VFSNODEINFO_H
#define VFSNODEINFO_H
#include<iostream>
#include<string>
#include<map>
#include<vector>
#include "VfsRepository.h"

using namespace std;
class VfsNodeInfo
	{
protected:
        string node_name;
	string node_pa;
	int node_type;
       	VfsRepository * vfsrepository;

public:
        VfsNodeInfo(string, int, string);
	virtual void getheader(struct HeaderRecord &headrec)=0;
	int getNodeType();
	string getname();
	virtual ~VfsNodeInfo();
	};
#endif

