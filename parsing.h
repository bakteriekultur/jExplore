#include <stdio.h>
#include <windows.h>
#include <string.h>
#include <stdbool.h>
struct pathStruct{
  char pathStr[50];
  char files[500][20];
  char tags[50][50];
  int amountOfFiles;
  int amountOfTags;
};
struct folderSettingsStruct{
  bool showDirs;
  bool openDirs;
};
struct folderStruct{
  char title[100];
  char extensions[50][8];
  int amountOfExtensions;
  char tags[50][50];
  int amountOfTags;
  struct folderSettingsStruct settings;
};
struct configStruct{
  struct pathStruct paths[20];
  int amountOfPaths;
  struct folderStruct folders[20];
  int amountOfFolders;
};

struct pathStruct readPath(struct pathStruct path){
  FILE *fp;
  char buffer[50];
  char* pathStr = path.pathStr;
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
  config.amountOfPaths = 0;
  config.amountOfFolders = 0;
  char buffer[200];
  int i, bracket, hard;
  char c = 0;
  i = 0;
  bracket = 0;
  hard = 0;
  bool equals = false;
  bool quote = false;
  short mode = 0; //0 = utanför hakar, 1=paths, 2=folders, 3=settings
  short submode = 0; //0 = inget läge, 1=titel/path, 2=tags, 3 = extensions, mer = settings
  if(0==(fp=(FILE*)fopen(configPath, "r"))){
    printf("Error reading file\n");
  }
  while((c=getc(fp))!=EOF){
    switch(c){
      case '[':hard++;break;
      case ']':hard--;break;
      case '{':bracket++;break;
      case '}':bracket--;break;
      case '"':quote = !quote; break;
      case '=':memset(buffer, 0, sizeof(buffer)); equals = true; break;
      case ',':break;
      case ' ':break;
      case '\n':break;
      default:buffer[i] = c;i++; 
    }
    if(c==']'&&hard==0){
      mode = 0;
      submode = 0;
      i=0;
      memset(buffer,0,sizeof(buffer));
    }
    switch(mode){
      case 0: {
                
                if(!strcmp(buffer, "#paths")){
                    mode = 1;
                }
                if(!strcmp(buffer, "#folders")){
                    mode = 2;
                    config.folders[config.amountOfFolders].amountOfExtensions=0;
                }
                if(!strcmp(buffer, "#settings")){
                    mode = 3;
                }
                hard=0;
                bracket=0;
                if(mode!=0){
                    memset(buffer, 0, sizeof(buffer));
                    i=0;
                }
                break;
              }
      case 1: {//läser #paths
                switch(submode){
                  case 0:{//läser submode i #paths

                    if(c==','&&bracket==0){
                      config.amountOfPaths++;
                    }
                    if(!strcmp(buffer, "path")){
                      submode=1;
                      memset(config.paths[config.amountOfPaths].pathStr,0,sizeof(config.paths[config.amountOfPaths].pathStr));
                    }
                    if(!strcmp(buffer, "tags")){
                      submode=2;
                      config.paths[config.amountOfPaths].amountOfTags=0;
                      memset(config.paths[config.amountOfPaths].tags[config.paths[config.amountOfPaths].amountOfTags],0,sizeof(config.paths[config.amountOfPaths].tags[config.paths[config.amountOfPaths].amountOfTags]));
                  }
                    if(submode!=0){
                      //printf("%s\n",buffer);
                      memset(buffer, 0, sizeof(buffer));
                      i=0;
                     }
                    break;
                   }
                  case 1: {//läser path i #paths
                        if(!quote && equals && strlen(buffer)>0){
                          memcpy(config.paths[config.amountOfPaths].pathStr, buffer, sizeof(config.paths[config.amountOfPaths].pathStr));
                          equals = false;
                          submode = 0;
                          memset(buffer, 0, sizeof(buffer));
                          i=0;
                        }
                        break;
                      }
                  case 2: {//läser in tags i #paths
                      if(bracket==1 && c=='}'){
                          equals = false;
                          submode = 0;
                          i=0;
                      }
                      if(bracket==2&&c==','){
                        config.paths[config.amountOfPaths].amountOfTags++;
                        memset(config.paths[config.amountOfPaths].tags[config.paths[config.amountOfPaths].amountOfTags],0,sizeof(config.paths[config.amountOfPaths].tags[config.paths[config.amountOfPaths].amountOfTags]));
                      }
                      if(!quote && equals&&strlen(buffer)>0){
                          memcpy(config.paths[config.amountOfPaths].tags[config.paths[config.amountOfPaths].amountOfTags], buffer, sizeof(config.paths[config.amountOfPaths].tags[config.paths[config.amountOfPaths].amountOfTags]));
                          memset(buffer,0,sizeof(buffer));
                          i=0;
                        }
                      break;
                          }
                }
                break;
                }
        case 2:{//läser #folders
                if(c==','&&bracket==0){
                  config.amountOfFolders++;
                  config.folders[config.amountOfFolders].amountOfExtensions=0;
                  i=0;
                  submode=0;
                  memset(buffer,0,sizeof(buffer));
                }
                switch(submode){
                  case 0:{//läser submode i #folders
                    if(!strcmp(buffer, "title")){
                      submode=1;
                      memset(config.folders[config.amountOfFolders].title,0,sizeof(config.folders[config.amountOfFolders].title));
                    }
                    if(!strcmp(buffer, "tags")){
                      submode=2;
                      config.folders[config.amountOfFolders].amountOfTags=0;
                      memset(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags],0,sizeof(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags]));
                  }
                    if(!strcmp(buffer, "extensions")){
                      submode=3;
                      memset(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions],0,sizeof(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions]));
                        config.folders[config.amountOfFolders].amountOfExtensions++;
                        }
                    if(submode!=0){
                      //printf("%s\n",buffer);
                      memset(buffer, 0, sizeof(buffer));
                      i=0;
                     }
                    break;
                         }
              case 1:{//läser in title i #folders
                  if(!quote && equals && strlen(buffer)>0){
                    memcpy(config.folders[config.amountOfFolders].title, buffer, sizeof(config.folders[config.amountOfFolders].title));
                    equals = false;
                    submode = 0;
                    memset(buffer, 0, sizeof(buffer));
                    i=0;
                  }
                  break;
                }

            case 2: {//läser in tags i #folders
                if(bracket==1 && c=='}'){
                    memcpy(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags], buffer, sizeof(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags]));
                    memset(buffer,0,sizeof(buffer));
                    equals = false;
                    submode = 0;
                    i=0;
                }
                if(bracket==2&&c==','){
                    memcpy(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags], buffer, sizeof(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags]));
                    //printf("Tag: %s\n",buffer);
                    memset(buffer,0,sizeof(buffer));
                    config.folders[config.amountOfFolders].amountOfTags++;
                    memset(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags],0,sizeof(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfTags]));
                    i=0;
                }
                break;
                    }
      case 3:{//läser in extensions i #folders
                if(bracket==1 && c=='}'){
                    memcpy(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions-1], buffer, sizeof(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions-1]));
                    memset(buffer,0,sizeof(buffer));
                    equals = false;
                    submode = 0;
                    i=0;
                }
                if(bracket==2&&c==','){
                    memcpy(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions-1], buffer, sizeof(config.folders[config.amountOfFolders].tags[config.folders[config.amountOfFolders].amountOfExtensions - 1]));
                    memset(buffer,0,sizeof(buffer));
                    memset(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions],0,sizeof(config.folders[config.amountOfFolders].extensions[config.folders[config.amountOfFolders].amountOfExtensions]));
                    config.folders[config.amountOfFolders].amountOfExtensions++;
                    i=0;
                }
                break;
             }


               }
                }
    }
  }
  return config;
}

  
