#include <stdio.h>
#include <time.h>
#include <string.h>

#define START_YEAR 2003
#define SECONDS_PER_DAY 60*60*24
#define HAGRAPH "../hagraph"
#define WIDTH 1800
#define HEIGHT 800

static int fileExists(const char *filename);

static int fileExists(const char *filename)
{
	FILE *fp = fopen(filename,"r");
	if(fp)
	{
		fclose(fp);
		return 1;
	}
	else
		return 0;
}

int main(int argc, char *argv[])
{
	struct tm thetime;
	struct tm *ptm;
	time_t rawtime, rawtime_today;
	char filename[1024];
	char execute_string[2048];

	time(&rawtime_today);
	
	thetime.tm_year = START_YEAR - 1900;
	thetime.tm_mon = 0;
	thetime.tm_mday = 1;
	thetime.tm_hour = 0;
	thetime.tm_min = 0;
	thetime.tm_sec = 0;

	rawtime = mktime(&thetime);
	ptm = localtime(&rawtime_today);

	/* delete files that affect today */
	strftime(filename,20, "%Y.png",ptm);
	unlink(filename);
	strftime(filename,20, "%Y-%m.png",ptm);
	unlink(filename);
	strftime(filename,20, "%Y-%m-%d.png",ptm);
	unlink(filename);

	while(rawtime < rawtime_today)
	{
		ptm = localtime(&rawtime);
		/* draw the year */
		strftime(filename,30, "%Y_bochum.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 3 -h 0 -i 3 -j 1 -f %d -z %s",HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the months */
		strftime(filename,30, "%Y-%m_bochum.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 3 -h 0 -i 3 -j 1 -f %d-%02d -z %s",HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, ptm->tm_mon+1, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the days */
		strftime(filename,30, "%Y-%m-%d_bochum.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 3 -h 0 -i 3 -j 1 -f %d-%02d-%02d -z %s",
			HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the year */
		strftime(filename,30, "%Y_oe.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 2 -h 0 -i 2 -j 1 -k 4 -l 0 -m 4 -n 1 -f %d -z %s",HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the months */
		strftime(filename,30, "%Y-%m_oe.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 2 -h 0 -i 2 -j 1 -k 4 -l 0 -m 4 -n 1 -f %d-%02d -z %s",HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, ptm->tm_mon+1, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the days */
		strftime(filename,30, "%Y-%m-%d_oe.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 2 -h 0 -i 2 -j 1 -k 4 -l 0 -m 4 -n 1 -f %d-%02d-%02d -z %s",
			HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the year */
		strftime(filename,30, "%Y_out.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 3 -h 1 -i 4 -j 0 -f %d -z %s",HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the months */
		strftime(filename,30, "%Y-%m_out.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 3 -h 1 -i 4 -j 0 -f %d-%02d -z %s",HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, ptm->tm_mon+1, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}
		/* draw the days */
		strftime(filename,30, "%Y-%m-%d_out.png",ptm);
		sprintf(execute_string,"%s -x %d -y %d -g 3 -h 0 -i 4 -j 0 -f %d-%02d-%02d -z %s",
			HAGRAPH, WIDTH, HEIGHT, ptm->tm_year+1900, ptm->tm_mon+1, ptm->tm_mday, filename);
		if(!fileExists(filename))
		{
			printf("creating %s\n",filename);
			system(execute_string);
		}

		rawtime += SECONDS_PER_DAY;
	}
	return 0;
}

	
