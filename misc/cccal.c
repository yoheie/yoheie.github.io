#include <stdio.h>
#include <stdlib.h>
#include <string.h>

static const char date[] = __DATE__;
static const char mname[12][3] = {
	"Jan", "Feb", "Mar", "Apr", "May", "Jun",
	"Jul", "Aug", "Sep", "Oct", "Nov", "Dec"
};

int main(void)
{
	int month;
	int year;
	int days;
	int weekday;
	int m, n;

	year = atoi(&date[7]);

	for (month = 1; month <= 12; month++)
	{
		if (memcmp(date, mname[month - 1], 3) == 0) {
			break;
		}
	}

	if (month == 2) {
		if (year % 4 == 0 && (year % 100 != 0 || year % 400 == 0)) {
			days = 29;
		}
		else {
			days = 28;
		}
	}
	else if (month == 4 || month == 6 || month == 9 || month == 11) {
		days = 30;
	}
	else {
		days = 31;
	}

	if (month <= 2) {
		year -= 1;
		month += 12;
	}

	weekday =
		(year + year/4 - year/100 + year/400 + (month*13 + 8)/5 + 1) % 7;

	puts("Su Mo Tu We Th Fr Sa");

	for (m = 0; m < weekday; m++) {
		printf("   ");
	}

	for (n = 1; n < days; n++) {
		printf("%2d%c",n, (m<6)?' ':'\n');
		if (++m > 6) m = 0;
	}

	printf("%2d\n",n);

	return 0;
}
