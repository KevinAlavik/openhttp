/*
 * main.c
 *
 * Name: openhttp
 * Description: Test file for OpenHTTP library
 * Version: 1.0.0
 *
 * Author: Kevin Alavik <kevin@alavik.se>
 * Year: 2024
 *
 * This file is used for testing the OpenHTTP library. It includes basic tests
 * for checking the initialization of the OpenHTTP server and ensures that
 * the functions are linked correctly. More advanced tests should be added as
 * the implementation of OpenHTTP progresses.
 */

#include <stdio.h>
#include <openhttp/openhttp.h>

int main(void)
{
    printf("OpenHTTP v%s\n", OPENHTTP_VERSION_STRING);

    if (openhttp_server_spawn(8080) != OPENHTTP_SUCCESS)
    {
        fprintf(stderr, "Failed to spawn OpenHTTP server, ERROR: %s\n", openhttp_error());
        return 1;
    }

    return 0;
}
