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

// Define the size of reply to 1 byte
#define REQUEST_SZ 1

// Define the size of reply to 4 bytes
#define REPLY_SZ 4

int counters[MAX_CLIENTS];

/* helper functions can be defined here and used below */

void server(int client_count, int req_fds[][2], int reply_fds[][2])
{

  fd_set read_fds;        /* file descriptor set for active clients      */
  fd_set query_fds;       /* file descriptor set passed to select()      */
  struct timeval timeout; /* time value structure for select() timeout   */
  int active_clients;     /* current count of clients; end server when 0 */
  int max_fds = 0;        /* must be one more than largest req_fd value  */
  int retval;             /* return value from select ()                 */
  // int found;              /* logical flag that a req_fd was identified   */ REMOVED BECAUSE UNNECESSARY
  int reply;              /* counter value sent as a reply               */
  char request;           /* single-character command sent to server     */
  int i;

  /* initially clear the file descriptor set for active clients */

  FD_ZERO(&read_fds);
  FD_ZERO(&query_fds);

  /* initialize for each client:            */
  /* - close unused ends of  pipes          */
  /* - set req_fd in the read_fds structure */
  /* - update max_fds if necessary          */
  /* - clear counter for client             */

  for (i = 0; i < client_count; i++)
  {
    FD_SET(req_fds[i][READ_FD], &read_fds);
    close(req_fds[i][WRITE_FD]);
    close(reply_fds[i][READ_FD]);
    max_fds = req_fds[i][0];
    counters[i] = 0;
    if (req_fds[i][0] > max_fds)
    {
      max_fds = reply_fds[i][0];
    }
    max_fds += 1;
  }

  /* display information about server and file descriptors */

  printf("server with %d clients, which have file descriptors\n",
         client_count);
  for (i = 0; i < client_count; i++)
  {
    printf("  %d %d %d %d\n",
           req_fds[i][0], req_fds[i][1], reply_fds[i][0], reply_fds[i][1]);
  }

  /* start processing with all clients active and continue until 0; */
  /*   stop requests should decrement the count of active clients   */

  timeout.tv_sec = 2;
  timeout.tv_usec = 0;

  active_clients = client_count;
  reply = 0;

  // printf("This is the active clients %d\n", active_clients);
  while (active_clients)
  {
    query_fds = read_fds;

    retval = select(max_fds, &query_fds, NULL, NULL, &timeout);

    if (retval == -1)
    {
      printf("error code returned from select()\n");
      active_clients = 0;
    }
    else if (retval)
    {
      for (i = 0; i < client_count; i++)
      {
        if (FD_ISSET(req_fds[i][0], &query_fds))
        {
          read(req_fds[i][0], &request, REQUEST_SZ);
          printf("message from client %d to server is %c\n", i, request);

          if (request == '+')
          {
            counters[i]++;
            reply = counters[i];
            write(reply_fds[i][1], (char *)&reply, REPLY_SZ);
          }
          else if (request == '-')
          {
            counters[i]--;
            reply = counters[i];
            write(reply_fds[i][1], (char *)&reply, REPLY_SZ);
          }
          else if (request == '!')
          {

            active_clients--;
            FD_CLR(req_fds[i][READ_FD], &query_fds);
          }
          else
          {
            counters[i] = 0;
            active_clients--;
            reply = counters[i];
            write(reply_fds[i][1], (char *)&reply, REPLY_SZ);
            printf("message from client %d to server is %c\n", i, request);
          }
        }
      }
    }
    else
    {
      active_clients = 0;
      printf("timeout returned from select()\n");
    }
  }
  /* close the other file descriptors */
  for (i = 0; i < client_count; i++)
  {
    close(req_fds[i][READ_FD]);
    close(reply_fds[i][WRITE_FD]);
  }
  exit(0);
}
