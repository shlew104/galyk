#include "glCommon.h"
#include "glSelect.h"
#include "glGetMemberInfo.h"
#include "glCompareScn.h"

/* Init MemberInfo */
glStatus glInitMemberInfo( glDsnInfo * aDsnInfo, int aMaxNodeNum )
{
    int sLoopCount = 0;

    for( sLoopCount = 0; sLoopCount < aMaxNodeNum; sLoopCount++ )
    {
        memset( &aDsnInfo[sLoopCount], 0x00, sizeof( glDsnInfo ) );
    }

    return GL_SUCCESS;
}

int main( int aArgc, char ** aArgv )
{
    /* Define */
    int         sNodeIndex        = 0;
    int         sMaxNodeNum       = 0;
    int         sMaxScnNodeIndex  = 100;
    glDsnInfo * sDsnInfo          = NULL;
    
    /* Get MaxNodeCount */
    sMaxNodeNum = 2;

    /* Declare MemberInfo */
    sDsnInfo = ( glDsnInfo * ) malloc( sizeof( glDsnInfo ) * sMaxNodeNum );

    /* Init MemberInfo */
    glInitMemberInfo( sDsnInfo, sMaxNodeNum );

    /* Connection */
    SQLHENV    sEnv = NULL;
    SQLHDBC    sDbc = NULL;

    SQLAllocHandle( SQL_HANDLE_ENV,
                    NULL,
                    &sEnv );

    SQLSetEnvAttr( sEnv,
                   SQL_ATTR_ODBC_VERSION,
                   (SQLPOINTER)SQL_OV_ODBC3,
                   0 );

    SQLAllocHandle( SQL_HANDLE_DBC,
                    sEnv,
                    &sDbc );

    /* G1N1 */
    SQLConnect( sDbc,
                (SQLCHAR*)"G1N1",
                SQL_NTS,
                (SQLCHAR*)"sys",
                SQL_NTS,
                (SQLCHAR*)"gliese",
                SQL_NTS );

    /* Get MemberInfo */
    sNodeIndex = 0;
    glGetMemberInfo( sDbc, sDsnInfo, sNodeIndex );
    glPrintMemberInfo( sDsnInfo, sNodeIndex );
    
    SQLDisconnect( sDbc );
	
    /* G1N2 */
    SQLConnect( sDbc,
                (SQLCHAR*)"G1N2",
                SQL_NTS,
                (SQLCHAR*)"sys",
                SQL_NTS,
                (SQLCHAR*)"gliese",
                SQL_NTS );

    /* Set MemberInfo */
    sNodeIndex = 1;
    glGetMemberInfo( sDbc, sDsnInfo, sNodeIndex );
    glPrintMemberInfo( sDsnInfo, sNodeIndex );

    SQLDisconnect( sDbc );

    /* Compare Scn */
    glCompareScn( sDsnInfo, sMaxNodeNum, &sMaxScnNodeIndex ); 
	printf( "MaxScnNodeIndex : %d", sMaxScnNodeIndex + 1 );

    /* Free */
    SQLFreeHandle( SQL_HANDLE_DBC, sDbc );
    SQLFreeHandle( SQL_HANDLE_ENV, sEnv );

    free( sDsnInfo );
    sDsnInfo = NULL;

    return 0;	
}
