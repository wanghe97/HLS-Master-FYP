#include "stdio.h"
#include <string.h>
#include <stdlib.h>
#include <time.h>

#define N 24
#define length 27
static int knum = 0;
static int num = 0;
char opt_flags[500] = { 0 };
static int opt = 0;
static float opt3 = 0;
int neg[length] = { -1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1,-1};
int count1 = 0;

int k = 0;
char* foldername[N]={"stencils","stencils","stencils","stencils","medley","medley","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/kernels","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/blas","linear-algebra/solvers","linear-algebra/solvers","linear-algebra/solvers","linear-algebra/solvers","data-mining"};
char* examples[N]={"/heat-3d","/jacobi-1d","/seidel-2d","/jacobi-2d","/nussinov","/floyd-warshall","/3mm","/2mm","/doitgen","/bicg","/mvt","/atas","/syrk","/gemver","/symm","/gesummv","/gemm","/trmm","/syr2k","/cholesky","/trisolv","/lu","/ludcmp","/covariance"};

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

//delete the element at in for a string array with length n

const char** rmv(const char** ptr, int in, const unsigned int n)
{
    //printf("!!!in is %d\n",in);
    static const char* nptr[length];
    unsigned int i;
    for(i=0; i<n; i++){
	if(i<in){
	    nptr[i] = ptr[i];
	    //printf("!!!%s at %d with %d\n",nptr[i],i,in);
	}else{
	    nptr[i] = ptr[i + 1];
	    //printf("!!!%s at %d with %d\n",nptr[i],i,in);
	}
    }

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

//update the negative flags pool
void rmvp(int *ptr,int in, int val, const unsigned int n)
{
   unsigned int i;
   for (i=in; i<n; i++) {
   	*(ptr+i)=*(ptr+i+1);
   }
   for(i=0;i<n-1;i++){
	if(ptr[i]>val){
	    ptr[i]=ptr[i]-1;
	}
	printf("!!!neg %d: %d\n",i,ptr[i]);
   }
}

//read merit from xml file
char* readmerit(char* merit, int num, char* path) {
    char buffer[100];
    static char value[20]={0};
    memset(value, 0, sizeof(char) * 20);
    char snum[10];
    sprintf(snum, "%d", num);
    const char* fname[] = { path,snum,".xml",NULL };
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
        if (strstr(buffer, merit) != NULL) {

            for (j = 0; j <= l; j++)
            {
                if (buffer[j] == '"' && buffer[j + 1] != '/')
                {
                    int i = 0;
                    while (buffer[j + 1] != '"')
                    {
                        value[i] = buffer[j + 1];
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

//create the negative flags pool for tc
void neg_pool1(const char** flaglist, char* path, char* cmd) {
    //initiate neg list
    int j = 0;
    while(neg[j]!=-1){
	neg[j] = -1;
	j++;
    }
    count1 = 0; //initiate count1
    int tc = opt; //store initial opt for this pass
    int current;
    int i = 0;
    //const char** curlist = NULL;
    char optcmd[500] = { 0 };
    strcpy(optcmd, cmd);
    while(flaglist[i]!=NULL){
        const char* curflag = NULL;
        curflag = flaglist[i];
        //get cmd for current flags
	//printf("!!!curflag is : %s\n",curflag);
	memset(optcmd, 0, sizeof(optcmd));
	strcpy(optcmd, cmd);
        strcat(optcmd, curflag);
	//printf("!!!cmd: %s\n",optcmd);

        //get value from xml file
        char* merit = "TOTAL_CYCLES";
	if(atoi(readmerit(merit, num-knum, path))==0){
	    system(optcmd);
        }
        char* Value = readmerit(merit, num-knum, path);
	//printf("!!!Value is %s, merit is %s, num is %d\n",Value, merit, num);
        current = atoi(Value);
        if (current < tc) {
	    
	    neg[count1] = i;
	    printf("neg value in %d is %d\n",count1,neg[count1]);
            count1++;
        }

        num++;
	//printf("!!!next num is %d, current is %d\n",num,current);
        i++;
    }

}

//find the optimal value; 
//path is the first element for .xml file; num is the index for xml files
//tc is the value when all flags are turn on; len is the length of flags array 
//cmd is the fixed part of command, changing with benchmark
int opt_tc1(const char** flaglist, char* path, char* cmd, int len) {
    int tc = opt; //store initial opt for this pass
    int current;
    int neg_i = -1;
    int i = 0;
    //const char** curlist = NULL;
    int opt_list_num = 0;
    char optcmd[500] = { 0 };
    strcpy(optcmd, cmd);
    while(neg[i]!=-1){
        //get cmd for current flags
	const char* curflag = NULL;
        curflag = flaglist[neg[i]];
        //curlist = rmv(flaglist, neg[i], length);
        //char* curflag = NULL;
        //curflag = ConStr(curlist);
	//printf("!!!curflag is : %s\n",curflag);
	memset(optcmd, 0, sizeof(optcmd));
	strcpy(optcmd, cmd);
        strcat(optcmd, curflag);

        //get value from xml file
        char* merit = "TOTAL_CYCLES";
	if(atoi(readmerit(merit, num-knum, path))==0){
	    system(optcmd);
        }
        char* Value = readmerit(merit, num-knum, path);
	//printf("!!!Value is %s, merit is %s, num is %d\n",Value, merit, num);
        current = atoi(Value);
        if (current < opt) {
	    neg_i = i;
            opt = current;
            //set currently optimal cmd
	    memset(opt_flags, 0, sizeof(opt_flags));
	    strcpy(opt_flags, optcmd);
	   
            opt_list_num = neg[i];
	    printf("\n");
	    printf("current opt value is %d, at num %d, opt_flags: %s, delete element %d\n",opt, num, opt_flags, opt_list_num);
	    
        }
	
        num++;
	//printf("!!!next num is %d, current is %d\n",num,current);
        i++;
    }

    if (opt == tc) {
        return opt;
    }
    else {
        //start from new optimal flaglist (delete one element)
	char cmd1[500] = { 0 }; //optimal cmd
	memset(cmd1, 0, sizeof(cmd1));
	strcpy(cmd1, opt_flags);
	//cmd1 = opt_flags;
	rmv1(flaglist,opt_list_num,len);
	printf("delete element %d, opt value is %d, opti_flags is: %s\n",opt_list_num,opt,opt_flags);
	len--;
	rmvp(neg, neg_i, opt_list_num, count1);
	count1--;
	//printf("!!new cmd is : %s\n",cmd1);
        return opt_tc1(flaglist, path, cmd1, len);
    }


    return opt;
}


//create the negative flags pool for luts
void neg_pool2(const char** flaglist, char* path, char* cmd) {
    //initiate neg list
    int j = 0;
    while(neg[j]!=-1){
	neg[j] = -1;
	j++;
    }
    count1 = 0; //initiate count1
    int tc = opt; //store initial opt for this pass
    int current;
    int i = 0;
    //const char** curlist = NULL;
    char optcmd[500] = { 0 };
    strcpy(optcmd, cmd);
    while(flaglist[i]!=NULL){
        const char* curflag = NULL;
        curflag = flaglist[i];
        //get cmd for current flags
	//printf("!!!curflag is : %s\n",curflag);
	memset(optcmd, 0, sizeof(optcmd));
	strcpy(optcmd, cmd);
        strcat(optcmd, curflag);
	//printf("!!!cmd: %s\n",optcmd);

        //get value from xml file
        char* merit = "SLICE_LUTS";
	if(atoi(readmerit(merit, num-knum, path))==0){
	    system(optcmd);
        }
        char* Value = readmerit(merit, num-knum, path);
	//printf("!!!Value is %s, merit is %s, num is %d\n",Value, merit, num);
        current = atoi(Value);
        if (current < tc) {
	    
	    neg[count1] = i;
	    printf("neg value in %d is %d\n",count1,neg[count1]);
            count1++;
        }

        num++;
	//printf("!!!next num is %d, current is %d\n",num,current);
        i++;
    }

}

//find the optimal value for luts; 
//path is the first element for .xml file; num is the index for xml files
//tc is the value when all flags are turn on; len is the length of flags array 
//cmd is the fixed part of command, changing with benchmark
int opt_tc2(const char** flaglist, char* path, char* cmd, int len) {
    int tc = opt; //store initial opt for this pass
    int current;
    int neg_i = -1;
    int i = 0;
    //const char** curlist = NULL;
    int opt_list_num = 0;
    char optcmd[500] = { 0 };
    strcpy(optcmd, cmd);
    while(neg[i]!=-1){
        //get cmd for current flags
	const char* curflag = NULL;
        curflag = flaglist[neg[i]];
        //curlist = rmv(flaglist, neg[i], length);
        //char* curflag = NULL;
        //curflag = ConStr(curlist);
	//printf("!!!curflag is : %s\n",curflag);
	memset(optcmd, 0, sizeof(optcmd));
	strcpy(optcmd, cmd);
        strcat(optcmd, curflag);

        //get value from xml file
        char* merit = "SLICE_LUTS";
	if(atoi(readmerit(merit, num-knum, path))==0){
	    system(optcmd);
        }
        char* Value = readmerit(merit, num-knum, path);
	//printf("!!!Value is %s, merit is %s, num is %d\n",Value, merit, num);
        current = atoi(Value);
        if (current < opt) {
	    neg_i = i;
            opt = current;
            //set currently optimal cmd
	    memset(opt_flags, 0, sizeof(opt_flags));
	    strcpy(opt_flags, optcmd);
	   
            opt_list_num = neg[i];
	    printf("\n");
	    printf("current opt value is %d, at num %d, opt_flags: %s, delete element %d\n",opt, num, opt_flags, opt_list_num);
	    
        }
	
        num++;
	//printf("!!!next num is %d, current is %d\n",num,current);
        i++;
    }

    if (opt == tc) {
        return opt;
    }
    else {
        //start from new optimal flaglist (delete one element)
	char cmd1[500] = { 0 }; //optimal cmd
	memset(cmd1, 0, sizeof(cmd1));
	strcpy(cmd1, opt_flags);
	//cmd1 = opt_flags;
	rmv1(flaglist,opt_list_num,len);
	printf("delete element %d, opt value is %d, opti_flags is: %s\n",opt_list_num,opt,opt_flags);
	len--;
	rmvp(neg, neg_i, opt_list_num, count1);
	count1--;
	//printf("!!new cmd is : %s\n",cmd1);
        return opt_tc2(flaglist, path, cmd1, len);
    }


    return opt;
}

//create the negative flags pool for frequency
void neg_pool3(const char** flaglist, char* path, char* cmd) {
    //initiate neg list
    int j = 0;
    while(neg[j]!=-1){
	neg[j] = -1;
	j++;
    }
    count1 = 0; //initiate count1
    float tc = opt3; //store initial opt for this pass
    float current;
    int i = 0;
    //const char** curlist = NULL;
    char optcmd[500] = { 0 };
    strcpy(optcmd, cmd);
    while(flaglist[i]!=NULL){
        const char* curflag = NULL;
        curflag = flaglist[i];
        //get cmd for current flags
	//printf("!!!curflag is : %s\n",curflag);
	memset(optcmd, 0, sizeof(optcmd));
	strcpy(optcmd, cmd);
        strcat(optcmd, curflag);
	//printf("!!!cmd: %s\n",optcmd);

        //get value from xml file
        char* merit = "FREQUENCY";
	if(atoi(readmerit(merit, num-knum, path))==0){
	    system(optcmd);
        }
        char* Value = readmerit(merit, num-knum, path);
	//printf("!!!Value is %s, merit is %s, num is %d\n",Value, merit, num);
        current = atof(Value);
        if (current > tc) {
	    
	    neg[count1] = i;
	    printf("neg value in %d is %d\n",count1,neg[count1]);
            count1++;
        }

        num++;
	//printf("!!!next num is %d, current is %d\n",num,current);
        i++;
    }

}

//find the optimal value; 
//path is the first element for .xml file; num is the index for xml files
//tc is the value when all flags are turn on; len is the length of flags array 
//cmd is the fixed part of command, changing with benchmark
float opt_tc3(const char** flaglist, char* path, char* cmd, int len) {
    float tc = opt3; //store initial opt for this pass
    float current;
    int neg_i = -1;
    int i = 0;
    //const char** curlist = NULL;
    int opt_list_num = 0;
    char optcmd[500] = { 0 };
    strcpy(optcmd, cmd);
    while(neg[i]!=-1){
        //get cmd for current flags
	const char* curflag = NULL;
        curflag = flaglist[neg[i]];
        //curlist = rmv(flaglist, neg[i], length);
        //char* curflag = NULL;
        //curflag = ConStr(curlist);
	//printf("!!!curflag is : %s\n",curflag);
	memset(optcmd, 0, sizeof(optcmd));
	strcpy(optcmd, cmd);
        strcat(optcmd, curflag);

        //get value from xml file
        char* merit = "FREQUENCY";
	if(atoi(readmerit(merit, num-knum, path))==0){
	    system(optcmd);
        }
        char* Value = readmerit(merit, num-knum, path);
	//printf("!!!Value is %s, merit is %s, num is %d\n",Value, merit, num);
        current = atof(Value);
        if (current > opt3) {
	    neg_i = i;
            opt3 = current;
            //set currently optimal cmd
	    memset(opt_flags, 0, sizeof(opt_flags));
	    strcpy(opt_flags, optcmd);
	   
            opt_list_num = neg[i];
	    printf("\n");
	    printf("current opt value is %f, at num %d, opt_flags: %s, delete element %d\n",opt3, num, opt_flags, opt_list_num);
	    
        }
	
        num++;
	//printf("!!!next num is %d, current is %d\n",num,current);
        i++;
    }

    if (opt3 == tc) {
        return opt3;
    }
    else {
        //start from new optimal flaglist (delete one element)
	char cmd1[500] = { 0 }; //optimal cmd
	memset(cmd1, 0, sizeof(cmd1));
	strcpy(cmd1, opt_flags);
	//cmd1 = opt_flags;
	rmv1(flaglist,opt_list_num,len);
	printf("delete element %d, opt value is %f, opti_flags is: %s\n",opt_list_num,opt3,opt_flags);
	len--;
	rmvp(neg, neg_i, opt_list_num, count1);
	count1--;
	//printf("!!new cmd is : %s\n",cmd1);
        return opt_tc3(flaglist, path, cmd1, len);
    }


    return opt3;
}


int main() {

    int m; //number represent the selected merit
    printf("Which merit do you want? please input a number\n");
    printf("%d: Minimum clock cycles;\n",1);
    printf("%d: Minimum area;\n",2);
    printf("%d: Maximum frequency\n",3);
    scanf("%d", &m);
    
    if(m==1){
	while(k<N){
	    //for each polybench each merit create a new folder,i.e., for each time running the c program, create a folder or change the index for xml.
	    char* head1 = "cd /home/edana/polybench-syn/";
	    char* head2 = "/tcc";
	    char* head4 = "&&/opt/panda/bin/bambu ..";
	    char* tail = ".c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O3";
	    char snum[10];
	    knum = num;
	    sprintf(snum, "%d", k);
	    const char* cmdarr[] = { head1,foldername[k],head2,snum,head4,examples[k],tail,NULL };
	    char* fixcmd = ConStr(cmdarr);

	    char cmd[500];
	    strcpy(cmd, fixcmd);
	    //char* path = "/home/edana/polybench-syn/stencils/luts3/bambu_results_";
	    char* path1 = "/home/edana/polybench-syn/";
	    char* path2 = "/tcc";
	    char* path3 = "/bambu_results_";
	    const char* patharr[] = { path1,foldername[k],path2,snum,path3,NULL };
	    char* path = ConStr(patharr);

	    const char* array[] = { " -fno-guess-branch-probability"," -fno-tree-ccp"," -fno-tree-ch"," -fno-tree-dominator-opts"," -fno-tree-forwprop"," -fno-tree-fre"," -fno-tree-slsr"," -fno-inline-small-functions"," -fno-ipa-cp"," -fno-code-hoisting"," -fno-tree-pre"," -fno-tree-vrp"," -fno-predictive-commoning"," -fno-tree-loop-vectorize"," -fno-tree-loop-distribution"," -fno-tree-loop-distribute-patterns"," -fno-peel-loops"," -fno-ipa-cp-clone"," -fno-expensive-optimizations"," -fno-ipa-cp"," -fno-inline-functions"," -fno-loop-interchange"," -fno-loop-unroll-and-jam"," -fno-tree-partial-pre"," -fno-partial-inlining"," -fno-unswitch-loops",NULL };
	    //-fipa-cp-clone -fexpensive-optimizations -finline-functions -floop-interchange -ftree-partial-pre
	    int len = length;
	    
	    FILE *fpWrite=fopen("/home/edana/polybench-syn/results/tc6.txt","a");
	    if(fpWrite==NULL){
		    return 0;
	    }
	    fprintf(fpWrite,"%s\n",examples[k]);
	    time_t now1,now2,h,m,s,t;

	    //calculate execution time
	    time(&now1); 
	    
	    char* merit = "TOTAL_CYCLES";
	    if(atoi(readmerit(merit, num-knum, path))==0){
		system(cmd);
	    }
	    char* Value = readmerit(merit, num-knum, path);
	    num++;


	    printf("Initial Total_cycles = %s\n", Value);
	    fprintf(fpWrite,"Initial Total_cycles = %s\n", Value);
	    int tc = atoi(Value);
	    opt = tc;
	    neg_pool1(array, path, fixcmd);
	    
	    for(int k=0;k<length;k++){
		fprintf(fpWrite,"%d\n",neg[k]);
	    }
	    
	    opt = tc;
	    opt = opt_tc1(array, path, fixcmd, len);
	    time(&now2); 
		
	    printf("The optimal Total_cycles = %d\n", opt);
	    printf("The optimal combination of flags: %s\n", opt_flags);
	    fprintf(fpWrite,"The optimal Total_cycles = %d\n", opt);
	    fprintf(fpWrite,"The optimal combination of flags: %s\n", opt_flags);
	  
	    t=now2-now1;  
	    h=t/3600;
	    t%=3600;    
	    m=t/60; 
	    s=t%60;
	    printf("Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	    fprintf(fpWrite,"Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	    fclose(fpWrite);
	    k++;
	}
    }

    if(m==2){
	while(k<N){
	    //for each polybench each merit create a new folder,i.e., for each time running the c program, create a folder or change the index for xml.
	    char* head1 = "cd /home/edana/polybench-syn/";
	    char* head2 = "/lutt";
	    char* head4 = "&&/opt/panda/bin/bambu ..";
	    char* tail = ".c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O3";
	    char snum[10];
	    knum = num;
	    sprintf(snum, "%d", k);
	    const char* cmdarr[] = { head1,foldername[k],head2,snum,head4,examples[k],tail,NULL };
	    char* fixcmd = ConStr(cmdarr);

	    char cmd[500];
	    strcpy(cmd, fixcmd);
	    //char* path = "/home/edana/polybench-syn/stencils/luts3/bambu_results_";
	    char* path1 = "/home/edana/polybench-syn/";
	    char* path2 = "/lutt";
	    char* path3 = "/bambu_results_";
	    const char* patharr[] = { path1,foldername[k],path2,snum,path3,NULL };
	    char* path = ConStr(patharr);

	    const char* array[] = { " -fno-guess-branch-probability"," -fno-tree-ccp"," -fno-tree-ch"," -fno-tree-dominator-opts"," -fno-tree-forwprop"," -fno-tree-fre"," -fno-tree-slsr"," -fno-inline-small-functions"," -fno-ipa-cp"," -fno-code-hoisting"," -fno-tree-pre"," -fno-tree-vrp"," -fno-predictive-commoning"," -fno-tree-loop-vectorize"," -fno-tree-loop-distribution"," -fno-tree-loop-distribute-patterns"," -fno-peel-loops"," -fno-ipa-cp-clone"," -fno-expensive-optimizations"," -fno-ipa-cp"," -fno-inline-functions"," -fno-loop-interchange"," -fno-loop-unroll-and-jam"," -fno-tree-partial-pre"," -fno-partial-inlining"," -fno-unswitch-loops",NULL };
	    //-fipa-cp-clone -fexpensive-optimizations -finline-functions
	    int len = length;
	    
	    FILE *fpWrite=fopen("/home/edana/polybench-syn/results/luts61.txt","a");
	    if(fpWrite==NULL){
		    return 0;
	    }
	    fprintf(fpWrite,"%s\n",examples[k]);
	    time_t now1,now2,h,m,s,t;

	    //calculate execution time
	    time(&now1); 
	    
	    char* merit = "SLICE_LUTS";
	    if(atoi(readmerit(merit, num-knum, path))==0){
		system(cmd);
	    }
	    char* Value = readmerit(merit, num-knum, path);
	    num++;

	    printf("Initial Luts = %s\n", Value);
	    fprintf(fpWrite,"Initial Luts = %s\n", Value);
	    int tc = atoi(Value);
	    opt = tc;
	    neg_pool2(array, path, fixcmd);
	    
	    for(int k=0;k<length;k++){
		fprintf(fpWrite,"%d\n",neg[k]);
	    }
	    
	    opt = tc;
	    opt = opt_tc2(array, path, fixcmd, len);
	    time(&now2); 
		
	    printf("The optimal Luts = %d\n", opt);
	    printf("The optimal combination of flags: %s\n", opt_flags);
	    fprintf(fpWrite,"The optimal Luts = %d\n", opt);
	    fprintf(fpWrite,"The optimal combination of flags: %s\n", opt_flags);
	  
	    t=now2-now1;  
	    h=t/3600;
	    t%=3600;    
	    m=t/60; 
	    s=t%60;
	    printf("Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);

	    
	    fprintf(fpWrite,"Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	    fclose(fpWrite);
	    k++;
	}
    }

    if(m==3){
	while(k<N){
	    //for each polybench each merit create a new folder,i.e., for each time running the c program, create a folder or change the index for xml.
	    char* head1 = "cd /home/edana/polybench-syn/";
	    char* head2 = "/fr";
	    char* head4 = "&&/opt/panda/bin/bambu ..";
	    char* tail = ".c --simulate --simulator=VERILATOR --accept-nonzero-return --evaluation --compiler=I386_GCC8 -O3";
	    char snum[10];
	    knum = num;
	    sprintf(snum, "%d", k);
	    const char* cmdarr[] = { head1,foldername[k],head2,snum,head4,examples[k],tail,NULL };
	    char* fixcmd = ConStr(cmdarr);

	    char cmd[500];
	    strcpy(cmd, fixcmd);
	    //char* path = "/home/edana/polybench-syn/stencils/luts3/bambu_results_";
	    char* path1 = "/home/edana/polybench-syn/";
	    char* path2 = "/fr";
	    char* path3 = "/bambu_results_";
	    const char* patharr[] = { path1,foldername[k],path2,snum,path3,NULL };
	    char* path = ConStr(patharr);

	    const char* array[] = { " -fno-guess-branch-probability"," -fno-tree-ccp"," -fno-tree-ch"," -fno-tree-dominator-opts"," -fno-tree-forwprop"," -fno-tree-fre"," -fno-tree-slsr"," -fno-inline-small-functions"," -fno-ipa-cp"," -fno-code-hoisting"," -fno-tree-pre"," -fno-tree-vrp"," -fno-predictive-commoning"," -fno-tree-loop-vectorize"," -fno-tree-loop-distribution"," -fno-tree-loop-distribute-patterns"," -fno-peel-loops"," -fno-ipa-cp-clone"," -fno-expensive-optimizations"," -fno-ipa-cp"," -fno-inline-functions"," -fno-loop-interchange"," -fno-loop-unroll-and-jam"," -fno-tree-partial-pre"," -fno-partial-inlining"," -fno-unswitch-loops",NULL };
	    //-fipa-cp-clone -fexpensive-optimizations -finline-functions
	    int len = length;
	    
	    FILE *fpWrite=fopen("/home/edana/polybench-syn/results/freq6.txt","a");
	    if(fpWrite==NULL){
		    return 0;
	    }
	    fprintf(fpWrite,"%s\n",examples[k]);
	    time_t now1,now2,h,m,s,t;

	    //calculate execution time
	    time(&now1); 
	    
	    char* merit = "FREQUENCY";
	    if(atoi(readmerit(merit, num-knum, path))==0){
		system(cmd);
	    }
	    char* Value = readmerit(merit, num-knum, path);
	    num++;


	    printf("Initial Frequency = %s\n", Value);
	    fprintf(fpWrite,"Initial Frequency = %s\n", Value);
	    float tc = atof(Value);
	    opt3 = tc;
	    neg_pool3(array, path, fixcmd);
	    
	    for(int k=0;k<length;k++){
		fprintf(fpWrite,"%d\n",neg[k]);
	    }
	    
	    opt3 = tc;
	    opt3 = opt_tc3(array, path, fixcmd, len);
	    time(&now2); 
		
	    printf("The optimal Frequency = %f\n", opt3);
	    printf("The optimal combination of flags: %s\n", opt_flags);
	    fprintf(fpWrite,"The optimal Frequency = %f\n", opt3);
	    fprintf(fpWrite,"The optimal combination of flags: %s\n", opt_flags);
	  
	    t=now2-now1;  
	    h=t/3600;
	    t%=3600;    
	    m=t/60; 
	    s=t%60;
	    printf("Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	    fprintf(fpWrite,"Execution time: %ld hour  %ld mintues  %ld seconds\n",h,m,s);
	    fclose(fpWrite);
	    k++;
	}
    }


    return 0;
}
