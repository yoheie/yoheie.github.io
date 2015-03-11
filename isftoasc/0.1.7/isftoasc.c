/*********************************************************************
* isftoasc                                                           *
*   Convert Tektronix ISF Oscilloscope Data to ASCII Data            *
*                                                                    *
*    !!!!! NO WARRANTY !!!!!  USE AT YOUR OWN RISK.                  *
*     Do not contact or ask Tektronix, Inc. about this program.      *
*                                                                    *
* 2014-01-31  Ver. 0.1.7               Yohei Endo <yoheie@gmail.com> *
*********************************************************************/
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#ifdef _WIN32
#include <fcntl.h>
#include <io.h>
#endif

#define ISFTOASC_VERSION "0.1.7"

#define ISF_HEADER_SIZE_MAX 511

typedef struct {
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
} isf_header_t;

static isf_header_t isf_header = {
	0,
	0,
	"",
	"",
	"",
	0,
	"",
	"",
	0.0,
	0,
	0.0,
	"",
	0.0,
	0.0,
	0.0,
	""
};

static char isf_header_str[ISF_HEADER_SIZE_MAX + 1];

int init_isf_header(void);
int check_isf_header(void);
int read_curve_bin(FILE *isffile);
int read_curve_asc(FILE *isffile);

int main(int argc, char *argv[])
{
	FILE *isffile;
	int i;
	int c;
	char check_str[14];

	if ((argc != 1) && (argc != 2)) {
		fprintf(stderr, " Usage : isftoasc [filename]\n");
		return 0;
	}

	if (argc == 1) {
#ifdef _WIN32
		_setmode(_fileno(stdin), _O_BINARY);
#endif
		isffile = stdin;
	}
	else {
		if ((isffile = fopen(argv[1], "rb")) == NULL) {
			fprintf(stderr, " Error : Can't Open file %s\n", argv[1]);
			return 1;
		}
	}

	printf("# Converted by isftoasc Ver. " ISFTOASC_VERSION "\n");

	/***** Check if the File is ISF format *****/
	fgets(check_str, 9, isffile);
	if ((strcmp(check_str, ":WFMPRE:")) != 0) {
		fprintf(stderr, " Error : File %s seems not ISF file\n", (argc == 1) ? "(stdin)" : argv[1]);
		fclose(isffile);
		return 1;
	}

	/***** Read Header to isf_header_str[] *****/
	i = 0;
	while (((c = fgetc(isffile)) != EOF) && (c != ':')) {
		if (i >= ISF_HEADER_SIZE_MAX) {
			fprintf(stderr, " Error : Too larege Header\n");
			fclose(isffile);
			return 1;
		}
		isf_header_str[i] = c;
		i++;
	}
	isf_header_str[i] = '\0';

	/***** Init and Check Header *****/
	if ((init_isf_header() != 0) || (check_isf_header() != 0)) {
		fclose(isffile);
		return 1;
	}

	/***** Read, Calc Data and Print *****/
	fgets(check_str, 7, isffile);
	if (strcmp(check_str, "CURVE ") != 0) {
		fprintf(stderr, " Error : CURVE read error\n");
		fclose(isffile);
		return 1;
	}
	if (strcmp(isf_header.encdg, "BIN") == 0) {
		if (read_curve_bin(isffile) != 0) {
			fprintf(stderr, " Error : CURVE read error\n");
			fclose(isffile);
			return 1;
		}
	}
	else {
		if (read_curve_asc(isffile) != 0) {
			fprintf(stderr, " Error : CURVE read error\n");
			fclose(isffile);
			return 1;
		}
	}

	/***** Check if isffile is finished *****/
	if ((c = fgetc(isffile)) != EOF) {
		fprintf(stderr, " Error : The file Contains more data\n");
		fclose(isffile);
		return 1;
	}

	fclose(isffile);
	return 0;
}


/****************************************
* init_isf_header                       *
*   Enter Header Value to isf_header.   *
****************************************/

int init_isf_header(void)
{
	char *header_name;
	char *header_value;

	if ((header_name = strtok(isf_header_str, " ")) == NULL) {
		fprintf(stderr, "Error : No headers found\n");
		return 1;
	}
	do {
		header_value = strtok(NULL, ";");
		if (strcmp(header_name, "BYT_NR") == 0) {
			isf_header.byt_nr = atoi(header_value);
		}
		else if (strcmp(header_name, "BIT_NR") == 0) {
			isf_header.bit_nr = atoi(header_value);
		}
		else if (strcmp(header_name, "ENCDG") == 0) {
			isf_header.encdg = header_value;
		}
		else if (strcmp(header_name, "BN_FMT") == 0) {
			isf_header.bn_fmt = header_value;
		}
		else if (strcmp(header_name, "BYT_OR") == 0) {
			isf_header.byt_or = header_value;
		}
		else if (strcmp(header_name, "NR_PT") == 0) {
			isf_header.nr_pt = atol(header_value);
		}
		else if (strcmp(header_name, "WFID") == 0) {
			isf_header.wfid = header_value;
		}
		else if (strcmp(header_name, "PT_FMT") == 0) {
			isf_header.pt_fmt = header_value;
		}
		else if (strcmp(header_name, "XINCR") == 0) {
			isf_header.xincr = atof(header_value);
		}
		else if (strcmp(header_name, "PT_OFF") == 0) {
			isf_header.pt_off = atol(header_value);
		}
		else if (strcmp(header_name, "XZERO") == 0) {
			isf_header.xzero = atof(header_value);
		}
		else if (strcmp(header_name, "XUNIT") == 0) {
			isf_header.xunit = header_value;
		}
		else if (strcmp(header_name, "YMULT") == 0) {
			isf_header.ymult = atof(header_value);
		}
		else if (strcmp(header_name, "YZERO") == 0) {
			isf_header.yzero = atof(header_value);
		}
		else if (strcmp(header_name, "YOFF") == 0) {
			isf_header.yoff = atof(header_value);
		}
		else if (strcmp(header_name, "YUNIT") == 0) {
			isf_header.yunit = header_value;
		}
		else {
			fprintf(stderr, " Error : Unsupported Header %s\n", header_name);
			return 1;
		}
		printf("# %s %s\n", header_name, header_value);
	} while ((header_name = strtok(NULL, " ")) != NULL);

	return 0;
}


/****************************************
* check_isf_header                      *
*   Check if Header Value is Correct.   *
****************************************/

int check_isf_header(void)
{
	/********** BYT_NR : 1 or 2 **********/
	if ((isf_header.byt_nr != 1) && (isf_header.byt_nr != 2)) {
		fprintf(stderr, " Error : Wrong BYT_NR (%d)\n", isf_header.byt_nr);
		return 1;
	}
	/********** BIT_NR : 8 or 16 **********/
	if ((isf_header.bit_nr != 8) && (isf_header.bit_nr != 16)) {
		fprintf(stderr, " Error : Wrong BIT_NR (%d)\n", isf_header.bit_nr);
		return 1;
	}
	/********** ENCDG : ASC or BIN **********/
	if ((strcmp(isf_header.encdg,"ASC") != 0) && (strcmp(isf_header.encdg,"BIN") != 0)) {
		fprintf(stderr, " Error : Wrong ENCDG (%s)\n", isf_header.encdg);
		return 1;
	}
	/********** BN_FMT : RI or RP **********/
	if ((strcmp(isf_header.bn_fmt,"RI") != 0) && (strcmp(isf_header.bn_fmt,"RP") != 0)) {
		fprintf(stderr, " Error : Wrong BN_FMT (%s)\n", isf_header.bn_fmt);
		return 1;
	}
	/********** BYT_OR : LSB or MSB **********/
	if ((strcmp(isf_header.byt_or,"LSB") != 0) && (strcmp(isf_header.byt_or,"MSB") != 0)) {
		fprintf(stderr, " Error : Wrong BYT_OR (%s)\n", isf_header.byt_or);
		return 1;
	}
	/********** PT_FMT : ENV or Y **********/
	if ((strcmp(isf_header.pt_fmt,"ENV") != 0) && (strcmp(isf_header.pt_fmt,"Y") != 0)) {
		fprintf(stderr, " Error : Wrong PT_FMT (%s)\n", isf_header.pt_fmt);
		return 1;
	}
	else if (strcmp(isf_header.pt_fmt,"ENV") == 0) {
		fprintf(stderr, " Error : Sorry, PT_FMT = ENV not supported\n");
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
	int i;
	long point;
	unsigned char tmp_point[2] = {0, 0};
	double x;
	double y;
	char tmp_check[10];

	fgets(tmp_check, 2, isffile);
	if (strcmp(tmp_check,"#") != 0) {
		return 1;
	}
	fgets(tmp_check, 2, isffile);
	i = atoi(tmp_check);
	fgets(tmp_check, i + 1, isffile);
	if (atol(tmp_check) != (isf_header.byt_nr * isf_header.nr_pt)) {
		fprintf(stderr, " Error : Num of Bytes in CURVE != BYT_NR x NR_PT\n");
		return 1;
	}

	for (i = 0; i < isf_header.nr_pt; i++) {
		if (fread(tmp_point, 1, isf_header.byt_nr, isffile) != isf_header.byt_nr) {
			fprintf(stderr, " Error : The file contains too few data\n");
			return 1;
		}
		if ((isf_header.byt_nr == 2) && (strcmp(isf_header.byt_or, "MSB") == 0)) {
			point = tmp_point[0] * 256 + tmp_point[1];
		}
		else {
			point = tmp_point[1] * 256 + tmp_point[0];
		}
		if ((strcmp(isf_header.bn_fmt, "RI") == 0) && (point > 32767)) {
			point = point - 65536;
		}
		y = isf_header.yzero + (isf_header.ymult * (point - isf_header.yoff));
		x = isf_header.xincr * i;
		/* x = isf_header.xzero + (isf_header.xincr * (i - isf_header.pt_off)); */
		printf("%E %E\n", x, y);
	}

	fgets(tmp_check, 2, isffile);
	if (strcmp(tmp_check, "\n") != 0) {
		return 1;
	}

	return 0;
}


/****************************************
* read_curve_asc                        *
*   Read, Calc ASCII data and Print.    *
****************************************/

int read_curve_asc(FILE *isffile)
{
	fprintf(stderr, " Sorry, ENCDG = ASC not supported\n");

	return 1;
}
