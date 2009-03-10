/* Bjoern Biesenbach <bjoern@bjoern-b.de>
 * 
 * 
 * 
		
		./hagraph -x 1300 -y 900 -f 2007-12-1 -t 2007-12-30 -g 2 -h 0 -i 2 -j 1 (Programm ausführen)


Paramter:
		-x (PIXEL)              Bildbreite in Pixel
		-y (PIXEL)              Bildhöhe in Pixel

		-f (XXXX-YY-ZZ)         Datum von       Format: XXXX-YY-ZZ (X=Jahr, Y=Monat, Z=Tag)
		-t (XXXX-YY-ZZ)         Datum bis

		-g (MODUL)              erstes Modul    (Farbe: rot)
		-h (SENSOR)             erster Sensor

		-i (MODUL)              zweites Modul (optional)        (Farbe: blau)
		-j (SENSOR)             zweiter Sensor (optional)

		-k (MODUL)              drittes Modul (optional)        (Farbe: grün)
		-l (SENSOR)             dritter Sensor (optional)

		-m (MODUL)              viertes Modul (optional)        (Farbe: violett)
		-n (SENSOR)             vierter Sensor (optional)

		-o (MODUL)              fünftes Modul (optional)        (Farbe: orange)
		-p (SENSOR)             fünfter Sensor (optional)

Module, Sensoren:
        Modul           Sensor          Ort
        1               0               Bochum Küche
        1               1               Bochum Gefrierfach
        1               2               Bochum Kühlschrank

        2               0               Vorlauf
        2               1               Rücklauf

        3               0               Bochum Schlafzimmer
        3               1               Bochum Aussen
 * 
 * 
 * 
 */


#include <stdio.h>
#include <stdlib.h>
#include <gd.h>
#include <gdfontl.h>
#include <gdfonts.h>
#include <mysql/mysql.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include <libhagraph/libhagraph.h>

#define MYSQL_HOST "bjoern-b.de"
#define MYSQL_USER "weather_read"
#define MYSQL_PASS "QPiEVtJ/6tLxQ"
#define MYSQL_PORT 3306
#define MYSQL_DATABASE "home_automation"
#define MYSQL_DATABASE_WS2000 "wetterstation"

int main(int argc, char *argv[])
{
	char file_output[255];
	char time_from[255], time_to[255], date[255];
	int modul_sensor[10][2];
	int c;
	int modul_count=0;
	time_t rawtime;
	struct tm *today;
	struct _graph_data graph;
	int width, height;
	int view;
	
	view = TB_DAY;
	memset(time_to, 0, sizeof(time_to));

	time(&rawtime);
	today = gmtime(&rawtime);
	strftime (date,255,"%Y-%m-%d",today);
	
	strcpy(file_output,"test.png");
	
	//strcpy(time_from,"2007-12-29");
	//strcpy(time_to,"2007-12-30");
	modul_sensor[0][0]=3;
	modul_sensor[0][1]=1;
	modul_count=1;

	width = 800;
	height = 400;

	while ((c = getopt (argc, argv, "f:t:x:y:g:h:i:j:k:l:m:n:o:p:q:r:z:")) != -1)
	{
		switch(c)
		{
			case 'f': 	strcpy(date,optarg); break;
			case 't': 	strcpy(time_to,optarg); break;
			case 'x': 	width = atoi(optarg); break;
			case 'y': 	height = atoi(optarg); break;
			case 'g': 	modul_sensor[0][0] = atoi(optarg); modul_count=0 ;break;
			case 'h': 	modul_sensor[0][1] = atoi(optarg); modul_count++; break;
			case 'i': 	modul_sensor[1][0] = atoi(optarg); break;
			case 'j': 	modul_sensor[1][1] = atoi(optarg); modul_count++; break;
			case 'k': 	modul_sensor[2][0] = atoi(optarg); break;
			case 'l': 	modul_sensor[2][1] = atoi(optarg); modul_count++; break;
			case 'm': 	modul_sensor[3][0] = atoi(optarg); break;
			case 'n': 	modul_sensor[3][1] = atoi(optarg); modul_count++; break;
			case 'o': 	modul_sensor[4][0] = atoi(optarg); break;
			case 'p': 	modul_sensor[4][1] = atoi(optarg); modul_count++; break;
			case 'q': 	modul_sensor[5][0] = atoi(optarg); break;
			case 'r': 	modul_sensor[5][1] = atoi(optarg); modul_count++; break;
			
			case 'z': 	strcpy(file_output,optarg); break;
		}
	}

	if(time_to[0] == 'm')
		view = TB_MONTH;
	else if(time_to[0] == 'y')
		view = TB_YEAR;
	
	if(!transformDate(time_from, time_to, date, view))
	{
		printf("Parse error for date: %s\n",date);
		return EXIT_FAILURE;
	}

	initGraph(&graph, time_from, time_to);
	
	for(c=0;c<modul_count;c++)
	{
		addGraphData(&graph, modul_sensor[c][0], modul_sensor[c][1],
			MYSQL_HOST,
			MYSQL_USER,
			MYSQL_PASS,
			MYSQL_DATABASE,
			MYSQL_DATABASE_WS2000);
	}

	drawGraphPng(file_output, &graph, width, height);
	freeGraph(&graph);

	return 0;
}

