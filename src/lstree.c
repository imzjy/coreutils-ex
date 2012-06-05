#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define INDENT 4

/* default do not listing hide file and directory */
static int flag_hide = 1; 

/* return NULL terminated string with dynamic allocated buffer */
/* do not forget release the buffer when use the function */
char*
readlink_malloc(const char *link_name)
{
	int size = 100;
	char *buffer = NULL;	

	while(1){
		buffer = (char *)realloc(buffer,size + 1);
		int nchars = readlink(link_name, buffer, size);
		if(nchars < 0){
			free(buffer);
			return NULL;
		}
		if(nchars < size){
			buffer[nchars] = '\0';
			return buffer;
		}
		size *= 2;
	}
}


/* print the directory content with indent */
int
print_dir(int indent_level, char *dirname)
{
	char indentBuf[1024] = "";
	char pathBuf[1024] = "";
	char *plink_fullpath;
	int blankspace = indent_level * INDENT;
	DIR *dirp;
	struct dirent *ptr;


	//calc indent space
	bzero(indentBuf,sizeof(indentBuf));
	while(blankspace > 0){
		strcat(indentBuf, " ");
		blankspace--;
	}		
		
	if((dirp = opendir(dirname)) != NULL){
		while((ptr = readdir(dirp))){

			if(strcmp(ptr->d_name, ".") == 0 ||
					strcmp(ptr->d_name, "..") == 0)
				continue;
			
			if(flag_hide && ptr->d_name[0] == '.'){
				//hide files/directory
				continue;
			}

		
			if(ptr->d_type == DT_REG){
				fprintf(stdout, "%s%s\n", indentBuf, ptr->d_name);
			}
			else if(ptr->d_type == DT_DIR){
				fprintf(stdout, "%s%s/\n", indentBuf, ptr->d_name);

				sprintf(pathBuf, "%s/%s", dirname, ptr->d_name);
				print_dir(indent_level + 1, pathBuf);					
			}
			else if(ptr->d_type == DT_LNK){
				sprintf(pathBuf,"%s/%s", dirname, ptr->d_name);

				plink_fullpath = readlink_malloc(pathBuf);
				fprintf(stdout, "%s%s@ -> %s\n", indentBuf, ptr->d_name, plink_fullpath);
				free(plink_fullpath);
			}
		}
	}
	return 0;
}


int
main(int argc, char *argv[])
{

	int opt;
	int index;
	char dirname[256] = ".";

	//options parameter
	while ((opt = getopt(argc, argv, "at:")) != -1) {
		switch (opt) {
			case 'a':
				flag_hide = 0;
		    	break;
			case 't':
			    //nsecs = atoi(optarg);
			    //tfnd = 1;
				break;
			default:
				break;
		}
	}

				
	//non-options paramter, assume a directory
	for(index = optind; index < argc; index++){
		sprintf(dirname, "%s", argv[index]);
		print_dir(0, dirname);
	}
	
	//check dirname is a direcotry	
	//printf("%s\n",dirname);

	return 0;	
}
