#include "socket.h"
#include "threadpool.h"
#include "logging.h"
#include "utilsCompat.h"
#include "error.h"
#include "files.h"

#include <stdlib.h>
#include <inttypes.h>

#define THREADS 3

static void thread_func(void *args, int id) {
	logf("Thread %d is doing stuff...\n", id);
	SLEEP(1000);
}

int main() {
	ThreadPool *tp = threadpool_create(THREADS);
	for(int i = 0; i < 2; i++)
		threadpool_add_task(tp, &thread_func, NULL);

	SLEEP(4000);
	threadpool_add_task(tp, &thread_func, NULL);
	threadpool_add_task(tp, &thread_func, NULL);
	threadpool_add_task(tp, &thread_func, NULL);
	threadpool_destroy(tp, SOFT_SHUTDOWN);

	socket_startup();

	Socket s = socket(AF_INET , SOCK_STREAM , 0);
	if(!is_socket_valid(s)) {
		perr_sock("error while creating socket");
		exit(1);
	}
	if(socket_close(s)) {
		perr_sock("error while closing socket");
		exit(1);
	}

	socket_cleanup();

	fsize_t fsize;
	if(get_file_size("/home/fabrizio/socket.tgz", &fsize)) {
		perr("Error: get_file_size");
		exit(1);
	}
	printf("%"PRIuMAX"\n", (uintmax_t) fsize);
}
