
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>

#include "sigrlog.h"
#include "EventsEmitter.h"

#define BUFFER_SIZE 128

bool
EventsEmitter::run() {

    if( pthread_create(&thread_, NULL, &EventsEmitter::emitFunc, this ) != 0) {
        SIGRLOG (SIGRWARNING, "EventsEmitter::run failed to thread" );
        return false;
    }
    return true;
}

void
EventsEmitter::stop() {
    SIGRLOG (SIGRDEBUG2, "EventsEmitter::stop enter");
    pthread_mutex_lock( &mutex_ );
    shutdownFlag_ = true;
    pthread_mutex_unlock( &mutex_ );
    pthread_join( thread_, NULL );
    SIGRLOG (SIGRDEBUG2, "EventsEmitter::stop thread exited");
}

void*
EventsEmitter::emitFunc(void* ptr) {
    EventsEmitter* emitter = static_cast<EventsEmitter*>(ptr);
    if(emitter == NULL)
        return NULL;

    int fd, err;
    char buffer[BUFFER_SIZE];
    bzero(buffer, BUFFER_SIZE);
    strcpy(buffer, USER_EVENT_MESSAGE);
    if( (err = mkfifo( USER_EVENT_FIFO, 0600 )) < 0 ) {
        SIGRLOG (SIGRWARNING, "EventsEmitter::emitFunc FIFO could not be created %d", err);
        return NULL;
    }
    if ( ( fd = open(USER_EVENT_FIFO, O_WRONLY) ) < 0 ) {
       SIGRLOG (SIGRWARNING, "EventsEmitter::emitFunc Could not open named pipe.");
       return NULL;
    }

    while(1) {
        bool bBreak = false;
        pthread_mutex_lock( &emitter->mutex_ );
            if( emitter->shutdownFlag_ ) bBreak = true;
        pthread_mutex_unlock( &emitter->mutex_ );
        if(bBreak) break;
        sleep(3);
        if( write (fd, buffer, strlen(buffer)+1 ) < 0 ) {
            SIGRLOG (SIGRWARNING, "EventsEmitter::emitFunc Error writing to named Pipe (%s)", USER_EVENT_FIFO);
            break;
        }
    }
    unlink(USER_EVENT_FIFO);
    close(fd);
}
