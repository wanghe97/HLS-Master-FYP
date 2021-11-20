//for orderings; change numall, .txt;
#include <stdlib.h>
#include <string.h>
#include <stdio.h>
#include <errno.h>
#include <time.h>

#define N 24
#define length 4

int num = 0;
int k = 0;
char* foldername[N]={"stencils","stencils","stencils","stencils","medley","medley","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/solvers","linear-algebra/solvers","linear-algebra/solvers","linear-algebra/solvers","data-mining"};
char* examples[N]={"/heat-3d","/jacobi-1d","/seidel-2d","/jacobi-2d","/nussinov","/floyd-warshall","/3mm","/2mm","/doitgen","/bicg","/mvt","/atas","/syrk","/gemver","/symm","/gesummv","/gemm","/trmm","/syr2k","/cholesky","/trisolv","/lu","/ludcmp","/covariance"};
//char* foldername[N]={"stencils","medley","linear-algebra/kernels","linear-algebra/blas","linear-algebra/solvers"};
//char* examples[N]={"/heat-3d","/nussinov","/3mm","/syrk","/trisolv"};

//char* head = "cd /home/edana/vericert2&&./bin/vericert benchmarks/polybench-syn/stencils/heat-3d.c -o benchmarks/polybench-syn/results/vericert.v -DSYNTHESIS -O0 -finline";
char* head1 = "cd /home/edana/vericert3&&./bin/vericert benchmarks/polybench-syn/";
char* head2 = ".c -o benchmarks/polybench-syn/results/vericert.v -DSYNTHESIS -O0 -finline";

char* tail = "&&iverilog -o vericert -- benchmarks/polybench-syn/results/vericert.v&&./vericert";

char* add12 = "&&cd benchmarks/polybench-syn/results&&cd numall2&&mkdir num";
char* add13 = "&&cd benchmarks/polybench-syn/results&&cd numall12&&mkdir num";
char* add2 = "&&cd num";
char* add3 = "&&. /opt/Xilinx/Vivado/2019.2/settings64.sh >& /dev/null&&vivado -mode batch -nojournal -nolog -source ../../synth.tcl";
char* path12 = "/home/edana/vericert3/benchmarks/polybench-syn/results/numall2/num";
char* path13 = "/home/edana/vericert3/benchmarks/polybench-syn/results/numall12/num";
char* path2 = "/encode_report.xml";
char* merit2 = "XILINX_SLICE_LUTS";
char* merit3 = "XILINX_DESIGN_DELAY";

char* flags = NULL;
int tc;
float tc3;

//convert array of string to a string
char* ConStr(const char** strings)
{
    int i = 0;              /* Loop index               */
    int count = 0;          /* Count of input strings   */
    char* result = NULL;   /* Result string            */
    int totalLength = 0;    /* Length of result string  */

    /* Check special case of NULL input pointer. */
    if (strings == NULL)
    {
        return NULL;
    }

    /*
     * Iterate through the input string array,
     * calculating total required length for destination string.
     * Get the total string count, too.
     */
    while (strings[i] != NULL)
    {
        totalLength += strlen(strings[i]);
        i++;
    }
    count = i;
    totalLength++;  /* Consider NUL terminator. */

    /*
     * Allocate memory for the destination string.
     */
    result = (char*)malloc(sizeof(char) * totalLength);
    memset(result, 0, sizeof(char) * totalLength);

    if (result == NULL)
    {
        /* Memory allocation failed. */
        return NULL;
    }

    /*
     * Concatenate the input strings.
     */
    for (i = 0; i < count; i++)
    {
        strcat(result, strings[i]);
    }

    return result;
}


//read merit from xml file
//for clock cycles
int readmerit1(char* cmd){
    int merit1;
    FILE *fstream=NULL;
    char buff[100];
    memset(buff,0,sizeof(buff));

    if(NULL==(fstream=popen(cmd,"r"))){
        fprintf(stderr,"execute command failed: %s",strerror(errno));
        return -1;
    }

    if(NULL!=fgets(buff, sizeof(buff),fstream)){
	//printf("==>%s\n",buff);
	char value[10]={0};
	int i = 8;
	int j = 0;
	while(buff[i]!='\0'){
	    value[j] = buff[i];
	    i++;
	    j++;
	}
	
	merit1 = atoi(value);
    }else{
	pclose(fstream);
	return -1;
    }
    pclose(fstream);

    return merit1;
}

//for luts
char* readmerit2() {
    char buffer[100];
    static char value[20]={0};
    memset(value, 0, sizeof(char) * 20);
    char snum[10];
    sprintf(snum, "%d", num);
    const char* fname[] = { path12,snum,path2,NULL };
    char* name = ConStr(fname);
    FILE* file = fopen(name, "r");
    // Checks if the file was opened successfully
    if (file == NULL)
    {
        fputs("Failed to open the file\n", stderr);
        return "fail to open the file";
    }

    // fgets here reads an entire line or 99 characters (+1 for \0) at a time, whichever comes first
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
	//printf("buffer is: %s\n",buffer);
        char* start = strstr(buffer, "<");
        char* end = strstr(buffer, ">");
        int l = end - start;
        int j;
        if (strstr(buffer, merit2) != NULL) {

            for (j = 0; j <= l; j++)
            {
                if (buffer[j] == 'e' && buffer[j + 1] == '=')
                {
                    int i = 0;
                    while (buffer[j + 3] != '"')
                    {
                        value[i] = buffer[j + 3];
			//printf("!!!buffer is:%s, value is: %c",buffer,value[i]);
                        i++;
                        j++;
                    }
                }
            }

        }

    }

    fclose(file);

    
    return value;
}

//for delay
char* readmerit3() {
    char buffer[100];
    static char value[20]={0};
    memset(value, 0, sizeof(char) * 20);
    char snum[10];
    sprintf(snum, "%d", num);
    const char* fname[] = { path13,snum,path2,NULL };
    char* name = ConStr(fname);
    FILE* file = fopen(name, "r");
    // Checks if the file was opened successfully
    if (file == NULL)
    {
        fputs("Failed to open the file\n", stderr);
        return "fail to open the file";
    }

    // fgets here reads an entire line or 99 characters (+1 for \0) at a time, whichever comes first
    while (fgets(buffer, sizeof(buffer), file) != NULL)
    {
	//printf("buffer is: %s\n",buffer);
        char* start = strstr(buffer, "<");
        char* end = strstr(buffer, ">");
        int l = end - start;
        int j;
        if (strstr(buffer, merit3) != NULL) {

            for (j = 0; j <= l; j++)
            {
                if (buffer[j] == 'e' && buffer[j + 1] == '=')
                {
                    int i = 0;
                    while (buffer[j + 3] != '"')
                    {
                        value[i] = buffer[j + 3];
			//printf("!!!buffer is:%s, value is: %c",buffer,value[i]);
                        i++;
                        j++;
                    }
                }
            }

        }

    }

    fclose(file);

    
    return value;
}

//delete the element at in for a string array with length n

const char** rmv(const char** ptr, int in, const unsigned int n)
{
    static const char* nptr[length];
    unsigned int i;
    for (i = 0; i < in; i++) {
        nptr[i] = ptr[i];
    }
    for (i = in; i < n - in; i++) {
        nptr[i] = ptr[i + 1];
    }
    //for (i = n - in; i < n; i++) {
    //    nptr[n - in] = NULL;
    //}
    return nptr;
}

//delete one element to get the optimal list 
void rmv1(const char* *ptr,int in,const unsigned int n)
{
   unsigned int i;
   for (i=in; i<n-1; i++) {
   *(ptr+i)=*(ptr+i+1);
   }
}

//find the optimal value; 
//tc is the value when all flags are turn on; len is the length of flags array 
//for clock cycles
int opt_tc1(const char** flaglist, int tc, int len) {
    int opt = tc; //initial--all flags
    int current;
    int i = 0;
    const char** curlist = NULL;
    int opt_list_num = 0;
    char* optcmd = NULL;

    while (flaglist[i] != NULL) {
        //get cmd for current flags
        curlist = rmv(flaglist, i, length);
        char* curflag = NULL;
        curflag = ConStr(curlist);
	//const char* cmdarr[] = { head,curflag,tail,NULL };
	const char* cmdarr[] = { head1,foldername[k],examples[k],head2,curflag,tail,NULL };
        optcmd = ConStr(cmdarr);

        //get value from xml file
        current = readmerit1(optcmd);
	printf("!!!current:%d\n",current);
        if (current < opt) {
	    
            opt = current;
            flags = curflag;
            opt_list_num = i;
	    printf("current opt value is %d, num is %d\n",opt, num);
	    
        }

        num++;
        i++;
    }
    if (opt == tc) {
        return opt;
    }
    else {
        //start from new optimal flaglist (delete one element)
	rmv1(flaglist,opt_list_num,len);
	printf("delete element %d, opt value is %d, opti_flags is: %s\n",opt_list_num,opt,flags);
	len--;
        return opt_tc1(flaglist, opt, len);
    }


    return opt;
}

//for luts
int opt_tc2(const char** flaglist, int tc, int len) {
    int opt = tc; //initial--all flags
    int current;
    int i = 0;
    const char** curlist = NULL;
    int opt_list_num = 0;
    char* optcmd = NULL;

    while (flaglist[i] != NULL) {
        //get cmd for current flags
        curlist = rmv(flaglist, i, length);
        char* curflag = NULL;
        curflag = ConStr(curlist);

	char snum[10];
        sprintf(snum, "%d", num);
	const char* cmdarr[] = { head1,foldername[k],examples[k],head2,curflag,add12,snum,add2,snum,add3,NULL };
        optcmd = ConStr(cmdarr);
	printf("!!!optcmd: %s\n",optcmd);

        //get value from xml file
	if(atoi(readmerit2())==0){
	    system(optcmd);
        }
        current = atoi(readmerit2());
	printf("!!!num %d, current:%d\n",num,current);

        if (current < opt) {
	    
            opt = current;
            flags = curflag;
            opt_list_num = i;
	    printf("current opt value is %d, num is %d\n",opt, num);
	    
        }

        num++;
        i++;
	printf("!!!optflags: %s\n",flags);
    }
    if (opt == tc) {
        return opt;
    }
    else {
        //start from new optimal flaglist (delete one element)
	rmv1(flaglist,opt_list_num,len);
	printf("delete element %d, opt value is %d, opti_flags is: %s\n",opt_list_num,opt,flags);
	//fprintf(fpWrite1,"delete element %d, opt value is %d, opti_flags is: %s\n",opt_list_num,opt,flags);
	len--;
        return opt_tc2(flaglist, opt, len);
    }

    //fclose(fpWrite1);
    return opt;
}

//for delay
float opt_tc3(const char** flaglist, float tc3, int len) {
    float opt = tc3; //initial--all flags
    float current;
    int i = 0;
    const char** curlist = NULL;
    int opt_list_num = 0;
    char* optcmd = NULL;

    while (flaglist[i] != NULL) {
        //get cmd for current flags
        curlist = rmv(flaglist, i, length);
        char* curflag = NULL;
        curflag = ConStr(curlist);

	char snum[10];
        sprintf(snum, "%d", num);
	const char* cmdarr[] = { head1,foldername[k],examples[k],head2,curflag,add13,snum,add2,snum,add3,NULL };
        optcmd = ConStr(cmdarr);

        //get value from xml file
	if(atoi(readmerit3())==0){
	    system(optcmd);
        }
        current = atof(readmerit3());
	printf("!!!current:%f\n",current);

        if (current < opt) {
	    
            opt = current;
            flags = curflag;
            opt_list_num = i;
	    printf("current opt value is %f, num is %d\n",opt, num);
	    
        }

        num++;
        i++;
    }
    if (opt == tc3) {
        return opt;
    }
    else {
        //start from new optimal flaglist (delete one element)
	rmv1(flaglist,opt_list_num,len);
	printf("delete element %d, opt value is %f, opti_flags is: %s\n",opt_list_num,opt,flags);
	len--;
        return opt_tc3(flaglist, opt, len);
    }


    return opt;
}

//data-mining/covariance
//stencils/heat-3d

int main(void){
    int m; //number represent the selected merit
    printf("Which merit do you want? please input a number\n");
    printf("%d: Minimum clock cycles;\n",1);
    printf("%d: Minimum area;\n",2);
    printf("%d: Maximum frequency\n",3);
    scanf("%d", &m);
    
    if(m==1){
	    while(k<N){
		FILE *fpWrite=fopen("/home/edana/vericert3/benchmarks/polybench-syn/results/tc2.txt","a");
		if(fpWrite==NULL){
		    return 0;
		}

		const char* array[length] = {" -fconst-prop", " -fcse", " -fif-conversion", NULL};
		int len = length;
		flags = ConStr(array);
		printf("num: %d, flags:%s\n", num, flags);

		//const char* cmdarr[] = { head,flags,tail,NULL };
		const char* cmdarr[] = { head1,foldername[k],examples[k],head2,flags,tail,NULL };
		char* cmd = ConStr(cmdarr);
		//printf("!!cmd: %s\n",cmd);
		printf("!!%s\n",examples[k]);
		fprintf(fpWrite,"%s, num %d\n",examples[k],num);
		
		time_t now1,now2,h,m,s,t;

		//calculate execution time
		time(&now1); 
		
		tc = readmerit1(cmd);
		num++;
		printf("Initial Total_cycles = %d\n", tc);
		fprintf(fpWrite,"Initial Total_cycles = %d\n", tc);
		tc = opt_tc1(array, tc, len);
		printf("The optimal Total_cycles = %d\n", tc);
		fprintf(fpWrite,"optimal flags:%s\n",flags);
		fprintf(fpWrite,"The optimal Total_cycles = %d\n", tc);
		
		time(&now2); 
		t=now2-now1;  
		h=t/3600;
		t%=3600;    
		m=t/60; 
		s=t%60;
		printf("Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	        fprintf(fpWrite,"Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	
		k++;
		fclose(fpWrite);
	    }
    }
    
    if(m==2){

	    while(k<N){
		FILE *fpWrite=fopen("/home/edana/vericert3/benchmarks/polybench-syn/results/luts2.txt","a");
		if(fpWrite==NULL){
		    return 0;
		}

		const char* array[length] = {" -fconst-prop", " -fcse", " -fif-conversion", NULL};
		int len = length;
		flags = ConStr(array);
		printf("num: %d, flags:%s\n", num, flags);

		char snum[10];
		sprintf(snum, "%d", num);
		const char* cmdarr[] = { head1,foldername[k],examples[k],head2,flags,add12,snum,add2,snum,add3,NULL };
		char* cmd = ConStr(cmdarr);
		//printf("!!cmd: %s\n",cmd);
		printf("!!%s\n",examples[k]);
		fprintf(fpWrite,"%s, num %d\n",examples[k],num);
		
		time_t now1,now2,h,m,s,t;

		//calculate execution time
		time(&now1); 

		if(atoi(readmerit2())==0){
		    system(cmd);
		}
		tc = atoi(readmerit2());
		num++;
		printf("Initial Luts = %d\n", tc);
		fprintf(fpWrite,"Initial Luts = %d\n", tc);
		tc = opt_tc2(array, tc, len);
		printf("The optimal Luts = %d\n", tc);
		fprintf(fpWrite,"optimal flags:%s\n",flags);
		fprintf(fpWrite,"The optimal Luts = %d\n", tc);
		
		time(&now2); 
		t=now2-now1;  
		h=t/3600;
		t%=3600;    
		m=t/60; 
		s=t%60;
		printf("Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	        fprintf(fpWrite,"Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);

		k++;
		fclose(fpWrite);
	    }
    }

    if(m==3){
	    while(k<N){
		FILE *fpWrite=fopen("/home/edana/vericert3/benchmarks/polybench-syn/results/freq2.txt","a");
		if(fpWrite==NULL){
		    return 0;
		}

		const char* array[length] = {" -fconst-prop", " -fcse", " -fif-conversion", NULL};
		int len = length;
		flags = ConStr(array);
		printf("num: %d, flags:%s\n", num, flags);

		char snum[10];
		sprintf(snum, "%d", num);
		const char* cmdarr[] = { head1,foldername[k],examples[k],head2,flags,add13,snum,add2,snum,add3,NULL };
		char* cmd = ConStr(cmdarr);
		//printf("!!cmd: %s\n",cmd);
		printf("!!%s\n",examples[k]);
		fprintf(fpWrite,"%s, num %d\n",examples[k],num);
		
		time_t now1,now2,h,m,s,t;

		//calculate execution time
		time(&now1); 
		
		if(atof(readmerit3())==0){
		    system(cmd);
		}
		tc3 = atof(readmerit3());
		num++;
		printf("Initial Delay = %f\n", tc3);
		fprintf(fpWrite,"Initial Delay = %f\n", tc3);
		tc3 = opt_tc3(array, tc3, len);
		printf("The optimal Delay = %f\n", tc3);
		fprintf(fpWrite,"optimal flags:%s\n",flags);
		fprintf(fpWrite,"The optimal Delay = %f\n", tc3);
		
		time(&now2); 
		t=now2-now1;  
		h=t/3600;
		t%=3600;    
		m=t/60; 
		s=t%60;
		printf("Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	        fprintf(fpWrite,"Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);

		k++;
		fclose(fpWrite);
            }
    }

    
    return 0;
}











