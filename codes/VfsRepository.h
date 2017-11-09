//VfsRepository.h
#ifndef VFSREPOSITORY_H
#define VFSREPOSITORY_H
#include<iostream>
#include<string>
#include<map>
#include<vector>
class VfsNodeInfo;
const int MAX_NAME_LEN=200;
const int MAX_PATH_LEN=200 ;
const int MAX_NODES = 3000;

using namespace std;
struct HeaderRecord{
public:
	char node_name[MAX_NAME_LEN];
	int node_type;
	int offset;
	int size;
	char folder_path[MAX_PATH_LEN];
	};

class VfsRepository
	{
	map <string, VfsNodeInfo *> nodes;
        string repo_name;
	int repo_status;
	string repo_file_path;
        vector <VfsNodeInfo*> vfsnodeinfo;
	int num_nodes;
	
public: 
	VfsRepository();
        VfsRepository(string repo_name,string external_file);
	void create(string, string);
	void open(string, string);
	void close();
	int makeDir(string dir_path, string dir_name);
	void list(string, vector <string> &content);
	void copyIn(string external_file_path, string repo_name);
	void copyOut(string internal_file_path, string external_file_path);

	};
#endif   
