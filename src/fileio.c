#include"../include/fileio.h"

void usage(char *prog){
	printf("Usage: %s [--mode=Mode] [--file=] [--size_bytes=n] [--iters=N]\n", prog);
    	printf("Modes:\n");
    	printf("  write: Moving data from UB to  KPC\n");
    	printf("  write_fsync: Moving data from UB to KPC to Disk\n");
    	printf("  stdio: Moving data from UB to SB to KPC\n");
    	printf("  stdio_fsync: Moving data from UB to SB to KPC to Disk\n");
    	printf("  osync: Moving data from UB to KPC to Disk \n");
    	printf("  odirect: Moving data from UB to Disk\n");
    	exit(1);
}

int main(int argc,char*argv[]){
	if (argc<5)
		usage(argv[0]);
	
	char *mode = malloc(256);
    	char *filename = malloc(256);
    	size_t size=4096;
    	int iters=1000;

    	int flags = O_CREAT | O_WRONLY | O_TRUNC;
    	int use_stdio = 0;
    	int do_fsync = 0;
    	int use_direct = 0;

    	if(!strcmp(mode, "write")){} 
	else if(!strcmp(mode, "write_fsync")) {
        	do_fsync = 1;
    	}
	else if(!strcmp(mode, "stdio")) {
        	use_stdio = 1;
    	}
	else if(!strcmp(mode, "stdio_fsync")) {
        	use_stdio = 1;
        	do_fsync = 1;
    	}
	else if(!strcmp(mode, "osync")) {
        	flags |= O_SYNC;
    	}
	else if (!strcmp(mode, "odirect")) {
        	flags |= O_DIRECT;
        	use_direct = 1;
    	}
	else{
        	usage(argv[0]);
    	}

    	int fd=-1;
    	FILE *fp=NULL;

    	if(use_stdio){
        	fp=fopen(filename, "w");
        if (!fp){
		perror("fopen"); exit(1); }
    	}
	else{
        	fd=open(filename, flags, 0666);
        	if(fd<0){
			perror("open"); exit(1);
		}
    	}

    	void *buf;
    	if(use_direct){
        	if(posix_memalign(&buf, ALIGNMENT, size)){
            		perror("memalign"); 
			exit(1);
        	}

    	} 
	else{
        	buf=malloc(size);
    	}
    	memset(buf, 'A', size);

    	double t_start = now_sec();

    	for(int i=0;i<iters;i++){
        	if(use_stdio){
            		if(fwrite(buf, 1, size, fp) != size){
                		perror("fwrite"); 
				exit(1);
            		}

            		fflush(fp);
            		if(do_fsync){
                		if(fsync(fileno(fp))< 0){
                    			perror("fsync"); 
					exit(1);
                		}
            		}

        	}
		else{
            		if(write(fd, buf, size) != size){
                		perror("write"); 
				exit(1);
            		}
            		if(do_fsync){
                		if(fsync(fd)<0){
                    			perror("fsync"); exit(1);
                		}
          		}
       		}
   	}

    	double t_end=now_sec();

    	double total=t_end-t_start;
    	double avg_us=(total/iters)*1e6;

    	printf("Mode: %s\n", mode);
    	printf("Size: %zu bytes\n", size);
    	printf("Iterations: %d\n", iters);
    	printf("Avg latency: %.3f us\n", avg_us);

    if(use_stdio)
	fclose(fp);
    else 
	close(fd);

    free(buf);
    return 0;
}
