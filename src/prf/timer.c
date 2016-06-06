/* timing function: difference between two calls is processor time
 * spent by your code (in seconds) 
 * hopefully portable, but surely there are systems it doesn't work on.
<<<<<<< timer.c
 * $Id: timer.c,v 1.3 2016/04/11 21:19:54 ssamal Exp $ 
=======
 * $Id: timer.c,v 1.1.1.1 2016/04/10 19:25:33 ssamal Exp $ 
>>>>>>> 1.1.1.1
 */

#include <sys/time.h>
#include <sys/resource.h>
#include <unistd.h>

#include "timer.h"

float timer ()
{
  struct rusage r;

  getrusage(0, &r);
  return (float)(r.ru_utime.tv_sec+r.ru_utime.tv_usec/(float)1000000);
}

