#ifndef GL_STARTUP_H
#define GL_STARTUP_H

#include "glCommon.h"

#define BUF_LEN 256

glStatus glStartupMount ( SQLHDBC aDbc, char * aDataBuffer );

glStatus glStartupLocalOpen ( SQLHDBC aDbc, char * aDataBuffer );

glStatus glStartupGlobalOpen ( SQLHDBC aDbc, char * aDataBuffer );

#endif
