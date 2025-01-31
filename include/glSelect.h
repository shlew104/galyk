#ifndef GL_SELECT_H
#define GL_SELECT_H

#include "glCommon.h"

#define BUF_LEN 256

glStatus glSelectOneValue( SQLHDBC aDbc, char * aSql, char * aDataBuffer);

glStatus glSelectMemberName( SQLHDBC aDbc, char * aDataBuffer );

glStatus glSelectScn( SQLHDBC aDbc, char * aDataBuffer );

glStatus glSelectStatus( SQLHDBC aDbc, char * aDataBuffer);

#endif
