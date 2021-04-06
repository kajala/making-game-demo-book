/*
* bin2c.c
* convert files to byte arrays for automatic loading with lua_dobuffer
* Luiz Henrique de Figueiredo (lhf@tecgraf.puc-rio.br)
* 02 Apr 2003 20:44:31
*/

#include <ctype.h>
#include <stdio.h>
#include <stdlib.h>

static void dump(FILE* f, int n)
{
 printf("static const unsigned char B%d[]={\n",n);
 for (n=1;;n++)
 {
  int c=getc(f); 
  if (c==EOF) break;
  printf("%3u,",c);
  if (n==20) { putchar('\n'); n=0; }
 }
 printf("\n};\n\n");
}

static void fdump(const char* fn, int n)
{
 FILE* f= fopen(fn,"rb");		/* must open in binary mode */
 if (f==0)
 {
  fprintf(stderr,"bin2c: cannot open ");
  perror(fn);
  exit(1);
 }
 else
 {
  printf("/* %s */\n",fn);
  dump(f,n);
  fclose(f);
 }
}

static void emit(const char* fn, int n)
{
 printf(" lua_dobuffer(L,(const char*)B%d,sizeof(B%d),\"%s\");\n",n,n,fn);
}

int main(int argc, char* argv[])
{
 printf("/* code automatically generated by bin2c -- DO NOT EDIT */\n");
 printf("{\n");
 if (argc<2)
 {
  dump(stdin,0);
  emit("=stdin",0);
 }
 else
 {
  int i;
  printf("/* #include'ing this file in a C program is equivalent to calling\n");
  for (i=1; i<argc; i++) printf("  lua_dofile(L,\"%s\");\n",argv[i]);
  printf("*/\n");
  for (i=1; i<argc; i++) fdump(argv[i],i);
  for (i=1; i<argc; i++) emit(argv[i],i);
 }
 printf("}\n");
 return 0;
}
