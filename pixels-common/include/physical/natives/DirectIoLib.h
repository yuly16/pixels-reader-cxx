//
// Created by yuly on 19.04.23.
//

#ifndef DUCKDB_DIRECTIOLIB_H
#define DUCKDB_DIRECTIOLIB_H

/**
 * Mapping Linux I/O functions to native methods.
 * Partially referenced the implementation of Jaydio (https://github.com/smacke/jaydio),
 * which is implemented by Stephen Macke and licensed under Apache 2.0.
 * <p>
 * Created at: 02/02/2023
 * Author: Liangyong Yu
 */

class DirectIoLib {
public:
	static int read(int fd, long fileOffset, char * buffer, int length);
};

#endif // DUCKDB_DIRECTIOLIB_H
