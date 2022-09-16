#ifndef _CELLPROD_H_
#define _CELLPROD_H_

#include "RTDS_Common.h"
#include "RTDS_CommonTypes.h"
#include "RTDS_String.h"
#include "RTDS_Set.h"


#define NBRPIECES 10

typedef RTDS_PID machinesArray[RTDS_INT_MAX - RTDS_INT_MIN];

int RTDS_machinesArray_cmp(machinesArray, machinesArray);

#define NBRMACHINES 3



#endif
