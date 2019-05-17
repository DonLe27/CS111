#include <poll.h>
#include <getopt.h>
#include <stdio.h>
#include <mraa.h>
#include <aio.h>
#include<math.h>
#include <mraa/aio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>
#include <string.h>
const int B = 4275;       
const int R0 = 100000;            

//Options
char scaleChar = 'F';
int period = 1;
  //Arg flags
int perFlag = 0;
int scaleFlag = 0;
int fileFlag = 0;
FILE* logFd = NULL;
int stopFlag = 0;
int offFlag = 0;
void writeReport(float tempRead)
{
  //Calculate temperature
  //Using http://wiki.seeedstudio.com/Grove-Temperature_Sensor_V1.2/
  float R = 1023.0/tempRead-1.0;
  R = R0*R;
  float temperature = 1.0/(log(R/R0)/B+1/298.15)-273.15; //Holds temp in C
  if (scaleChar == 'F')
    temperature = (temperature * 9.0/5.0) + 32;

  time_t rawtime;
  struct tm * timeinfo;
  time(&rawtime);
  timeinfo = localtime(&rawtime);
  if (offFlag)
    {
      fprintf(stdout,"%02d:%02d:%02d SHUTDOWN \n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec);  //Why doesn't this work
      if (fileFlag){
	fprintf(logFd,"%02d:%02d:%02d SHUTDOWN \n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec); 
	fclose(logFd);
      }
      exit(0);
    }
  fprintf(stdout,"%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, temperature); 
  if (fileFlag){
    fprintf(logFd,"%02d:%02d:%02d %.1f\n", timeinfo->tm_hour, timeinfo->tm_min, timeinfo->tm_sec, temperature); 

  }
}
void checkError(int ret, char * message)
{
  if (ret < 0)
    {
      fprintf(stderr, message);
      if (fileFlag)
	fclose(logFd);
      exit(1);
    }
}


void commandFunction(char * command)
{
  if (fileFlag)
      fprintf(logFd, command);
  if(!strcmp(command, "STOP\n"))
    stopFlag = 1;
  else if(!strcmp(command, "START\n"))
    stopFlag = 0;
  else if(!strcmp(command, "SCALE=F\n"))
      scaleChar = 'F';
  else if(!strcmp(command, "SCALE=C\n"))
      scaleChar = 'C';
  else if(!strcmp(command, "SCALE=C\n"))
      scaleChar = 'C';
  else if(!strcmp(command, "OFF\n"))
    {
      offFlag = 1;
      writeReport(1);
    }
  else //Command changes log of period
    {
      /* Already logged all commands above so don't really need to check.
      //Check for log
      char * logString = "LOG";
      int fileCommand = 1;
      unsigned int i = 0;
      for (; i < 3; i++){
	if (command[i] != logString[i])
	  {
	    fileCommand = 0;
	    break;
	  }
      }
      
      if (fileCommand && fileFlag) 
	{
	  unsigned int j = 3;
	  int i = 0;
	  char printString[20];
	  while (command[j] != '\n' && j < strlen(command) && command[j] != '\0')
	    {
	      printString[i] = command[j];
	      i++;
	      j++;
	    }
	  //	  fprintf(logFd,"LOG");
	  //	  fprintf(logFd,printString); 	  
	  }*/
      
      char * perString = "PERIOD=";
      int perCommand = 1;
      unsigned int i = 0;
      for (; i < 7; i++){
	if (command[i] != perString[i])
	  {
	    perCommand = 0;
	    break;
	  }
      }
      if (perCommand) 
	{
	  unsigned int j = 7;
	  int i = 0;
	  char intString[20];
	  while (command[j] != '\n' && j < strlen(command) && command[j] != '\0')
	    {
	      intString[i] = command[j];
	      i++;
	      j++;
	    }
	  period = atoi(intString);
	  if (period == 0)
	    checkError(-1, "There was an error using an invalid period.");
	    }
    }
}
int main(int argc, char * argv[])
{

  static struct option command_options[] = {
    {"scale", required_argument, 0, 's'},
    {"period", optional_argument, 0, 'p'},
    {"log", required_argument, 0, 'l'},
    {NULL, 0, NULL, 0}
  };
  int option_index = 0;
  while (1)
    {
      int c = getopt_long(argc, argv, "l:p:s:", command_options, &option_index);
      if (c == -1)
        break;
      switch(c)
        {
        case 'p':
	  period = atoi(optarg);
	  perFlag = 1;
          break;
	case 's':
	  scaleChar = optarg[0];
	  scaleFlag = 1;
	  break;
	case 'l':
	  logFd = fopen(optarg, "w");
	  if (logFd == NULL)
	    {
	      fprintf(stderr, "Couldn't open file");
	      exit(1);
	    }
	  fileFlag = 1;
	  break;
	default:
	  checkError(-1, "Invalid option. Use --period=#, --scale=[FC], or --log=filename");
	  break;
	}
    }

  //Implement polling
  //Used tutorial on https://linux.die.net/man/3/poll
  struct pollfd fds[1];
  fds[0].fd = 0; //Holds stdin
  fds[0].events = POLLIN | POLLHUP | POLLERR;
//Set up the botton
  mraa_gpio_context buttonPin;
  buttonPin = mraa_gpio_init(60);
  mraa_gpio_dir(buttonPin, MRAA_GPIO_IN);
  while(1)
    {
      //Check for commands
      if(mraa_gpio_read(buttonPin)){
	offFlag = 1;
	writeReport(0);
      }
      int ret = poll(fds, 1, 0);
      checkError(ret, "Error polling");
      if ((fds[0].revents & POLLIN) || (fds[0].revents & POLLHUP))
	{
	  char input[20];
	  fgets(input, 20, stdin);
	  commandFunction(input);
	}
      if (!stopFlag){
	time_t beginTime, endTime;
	time(&beginTime);
	time(&endTime);
	while(1){
	  time(&endTime);
	  if(mraa_gpio_read(buttonPin)){
		offFlag=1;
		writeReport(0);
	  }
	  if (difftime(endTime, beginTime) >= period){
	    break;
	}
	}
	float rawValue;
  	mraa_aio_context tempPin;
  	tempPin = mraa_aio_init(1); //Address at pin #1

  	rawValue = mraa_aio_read(tempPin);
 
	writeReport(rawValue);
      }
    }
  
    if (fileFlag)
      fclose(logFd);
    return 0;
}
