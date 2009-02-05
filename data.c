#include <stdlib.h>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <string.h>
#ifdef _WIN32
#include <mysql/my_global.h>
#endif
#include <mysql/mysql.h>

#include "data.h"

#define MYSQL_SERVER    "bjoern-b.de"
#define MYSQL_USER      "weather_read"
#define MYSQL_PASS      "QPiEVtJ/6tLxQ"
#define MYSQL_DB        "home_automation"
#define MYSQL_DB_WS2000	"wetterstation"

static int decideView(const char *time_from, const char *time_to);

void initGraph(struct _graph_data *graph, const char *time_from, const char *time_to)
{
	memset(graph, 0, sizeof(struct _graph_data));
	strncpy(graph->time_from, time_from, 10);
	strncpy(graph->time_to, time_to, 10);
	
	graph->view = decideView(graph->time_from, graph->time_to);
	
	graph->min = 999.99;
	graph->max = -999.99;

	graph->num_graphs = 0;
	graph->graphs = NULL;
}

void freeGraph(struct _graph_data *graph)
{
	int i;
	for(i=0; i< graph->num_graphs;i++)
	{
		free(graph->graphs[i].points);
	}
	free(graph->graphs);
}

void addGraphData(struct _graph_data *graph, int modul, int sensor)
{
	MYSQL_RES *mysql_res;
	MYSQL_ROW mysql_row;
	MYSQL *mysql_helper_connection;

	int i=0;
	int num_points;
	double seconds, temperature;
	char query[1024];
	int day_of_week, day_of_month, day_of_year;
	double max = -999.9;
	double min = 999.9;
	
	/* allocate new space for _one_graph_data */
	graph->graphs = realloc(graph->graphs, sizeof(struct _one_graph_data)*(graph->num_graphs+1));
	graph->graphs[graph->num_graphs].modul = modul;
	graph->graphs[graph->num_graphs].sensor = sensor;
	
	MYSQL *mysql_connection;

	mysql_connection = mysql_init(NULL);
	if (!mysql_real_connect(mysql_connection, MYSQL_SERVER, MYSQL_USER, MYSQL_PASS, MYSQL_DB, 0, NULL, 0))
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_connection));
		return;
	}
	mysql_connection->reconnect=1;
	
	mysql_helper_connection = mysql_connection;
	if(modul==4)
	{
		mysql_connection = mysql_init(NULL);
		if (!mysql_real_connect(mysql_connection, MYSQL_SERVER, MYSQL_USER, MYSQL_PASS, MYSQL_DB_WS2000, 0, NULL, 0))
		{
			fprintf(stderr, "%s\n", mysql_error(mysql_connection));
			exit(0);
		}
		if(sensor == 0)
			sprintf(query,"SELECT UNIX_TIMESTAMP(CONCAT(date,\" \",time)),\
			DAYOFWEEK(date),\
			DAYOFMONTH(date),\
			DAYOFYEAR(date),\
			T_1*1000\
			FROM sensor_1_8\
			WHERE date>='%s'\
			AND date<'%s'\
			AND ok_1='0'\
			ORDER BY date,time asc",graph->time_from, graph->time_to);
		if(sensor == 1)
			sprintf(query,"SELECT UNIX_TIMESTAMP(CONCAT(date,\" \",time)),\
			DAYOFWEEK(date),\
			DAYOFMONTH(date),\
			DAYOFYEAR(date),\
			T_i*1000\
			FROM inside\
			WHERE date>='%s'\
			AND date<'%s'\
			AND ok='0'\
			ORDER BY date,time asc",graph->time_from, graph->time_to);
	}
	else
	{
		sprintf(query,"SELECT UNIX_TIMESTAMP(CONVERT_TZ(date,'UTC','MET')),\
			DAYOFWEEK(CONVERT_TZ(date,'UTC','MET')),\
			DAYOFMONTH(CONVERT_TZ(date,'UTC','MET')),\
			DAYOFYEAR(CONVERT_TZ(date,'UTC','MET')),\
			temperature*1000 \
			FROM temperatures WHERE modul_id='%d'\
			AND sensor_id='%d' \
			AND CONVERT_TZ(date,'UTC','MET')>'%s'\
			AND CONVERT_TZ(date,'UTC','MET')<'%s'\
			ORDER BY date asc", modul, sensor, graph->time_from, graph->time_to);
	}

	if(mysql_query(mysql_connection,query))
	{
		fprintf(stderr, "%s\n", mysql_error(mysql_connection));
		exit(0);
	}
	mysql_res = mysql_store_result(mysql_connection);

	/* lets decide how much memory to allocate */

	num_points = mysql_num_rows(mysql_res);

	if(!num_points)
	{
		if(modul==4)
			mysql_close(mysql_connection);
		mysql_connection = mysql_helper_connection;
#ifdef _DEBUG
		printf("no points ..\n");
		printf("empty query was: \n%s\n",query);
#endif
		return;
	}

	graph->graphs[graph->num_graphs].num_points = num_points;
	graph->graphs[graph->num_graphs].points = malloc(sizeof(struct _graph_point)*num_points);

	struct _graph_point *helper = graph->graphs[graph->num_graphs].points;
	
	while((mysql_row = mysql_fetch_row(mysql_res)))
	{
		
		if(!mysql_row)
		{	
			fprintf(stderr, "%s\n", mysql_error(mysql_connection));
			exit(0);
		}
		
		if(mysql_row[0]) seconds	= (long long)atoi(mysql_row[0]);
		
		if(strcmp(mysql_row[4],"0.0")) temperature = (double)atoi(mysql_row[4])/1000;

		day_of_week = atoi(mysql_row[1]) -2;	// MYSQL gibt Sonntag = 1... zurück
		if(day_of_week == -1) day_of_week = 6;		// jetzt Montag=1, Sonntag=7
		day_of_month = atoi(mysql_row[2]) -1;
		day_of_year = atoi(mysql_row[3]) -1;
			
		helper[i].x = seconds;
		helper[i].y = temperature;

		if(temperature > max)
			max = temperature;
		if(temperature < min)
			min = temperature;
		i++;
	}
	if(modul==4)
		mysql_close(mysql_connection);
	mysql_connection = mysql_helper_connection;
	
	mysql_close(mysql_connection);

	if(min < graph->min)
		graph->min = floor(min/10.0)*10;
	if(max > graph->max)
		graph->max = ceil(max/10.0)*10; 
	//graph->graphs[graph->num_graphs].min = min;
	//graph->graphs[graph->num_graphs].max = max;
	
	graph->num_graphs++;

	return;
}

static int decideView(const char *time_from, const char *time_to)
{
	char c_from[255], c_to[255];
	struct tm from, to;
	
	strcpy(c_from,time_from);
	strcpy(c_to,time_to);
	
	from.tm_year = atoi(strtok(c_from,"-")) -1900;
	from.tm_mon = atoi(strtok(NULL,"-")) - 1;
	from.tm_mday = atoi(strtok(NULL,"-"));
	from.tm_hour = 0;
	from.tm_min = 0;
	from.tm_sec = 0;
	
	to.tm_year = atoi(strtok(c_to,"-")) -1900;
	to.tm_mon = atoi(strtok(NULL,"-")) - 1;
	to.tm_mday = atoi(strtok(NULL,"-"));
	to.tm_hour = 0;
	to.tm_min = 0;
	to.tm_sec = 0;
	
	if(mktime(&to)-mktime(&from) <= SECONDS_PER_DAY)
		return TB_DAY;
	else if(mktime(&to)-mktime(&from) <= SECONDS_PER_WEEK)
		return TB_WEEK;
	else if(mktime(&to)-mktime(&from) <= SECONDS_PER_MONTH)
		return TB_MONTH;
	else
		return TB_YEAR;
}
	

