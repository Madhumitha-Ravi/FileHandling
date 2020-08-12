#include <stdio.h>
#include<stdbool.h>
#include<stdlib.h>
#include<signal.h>
#include<errno.h>

bool bBreak = false;

void SignalHandler(int SignalNum)
{
    // If it is interupt make the boolean true and break the getline loop
    if(SignalNum == SIGINT)
    {
        bBreak = true ; 
    }
}

int main(void) 
{

  signal(SIGINT,SignalHandler); // call the signal handler
  char *Input;
  size_t nBuff = 250; // decent size for storing a single line

  FILE *pFile;
  int nRet = 0;
  pFile = fopen("Output.txt", "a");
  if(pFile == NULL)
  {
      perror("Error");
      return errno;
  }

  Input = (char*)malloc(nBuff);
  if(Input == NULL)
  {
        // memory alloacation failed
        perror("Error");
        return errno;
  }
  printf("Please press ctr+c to stop giving the input\n");
  printf("Enter the data that needs to be written in file\n");
  while(getline(&Input,&nBuff,stdin) )
  {
    // why getline ? Getline will read the entire line and we can chunk that entire line into the file instead of character by character
    if(bBreak)
    {
        break;
    }
    if(Input == NULL)
    {
        perror("Error:");
        return errno;
    }
    nRet = fprintf(pFile,"%s",Input);
    if(nRet < 0)
    {
        // Some problem 
        perror("Error");
        return errno;
    }
    // Why not reallocate ?
    // since I am writing to the file line by line , it is not neccessary for us to use reallaoc
    free(Input);
    Input = NULL;    
    Input = (char*)malloc(nBuff);
    if(Input == NULL)
    {
        // memory alloacation failed
        perror("Error");
        return errno;
    }
  }
  fclose(pFile);
  return 0;
}
