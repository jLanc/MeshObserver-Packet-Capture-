/*
 * To change this license header, choose License Headers in Project Properties.
 * To change this template file, choose Tools | Templates
 * and open the template in the editor.
 */

/* 
 * File:   captureme.c
 * Author: honours
 *
 * Created on November 13, 2018, 5:53 PM
 */

#include <stdio.h>
#include <stdlib.h>
#include <pcap.h>
#include <unistd.h>
#include <termios.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <errno.h>
#include <string.h>

/*
 * 
 */

//Decodes hex values found in lbard's encoding system

int hextochar(int h)
{
  int retVal = '?';  

  do 
  {
    if ((h >= 0) && (h < 10)) 
    {
      retVal = h + '0';
      break;
    }

    if ((h >= 10) && (h < 16)) 
    {
      retVal = h+'A'-10;
      break;
    }
  }
  while (0);

  return retVal;
}


//decode hex function pulled from lbard's util.c file
char hex_decode(unsigned char *in, unsigned char *out)
{
  int i;
  int out_count = 0;

  int inLen = strlen(in);
  printf ("in = %s", in);

  for (i = 0; i < inLen; i += 2)
  {
    int v = hextochar(in[i + 0]) << 4;
    v |= hextochar(in[i + 1]);

    out[out_count++] = v;
    if (out_count >= 254)
    {
      printf("trying to write more than out_len chars");
      break;
    }    
  }
  out[out_count] = 0;
  return *out;
}

//dump bytes function pulled from the lbard util.c file
int dump_bytes(FILE *f, char *msg, unsigned char *bytes, int length)
{
  int retVal = -1;

  do
  {
    fprintf(f, "%s:\n", msg);
    for (int i = 0; i < length; i += 16)
    {
      fprintf(f, "%04X: ", i);
      for (int j = 0; j < 16; j++)
        if (i + j < length)
          fprintf(f, " %02X", bytes[i + j]);
      fprintf(f, "  ");
      for (int j = 0; j < 16; j++)
      {
        int c;
        if (i + j < length)
          c = bytes[i + j];
        else
          c = ' ';
        if (c < ' ')
          c = '.';
        if (c > 0x7d)
          c = '.';
        fprintf(f, "%c", c);
      }
      fprintf(f, "\n");
    }
    retVal = 0;
  } while (0);

  return retVal;
}

int main(int argc, char **argv)
{
  int retVal = 0;

  do
  {
    //open file containing hex dump
    FILE *inFile = fopen("testFile", "r");  //read file
    FILE *outFile = fopen("decoded", "ab"); //coded file
    char readBuffer[254];
    unsigned char writeBuffer;

    //read file line by line
    while (fgets(readBuffer, sizeof(readBuffer), inFile))
    {
      writeBuffer = hex_decode(readBuffer, &writeBuffer);
      printf ("writeBuffer = %c\n", writeBuffer);
      //fprintf(outFile, "%s\n", writeBuffer);
    }

  } while (0);

  return (retVal);
}