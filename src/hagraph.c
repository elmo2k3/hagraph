/*
 * Copyright (C) 2007-2010 Bjoern Biesenbach <bjoern@bjoern-b.de>
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License as
 * published by the Free Software Foundation; either version 2 of the
 * License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
 * General Public License for more details.
 *
 * You should have received a copy of the GNU General Public
 * License along with this program; if not, write to the
 * Free Software Foundation, Inc., 59 Temple Place - Suite 330,
 * Boston, MA 02111-1307, USA.
        
        ./hagraph -x 1300 -y 900 -f 2007-12-1 -t 2007-12-30 -g 2 -h 0 -i 2 -j 1 (Programm ausführen)

 */


#include <stdio.h>
#include <stdlib.h>
#include <mysql/mysql.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <unistd.h>

#include <libhagraph/libhagraph.h>

void printUsage(char *progname)
{
    printf("\n\
%s -x 1300 -y 900 -f 2007-12-1 -t 2007-12-30 -g 2 -h 0 -i 2 -j 1 (Programm ausführen)\n\
\n\
    Paramter:\n\
            -x (PIXEL)              Bildbreite in Pixel\n\
            -y (PIXEL)              Bildhöhe in Pixel\n\
\n\
            -f (XXXX-YY-ZZ)         Datum von       Format: XXXX-YY-ZZ (X=Jahr, Y=Monat, Z=Tag)\n\
            -t (XXXX-YY-ZZ)         Datum bis\n\
\n\
            -g (MODUL)              erstes Modul    (Farbe: rot)\n\
            -h (SENSOR)             erster Sensor\n\
\n\
            -i (MODUL)              zweites Modul (optional)        (Farbe: blau)\n\
            -j (SENSOR)             zweiter Sensor (optional)\n\
\n\
            -k (MODUL)              drittes Modul (optional)        (Farbe: grün)\n\
            -l (SENSOR)             dritter Sensor (optional)\n\
\n\
            -m (MODUL)              viertes Modul (optional)        (Farbe: violett)\n\
            -n (SENSOR)             vierter Sensor (optional)\n\
\n\
            -o (MODUL)              fünftes Modul (optional)        (Farbe: violett)\n\
            -p (SENSOR)             fünftes Sensor (optional)\n\
\n\
            -q (MODUL)              sechses Modul (optional)        (Farbe: orange)\n\
            -r (SENSOR)             sechses Sensor (optional)\n\
\n\
            -z filename             Dateiname \n\
            -u                      Transparenz\n\
            -v                      Invertiert\n\n", progname);
}

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
    
    modul_sensor[0][0]=3;
    modul_sensor[0][1]=1;
    modul_count=1;

    width = 800;
    height = 400;

    if(argc < 2)
    {
        printUsage(argv[0]);
        return 0;
    }

    while ((c = getopt (argc, argv, "f:t:x:y:g:h:i:j:k:l:m:n:o:p:q:r:z:uv")) != -1)
    {
        switch(c)
        {
            case 'f':   strcpy(date,optarg); break;
            case 't':   strcpy(time_to,optarg); break;
            case 'x':   width = atoi(optarg); break;
            case 'y':   height = atoi(optarg); break;
            case 'g':   modul_sensor[0][0] = atoi(optarg); modul_count=0 ;break;
            case 'h':   modul_sensor[0][1] = atoi(optarg); modul_count++; break;
            case 'i':   modul_sensor[1][0] = atoi(optarg); break;
            case 'j':   modul_sensor[1][1] = atoi(optarg); modul_count++; break;
            case 'k':   modul_sensor[2][0] = atoi(optarg); break;
            case 'l':   modul_sensor[2][1] = atoi(optarg); modul_count++; break;
            case 'm':   modul_sensor[3][0] = atoi(optarg); break;
            case 'n':   modul_sensor[3][1] = atoi(optarg); modul_count++; break;
            case 'o':   modul_sensor[4][0] = atoi(optarg); break;
            case 'p':   modul_sensor[4][1] = atoi(optarg); modul_count++; break;
            case 'q':   modul_sensor[5][0] = atoi(optarg); break;
            case 'r':   modul_sensor[5][1] = atoi(optarg); modul_count++; break;
            case 'u':   libhagraphSetTransparent(1); break;
            case 'v':   libhagraphSetInverted(1); break;
            
            case 'z':   strcpy(file_output,optarg); break;
        }
    }

    if(time_to[0] == 'm')
        view = TB_MONTH;
    else if(time_to[0] == 'y')
        view = TB_YEAR;
    
    transformDate(time_from, time_to, date, view);

    initGraph(&graph, time_from, time_to);
    
    for(c=0;c<modul_count;c++)
    {
        addGraphData(&graph, modul_sensor[c][0], modul_sensor[c][1]);
    }

    drawGraphPng(file_output, &graph, width, height);
    freeGraph(&graph);

    return 0;
}

