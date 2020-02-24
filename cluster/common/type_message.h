#ifndef _TYPE_MESSAGE_H_INCLUDED_
#define _TYPE_MESSAGE_H_INCLUDED_ 

#include<pw13/pw13.h>

typedef enum {
  PW13_CREATE_PATCH,
  PW13_CREATE_PATCH_OUTPUT,
  PW13_DESTROY_PATCH,
  PW13_CONNECT_PATCH_LOCAL,
  PW13_CONNECT_PATCH_DISTANT,
  PW13_BIND_PATCH_INPUT,
  PW13_ASK_PATCH_START,
  PW13_ASK_PATCH_STOP,
  PW13_ASK_PATCH_PUMP,
  PW13_ASK_PW_START,
  PW13_ASK_PW_STOP,
  PW13_ASK_PW_PUMP  
} pw13_cluster_msg_id;


typedef struct {
  Pw13_Time *temps;
} pw13_cluster_ask_pump;


typedef struct {
  char * classpath;
  const char * name;
} pw13_cluster_create_patch;


typedef struct {
  Pw13_Patch *patch;
} pw13_cluster_destroy_patch;


typedef struct {
  Pw13_Output *from;
  Pw13_Input *to;
} pw13_cluster_connect_local;

typedef struct {
  Pw13_Patch *source;
  char* output_id;
  char * dest_serv;
  Pw13_Patch *dest; /* ? */
  char* input_id;
} pw13_cluster_connect_distant;


typedef struct {
  Pw13_Patch *dest;
  char* input_id;
} pw13_cluster_bind_patch_input;
  

typedef struct {
  Pw13_Patch *patch;
  Pw13_List *succ;
  Pw13_List *pred;
} s_succ_pred;


typedef struct {
  Pw13_Output *o;
  int* socket;
} s_output_liste; /* type de pw13_liste succ */
  

typedef struct {
  Pw13_Input *i;
  int* socket;
} s_input_liste; /* type de pw13_list pred */
  

typedef struct {
  Pw13_Output *o;
  int* socket;
} s_output_liste_local; 


typedef struct {
  Pw13_Input *i;
  int* socket;
} s_input_liste_local;


#endif /* _TYPE_MESSAGE_H_INCLUDED_ */
