#include <stdio.h> 
#include <sys/stat.h>
#include <stdbool.h>
#include <dirent.h>
#include <string.h>
#include <time.h>
#include <syslog.h>
#include <linux/limits.h>

bool copyFile(char *sourcePath, char *destinationPath);
void copyRecursively(char *sourcePath, char *targetPath)
{
	DIR *sourceDir = opendir(sourcePath);
	int priority = LOG_INFO | LOG_DAEMON;
	
	openlog(NULL, LOG_CONS , priority);
	
	if(mkdir(targetPath, S_IRWXU) != 0)
	{
		return;
	}
	
	DIR *targetDir = opendir(targetPath);
	struct dirent *dir;
	if(sourceDir)
	{
		while((dir = readdir(sourceDir)) != NULL)
		{	
			if(!strcmp(dir->d_name, "."))
			{
				continue;
			}
			else if(!strcmp(dir->d_name, ".."))
			{
				continue;
			}
			else if(dir->d_type == 4)
			{
				char targetFilename[PATH_MAX];
				sprintf(targetFilename, "%s/%s", targetPath, dir->d_name);
				
				
				char sourceFilename[PATH_MAX];
				sprintf(sourceFilename, "%s/%s", sourcePath, dir->d_name);
				
				
				copyRecursively(sourceFilename, targetFilename);
			}
			else
			{
				char targetFilename[PATH_MAX];
				char sourceFilename[PATH_MAX];
				sprintf(sourceFilename, "%s/%s", sourcePath, dir->d_name);
				sprintf(targetFilename, "%s/%s", targetPath, dir->d_name);
				copyFile(sourceFilename, targetFilename);
			} 
		}
		
		char logMessage[PATH_MAX + 100];
		
		time_t currtime = time(NULL);
		char *currtimeS = ctime(&currtime);
		sprintf(logMessage, "Katalog %s został skopiowany pomyślnie %s.", sourcePath, currtimeS);
	
		syslog(priority, logMessage);
		closelog();
	}
}
