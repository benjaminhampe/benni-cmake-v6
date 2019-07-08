/*
   minibz2
      by Yoshioka Tsuneo (tsuneo@rr.iij4u.or.jp)
      This file is Public Domain.  Welcome any email to me.
*/

#include <stdio.h>
#include <stdlib.h>
#include <bzlib.h>

void usage(void)
{
   puts("usage: minibz2 [-d] [-{1,2,..9}] [[srcfilename] destfilename]");
}

int main(int argc,char *argv[])
{
   int decompress = 0;
   int level = 9;
   char *fn_r = NULL;
   char *fn_w = NULL;

   while(++argv,--argc){
      if(**argv =='-' || **argv=='/'){
         char *p;

         for(p=*argv+1;*p;p++){
            if(*p=='d'){
               decompress = 1;
            }else if('1'<=*p && *p<='9'){
               level = *p - '0';
            }else{
               usage();
               exit(1);
            }
         }
      }else{
         break;
      }
   }
   if(argc>=1){
      fn_r = *argv;
      argc--;argv++;
   }else{
      fn_r = NULL;
   }
   if(argc>=1){
      fn_w = *argv;
      argc--;argv++;
   }else{
      fn_w = NULL;
   }
   {
      int len;
      char buff[0x1000];
      char mode[10];

      if(decompress){
         BZFILE *BZ2fp_r = NULL;
         FILE *fp_w = NULL;

         if(fn_w){
            if((fp_w = fopen(fn_w,"wb"))==NULL){
               printf("can't open [%s]\n",fn_w);
               perror("reason:");
               exit(1);
            }
         }else{
            fp_w = stdout;
         }
         if((fn_r == NULL && (BZ2fp_r = BZ2_bzdopen(fileno(stdin),"rb"))==NULL)
            || (fn_r != NULL && (BZ2fp_r = BZ2_bzopen(fn_r,"rb"))==NULL)){
            printf("can't bz2openstream\n");
            exit(1);
         }
         while((len=BZ2_bzread(BZ2fp_r,buff,0x1000))>0){
            fwrite(buff,1,len,fp_w);
         }
         BZ2_bzclose(BZ2fp_r);
         if(fp_w != stdout) fclose(fp_w);
      }else{
         BZFILE *BZ2fp_w = NULL;
         FILE *fp_r = NULL;

         if(fn_r){
            if((fp_r = fopen(fn_r,"rb"))==NULL){
               printf("can't open [%s]\n",fn_r);
               perror("reason:");
               exit(1);
            }
         }else{
            fp_r = stdin;
         }
         mode[0]='w';
         mode[1] = '0' + level;
         mode[2] = '\0';

         if((fn_w == NULL && (BZ2fp_w = BZ2_bzdopen(fileno(stdout),mode))==NULL)
            || (fn_w !=NULL && (BZ2fp_w = BZ2_bzopen(fn_w,mode))==NULL)){
            printf("can't bz2openstream\n");
            exit(1);
         }
         while((len=fread(buff,1,0x1000,fp_r))>0){
            BZ2_bzwrite(BZ2fp_w,buff,len);
         }
         BZ2_bzclose(BZ2fp_w);
         if(fp_r!=stdin)fclose(fp_r);
      }
   }
   return 0;
}
