#include <stdio.h>
#include <unistd.h>
#include <dirent.h>
#include <string.h>

#define INDENT 4

static int flag_hide = 1; //default do not listing hide file/directory

int
print_dir(int indent_level, char *dirname)
{
	char indentBuf[1024] = "";
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

				char dirpath[256];
				sprintf(dirpath,"%s/%s", dirname, ptr->d_name);
				print_dir(indent_level + 1, dirpath);					
			}
			else if(ptr->d_type == DT_LNK){
				fprintf(stdout, "%s%s@\n", indentBuf, ptr->d_name);
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
	for(index = optind; index < argc; index++)
		sprintf(dirname, "%s", argv[index]);
	
	//check dirname is a direcotry	
	//printf("%s\n",dirname);

	print_dir(0,dirname);

	return 0;	
}
