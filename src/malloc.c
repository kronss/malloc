#include <pthread.h>
#include <stdio.h>
#include <unistd.h>


pthread_mutex_t mutex_malloc = PTHREAD_MUTEX_INITIALIZER;

//
//int main()
//{
//	printf("%d", PAGE_SIZE);
//}




void *malloc(size_t size)
{
	void *retval;

	pthread_mutex_lock(&mutex_malloc);

	if (size <= 0) {
		pthread_mutex_unlock(&mutex_malloc);
		return (void *)0x0L;
	}

	if (size < (size_t)-32) {
		ALIGN_META_INFO(size);
	}


	if () {

	} else if () {

	} else {

	}



	pthread_mutex_unlock(&mutex_malloc);
	if (NULL == retval) {
//		errno = ENOMEM;

	}
)
	return (ptr);
}
