#include <stdio.h>    // Standard input/output definitions
#include <stdlib.h>
#include <string.h>   // String function definitions
#include <unistd.h>   // para usleep()
#include <getopt.h>
#include <math.h>

#include "arduino-serial-lib.h"

float calculateSD(float data[]);

void error(char* msg)
{
    fprintf(stderr, "%s\n",msg);
    exit(EXIT_FAILURE);
}

int main(int argc, char *argv[])
{
	int fd = -1;
	int baudrate = 9600;  // default
	float temperaturas[12];
	float humedades[12];
	int minT = 100;
	int maxT = 0;
	int minH = 100;
	int maxH = 0;

	fd = serialport_init("/dev/ttyACM0", baudrate);

	if( fd==-1 )
	{
		error("Couldn't open port");
		return -1;
	}
	
	int i = 0;
	char t = 't';
	char h = 'h';

	while(i<12){
		int temp = 0;
		write(fd,&t,1);
		read(fd,&temp,1);
		temperaturas[i] = (float)temp;
		printf("Temperatura: %d\n",temp);
		usleep(500000);
		if (temp<minT)
			minT=temp;
		if (temp>maxT)
			maxT=temp;

		int hum = 0;
		write(fd,&h,1);
		read(fd,&hum,1);
		humedades[i] = (float)hum;
		printf("Humedad: %d\n",hum);
		i++;
		if (hum<minH)
			minH=hum;
		if (hum>maxH)
			maxH=hum;
		
	}
	
	float desviacionT = calculateSD(temperaturas);
	float desviacionH = calculateSD(humedades);
	printf("La desviacion estandar de las temperaturas es: %f\n",desviacionT);
	printf("La desviacion estandar de las humedades es: %f\n",desviacionH);
	printf("La temperatura minima es: %d\n",minT);
	printf("La temperatura maxima es: %d\n",maxT);
	printf("La humedad minima es: %d\n",minH);
	printf("La humedad maxima es: %d\n",maxH);
	close( fd );
	return 0;	
}

/* Ejemplo para calcular desviacion estandar y media */
float calculateSD(float data[])
{
    float sum = 0.0, mean, standardDeviation = 0.0;

    int i;

    for(i = 0; i < 10; ++i)
    {
        sum += data[i];
    }

    mean = sum/10;

    for(i = 0; i < 10; ++i)
        standardDeviation += pow(data[i] - mean, 2);

    return sqrt(standardDeviation / 10);
}

