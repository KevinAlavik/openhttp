/*
 * linux.c
 *
 * Name: openhttp
 * Description: Open-source HTTP server written in pure C
 * Version: 1.0.0
 *
 * Author: Kevin Alavik <kevin@alavik.se>
 * Year: 2024
 *
 * This file contains linux-specific code for the OpenHTTP server.
 *
 * Contributors:
 *  - Kevin Alavik <kevin@alavik.se>
 */

#include <openhttp.h>
#include <stdlib.h>
#include <stdio.h>

#ifdef __linux__

int _openhttp_linux_server_spawn(int _port)
{
    _openhttp_raise_error(OPENHTTP_UNKNOWN_ERROR, "Server spawning not supported on Linux");
    return OPENHTTP_UNKNOWN_ERROR;
}

#endif // __linux__

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
