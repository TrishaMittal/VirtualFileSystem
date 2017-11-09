#include"VfsFile.h"
#include<iostream>
#include<string>
#include<cstring>
#include<fstream>
#include<sstream>
using namespace std;

VfsFile::VfsFile()
{
	char *data_bytes='\0';
}
void VfsFile::exportfile(string reponame, fstream &ext, int offset, long bytes)
{
	fstream constr;
	constr.open(reponame,  ios::in | ios::out | ios::binary );
	if(!constr.good())
		cout << "Stream not good"<<endl;
	char buffer[1024];
	int count = (bytes/1024);
	constr.seekg(offset, ios::beg);
	for(int i = 0; i < count; i++)
		{
		constr.read(buffer,1024);
		ext.write(buffer,1024);
		}
	int rem = bytes%1024;
	constr.read(buffer,rem);
	ext.write(buffer,rem);
	constr.close();
	ext.close();
}

void VfsFile::SaveInVfs(string reponame, fstream &ext, int &offset , long &size)
{
	char buffer[1024];
	fstream constr;
	int si=1024;
	ext.seekg(0,ios::end);
	size = ext.tellg();
	ext.seekg(0, ios::beg);
	constr.open(reponame, ios::in | ios::out | ios::binary  );
	constr.seekp(0,ios::end);
	if( !constr.good())
	{
		cout << "Stream not good " << endl;
		exit(1);
	}
	offset = constr.tellp();
	while (!ext.eof())
	{
		ext.read(buffer,si);
		constr.write(buffer,si);
	}
	ext.close();
	constr.close();	
}

VfsFile::~VfsFile()
{
}
