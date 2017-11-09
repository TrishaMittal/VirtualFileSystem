//VfsNodeInfo.cc
#include<iostream>
#include<string>
#include "VfsNodeInfo.h"
using namespace std;

VfsNodeInfo::VfsNodeInfo(string name, int type, string path)
{
        node_type=type;
	node_name=name;
	node_pa=path;
}

int VfsNodeInfo::getNodeType()
{
        return node_type;
}

string VfsNodeInfo::getname()
{
	return node_name;
}

void VfsNodeInfo::getheader(HeaderRecord &headrec)
{
}

VfsNodeInfo::~VfsNodeInfo()
{
}

