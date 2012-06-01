#include <stdio.h>
#include <dirent.h>
#include <string.h>

int
print_dir(char *dirname)
{
	DIR *dirp;
	struct dirent *ptr;


	if((dirp = opendir(dirname)) != NULL){
		while((ptr = readdir(dirp))){

			if(strcmp(ptr->d_name, ".") == 0 ||
					strcmp(ptr->d_name, "..") == 0)
				continue;


			printf("%s\n", ptr->d_name);	

			if (ptr->d_type == DT_DIR){
				char dirpath[256];
				sprintf(dirpath,"%s/%s", dirname, ptr->d_name);
				//printf("%s\n",dirpath);
				print_dir(dirpath);					
			}
		}
	}
	return 0;
}


int
main()
{
	print_dir(".");

	return 0;	
}
