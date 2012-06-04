#include <stdio.h>
#include <dirent.h>
#include <string.h>

#define INDENT 4

int
print_dir(int indent_level, char *dirname)
{
	char indentBuf[1024] = "";
	int blankspace = indent_level * INDENT;
	DIR *dirp;
	struct dirent *ptr;


	if((dirp = opendir(dirname)) != NULL){
		while((ptr = readdir(dirp))){

			if(strcmp(ptr->d_name, ".") == 0 ||
					strcmp(ptr->d_name, "..") == 0)
				continue;
			
			//calc indent space
			while(blankspace > 0){
				strcat(indentBuf, " ");
				blankspace--;
			}		

			if(ptr->d_type == DT_REG){
				fprintf(stdout, "%s%s\n", indentBuf, ptr->d_name);
			}
			else if(ptr->d_type == DT_DIR){
				fprintf(stdout, "%s%s/\n", indentBuf, ptr->d_name);

				char dirpath[256];
				sprintf(dirpath,"%s/%s", dirname, ptr->d_name);
				print_dir(++indent_level, dirpath);					
			}
		}
	}
	return 0;
}


int
main()
{
	print_dir(0,".");

	return 0;	
}
