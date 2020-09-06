#pragma once
#include <stdio.h>
#include <windows.h>
#include <string.h>

struct pathStruct{
  char files[1000][50];
  char tags[50][50];
  int amountOfFiles;
  int amountOfTags;
};
struct folderSettingsStruct{
  BOOL showDirs;
  BOOL openDirs;
};
struct folderStruct{
  char name[100];
  char extentions[50][8];
  char tags[50][50];
  int amountOfTags;
  struct folderSettingsStruct;
};

struct pathStruct readPath(char* pathStr){
  FILE *fp;
  struct pathStruct path;
  char buffer[50];
  char c=0;
  int i=0;
  int j=0;
  char command[sizeof(pathStr)+11];
  strcpy(command, "dir ");
  strcat(command, pathStr);
  strcat(command, " /a-d/b");
  if(0==(fp=(FILE*)popen(command, "r"))){
    perror("popen() failed\n");
    exit(EXIT_FAILURE);
  }
  memset(buffer, 0, sizeof(buffer));
  while(fread(&c, sizeof(c), 1, fp)){
    if(c!='\n'){
      buffer[i]=c;
      i++;
    }
    else{
      memcpy(path.files[j], buffer, sizeof(path.files[j]));
      memset(buffer, 0, sizeof(buffer));
      i=0;
      j++;
    }
  }
  pclose(fp);
  path.amountOfFiles = j;
  return path;
}






















