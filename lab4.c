  /*
 * commandline.c
 *
 *  Created on: Feb 15, 2017
 *      Author: panth
 */
#include <stdio.h>
#include <stdlib.h>

int max(int* filedata);
double average(int* filedata);
void scale(int* filedata,double scale,int filenumber);
void offset(int* filedata,double offset,int filenumber);
int* readfile(int filenumber);
void help(void);
int refile(int* filedata, char* filename);

void statistics(int* filedata);
void centered(int* filedata);
void normal(int* filedata);

int main(int argc,char** argv){

	int counter=1,data =-1,stat =0,center =0,norm = 0;
	double* offsets=NULL;
	double offnum=0;
	double scaled;
	char* filename=NULL;

	if(argc == 1){
		help();
		return 0;
	}

	while(counter < argc)
	{

		if(argv[counter][0]== '-')
		{

			if(argv[counter][2] == '\0')
			{

				switch(argv[counter][1])
				{

					case 'n':
					{
						counter++;
						data = (int) atoi(argv[counter]);
						if(argc <= counter)
						{
							printf("Improper format for file number\n");
							return 0;
						}
						if(data <= 0 || data >99)
						{

							printf("Improper value for file number. Must be between 1-99\n");
							return 0;
						}

						break;
					}

					case 'o':
					{

						counter++;
						if(argc <= counter)
						{
							printf("Improper format for offset. Must include a number of type double following -o\n");
							return 0;
						}

						char* convertdouble = NULL;
						offsets = (double*) malloc(sizeof(double));
						*offsets = strtod(argv[counter],&convertdouble);

						if(*convertdouble != '\0')
						{
							printf("Improper use of offset. The option must be followed by a double value\n");
							free(convertdouble);
							free(offsets);
							return 0;
						}
						break;
					}

					case 'r':
					{
						counter++;
						if(argc<= counter)
						{
							printf("Improper use of renaming file. The file name must follow [-r]");
							return 0;
						}
						filename = argv[counter];
						break;
					}

					case 's':
					{

						counter++;
						if(argc<= counter)
						{

							printf("Improper use of scale. A type double greater than zero must follow [-s]\n");
							return 0;
						}
						char* convertdouble = NULL;
						scaled = strtod(argv[counter],&convertdouble);
						if(*convertdouble != '\0' || scaled == 0)
						{

							printf("Improper use of scale. Type double following [-s] must be greater than zero\n");
							free(convertdouble);
							return 0;
						}

						break;
					}

					case 'h':
					{
						help();
						break;
					}

					case 'S':
					{
						counter++;
						if(argc > counter)
						{

							printf("***************ERROR*****************\n statistic does not require a number following the [-S] command\n");
							return 0;
						}

						stat = 1;
						break;
					}

					case 'C':
					{
						counter++;
						if(argc > counter)
						{
							printf("*****************ERROR****************\n Center does not require a number following the [-C] command\n");
							return 0;
						}
						center = 1;
						break;
					}

					case 'N':
					{
						counter++;
						if(argc > counter)
						{
							printf("********************ERROR*****************\n Normalize does not require a number following the [-N] option");
							return 0;
						}

						norm = 1;
						break;
					}

					default:
					{
						printf("Improper selection. Please use [-h] if help is needed on formatting\n");
						return 0;
						break;
					}
				}

			}
			else
			{
				printf("*************************ERROR*****************\nYou entered an invalid option\n");
				return 0;
			}

		}
		else
		{
			printf("********************ERROR**********************\n");
		}

		counter++;
	}

	int* filedata = readfile(data);

	if(filedata == NULL)
	{

		printf("Error");
		return 0;
	}

	if(offsets != NULL)
	{

		offset(filedata,*offsets,data);
		free(offsets);
	}

	if(filename != NULL)
	{
		if(refile(filedata,filename)!= 0)
		{
			printf("**************ERROR**************\n [-r] did not work properly \n");
			return 0;
		}
	}

	if(scaled != 0)
	{

		scale(filedata,scaled,data);

	}

	if(stat ==1)
	{

		statistics(filedata);
	}

	if(center ==1)
	{
		centered(filedata);
	}

	if(norm ==1 )
	{
		normal(filedata);
	}

	free(filedata);
	return 0;
}

void help(void){

	printf("To Run This Program You Must Use The Following Format\n\t ./a.exe <options> <value>\nOptions:\n\t-n:\tFile Number (value needed)\n\t-o:\tOffset Number (value needed)\n\t-s:\tScaling Factor (value needed)\n\t-r:\tRename File (name needed)\n\t-h:\tHelp Menu (Display the format of command line arguments)\n\n");
	return;
}

int* readfile(int filenumber){

	char* name = (char*) malloc(sizeof(char)*15);

	if(sprintf(name,"Raw_data_%02d.txt",filenumber)!=15){
		free(name);
		return NULL;
	}

	FILE *fp = fopen(name,"r");
	if(fp == NULL){

		free(name);
		fclose(fp);
		return NULL;
	}
	int s = -1;
	fscanf(fp,"%d",s);
	if(s == 0){

		printf("****************ERROR******************\nEither your file contains zero integers or fscanf() did not work properly");
		free(name);
		fclose(fp);
		return NULL;
	}
         int* array = malloc(sizeof(int)*(s+2));
         array[0] = s;
	for(int i = 2;i<array[0]+2;i++){
		fscanf(fp,"%d\n",(array+i));
	}

	free(name);
	fclose(fp);
	return array;
}

void scale(int* filedata,double scale,int filenumber){

	char* outfile = (char*) malloc(sizeof(char)*18);

	if(sprintf(outfile,"Scaled_data_%02d.txt",filenumber)!=18){
		free(outfile);
		return;
	}
	FILE *fp = fopen(outfile,"w");

	if(fp == NULL){

		printf("***********ERROR**************\n file was not created\n");
		fclose(fp);
		free(outfile);
		return;
	}

	fprintf(fp,"%d %f\n",*filedata,(double) *(filedata+1) * scale);

	for(int i = 2;i<filedata[0]+2;i++){
		fprintf(fp,"%f\n",(double) *(filedata+i) * scale);
	}

	fclose(fp);
	free(outfile);
	return;
}

void offset(int* filedata,double offset,int filenumber){
	char* outfile = (char*) malloc(sizeof(char)*18);

		if(sprintf(outfile,"Offset_data_%02d.txt",filenumber)!=18){
			free(outfile);
			return;
		}
		FILE *fp = fopen(outfile,"w");

		if(fp == NULL){

			printf("***********EEROR**************\nfile was not created\n");
			fclose(fp);
			free(outfile);
			return;
		}

		fprintf(fp,"%d %f\n",*filedata,*(filedata + 1) + offset);

		for(int i = 2;i<filedata[0]+2;i++){

			fprintf(fp,"%f\n",*(filedata +i)+offset);
		}

		fclose(fp);
		free(outfile);
		return;

}

int refile(int* filedata, char* filename){

	FILE *fp = fopen(filename,"w");

	if(fp == NULL){

		printf("***********ERROR************\n file could not be created\n");
		fclose(fp);
	}

	fprintf(fp,"%d %d",filedata[0],filedata[1]);

	for(int i = 2;i < filedata[0]+2;i++){

		fprintf(fp,"%d\n",*(filedata + i));
	}
	fclose(fp);
	return 0;
}

double average(int* filedata){
	double sum,average;

	for(int i = 2;i<filedata[0]+2;i++){
		sum =+ *(filedata + i);

	}

	average = sum/filedata[0];
	return average;
}

int max(int* filedata){

	int max = filedata[2];

	for(int i = 2;i<filedata[0]+2;i++){

		if( *(filedata+i)>max){
			max = *(filedata+i);
		}
	}

	return max;
}

void statistics(int* filedata)
{

	double ave = average(filedata);
	int m = max(filedata);

	char* outfile = (char*) malloc(sizeof(char)*22);

	if(sprintf(outfile,"Statistics_data_nn.txt")!=22)
	{
		free(outfile);
		return;
	}
	FILE *fp = fopen(outfile,"w");

	if(fp == NULL)
	{

		printf("***********ERROR**************\n file was not created\n");
		fclose(fp);
		free(outfile);
		return;
	}

	fprintf(fp,"%f %d",ave,m);

	fclose(fp);
	free(outfile);
	return;
}

void centered(int* filedata)
{
	double ave = average(filedata);

	char* outfile = (char*) malloc(sizeof(char)*20);

	if(sprintf(outfile,"Centered_data_nn.txt")!=20)
	{
		free(outfile);
		return;
	}
	FILE *fp = fopen(outfile,"w");

	if(fp == NULL)
	{

		printf("***********ERROR**************\n file was not created\n");
		fclose(fp);
		free(outfile);
		return;
	}

	for(int i = 2;i<filedata[0]+2;i++)
	{
		fprintf(fp, "%f\n",*(filedata+i)-ave);
	}
	return;
}

void normal(int* filedata)
{
	int m = max(filedata);

	char* outfile = (char*) malloc(sizeof(char)*22);

	if(sprintf(outfile,"Normalized_data_nn.txt")!=22)
	{
		free(outfile);
		return;
	}
	FILE *fp = fopen(outfile,"w");

	if(fp == NULL)
	{

		printf("***********ERROR**************\n file was not created\n");
		fclose(fp);
		free(outfile);
		return;
	}
	fprintf(fp,"%d %f\n",filedata[0],(double) filedata[1]/m);
	for(int i = 2;i<filedata[0]+2;i++)
	{
		fprintf(fp, "%f\n",(double)*(filedata+i)/m);
	}

}
