#
# Copyright 2018-2019, Intel Corporation
#
# Redistribution and use in source and binary forms, with or without
# modification, are permitted provided that the following conditions
# are met:
#
#     * Redistributions of source code must retain the above copyright
#       notice, this list of conditions and the following disclaimer.
#
#     * Redistributions in binary form must reproduce the above copyright
#       notice, this list of conditions and the following disclaimer in
#       the documentation and/or other materials provided with the
#       distribution.
#
#     * Neither the name of the copyright holder nor the names of its
#       contributors may be used to endorse or promote products derived
#       from this software without specific prior written permission.
#
# THIS SOFTWARE IS PROVIDED BY THE COPYRIGHT HOLDERS AND CONTRIBUTORS
# "AS IS" AND ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT
# LIMITED TO, THE IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR
# A PARTICULAR PURPOSE ARE DISCLAIMED. IN NO EVENT SHALL THE COPYRIGHT
# OWNER OR CONTRIBUTORS BE LIABLE FOR ANY DIRECT, INDIRECT, INCIDENTAL,
# SPECIAL, EXEMPLARY, OR CONSEQUENTIAL DAMAGES (INCLUDING, BUT NOT
# LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS OR SERVICES; LOSS OF USE,
# DATA, OR PROFITS; OR BUSINESS INTERRUPTION) HOWEVER CAUSED AND ON ANY
# THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT LIABILITY, OR TORT
# (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY OUT OF THE USE
# OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF SUCH DAMAGE.

cmake_minimum_required(VERSION 3.3)
project(vmemcache C)

add_cstyle(src)
add_check_whitespace(src)

set(SOURCES
	out.c
	os_posix.c
	os_thread_posix.c
	util.c
	util_posix.c
	file.c
	file_posix.c
	mmap.c
	mmap_posix.c
	libvmemcache.c
	critnib.c
	ringbuf.c
	vmemcache.c
	vmemcache_heap.c
	vmemcache_index.c
	vmemcache_repl.c)

add_library(vmemcache SHARED ${SOURCES})
target_link_libraries(vmemcache PRIVATE
	${CMAKE_THREAD_LIBS_INIT}
	-Wl,--version-script=${CMAKE_SOURCE_DIR}/src/libvmemcache.map)
set_target_properties(vmemcache PROPERTIES SOVERSION 0)

target_compile_definitions(vmemcache PRIVATE SRCVERSION="${VERSION}")

if(STATS_ENABLED)
	target_compile_definitions(vmemcache PRIVATE STATS_ENABLED=1)
endif()

if(VALGRIND_FOUND)
	target_compile_definitions(vmemcache PRIVATE VALGRIND_ENABLED=1)
endif()

install(TARGETS vmemcache
	LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
	DESTINATION ${CMAKE_INSTALL_LIBDIR}/)
install(FILES libvmemcache.h DESTINATION ${CMAKE_INSTALL_INCLUDEDIR})
