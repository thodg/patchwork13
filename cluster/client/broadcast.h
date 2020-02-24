#ifndef PW13CC_BROADCAST_H_INCLUDED
#define PW13CC_BROADCAST_H_INCLUDED 1

#include "servlist.h"

int pw13cc_Bcast_init ();
void * pw13cc_Bcast (void *serverlist,
		     void (*callback)(pident,void*),
		     void *param);


#endif
/* ndef PW13CC_BROADCAST_H_INCLUDED */
