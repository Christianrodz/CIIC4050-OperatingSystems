#include  <stdio.h>
#include  <sys/types.h>
#include  <signal.h>


void SIGINT_handler(int sig){
    printf("Received signal SIGINT!!!\n");
}
void SIGQUIT_handler(int sig){
    printf("Received signal SIGQUIT!!!\n");
}
void SIGTERM_handler(int sig){
    printf("Received signal SIGTERM!!!\n");
}
void SIGFPE_handler(int sig){
    printf("Received signal SIGFPE!!!\n");
}
void SIGSEGV_handler(int sig){
    printf("Received signal SIGSEV!!!\n");
}
void SIGILL_handler(int sig){
    printf("Received signal SIGILL!!!\n");
}


void setup_signal_handlers() {
    // Setup handlers here
    if(signal (SIGINT, SIGINT_handler) == SIG_IGN){
        signal(SIGINT, SIG_IGN);
    }
    if(signal (SIGQUIT, SIGQUIT_handler) == SIG_IGN){
        signal(SIGQUIT, SIG_IGN);
    }
    if(signal (SIGTERM, SIGTERM_handler) == SIG_IGN){
        signal (SIGTERM, SIG_IGN);
    }
    if(signal (SIGFPE, SIGFPE_handler) == SIG_IGN){
        signal(SIGFPE, SIG_IGN);
    }
    if(signal (SIGSEGV,SIGSEGV_handler) == SIG_IGN){
        signal(SIGSEGV,SIG_IGN);
    }
    if(signal (SIGILL, SIGILL_handler) == SIG_IGN){
        signal(SIGILL, SIG_IGN);
    }

}