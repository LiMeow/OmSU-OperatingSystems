#include <iostream>
#include <dirent.h>
#include <cstring>
#include <unistd.h>
#include <cstdlib>
#include <sys/stat.h>
#include <limits.h>
 
using namespace std;
 
bool sameStat(struct stat s1, struct stat s2) {
  return s1.st_dev == s2.st_dev && s1.st_ino == s2.st_ino;
}
  
int walkUpAndCountLinks() {
  int linkCount = 0;
   string resultPath;
int level=0;
  struct stat root;
  struct stat currentPathStat;
  struct stat fileStat;
  char filePath[1024];
  char outerDirectory[1024];
  char currentPath[1024];
  char originPath[1024];
  dirent *fileDirent;


  strcpy(currentPath, ".");
  realpath(currentPath, originPath);
 
  if (lstat("/", &root) == -1) {
     cout << "Error: lstat for /" << endl;
     return -1;
  }
 
  while (true) {
    if (lstat(currentPath, &currentPathStat) == -1) {
      cout << "Error: lstat for" << currentPath << endl;
             return -1;
    }
    if (sameStat(root, currentPathStat)) {
      cout << "Path: " << resultPath << '\n';
      return linkCount;
    }

    strcpy(outerDirectory, currentPath);
    strcat(outerDirectory, "/..");

    DIR *outerDir = opendir(outerDirectory);
    if (outerDir == nullptr) {
       cout << "Error:opendir"<< endl;
            return -1;
    }
    while ((fileDirent = readdir(outerDir)) != nullptr) {
      if ((strcmp(fileDirent->d_name, ".")) == 0) continue;
      if ((strcmp(fileDirent->d_name, "..")) == 0) continue;

      strcpy(filePath, outerDirectory);
      strcat(filePath, "/");
      strcat(filePath, fileDirent->d_name);

      if (lstat(filePath, &fileStat) == -1) {
         cout << "Error: lstat for" << filePath << endl;
                return -1;
      }
      if (S_ISLNK(fileStat.st_mode)) 
            {
                char linkPath[1024];
                int n = readlink(filePath, linkPath, 1024);
                if (n == -1)
                {
                    cout << "Error: readlink" << endl;
                    return -1;
                }
 
                linkPath[n] = 0;
                if (strcmp(originPath, linkPath) == 0 && level%2==0)
                {
                   linkCount++;
                }
            }
      if (sameStat(fileStat, currentPathStat)) {
        cout<<"level  "<< level<<endl;
        resultPath = '/' + string(fileDirent->d_name) + resultPath;
        level++;
        cout<<"resultPath  "<<resultPath<<endl;
      }
    }
    strcat(currentPath, "/..");
  }
}
 
int main() {
  int res= walkUpAndCountLinks();
  cout<<"countLinks:"<<res<<endl;
  return 0;
}