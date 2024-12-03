#include <stdio.h>
#include <stdlib.h>
#include <signal.h>
#include <openhttp/openhttp.h>

static volatile int running = 1;

void handle_signal(int sig)
{
    openhttp_cleanup();
    exit(0);
}

int main(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGABRT, handle_signal);
    signal(SIGQUIT, handle_signal);

    printf("OpenHTTP v%s\n", OPENHTTP_VERSION_STRING);

    if (openhttp_server_spawn(6969) != OPENHTTP_SUCCESS)
    {
        fprintf(stderr, "Error: %s\n", openhttp_error());
        return 1;
    }

    return 0;
}
