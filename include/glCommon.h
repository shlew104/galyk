#ifndef GL_COMMON_H
#define GL_COMMON_H

#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <goldilocks.h>

/* MACRO */
#define MAX_NAME_LENGTH   128
#define MAX_NODE_COUNT    128
#define MAX_DSN_LENGTH    32 
#define MAX_STATUS_LENGTH 16

/* 성공한다면 GL_SUCCESS, 
 * 그렇지 않다면 GL_FAILURE */
typedef enum
{
    GL_SUCCESS = 0,
    GL_FAILURE
} glStatus;

#define GL_FALSE 0
#define GL_TRUE  1

/* GL_THROW
 * 인자의 레이블(램프)로 점프 */
#define GL_THROW( aLabel ) goto aLabel;

/* GL_TRY_THROW 
 * 첫 번째 인자의 내용이 거짓이면
 * 두 번째 인자(레이블)로 점프
 * try-catch 구문의 try 와 유사 */
#define GL_TRY_THROW( aExpression, aLabel )    \
    do                                         \
    {                                          \
        if( !(aExpression) )                   \
        {                                      \
            goto aLabel;                       \
        }                                      \
    } while( 0 )

/* GL_TRY
 * 인자의 내용이 거짓이면
 * 종료 레이블로 점프
 * try-catch 구문의 try 와 유사 */
#define GL_TRY( aExpression )                  \
    do                                         \
    {                                          \
        if( !(aExpression) )                   \
        {                                      \
            goto GL_FINISH_LABEL;              \
        }                                      \
    } while( 0 )

/* GL_CATCH
 * 인자의 레이블(램프)로 점프되어 내용을 수행
 * 수행 후 종료 레이블로 점프
 * try-catch 구문의 catch 와 유사 */
#define GL_CATCH( aLabel )                     \
    goto GL_FINISH_LABEL;                      \
aLabel:

/* GL_FINISH
 * 비정상 종료 레이블
 * return success 가 아닌 경우 */
#define GL_FINISH                              \
    GL_FINISH_LABEL:

/* GL_RAMP
 * 인자의 레이블(램프) 정의 */
#define GL_RAMP( aLabel ) aLabel:

/* structure */
typedef struct glDsnInfo
{
    char mName[MAX_DSN_LENGTH];
    int  mNameLength;
    char mMemberName[MAX_NAME_LENGTH];
    int  mMemberId;
    char mStatus[16];
    int  mGlobalScn;
    int  mDomainScn;
} glDsnInfo;

glStatus glInitDsnInfo( glDsnInfo  * aDsnInfo,
                        char       * aDsnName );

#endif
