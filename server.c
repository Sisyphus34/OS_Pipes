/* CPSC/ECE 3220 server for client server
 *
 * simple server that maintains a counter for each client
 *
 * request    meaning              reply
 * -------    -------              -----
 *   '0'      clear counter        counter value
 *   '+'      increment counter    counter value
 *   '-'      decrement counter    counter value
 *   '!'      stop                 <no reply>
 *
 * there is a timeout of 2 seconds for the call to select()
 */

#include "client_server.h"


int counters[MAX_CLIENTS];



/* helper functions can be defined here and used below */



void server( int client_count, int req_fds[][2], int reply_fds[][2] ){

  fd_set read_fds;        /* file descriptor set for active clients      */
  fd_set query_fds;       /* file descriptor set passed to select()      */
  struct timeval timeout; /* time value structure for select() timeout   */
  int active_clients;     /* current count of clients; end server when 0 */
  int max_fds = 0;        /* must be one more than largest req_fd value  */
  int retval;             /* return value from select ()                 */
  int found;              /* logical flag that a req_fd was identified   */
  int reply;              /* counter value sent as a reply               */
  char request;           /* single-character command sent to server     */
  int i;


  /* initially clear the file descriptor set for active clients */
    FD_ZERO(&read_fds);
    FD_Set(&read_fds);



  /* initialize for each client:            */
  /* - close unused ends of  pipes          */
  /* - set req_fd in the read_fds structure */
  /* - update max_fds if necessary          */
  /* - clear counter for client             */



  /* set timeout value for 2 seconds */

  timeout.tv_sec = 2;
  timeout.tv_usec = 0;

  /* display information about server and file descriptors */

  printf( "server with %d clients, which have file descriptors\n",
    client_count );
  for( i = 0; i < client_count; i++ ){
    printf( "  %d %d %d %d\n",
      req_fds[i][0], req_fds[i][1], reply_fds[i][0], reply_fds[i][1] );
  }

  /* start processing with all clients active and continue until 0; */
  /*   stop requests should decrement the count of active clients   */

  active_clients = client_count;
  while( active_clients ){



  }

  /* close the other file descriptors */



  exit( 0 );
}
