/*
 * out_of_memory.c                                 (c) 2001 Staf Wagemakers
 */

#include "out_of_memory.h"
void out_of_memory(void) 
{
   print_txt_msg("[CGIpaf]: out of memory\n");
   write_log(LOG_USER,LOG_EMERG,"out of memory");
   exit(1);
}
