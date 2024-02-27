#ifndef PRINT_CONTROL_H
#define PRINT_CONTROL_H

#define DEBUG_MSG_ENABLE

#ifdef DEBUG_MSG_ENABLE

#define PRINT_DEBUG_MSG(MSG,args...) fprintf(stderr,MSG,##args);

#else

#define PRINT_DEBUG_MSG(MSG,...) //EMPTY

#endif

#endif
