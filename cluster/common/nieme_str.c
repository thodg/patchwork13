#include"nieme_str.h"



char * nieme_str(char * str, int n)
{
  char * ch;
  int i,p;
  i=0;

  ch=malloc(strlen(str)*sizeof(char));

  if(str==NULL) {
    return str;
  } else {
    
    while( n>0 ) {
      if( str[i]=='#') {
	n--;
      }
      i++;
    }
    
    p=0;
    while( str[i]!='#' ) {
      ch[p]=str[i];
      p++;
      i++;
    }
    ch[p]='\0';
    
    realloc(ch, (p+1)*sizeof(char));

    return ch;
  }
}

