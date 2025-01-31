#ifndef GL_GET_MEMBERINFO_H
#define GL_GET_MEMBERINFO_H

/* MACRO */
#define MAX_SCN_LENGTH 64

/* function declaration */
/* Get MemberInfo */
glStatus glGetMemberInfo( SQLHDBC aDbc, glDsnInfo * aDsnInfo, int aNodeIndex );
glStatus glGetScn( SQLHDBC aDbc, glDsnInfo * aDsnInfo, int aNodeIndex );
/* Print MemberInfo */
glStatus glPrintMemberInfo( glDsnInfo * aDsnInfo, int aNodeIndex );

#endif
