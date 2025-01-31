#include "glCommon.h"
#include "glJoin.h"
#include "glPrintDiagnosticRecord.h"
#include "glSelect.h"

glStatus glJoinDatabase ( SQLHDBC aDbc, char * aDataBuffer)
{
    SQLHSTMT        sStmt      = NULL;
    int             sRc        = 0;
    int             sStatus    = 0;

    GL_TRY_THROW ( SQLAllocHandle ( SQL_HANDLE_STMT,
                                    aDbc,
                                    &sStmt ) == SQL_SUCCESS,
                   RAMP_ERR_ALLOC_STMT );                 

    sStatus = 1;

    sRc = SQLExecDirect ( sStmt,
                          (SQLCHAR*)"ALTER SYSTEM JOIN DATABASE",
                          SQL_NTS );

    if ( sRc == SQL_SUCCESS )
    {
        printf( "Database Join Complete.\n");
    }
    else if ( sRc == SQL_SUCCESS_WITH_INFO )
    {
        printf( "Database Join Complete. NOT ERROR, Just INFO\n");
        glPrintDiagnosticRecord( SQL_HANDLE_STMT, sStmt );
    }
    else
    {
        GL_THROW ( RAMP_ERR_JOIN );
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

    GL_CATCH ( RAMP_ERR_JOIN )
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
