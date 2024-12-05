/*
 * openhttp.c
 *
 * Name: openhttp
 * Description: Open-source HTTP server written in pure C
 * Version: 1.0.0
 *
 * Author: Kevin Alavik <kevin@alavik.se>
 * Year: 2024
 *
 * This file serves as the entry point for the OpenHTTP server.
 *
 * Contributors:
 *  - Kevin Alavik <kevin@alavik.se>
 */

#include <openhttp.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

// --- START ---
#ifndef __linux__
#error "OpenHTTP only supports Linux as of now."
#endif // __linux__

// ------- GLOBALS --------------------
static __thread int _openhttp_last_error_code = OPENHTTP_SUCCESS;
static __thread const char *_openhttp_last_error_message = "Success";

// ------- SERVER ---------------------
int openhttp_server_spawn(openhttp_server_t *server, int port, _openhttp_write_callback callback)
{
    server->_callback = callback;
    return OPENHTTP_SYSTEM_PREFIX(server_spawn)(server, port, OPENHTTP_SYSTEM_PREFIX(server_callback));
}

int openhttp_cleanup()
{
    return OPENHTTP_SYSTEM_PREFIX(cleanup)();
}

int openhttp_write(const char *data)
{
    return OPENHTTP_SYSTEM_PREFIX(write_callback)(data);
}

char *openhttp_generate_response(const char *code, const char *file_path)
{
    if (!code || !file_path)
    {
        return NULL;
    }

    const char *mime_type = "application/octet-stream";
    const char *ext = strrchr(file_path, '.');
    if (ext)
    {
        if (strcmp(ext, ".html") == 0 || strcmp(ext, ".htm") == 0)
            mime_type = "text/html; charset=UTF-8";
        else if (strcmp(ext, ".css") == 0)
            mime_type = "text/css";
        else if (strcmp(ext, ".js") == 0)
            mime_type = "application/javascript";
        else if (strcmp(ext, ".json") == 0)
            mime_type = "application/json";
        else if (strcmp(ext, ".png") == 0)
            mime_type = "image/png";
        else if (strcmp(ext, ".jpg") == 0 || strcmp(ext, ".jpeg") == 0)
            mime_type = "image/jpeg";
        else if (strcmp(ext, ".gif") == 0)
            mime_type = "image/gif";
        else if (strcmp(ext, ".txt") == 0)
            mime_type = "text/plain; charset=UTF-8";
    }

    FILE *file = fopen(file_path, "rb");
    if (!file)
    {
        _openhttp_raise_error(OPENHTTP_SYSTEM_ERROR, "Failed to open file for generating HTTP response.");
        return NULL;
    }

    fseek(file, 0, SEEK_END);
    size_t file_size = ftell(file);
    rewind(file);

    char *file_content = (char *)malloc(file_size);
    if (!file_content)
    {
        fclose(file);
        _openhttp_raise_error(OPENHTTP_SYSTEM_ERROR, "Failed to allocate memory for file content.");
        return NULL;
    }
    fread(file_content, 1, file_size, file);
    fclose(file);

    const char *header_template = "HTTP/1.1 %s\r\n"
                                  "Content-Length: %zu\r\n"
                                  "Content-Type: %s\r\n"
                                  "\r\n";
    size_t header_size = snprintf(NULL, 0, header_template, code, file_size, mime_type);
    size_t total_size = header_size + file_size;

    char *response = (char *)malloc(total_size + 1);
    if (!response)
    {
        free(file_content);
        _openhttp_raise_error(OPENHTTP_SYSTEM_ERROR, "Failed to allocate memory for HTTP response.");
        return NULL;
    }

    snprintf(response, header_size + 1, header_template, code, file_size, mime_type);
    memcpy(response + header_size, file_content, file_size);
    response[total_size] = '\0';

    free(file_content);
    return response;
}
// -------- DEBUG ---------------------
int _openhttp_loopback(int a)
{
    return a;
}

void _openhttp_raise_error(int error_code, const char *error_message)
{
    _openhttp_last_error_code = error_code;
    _openhttp_last_error_message = error_message;
}

const char *openhttp_error(void)
{
    return _openhttp_last_error_message;
}

// --- END ---

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
