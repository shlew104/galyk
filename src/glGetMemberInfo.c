#include "glCommon.h"
#include "glSelect.h"
#include "glGetMemberInfo.h"

/* Get all MemberInfo */
glStatus glGetMemberInfo( SQLHDBC aDbc, glDsnInfo * aDsnInfo, int aNodeIndex )
{
    glDsnInfo * sDsnInfo = aDsnInfo;

    sDsnInfo->mMemberId = aNodeIndex;  /* Member Id 는 입력받은 순서대로 0부터 */

    GL_TRY( glGetScn( aDbc, sDsnInfo, aNodeIndex ) == GL_SUCCESS );

    return GL_SUCCESS;

    GL_FINISH;

    perror("Error while getting member info");

    return GL_FAILURE;
}

/* Get Scn */
glStatus glGetScn( SQLHDBC aDbc, glDsnInfo * aDsnInfo, int aNodeIndex )
{
    char   sDataBuffer[BUF_LEN] = "";
    char * sTempScn;

    GL_TRY( glSelectScn( aDbc, sDataBuffer ) == GL_SUCCESS );
	
    sTempScn = strtok( sDataBuffer, "." );
    GL_TRY( sTempScn != NULL );
    sscanf( sTempScn, "%d", &aDsnInfo[aNodeIndex].mGlobalScn );
	
    sTempScn = strtok( NULL, "." );
    GL_TRY( sTempScn != NULL );
    sscanf( sTempScn, "%d", &aDsnInfo[aNodeIndex].mDomainScn );

    return GL_SUCCESS;

    GL_FINISH;

    perror("Error while getting scn");

    return GL_FAILURE;
}

/* Print MemberInfo */
glStatus glPrintMemberInfo( glDsnInfo * aDsnInfo, int aNodeIndex )
{
    if( aDsnInfo != NULL )
    {
        printf( "=============================================================\n" );
        printf( "[ %d ]\n", aNodeIndex + 1 );
        printf( "Member Name: %s\n", aDsnInfo[aNodeIndex].mMemberName );
        printf( "Member ID: %d\n", aDsnInfo[aNodeIndex].mMemberId );
        printf( "Status: %s\n", aDsnInfo[aNodeIndex].mStatus );
        printf( "Global Scn: %d\n", aDsnInfo[aNodeIndex].mGlobalScn );
        printf( "Domain Scn: %d\n", aDsnInfo[aNodeIndex].mDomainScn );
        printf( "=============================================================\n" );
        printf( "\n" );
    }
    else
    {
        printf( "Invalid member information.\n" );
    }

    return GL_SUCCESS;
}
