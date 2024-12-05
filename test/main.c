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

int callback(openhttp_server_t *server, const char *request)
{
    (void)server;

    char *response = openhttp_generate_response("200 OK", "test.html");
    if (response == NULL)
    {
        fprintf(stderr, "Error: %s\n", openhttp_error());
        return OPENHTTP_SYSTEM_ERROR;
    }

    openhttp_write(response);
    free(response);
    return OPENHTTP_SUCCESS;
}

int main(void)
{
    signal(SIGINT, handle_signal);
    signal(SIGTERM, handle_signal);
    signal(SIGABRT, handle_signal);
    signal(SIGQUIT, handle_signal);

    printf("OpenHTTP v%s\n", OPENHTTP_VERSION_STRING);

    openhttp_server_t *server = (openhttp_server_t *)malloc(sizeof(openhttp_server_t));
    if (server == NULL)
    {
        fprintf(stderr, "Error: Failed to allocate memory for server\n");
        return 1;
    }

    if (openhttp_server_spawn(server, 8080, callback) != OPENHTTP_SUCCESS)
    {
        fprintf(stderr, "Error: %s\n", openhttp_error());
        free(server);
        return 1;
    }

    free(server);
    return 0;
}
