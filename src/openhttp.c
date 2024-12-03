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

// --- START ---
#ifndef __linux__
#error "OpenHTTP only supports Linux as of now."
#endif // __linux__

// ------- GLOBALS --------------------
static __thread int _openhttp_last_error_code = OPENHTTP_SUCCESS;
static __thread const char *_openhttp_last_error_message = "Success";

// ------- SERVER ---------------------
int openhttp_server_spawn(int port)
{
    return OPENHTTP_SYSTEM_PREFIX(server_spawn)(port);
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
