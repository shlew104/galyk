#include "glCommon.h"
#include "glStartup.h"
#include "glPrintDiagnosticRecord.h"
#include "glSelect.h"

glStatus glStartupMount ( SQLHDBC aDbc, char * aDataBuffer )
{
    SQLHSTMT        sStmt    = NULL;
    int             sStatus  = 0;

    GL_TRY_THROW ( SQLAllocHandle ( SQL_HANDLE_STMT,
                                    aDbc,
                                    &sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_ALLOC_STMT );

    sStatus = 1;

    GL_TRY_THROW ( SQLExecDirect ( sStmt,
                                   (SQLCHAR*)"ALTER SYSTEM MOUNT DATABASE",
                                   SQL_NTS ) == SQL_SUCCESS,
                   RAMP_ERR_STARTUP_MOUNT );                

    GL_TRY ( glSelectOneValue( aDbc, 
                               "SELECT STATUS FROM X$INSTANCE@LOCAL", 
                               aDataBuffer ) == GL_SUCCESS );

    sStatus = 0;

    GL_TRY_THROW ( SQLFreeHandle ( SQL_HANDLE_STMT, sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_FREE_STMT );

    return GL_SUCCESS;

    GL_CATCH ( RAMP_ERR_ALLOC_STMT )
    {
        printf ("Error at SQLAllocHandle(stmt)\n");
    }

    GL_CATCH ( RAMP_ERR_STARTUP_MOUNT )
    {
        printf ("Error at Startup mount\n");
    }

    GL_CATCH ( RAMP_ERR_FREE_STMT )
    {
        printf ("Error at SQLFreeHandle(stmt)\n");
    }

    GL_FINISH;

    switch ( sStatus )
    {
        case 1:
            (void) SQLFreeHandle ( SQL_HANDLE_STMT, sStmt );
    }

    return GL_FAILURE;

}

glStatus glStartupLocalOpen ( SQLHDBC aDbc, char * aDataBuffer )
{
    SQLHSTMT        sStmt    = NULL;
    int             sRc      = 0;
    int             sStatus  = 0;

    GL_TRY_THROW ( SQLAllocHandle ( SQL_HANDLE_STMT,
                                    aDbc,
                                    &sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_ALLOC_STMT );

    sStatus = 1;

    sRc = SQLExecDirect ( sStmt,
                          (SQLCHAR*)"ALTER SYSTEM OPEN LOCAL DATABASE",
                          SQL_NTS );

    if ( sRc == SQL_SUCCESS )
    {
        printf( "Startup Local Open Complete.\n");
    }
    else if ( sRc == SQL_SUCCESS_WITH_INFO )
    {
        printf( "Startup Local Open Complete. NOT ERROR, Just INFO\n");
        glPrintDiagnosticRecord( SQL_HANDLE_STMT, sStmt );
    }
    else
    {
        GL_THROW ( RAMP_ERR_STARTUP_LOCAL_OPEN );
    }

    GL_TRY ( glSelectOneValue( aDbc, "SELECT STATUS FROM X$INSTANCE@LOCAL", aDataBuffer) == GL_SUCCESS );

    sStatus = 0;

    GL_TRY_THROW ( SQLFreeHandle ( SQL_HANDLE_STMT, sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_FREE_STMT );

    return GL_SUCCESS;

    GL_CATCH ( RAMP_ERR_ALLOC_STMT )
    {
        printf ("Error at SQLAllocHandle(stmt)\n");
    }

    GL_CATCH ( RAMP_ERR_STARTUP_LOCAL_OPEN )
    {
        glPrintDiagnosticRecord( SQL_HANDLE_STMT, sStmt );
    }

    GL_CATCH ( RAMP_ERR_FREE_STMT )
    {
        printf ("Error at SQLFreeHandle(stmt)\n");
    }

    GL_FINISH;

    switch ( sStatus )
    {
        case 1:
            (void)SQLFreeHandle ( SQL_HANDLE_STMT, aDbc );
    }

    return GL_FAILURE;

}

glStatus glStartupGlobalOpen ( SQLHDBC aDbc, char * aDataBuffer )
{
    SQLHSTMT        sStmt    = NULL;
    int             sRc      = 0;
    int             sStatus  = 0;

    GL_TRY_THROW ( SQLAllocHandle ( SQL_HANDLE_STMT,
                                    aDbc,
                                    &sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_ALLOC_STMT );

    sStatus = 1;

    sRc = SQLExecDirect ( sStmt,
                          (SQLCHAR*)"ALTER SYSTEM OPEN GLOBAL DATABASE",
                          SQL_NTS );

    if ( sRc == SQL_SUCCESS )
    {
        printf( "Startup Global Open Complete.\n");
    }
    else if ( sRc == SQL_SUCCESS_WITH_INFO )
    {
        printf( "Startup Global Open Complete. NOT ERROR, Just INFO\n");
        glPrintDiagnosticRecord( SQL_HANDLE_STMT, sStmt );
    }
    else
    {
        GL_THROW ( RAMP_ERR_STARTUP_GLOBAL_OPEN );
    }

    GL_TRY ( glSelectOneValue( aDbc, "SELECT STATUS FROM X$INSTANCE@LOCAL", aDataBuffer) == GL_SUCCESS );

    sStatus = 0;

    GL_TRY_THROW ( SQLFreeHandle ( SQL_HANDLE_STMT, sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_FREE_STMT );

    return GL_SUCCESS;

    GL_CATCH ( RAMP_ERR_ALLOC_STMT )
    {
        printf ("Error at SQLAllocHandle(stmt)\n");
    }

    GL_CATCH ( RAMP_ERR_STARTUP_GLOBAL_OPEN )
    {
        glPrintDiagnosticRecord( SQL_HANDLE_STMT, sStmt );
    }

    GL_CATCH ( RAMP_ERR_FREE_STMT )
    {
        printf ("Error at SQLFreeHandle(stmt)\n");
    }

    GL_FINISH;

    switch ( sStatus )
    {
        case 1:
            (void)SQLFreeHandle ( SQL_HANDLE_STMT, aDbc );
    }

    return GL_FAILURE;

}
