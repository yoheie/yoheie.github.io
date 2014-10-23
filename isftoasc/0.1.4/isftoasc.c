/*********************************************************************
* isftoasc                                                           *
*   Convert Tektronix ISF Oscilloscope Data to ASCII Data            *
*                                                                    *
*    !!!!! NO WARRANTY !!!!!  USE AT YOUR OWN RISK.                  *
*     Do not contact or ask Tektronix, Inc. about this program.      *
*                                                                    *
* 2011-07-12  Ver. 0.1.4               Yohei Endo <yoheie@gmail.com> *
*********************************************************************/
#include<stdio.h>
#include<string.h>
#include<stdlib.h>

#define VERSION "0.1.4"

#define MAXHEADERSIZE 511

typedef struct IsfHeader{
	int byt_nr;
	int bit_nr;
	char *encdg;
	char *bn_fmt;
	char *byt_or;
	long nr_pt;
	char *wfid;
	char *pt_fmt;
	double xincr;
	long pt_off;
	double xzero;
	char *xunit;
	double ymult;
	double yzero;
	double yoff;
	char *yunit;
}IsfHeader;

IsfHeader isfheader;

int initisfheader(char *header_string);
int checkisfheader(void);
int read_curve_bin(FILE *isffile);

int main(int argc, char *argv[])
{
	FILE *isffile;
	int i=0;
	int c;
	char header_string[MAXHEADERSIZE+1];
	char check_str[14];

	if(argc!=2){
		fprintf(stderr," Usage : isftoasc filename\n");
		return 0;
	}

	if((isffile=fopen(argv[1],"rb"))==NULL){
		fprintf(stderr," Error : Can't Open file %s\n",argv[1]);
		return 1;
	}

	printf("# Converted by isftoasc Ver. %s\n",VERSION);

	/***** Check if the File is ISF format *****/
	fgets(check_str,9,isffile);
	if((strcmp(check_str,":WFMPRE:"))!=0){
		fprintf(stderr," Error : File %s seems not ISF file\n",argv[1]);
		fclose(isffile);
		return 1;
	}

	/***** Read Header to header_string[] *****/
	while((c=fgetc(isffile))!=EOF && c!=':'){
		header_string[i]=c;
		i++;
		if(i==MAXHEADERSIZE){
			fprintf(stderr," Error : Too larege Header\n");
			fclose(isffile);
			return 1;
		}
	}
	header_string[i]=0;

	/***** Init and Check Header *****/
	if((initisfheader(header_string))!=0 || (checkisfheader())!=0){
		fclose(isffile);
		return 1;
	}

	/***** Read, Calc Data and Print *****/
	fgets(check_str,7,isffile);
	if((strcmp(check_str,"CURVE "))!=0){
		fprintf(stderr," Error : CURVE read error\n");
		fclose(isffile);
		return 1;
	}
	if(strcmp(isfheader.encdg,"BIN")==0){
		if((read_curve_bin(isffile))!=0){
			fprintf(stderr," Error : CURVE read error\n");
			fclose(isffile);
			return 1;
		}
	}
	else if(strcmp(isfheader.encdg,"ASC")==0){
		fprintf(stderr," Sorry, ENCDG = ASC not supported\n");
		fclose(isffile);
		return 1;
	}

	/***** Check if isffile is finished *****/
	if((c=fgetc(isffile))!=EOF){
		fprintf(stderr," Error : The file Contains more data\n");
		fclose(isffile);
		return 1;
	}

	fclose(isffile);
	return 0;
}


/****************************************
* initisfheader                         *
*   Enter Header Value to isfheader.    *
****************************************/

int initisfheader(char *header_string)
{
	char *header_name,*header_value;

	if((header_name=strtok(header_string," "))==NULL){
		fprintf(stderr,"Error : No headers found\n");
		return 1;
	}
	do{
		header_value=strtok(NULL,";");
		if(strcmp(header_name,"BYT_NR")==0){
			isfheader.byt_nr=atoi(header_value);
		}
		else if(strcmp(header_name,"BIT_NR")==0){
			isfheader.bit_nr=atoi(header_value);
		}
		else if(strcmp(header_name,"ENCDG")==0){
			isfheader.encdg=header_value;
		}
		else if(strcmp(header_name,"BN_FMT")==0){
			isfheader.bn_fmt=header_value;
		}
		else if(strcmp(header_name,"BYT_OR")==0){
			isfheader.byt_or=header_value;
		}
		else if(strcmp(header_name,"NR_PT")==0){
			isfheader.nr_pt=atol(header_value);
		}
		else if(strcmp(header_name,"WFID")==0){
			isfheader.wfid=header_value;
		}
		else if(strcmp(header_name,"PT_FMT")==0){
			isfheader.pt_fmt=header_value;
		}
		else if(strcmp(header_name,"XINCR")==0){
			isfheader.xincr=atof(header_value);
		}
		else if(strcmp(header_name,"PT_OFF")==0){
			isfheader.pt_off=atol(header_value);
		}
		else if(strcmp(header_name,"XZERO")==0){
			isfheader.xzero=atof(header_value);
		}
		else if(strcmp(header_name,"XUNIT")==0){
			isfheader.xunit=header_value;
		}
		else if(strcmp(header_name,"YMULT")==0){
			isfheader.ymult=atof(header_value);
		}
		else if(strcmp(header_name,"YZERO")==0){
			isfheader.yzero=atof(header_value);
		}
		else if(strcmp(header_name,"YOFF")==0){
			isfheader.yoff=atof(header_value);
		}
		else if(strcmp(header_name,"YUNIT")==0){
			isfheader.yunit=header_value;
		}
		else{
			fprintf(stderr," Error : Unsupported Header %s\n",header_name);
			return 1;
		}
		printf("# %s %s\n",header_name,header_value);
	}while((header_name=strtok(NULL," "))!=NULL);

	return 0;
}


/****************************************
* checkisfheader                        *
*   Check if Header Value is Correct.   *
****************************************/

int checkisfheader(void)
{
	/********** BYT_NR : 1 or 2 **********/
	if((isfheader.byt_nr!=1)&&(isfheader.byt_nr!=2)){
		fprintf(stderr," Error : Wrong BYT_NR (%d)\n",isfheader.byt_nr);
		return 1;
	}
	/********** BIT_NR : 8 or 16 **********/
	if((isfheader.bit_nr!=8)&&(isfheader.bit_nr!=16)){
		fprintf(stderr," Error : Wrong BIT_NR (%d)\n",isfheader.bit_nr);
		return 1;
	}
	/********** ENCDG : ASC or BIN **********/
	if(((strcmp(isfheader.encdg,"ASC"))!=0)&&((strcmp(isfheader.encdg,"BIN"))!=0)){
		fprintf(stderr," Error : Wrong ENCDG (%s)\n",isfheader.encdg);
		return 1;
	}
	/********** BN_FMT : RI or RP **********/
	if(((strcmp(isfheader.bn_fmt,"RI"))!=0)&&((strcmp(isfheader.bn_fmt,"RP"))!=0)){
		fprintf(stderr," Error : Wrong BN_FMT (%s)\n",isfheader.bn_fmt);
		return 1;
	}
	/********** BYT_OR : LSB or MSB **********/
	if(((strcmp(isfheader.byt_or,"LSB"))!=0)&&((strcmp(isfheader.byt_or,"MSB"))!=0)){
		fprintf(stderr," Error : Wrong BYT_OR (%s)\n",isfheader.byt_or);
		return 1;
	}
	/********** PT_FMT : ENV or Y **********/
	/* if(((strcmp(isfheader.pt_fmt,"ENV"))!=0)&&((strcmp(isfheader.pt_fmt,"Y"))!=0)){ */
	if((strcmp(isfheader.pt_fmt,"Y"))!=0){
		fprintf(stderr," Error : Sorry, PT_FMT = %s not supported\n",isfheader.pt_fmt);
		return 1;
	}

	return 0;
}


/****************************************
* read_curve_bin                        *
*   Read, Calc BINARY data and Print.   *
****************************************/

int read_curve_bin(FILE *isffile)
{
	int i,j;
	long point;
	unsigned char tmp_point[2]="";
	double x,y;
	char tmp_check[10];

	fgets(tmp_check,2,isffile);
	if((strcmp(tmp_check,"#"))!=0){
		return 1;
	}
	fgets(tmp_check,2,isffile);
	i=atoi(tmp_check);
	fgets(tmp_check,i+1,isffile);
	if(atol(tmp_check)!=isfheader.byt_nr*isfheader.nr_pt){
		fprintf(stderr," Error : Num of Bytes in CURVE != BYT_NR x NR_PT\n");
		return 1;
	}

	for(i=0;i<isfheader.nr_pt;i++){
		for(j=0;j<isfheader.byt_nr;j++){
			if(fread(&tmp_point[j],1,1,isffile)!=1){
				fprintf(stderr," Error : The file contains too few data\n");
				return 1;
			}
		}
		if((isfheader.byt_nr==2) && ((strcmp(isfheader.byt_or,"MSB"))==0)){
			point=tmp_point[0]*256+tmp_point[1];
		}
		else{
			point=tmp_point[1]*256+tmp_point[0];
		}
		if((strcmp(isfheader.bn_fmt,"RI")==0) && (point > 32767)){
			point = point - 65536;
		}
		y=isfheader.yzero+isfheader.ymult*(point-isfheader.yoff);
		x=isfheader.xincr*i;
		/* x=isfheader.xzero+isfheader.xincr*(i-isfheader.pt_off); */
		printf("%E %E\n",x,y);
	}

	fgets(tmp_check,2,isffile);
	if((strcmp(tmp_check,"\n"))!=0){
		return 1;
	}

	return 0;
}
