#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <goldilocks.h>
#include "glCommon.h"
#include "glMenu.h"
#include "glPrintDiagnosticRecord.h"
#include "glSelect.h"
#include "glCompareScn.h"

int main( int aArgc, char** aArgv )
{

    SQLHENV      sEnv     = NULL;
    SQLHDBC      sDbc     = NULL;

    glDsnInfo  * sDsnInfo = NULL;

    int          sModeCount   = 0;
    int          sDsnCount    = 0;
    int          sSize        = 0;
    int          sCount       = 0;
    int          sStatus      = 0;

    /******************************
     * Parsing
     ******************************/

    /*
     * 미래를 위해
     * 모드 개수가 늘어날 수록
     * sModeCount 값을 변화시켜야 함
     *
     * <현재>
     * galyk  <mode>  <option>
     * ^^^^^  ^^^^^^
     * 총 2개로, sModeCount 는 2이다.
     * 추후 기능 추가가 되면 이 값을 바꿔야 함!!!
     *
     *                     by anbo@sunjesoft.com
     */
    
    sModeCount = 2; 

    /* 
     * <mode> check
     */ 
    
    if( aArgc <= sModeCount )
    {
        GL_THROW( RAMP_INVALID_ARGUMENTS );
    }

    if( ( strcmp( "-d", aArgv[1] ) != 0 ) &&
        ( strcmp( "--dsn", aArgv[1] ) != 0  ) ) 
    {
        GL_THROW( RAMP_INVALID_ARGUMENTS );
    }

    /* 
     * 입력받은 dsn 개수만큼 malloc 
     * mode 추가에 따라 이 로직도 수정해야 함
     */
    sDsnCount = aArgc - sModeCount;
    sSize = sizeof( glDsnInfo ) * sDsnCount;

    sDsnInfo = malloc( sSize );

    memset( sDsnInfo,
            0x00,
            sSize );

    /* 입력받은 인자(들)로 dsn name 저장 */
    for( sCount = 0; sCount < sDsnCount; sCount++ )
    {
        GL_TRY( glInitDsnInfo( &sDsnInfo[sCount],
                               aArgv[sCount + sModeCount]) /* Dsn Name */
                 == GL_SUCCESS ); 
    }

    GL_TRY_THROW( SQLAllocHandle ( SQL_HANDLE_ENV,
                                   NULL,
                                   &sEnv ) == SQL_SUCCESS,
                  RAMP_ERR_ALLOC_ENV );
    sStatus = 1;

    GL_TRY( SQLSetEnvAttr ( sEnv,
                            SQL_ATTR_ODBC_VERSION,
                            (SQLPOINTER)SQL_OV_ODBC3,
                            0 ) == SQL_SUCCESS );

    GL_TRY_THROW( SQLAllocHandle ( SQL_HANDLE_DBC,
                                   sEnv,
                                   &sDbc ) == SQL_SUCCESS,
                  RAMP_ERR_ALLOC_DBC );
    sStatus = 2;

    /* Menu */
    GL_TRY( glGalykMenu( sDbc,
                         sDsnInfo,
                         sDsnCount ) == GL_SUCCESS );

    sStatus = 1;
    GL_TRY_THROW( SQLFreeHandle ( SQL_HANDLE_DBC, sDbc ) == SQL_SUCCESS,
                  RAMP_ERR_FREE_DBC );

    sStatus = 0;
    GL_TRY_THROW( SQLFreeHandle ( SQL_HANDLE_ENV, sEnv ) == SQL_SUCCESS,
                  RAMP_ERR_FREE_ENV );

    free( sDsnInfo );

    return GL_SUCCESS;

    GL_CATCH( RAMP_INVALID_ARGUMENTS )
    {
        printf("\n");
        printf("==========================================\n");
        printf("Usage\n\n");
        
        printf("    galyk  mode  [options]\n\n");
        
        printf("mode: galyk's mode.\n");
        printf("    -h,  --help             도움말 모드\n");
        printf("    -d,  --dsn              dsn 입력 모드\n");
        printf("\n");

        printf("options: options for each modes.\n");
        printf("    dsn_list                dsn 리스트\n");
        printf("\n");
        
        printf("example:\n");
        printf("    galyk --help\n");
        printf("    galyk -d g1n1 g1n2\n");
        printf("==========================================\n\n");
    }
    GL_CATCH( RAMP_ERR_ALLOC_ENV )
    {
        printf("Error at SQLAllocHandle(env)\n");
    }

    GL_CATCH( RAMP_ERR_ALLOC_DBC )
    {
        printf("Error at SQLAllocHandle(dbc)\n");
    }

    GL_CATCH( RAMP_ERR_FREE_DBC )
    {
        printf("Error at SQLFreeHandle(dbc)\n");
    }

    GL_CATCH( RAMP_ERR_FREE_ENV )
    {
        printf("Error at SQLFreeHandle(env)\n");
    }

    GL_FINISH;

    switch( sStatus )
    {
        case 2:
            (void)SQLFreeHandle ( SQL_HANDLE_DBC, sDbc );
        case 1:
            (void)SQLFreeHandle ( SQL_HANDLE_ENV, sEnv );
    }

    free( sDsnInfo );

    return GL_FAILURE;
}

/**
 * @brief DSN 정보 초기화
 * @param [in] glDsnInfo       DSN Information
 */
glStatus glInitDsnInfo( glDsnInfo  * aDsnInfo,
                        char       * aDsnName )
{
    snprintf( aDsnInfo->mName,         /* Dsn Name */
              MAX_DSN_LENGTH,
              "%s", aDsnName );

    aDsnInfo->mNameLength  = 0;        /* Dsn Name Length */

    snprintf( aDsnInfo->mMemberName,   /* Member Name */
              MAX_NAME_LENGTH,
              "%s", aDsnInfo->mName );

    aDsnInfo->mMemberId    = 2503;     /* Member ID */

    snprintf( aDsnInfo->mStatus,       /* Status */
              MAX_STATUS_LENGTH,
              "UNKNOWN" );

    aDsnInfo->mGlobalScn   = 0;        /* Global SCN */
    aDsnInfo->mDomainScn   = 0;        /* Domain SCN */

    return GL_SUCCESS;
}

