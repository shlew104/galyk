#include "glCommon.h"
#include "glSelect.h"
#include "glStartup.h"
//#include "glShutdown.h"

int main( int aArgc, char** aArgv )
{
    SQLHENV    sEnv = NULL;
    SQLHDBC    sDbc = NULL;
    char       aDataBuffer[BUF_LEN];

    memset(aDataBuffer, 0, sizeof(aDataBuffer));
    
    SQLAllocHandle ( SQL_HANDLE_ENV,
                     NULL,
                     &sEnv ); 
    
    SQLSetEnvAttr ( sEnv,
                    SQL_ATTR_ODBC_VERSION,
                    (SQLPOINTER)SQL_OV_ODBC3,
                    0 );

    SQLAllocHandle ( SQL_HANDLE_DBC,
                     sEnv,
                     &sDbc );

    SQLConnect ( sDbc,
                 (SQLCHAR*)"G1N1",
                 SQL_NTS,
                 (SQLCHAR*)"sys",
                 SQL_NTS,
                 (SQLCHAR*)"gliese as sysdba",
                 SQL_NTS ); 

    glSelectStatus ( sDbc, aDataBuffer );
    
    printf ("status is %s\n", aDataBuffer); 

    SQLDisconnect ( sDbc );

    SQLConnect ( sDbc,
                 (SQLCHAR*)"G1N1",
                 SQL_NTS,
                 (SQLCHAR*)"sys",
                 SQL_NTS,
                 (SQLCHAR*)"gliese as sysdba with startup",
                 SQL_NTS ); 

//    glShutdownAbort( sDbc );

    glStartupMount( sDbc, aDataBuffer );

//    glStartupLocalOpen ( sDbc, aDataBuffer );

//    glStartupGlobalOpen ( sDbc, aDataBuffer );

    SQLDisconnect ( sDbc );

    SQLFreeHandle ( SQL_HANDLE_DBC, sDbc );

    SQLFreeHandle ( SQL_HANDLE_ENV, sEnv );

    return GL_SUCCESS;   

}
