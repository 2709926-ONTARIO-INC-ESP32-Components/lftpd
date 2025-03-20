#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <stdarg.h>
#include <stdbool.h>
#include <unistd.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <dirent.h>
#include <errno.h>
#include <sys/stat.h>

#include "private/lftpd_io.h"

char* lftpd_io_canonicalize_path(const char* base, const char* name) {
	// if either argument is null, treat it as empty
	if (base == NULL) {
		base = "";
	}
	if (name == NULL) {
		name = "";
	}

	// if name is absolute, ignore the base and use name as the
	// full path
	char* path = NULL;
	if (name[0] == '/') {
		path = strdup(name);
	}
	// otherwise append name to base with / as a separator
	else {
		int err = asprintf(&path, "%s/%s", base, name);
		if (err < 0) {
			return NULL;
		}
	}

	// allocate enough room for the absolute path, which can never be
	// longer than the path, plus 1 for a / and 1 for the terminator
	size_t abs_path_len = strlen(path) + 2;
	char* abs_path = malloc(abs_path_len);
	if (abs_path == NULL) {
        free(path);
        return NULL;
    }
	memset(abs_path, 0, abs_path_len);

	// run through the path a segment at a time, adding each to
	// abs_path with with a preceding / and resolving . and ..
	char* save_pointer = NULL;
	char* token = strtok_r(path, "/", &save_pointer);
	while (token) {
		if (strncmp(token, ".", 2) == 0) {
			// ignore it
		}
		else if (strncmp(token, "..", 3) == 0) {
			// go back one element
			char* p = strrchr(abs_path, '/');
			if (p != NULL) {
				p[0] = '\0';
			}
		}
		else {
			strncat(abs_path, "/", abs_path_len - strlen(abs_path) - 1);
			strncat(abs_path, token, abs_path_len - strlen(abs_path) - 1);
		}

		token = strtok_r(NULL, "/", &save_pointer);
	}
	free(path);

	// a path like /test/.. might have removed everything and left
	// an empty path, so detect that condition and fix it
	if (strlen(abs_path) == 0) {
		strncpy(abs_path, "/", abs_path_len - 1);
        abs_path[abs_path_len - 1] = '\0';
	}
	
	return abs_path;
}