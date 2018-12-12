/*
 * Licensed to the OpenAirInterface (OAI) Software Alliance under one or more
 * contributor license agreements.  See the NOTICE file distributed with
 * this work for additional information regarding copyright ownership.
 * The OpenAirInterface Software Alliance licenses this file to You under
 * the OAI Public License, Version 1.1  (the "License"); you may not use this file
 * except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.openairinterface.org/?page_id=698
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 *-------------------------------------------------------------------------------
 * For more information about the OpenAirInterface (OAI) Software Alliance:
 *      contact@openairinterface.org
 */

#ifdef USER_MODE

#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>
#include <sys/time.h>
#include <sys/stat.h>
#include <defs.h>
#include <math.h>
extern char path_of_output[128];
extern uint16_t NUMBER_OF_ESTIMATION_F;
amp_phase  abs_with_angle(short real16, short im16)
{
  
  amp_phase amphase;
 
  amphase.amp = sqrt((double)(pow(real16,2)+pow(im16,2)));
  amphase.phase = atan2((double)im16,(double)real16);
  return amphase;  	    
}

int write_output(char *fname,const char *vname,void *data,int length,int dec,char format)
{
  //char name[50]={0};
  //struct timevl ustime;
  //gettimeofday(&ustime,NULL);
  FILE *fp=NULL;
  int i;


  printf("Writing %d elements of type %d to %s\n",length,format,fname);


  if (format == 10 || format ==11 || format == 12 || format == 13 || format == 14) {
    fp = fopen(fname,"a+");
  } else if (format != 10 && format !=11  && format != 12 && format != 13 && format != 14) {
    fp = fopen(fname,"w+");
  }



  if (fp== NULL) {
    printf("[OPENAIR][FILE OUTPUT] Cannot open file %s\n",fname);
    return(-1);
  }

  if (format != 10 && format !=11  && format != 12 && format != 13 && format != 14)
    fprintf(fp,"%s = [",vname);


  switch (format) {
  case 0:   // real 16-bit

    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((short *)data)[i]);
    }

    break;

  case 1:  // complex 16-bit
  case 13:
  case 14:
  case 15:

    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((short *)data)[i],((short *)data)[i+1]);

    }


    break;

  case 2:  // real 32-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((int *)data)[i]);
    }

    break;

  case 3: // complex 32-bit
    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((int *)data)[i],((int *)data)[i+1]);
    }

    break;

  case 4: // real 8-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((char *)data)[i]);
    }

    break;

  case 5: // complex 8-bit
    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((char *)data)[i],((char *)data)[i+1]);
    }

    break;

  case 6:  // real 64-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%lld\n",((long long*)data)[i]);
    }

    break;

  case 7: // real double
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%g\n",((double *)data)[i]);
    }

    break;

  case 8: // complex double
    for (i=0; i<length<<1; i+=2*dec) {
      fprintf(fp,"%g + j*(%g)\n",((double *)data)[i], ((double *)data)[i+1]);
    }

    break;

  case 9: // real unsigned 8-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((unsigned char *)data)[i]);
    }

    break;


  case 10 : // case eren 16 bit complex :

    for (i=0; i<length<<1; i+=(2*dec)) {

      if((i < 2*(length-1)) && (i > 0))
        fprintf(fp,"%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);
      else if (i == 2*(length-1))
        fprintf(fp,"%d + j*(%d);",((short *)data)[i],((short *)data)[i+1]);
      else if (i == 0)
        fprintf(fp,"\n%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);



    }

    break;

  case 11 : //case eren 16 bit real for channel magnitudes:
    for (i=0; i<length; i+=dec) {

      if((i <(length-1))&& (i > 0))
        fprintf(fp,"%d,",((short *)data)[i]);
      else if (i == (length-1))
        fprintf(fp,"%d;",((short *)data)[i]);
      else if (i == 0)
        fprintf(fp,"\n%d,",((short *)data)[i]);
    }

    printf("\n erennnnnnnnnnnnnnn: length :%d",length);
    break;

  case 12 : // case eren for log2_maxh real unsigned 8 bit
    fprintf(fp,"%d \n",((unsigned char *)&data)[0]);
    break;

  }

  if (format != 10 && format !=11 && format !=12 && format != 13 && format != 15) {
    fprintf(fp,"];\n");
    fclose(fp);
    return(0);
  } else if (format == 10 || format ==11 || format == 12 || format == 13 || format == 15) {
    fclose(fp);
    return(0);
  }

  return 0;
}
int write_output1(char *fname,const char *vname,void *data,int length,int dec,char format,char abs_angle)
{
  FILE *fp=NULL;
  int i;
  struct tm *tt;
  char name[256]="//home/dell/data/data_tele_";
  char fold_name[50]={0};
  //strcpy(name, path_of_output);
  struct timeval tv;
  gettimeofday(&tv,NULL);
  tt = localtime(&(tv.tv_sec));                             
  strftime(fold_name,50,"%m%d/",tt);              
  strcat(name,fold_name);
  if(access(name,0)!=0)
    if(mkdir(name,0777)!=0)
	  printf("dictory fail to build zhangheng\n");
  strcat(strcat(name,fname),vname);
  strcat(name,path_of_output);
  strcat(name,".md");
 // printf("Writing %d elements of type %d to %s\n",length,format,fname);


  if (format == 10 || format ==11 || format == 12 || format == 13 || format == 14) {
    fp = fopen(name,"a+");
  } else if (format != 10 && format !=11  && format != 12 && format != 13 && format != 14) {
    fp = fopen(name,"a+");
  }

  if (fp== NULL) {
    printf("[OPENAIR][FILE OUTPUT] Cannot open file %s\n",fname);
    return(-1);
  }

  if (format != 10 && format !=11  && format != 12 && format != 13 && format != 14)
    //fprintf(fp,"%s = [\n",vname);

  

  switch (format) {
  case 0:   // real 16-bit

    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((short *)data)[i]);
    }

    break;

  case 1:  // complex 16-bit
  case 13:
  case 14:
  case 15:
  	
	for (i=0; i<length<<1; i+=(2*dec)) {
	  
      //fprintf(fp,"%d j*(%d)\n",((short *)data)[i],((short *)data)[i+1]);
      
	  if(abs_angle)
	  	{
	  	    amp_phase ap;
			ap = abs_with_angle(((short *)data)[i], ((short *)data)[i+1]);
	  		fprintf(fp, "%d %.2f ",(short)ap.amp,ap.phase);
			
			//fprintf(fp,"%f ",ap.amp);
	  	}

    }
		fprintf(fp, "\n");
 
	


    break;

  case 2:  // real 32-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((int *)data)[i]);
    }

    break;

  case 3: // complex 32-bit
    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((int *)data)[i],((int *)data)[i+1]);
    }

    break;

  case 4: // real 8-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((char *)data)[i]);
    }

    break;

  case 5: // complex 8-bit
    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((char *)data)[i],((char *)data)[i+1]);
    }

    break;

  case 6:  // real 64-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%lld\n",((long long*)data)[i]);
    }

    break;

  case 7: // real double
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%g\n",((double *)data)[i]);
    }

    break;

  case 8: // complex double
    for (i=0; i<length<<1; i+=2*dec) {
      fprintf(fp,"%g + j*(%g)\n",((double *)data)[i], ((double *)data)[i+1]);
    }

    break;

  case 9: // real unsigned 8-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((unsigned char *)data)[i]);
    }

    break;


  case 10 : // case eren 16 bit complex :

    for (i=0; i<length<<1; i+=(2*dec)) {

      if((i < 2*(length-1)) && (i > 0))
        fprintf(fp,"%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);
      else if (i == 2*(length-1))
        fprintf(fp,"%d + j*(%d);",((short *)data)[i],((short *)data)[i+1]);
      else if (i == 0)
        fprintf(fp,"\n%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);



    }

    break;

  case 11 : //case eren 16 bit real for channel magnitudes:
    for (i=0; i<length; i+=dec) {

      if((i <(length-1))&& (i > 0))
        fprintf(fp,"%d,",((short *)data)[i]);
      else if (i == (length-1))
        fprintf(fp,"%d;",((short *)data)[i]);
      else if (i == 0)
        fprintf(fp,"\n%d,",((short *)data)[i]);
    }

    printf("\n erennnnnnnnnnnnnnn: length :%d",length);
    break;

  case 12 : // case eren for log2_maxh real unsigned 8 bit
    fprintf(fp,"%d \n",((unsigned char *)&data)[0]);
    break;

  }

  if (format != 10 && format !=11 && format !=12 && format != 13 && format != 15) {
    //fprintf(fp,"];\n");
    fclose(fp);
    return(0);
  } else if (format == 10 || format ==11 || format == 12 || format == 13 || format == 15) {
    fclose(fp);
    return(0);
  }

  return 0;
}



int write_output2(char *fname,const char *vname,void *data,void *data2,int length,int dec,char format,char abs_angle)
{
  FILE *fp=NULL;
  int i;
  struct tm *tt;
  char name[256]="/home/heng/data/data_tele_";
  char fold_name[50]={0};
  //strcpy(name, path_of_output);
  struct timeval tv;
  gettimeofday(&tv,NULL);
  tt = localtime(&(tv.tv_sec));                             
  strftime(fold_name,50,"%m%d/",tt);              
  strcat(name,fold_name);
  if(access(name,0)!=0)
    if(mkdir(name,0777)!=0)
	  printf("dictory fail to build zhangheng\n");
  strcat(strcat(name,fname),vname);
  strcat(name,path_of_output);
  strcat(name,".md");
 // printf("Writing %d elements of type %d to %s\n",length,format,fname);


  if (format == 10 || format ==11 || format == 12 || format == 13 || format == 14) {
    fp = fopen(name,"a+");
  } else if (format != 10 && format !=11  && format != 12 && format != 13 && format != 14) {
    fp = fopen(name,"a+");
  }

  if (fp== NULL) {
    printf("[OPENAIR][FILE OUTPUT] Cannot open file %s\n",fname);
    return(-1);
  }

  if (format != 10 && format !=11  && format != 12 && format != 13 && format != 14)
    //fprintf(fp,"%s = [\n",vname);

  

  switch (format) {
  case 0:   // real 16-bit

    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((short *)data)[i]);
    }

    break;

  case 1:  // complex 16-bit
  case 13:
  case 14:
  case 15:
  	if(abs_angle){
	  amp_phase ap1,ap2;
	  for (i=0; i<length<<1; i+=(2*dec)) {
	  
      //fprintf(fp,"%d j*(%d)\n",((short *)data)[i],((short *)data)[i+1]);
      
	 
			ap1 = abs_with_angle(((short *)data)[i], ((short *)data)[i+1]);
	        ap2 = abs_with_angle(((short *)data2)[i], ((short *)data2)[i+1]);
	        fprintf(fp, "%f %f %f %f %f %f  ",ap1.amp,ap2.amp,(ap1.amp-ap2.amp),ap1.phase,ap2.phase,(ap1.phase - ap2.phase));
	  		//fprintf(fp, "%f %f ",(ap1.amp-ap2.amp),(ap1.phase - ap2.phase);
			//fprintf(fp,"%f ",ap.amp);
	  	

      }
		fprintf(fp, "\n");
  	}
 
	


    break;

  case 2:  // real 32-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((int *)data)[i]);
    }

    break;

  case 3: // complex 32-bit
    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((int *)data)[i],((int *)data)[i+1]);
    }

    break;

  case 4: // real 8-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((char *)data)[i]);
    }

    break;

  case 5: // complex 8-bit
    for (i=0; i<length<<1; i+=(2*dec)) {
      fprintf(fp,"%d + j*(%d)\n",((char *)data)[i],((char *)data)[i+1]);
    }

    break;

  case 6:  // real 64-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%lld\n",((long long*)data)[i]);
    }

    break;

  case 7: // real double
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%g\n",((double *)data)[i]);
    }

    break;

  case 8: // complex double
    for (i=0; i<length<<1; i+=2*dec) {
      fprintf(fp,"%g + j*(%g)\n",((double *)data)[i], ((double *)data)[i+1]);
    }

    break;

  case 9: // real unsigned 8-bit
    for (i=0; i<length; i+=dec) {
      fprintf(fp,"%d\n",((unsigned char *)data)[i]);
    }

    break;


  case 10 : // case eren 16 bit complex :

    for (i=0; i<length<<1; i+=(2*dec)) {

      if((i < 2*(length-1)) && (i > 0))
        fprintf(fp,"%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);
      else if (i == 2*(length-1))
        fprintf(fp,"%d + j*(%d);",((short *)data)[i],((short *)data)[i+1]);
      else if (i == 0)
        fprintf(fp,"\n%d + j*(%d),",((short *)data)[i],((short *)data)[i+1]);



    }

    break;

  case 11 : //case eren 16 bit real for channel magnitudes:
    for (i=0; i<length; i+=dec) {

      if((i <(length-1))&& (i > 0))
        fprintf(fp,"%d,",((short *)data)[i]);
      else if (i == (length-1))
        fprintf(fp,"%d;",((short *)data)[i]);
      else if (i == 0)
        fprintf(fp,"\n%d,",((short *)data)[i]);
    }

    printf("\n erennnnnnnnnnnnnnn: length :%d",length);
    break;

  case 12 : // case eren for log2_maxh real unsigned 8 bit
    fprintf(fp,"%d \n",((unsigned char *)&data)[0]);
    break;

  }

  if (format != 10 && format !=11 && format !=12 && format != 13 && format != 15) {
    //fprintf(fp,"];\n");
    fclose(fp);
    return(0);
  } else if (format == 10 || format ==11 || format == 12 || format == 13 || format == 15) {
    fclose(fp);
    return(0);
  }

  return 0;
}


#endif // USER_MODE
