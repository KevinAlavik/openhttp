/*
 * openhttp.h
 *
 * Name: openhttp
 * Description: Open-source HTTP server written in pure C
 * Version: 1.0.0
 *
 * Usage:
 * To compile a program with the openhttp library, use the following:
 *
 *   gcc -o myprogram myprogram.c -L/usr/local/lib -lopenhttp
 *
 * Or, use pkg-config for automatic flag management:
 *
 *   gcc -o myprogram myprogram.c $(pkg-config --cflags --libs openhttp)
 *
 * To include the header file in your code:
 *
 *   #include <openhttp/openhttp.h>
 *
 * Author: Kevin Alavik <kevin@alavik.se>
 * Year: 2024
 *
 * This file is part of the OpenHTTP library.
 */

#ifndef OPENHTTP_H
#define OPENHTTP_H

/*
 * Generic standard library includes.
 */
#include <stdint.h>
#include <stddef.h>

/*
 * Information macro declarations for the OpenHTTP library.
 *
 * OPENHTTP_VERSION_STRING: The version of the OpenHTTP library.
 */
#define OPENHTTP_VERSION_STRING "1.0.0-alpha1"

/*
 * Success and error codes for the OpenHTTP library.
 *
 * OPENHTTP_SUCCESS         : The operation was successful.
 * OPENHTTP_UNKOWN_ERROR    : An unkown serror occurred.
 * OPENHTTP_SYSTEM_ERROR    : A system error occurred.
 */

#define OPENHTTP_SUCCESS 0
#define OPENHTTP_UNKNOWN_ERROR -1
#define OPENHTTP_SYSTEM_ERROR -2

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Generic server functions for the OpenHTTP library.
 *
 * Note: These functions are implemented in the openhttp.c file.
 * * * * * * * * *  * * * * * * * *  * * * * * * * *  * * * * * * */

// ------------------------ BEGIN -----------------------------
/**
 * Context structure for the OpenHTTP server.
 *
 * This structure should be allocated by the user and passed to the openhttp_server_spawn() function.
 */
typedef struct openhttp_server
{
    int (*_callback)(struct openhttp_server *, const char *request);
} openhttp_server_t;

/**
 * Callback function for handling incoming HTTP requests.
 *
 * Returns:
 * - OPENHTTP_SUCCESS if the request was handled successfully, unless an error occurred.
 */
typedef int (*_openhttp_write_callback)(openhttp_server_t *, const char *);

/**
 * Spawns a new HTTP server on the specified port.
 *
 * Returns:
 * - OPENHTTP_SUCCESS if the server was successfully spawned, unless an error
 */
int openhttp_server_spawn(openhttp_server_t *server, int port, _openhttp_write_callback callback);

/**
 * Cleans up any resources allocated by the OpenHTTP library.
 *
 * Returns:
 * - OPENHTTP_SUCCESS if the cleanup was successful, unless an error occurred.
 */
int openhttp_cleanup();

/**
 * Writes the provided data to the client.
 *
 * Returns:
 * - OPENHTTP_SUCCESS if the write was successful, unless an error occurred.
 */
int openhttp_write(const char *data);

/**
 * Generates a HTTP response based on the provided source, supports different content types.
 *
 * Returns:
 * - The generated HTTP response, or NULL if an error occurred.
 */
char *openhttp_generate_response(const char *code, const char *file_path);

// ------------------------- END ------------------------------

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * System specific functions for the OpenHTTP library.
 *
 * Note: These functions are implemented in the openhttp.c file.
 * * * * * * * * *  * * * * * * * *  * * * * * * * *  * * * * * * */

// ------------------------ BEGIN -----------------------------

/**
 * Sets the client handler function for the OpenHTTP library
 *
 * Note: System specific.
 */
#ifdef __linux__
#define OPENHTTP_SYSTEM_PREFIX(func) _openhttp_linux_##func
typedef int (*_openhttp_client_handler_t)(openhttp_server_t *, int client_fd, const char *request);
#else
#define OPENHTTP_SYSTEM_PREFIX(func) func
typedef int (*_openhttp_client_handler_t)(openhttp_server_t *, const char *request);
#endif // __linux__

/**
 * Spawns a new HTTP server on the specified port.
 *
 * Returns:
 *  - OPENHTTP_SUCCESS if the server was successfully spawned, unless an error occurred.
 */
int OPENHTTP_SYSTEM_PREFIX(server_spawn)(openhttp_server_t *, int, _openhttp_client_handler_t);

/**
 * Handles HTTP requests based on the provided request information.
 *
 * Returns:
 *  - OPENHTTP_SUCCESS if the request was handled successfully, unless an error occurred.
 */

#ifdef __linux__
int OPENHTTP_SYSTEM_PREFIX(server_callback)(openhttp_server_t *, int, const char *);
#else
int OPENHTTP_SYSTEM_PREFIX(server_callback)(openhttp_server_t *, const char *);
#endif // __linux__

/**
 * Cleans up any resources allocated by the OpenHTTP library.
 *
 * Returns:
 * - OPENHTTP_SUCCESS if the cleanup was successful, unless an error occurred.
 */
int OPENHTTP_SYSTEM_PREFIX(cleanup)(void);

/**
 * Writes the provided data to the client socket.
 *
 * Returns:
 * - OPENHTTP_SUCCESS if the write was successful, unless an error occurred.
 */
int OPENHTTP_SYSTEM_PREFIX(write_callback)(const char *);

// ------------------------- END ------------------------------

/* * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * * *
 * Debug function declarations for the OpenHTTP library.
 *
 * Note: These functions are implemented in the openhttp.c file.
 * * * * * * * * *  * * * * * * * *  * * * * * * * *  * * * * * * */

// ------------------------ BEGIN -----------------------------
/**
 * Retrieves the last error message as a human-readable string.
 *
 * Returns:
 *  - A pointer to the error message string.
 */
const char *openhttp_error(void);

/**
 * Sets the last error code and message for the current thread.
 *
 * Parameters:
 *  - error_code: The error code to set.
 *  - error_message: A human-readable error message.
 */
void _openhttp_raise_error(int error_code, const char *error_message);
// ------------------------- END ------------------------------

#endif // OPENHTTP_H

/*
 * License: MIT License
 *
 * Permission is hereby granted, free of charge, to any person obtaining a copy
 * of this software and associated documentation files (the "Software"), to deal
 * in the Software without restriction, including without limitation the rights
 * to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 * copies of the Software, and to permit persons to whom the Software is
 * provided to do so, subject to the following conditions:
 *
 * The above copyright notice and this permission notice shall be included in
 * all copies or substantial portions of the Software.
 *
 * THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 * IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 * FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 * AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 * LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 * OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN THE
 * SOFTWARE.
 */
