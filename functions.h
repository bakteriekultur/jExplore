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
struct configStruct{
  char paths[100][80];
  struct folderStruct folders[100];
};

struct pathStruct readPath(char* pathStr){
  FILE *fp;
  struct pathStruct path;
  char buffer[50];
  char c=0;
  int i=0;
  int j=0;
  char command[strlen(pathStr)+12];
  strcpy(command, "dir ");
  strcat(command, pathStr);
  strcat(command, " /a-d/b");
  if(0==(fp=(FILE*)popen(command, "r"))){
    perror("popen() failed\n");
    exit(EXIT_FAILURE);
  }
  memset(buffer, 0, strlen(buffer));
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

struct configStruct readConfig(char* configPath){
  FILE *fp = fopen(configPath, "r");
  struct configStruct config;
  char buffer[200];
  int c, i, bracket, hard;
  br
  i = 0;
  bracket = 0;
  hard = 0;
  bool quote = false;
  short mode = 0; //0 = utanför hakar, 1=paths, 2=folders, 3=settings
  short submode = 0; //0 = inget läge, 1=titel/path, 2=tags, 3 = extentions, mer = settings
  while((c=getc(fp))!=EOF){
    buffer[i] = c;
    switch(c){
      case '[':hard++;break;
      case ']':hard--;break;
      case '{':bracket++;break;
      case '}':bracket--;break;
      case '"':quote = !quote; break;
    }
    switch(mode){
      case 0: {
                if(!strcmp(buffer, "#paths")){
                    mode = 1;
                    memset(buffer, 0, sizeof(buffer));
                    i=0
                }
                if(!strcmp(buffer, "#folders")){
                    mode = 2;
                    memset(buffer, 0, sizeof(buffer));
                    i=0
                }
                if(!strcmp(buffer, "#settings")){
                    mode = 3;
                    memset(buffer, 0, sizeof(buffer));
                    i=0
                }
                hard=0;
                bracket=0;
                break;
              }
      case 1: {//läser paths
                if(hard==0){
                  mode = 0;
                  break;
                }
                switch(submode){
                  case 0:{//läser submode i paths
                    if(!strcmp(buffer, "#path")){
                      mode=1;
                      memset(buffer, 0, sizeof(buffer));
                      i=0;
                    }
                    if(!strcmp(buffer, "#tags")){
                      mode=2;
                      memset(buffer, 0, sizeof(buffer));
                      i=0;
                    }
                     }
                  case 1: {//läser path i paths




              



                }
      case 3: {



              



                }
                }
  return config;
}

  
