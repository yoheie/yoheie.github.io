{
	year=$1
	month=$2

	if (month==2) {
		if (year%4==0 && (year%100!=0 || year%400==0)) {
			days=29
		}
		else {
			days=28
		}
	}
	else if (month==4 || month==6 || month==9 || month==11) {
		days=30
	}
	else {
		days=31
	}

	print "      " year "-" month
	print "Su Mo Tu We Th Fr Sa"

	if (month<=2) {
		year-=1
		month+=12
	}
	day=1-(year + int(year/4) - int(year/100) + int(year/400) + int((month*13 + 8)/5) + 1)%7

	weekday=0
	while (day<days) {
		if (weekday==0) {
			weektext=daytext(day,days)
		}
		else {
			weektext=weektext " " daytext(day,days)
		}
		weekday+=1
		day+=1
		if (weekday==7) {
			print weektext
			weekday=0
		}
	}
	if (weekday==0) {
		weektext=daytext(day,days)
	}
	else {
		weektext=weektext " " daytext(day,days)
	}
	print weektext
}

function daytext(d, md) {
	if (d<1) {
		text="  "
	}
	else if (d<10) {
		text=" " d
	}
	else if (d<=md) {
		text=d
	}
	else {
		text=""
	}

	return text
}
