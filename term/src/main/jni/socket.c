/*
 * Copyright (C) 2019 Roumen Petrov.  All rights reserved.
 *
 * Licensed under the Apache License, Version 2.0 (the "License");
 * you may not use this file except in compliance with the License.
 * You may obtain a copy of the License at
 *
 *      http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing, software
 * distributed under the License is distributed on an "AS IS" BASIS,
 * WITHOUT WARRANTIES OR CONDITIONS OF ANY KIND, either express or implied.
 * See the License for the specific language governing permissions and
 * limitations under the License.
 */

#include <memory.h>
#include <string.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <sys/un.h>
#include <unistd.h>

#include "appinfo.h"

int open_socket(const char *name) {
	int sock;
	struct sockaddr_un addr;
	size_t namelen;
	socklen_t addrlen;

	sock = socket(AF_UNIX, SOCK_STREAM, 0);
	if (sock == -1) {
		return -1;
	}

	memset(&addr, 0, sizeof(addr));
	addr.sun_family = AF_UNIX;

	namelen = strlen(name);
	if (namelen >= sizeof(addr.sun_path))
		namelen = sizeof(addr.sun_path) - 1;
	strncpy(addr.sun_path + 1, name, namelen);
	addrlen = (socklen_t)offsetof(struct sockaddr_un, sun_path) + 1 +
	          (socklen_t)namelen;

	if (connect(sock, (const struct sockaddr *)&addr, addrlen) == -1) {
		close(sock);
		return -1;
	}

	return sock;
}
