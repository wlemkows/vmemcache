/*
 * Copyright 2019, Intel Corporation
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 *     * Redistributions of source code must retain the above copyright
 *       notice, this list of conditions and the following disclaimer.
 *
 *     * Redistributions in binary form must reproduce the above copyright
 *       notice, this list of conditions and the following disclaimer in
 *       the documentation and/or other materials provided with the
 *       distribution.
 *
 *     * Neither the name of the copyright holder nor the names of its
 *       contributors may be used to endorse or promote products derived
 *       from this software without specific prior written permission.
 *
 * THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
 * "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
 * LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
 * A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
 * OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
 * SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
 * LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
 * DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
 * THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
 * (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
 * OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.
 */

#include <libvmemcache.h>
#include <stdio.h>
#include <string.h>

#define STR_AND_LEN(x) (x), strlen(x)

static VMEMcache *cache;

static void
on_miss(VMEMcache *cache, const void *key, size_t key_size, void *arg)
{
	vmemcache_put(cache, STR_AND_LEN("meow"),
		STR_AND_LEN("Cthulhu fthagn"));
}

static void
get(const char *key)
{
	char buf[128];
	ssize_t len = vmemcache_get(cache, STR_AND_LEN(key),
		buf, sizeof(buf), 0, NULL);
	if (len >= 0)
		printf("%.*s\n", (int)len, buf);
	else
		printf("(not found)\n");
}

int
main()
{
	cache = vmemcache_new("/tmp", 1048576, 256, VMEMCACHE_REPLACEMENT_LRU);

	/* Query a non-existent key. */
	get("meow");

	/* Insert then query. */
	vmemcache_put(cache, STR_AND_LEN("bark"), STR_AND_LEN("Lorem ipsum"));
	get("bark");

	/* Install an on-miss handler. */
	vmemcache_callback_on_miss(cache, on_miss, 0);
	get("meow");

	vmemcache_delete(cache);
	return 0;
}
