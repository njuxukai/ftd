/* Generated by eXtremeDB Schema Compiler, build 1797, rev. stable, rev. 23320
 * at Mon Dec 31 21:26:05 2018
 */

#if defined(MCO_CPP_INTERFACE) && defined(__MCO__genericdb__H__)
#error C API schema file genericdb.h should not be included directly from C++ application
#endif
#ifndef __MCO__genericdb__H__
#define __MCO__genericdb__H__

#ifdef __cplusplus
extern "C" { 
#endif

#include "mco.h"
#include "mcouda.h"

#ifndef MCO_COMP_VER_MAJOR 
#define MCO_COMP_VER_MAJOR 8
#define MCO_COMP_VER_MINOR 0
#define MCO_COMP_BUILD_NUM 1797
#define MCO_COMP_REVISION "stable, rev. 23320"
#endif /*MCO_COMP_VER_MAJOR*/ 

#if !defined(MCO_MEMCPY_DEFINED) && !MCO_CFG_BUILTIN_MEMCPY
#define MCO_MEMCPY_DEFINED
void mco_memcpy(void* dest, const void* src, mco_size_t nbt);
#endif /*MCO_MEMCPY_DEFINED*/





/*---------------------------------------------------------------------*/
/* Dictionary                                                          */

mco_dictionary_h genericdb_get_dictionary(void);

mco_calculator_h genericdb_get_calculator(void);

#ifdef MCO_CPP_INTERFACE
namespace genericdb
{
#endif


/*---------------------------------------------------------------------*/
/* Handles and Class Codes                                             */

#ifndef MCO_CPP_INTERFACE
typedef struct A_          { MCO_Hf h; }  A;
#else
class A;
#endif
#define                                   A_code           1
#ifndef MCO_CPP_INTERFACE
typedef struct B_          { MCO_Hf h; }  B;
#else
class B;
#endif
#define                                   B_code           2
#ifndef MCO_CPP_INTERFACE
typedef struct Part_       { MCO_Hf h; }  Part;
#else
class Part;
#endif
#define                                   Part_code        3
#ifndef MCO_CPP_INTERFACE
typedef struct Part2_      { MCO_Hf h; }  Part2;
#else
class Part2;
#endif
#define                                   Part2_code       4
#ifndef MCO_CPP_INTERFACE
typedef struct User_       { MCO_Hf h; }  User;
#else
class User;
#endif
#define                                   User_code        5
#ifndef MCO_CPP_INTERFACE
typedef struct Dimensions_ { MCO_Hf h; }  Dimensions;
#else
struct Dimensions;
#endif


/*---------------------------------------------------------------------*/
/* Structures for fixed part of the structure                          */

typedef struct Dimensions_fixed_ {
  float width;
  float length;
  float height;
} Dimensions_fixed;

typedef struct A_fixed_ {
  uint4 ui4;
} A_fixed;

typedef struct B_fixed_ {
  uint4 ui4;
} B_fixed;

typedef struct Part_fixed_ {
  float price;
  uint4 code;
  Dimensions_fixed dim;
  uint4 options[10];
} Part_fixed;

typedef struct Part2_fixed_ {
  int4 broker_id;
  char password[8];
  uint4 user_id;
} Part2_fixed;

typedef struct User_fixed_ {
  int4 broker_id;
  uint4 user_id;
  char password[8];
} User_fixed;



/*---------------------------------------------------------------------*/
/* class A methods                                                     */

MCO_RET      A_new                           ( mco_trans_h t, /*OUT*/ A *handle );
MCO_RET      A_delete                        ( A *handle );
MCO_RET      A_delete_all                    ( mco_trans_h t );
MCO_RET      A_checkpoint                    ( A *handle );

MCO_RET      A_ui4_get                       ( A *handle, /*OUT*/ uint4 * result);
MCO_RET      A_ui4_put                       ( A *handle, uint4 value );

MCO_RET      A_from_cursor                   ( mco_trans_h t, mco_cursor_h c, /*OUT*/ A *handle );
MCO_RET      A_list_cursor                   ( mco_trans_h t, /*OUT*/ mco_cursor_h c );

/*---------------------------------------------------------------------*/
/* class B methods                                                     */

MCO_RET      B_new                           ( mco_trans_h t, /*OUT*/ B *handle );
MCO_RET      B_delete                        ( B *handle );
MCO_RET      B_set_allocation_block_size    (mco_trans_h t, mco_size_t block_size);
MCO_RET      B_set_caching_priority         (mco_trans_h t, int priority);
MCO_RET      B_delete_all                    ( mco_trans_h t );
MCO_RET      B_checkpoint                    ( B *handle );

MCO_RET      B_ui4_get                       ( B *handle, /*OUT*/ uint4 * result);
MCO_RET      B_ui4_put                       ( B *handle, uint4 value );

MCO_RET      B_from_cursor                   ( mco_trans_h t, mco_cursor_h c, /*OUT*/ B *handle );
MCO_RET      B_list_cursor                   ( mco_trans_h t, /*OUT*/ mco_cursor_h c );

/*---------------------------------------------------------------------*/
/* class Part methods                                                  */

MCO_RET      Part_new                           ( mco_trans_h t, /*OUT*/ Part *handle );
MCO_RET      Part_delete                        ( Part *handle );
MCO_RET      Part_delete_all                    ( mco_trans_h t );
MCO_RET      Part_checkpoint                    ( Part *handle );
MCO_RET      Part_pack                          ( Part *handle, /*OUT*/ uint4 * pages_released );

MCO_RET      Part_type_get                      ( Part *handle, /*OUT*/ char * dest, uint2 dest_size, /*OUT*/ uint2 * len);
MCO_RET      Part_type_size                     ( Part *handle, /*OUT*/ uint2 *result);
MCO_RET      Part_type_put                      ( Part *handle, const char * src, uint2 len);

MCO_RET      Part_name_get                      ( Part *handle, /*OUT*/ char * dest, uint2 dest_size, /*OUT*/ uint2 * len);
MCO_RET      Part_name_size                     ( Part *handle, /*OUT*/ uint2 *result);
MCO_RET      Part_name_put                      ( Part *handle, const char * src, uint2 len);

MCO_RET      Part_price_get                     ( Part *handle, /*OUT*/ float * result);
MCO_RET      Part_price_put                     ( Part *handle, float value );

MCO_RET      Part_code_get                      ( Part *handle, /*OUT*/ uint4 * result);
MCO_RET      Part_code_put                      ( Part *handle, uint4 value );

MCO_RET      Part_dim_read_handle               ( Part *handle, /*OUT*/ Dimensions * h );
MCO_RET      Part_dim_write_handle              ( Part *handle, /*OUT*/ Dimensions * h );

#define Part_options_length 10
MCO_RET      Part_options_at                    ( Part *handle, uint2 index, /*OUT*/ uint4 *result);
MCO_RET      Part_options_get_range             ( Part *handle, uint2 start_index, uint2 num, /*OUT*/ uint4 *dest );
MCO_RET      Part_options_put                   ( Part *handle, uint2 index, uint4 value );
MCO_RET      Part_options_put_range             ( Part *handle, uint2 start_index, uint2 num, const uint4 *src );

MCO_RET      Part_from_cursor                   ( mco_trans_h t, mco_cursor_h c, /*OUT*/ Part *handle );

MCO_RET      Part_ByCode_index_cursor           ( mco_trans_h t, /*OUT*/ mco_cursor_h c );
MCO_RET      Part_ByCode_compare                ( mco_trans_h t, mco_cursor_h c, uint4 code_key_, /*OUT*/ int *result_ );
MCO_RET      Part_ByCode_find                   ( mco_trans_h t, uint4 code_key_, /*OUT*/ Part *handle_);

MCO_RET      Part_ByType_index_cursor           ( mco_trans_h t, /*OUT*/ mco_cursor_h c );
MCO_RET      Part_ByType_search                 ( mco_trans_h t, /*INOUT*/ mco_cursor_h c, MCO_OPCODE op_, const char *type_key_, uint2 sizeof_type_key_, const char *name_key_, uint2 sizeof_name_key_ );
MCO_RET      Part_ByType_compare                ( mco_trans_h t, mco_cursor_h c, const char *type_key_, uint2 sizeof_type_key_, const char *name_key_, uint2 sizeof_name_key_, /*OUT*/ int *result_ );
MCO_RET      Part_ByType_pattern_size           ( const char *type_key_, uint2 sizeof_type_key_, const char *name_key_, uint2 sizeof_name_key_, /*OUT*/ uint4 *size_);
MCO_RET      Part_ByType_pattern_search         ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern, mco_size_t memsize , const char *type_key_, uint2 sizeof_type_key_, const char *name_key_, uint2 sizeof_name_key_ );
MCO_RET      Part_ByType_pattern_next           ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern );
MCO_RET      Part_ByType_locate                 ( mco_trans_h t, /*OUT*/ mco_cursor_h c, Part * handle);

MCO_RET      Part_IdxCode_index_cursor          ( mco_trans_h t, /*OUT*/ mco_cursor_h c );
MCO_RET      Part_IdxCode_search                ( mco_trans_h t, /*INOUT*/ mco_cursor_h c, MCO_OPCODE op_, uint4 code_key_ );
MCO_RET      Part_IdxCode_compare               ( mco_trans_h t, mco_cursor_h c, uint4 code_key_, /*OUT*/ int *result_ );
MCO_RET      Part_IdxCode_pattern_size          ( uint4 code_key_, /*OUT*/ uint4 *size_);
MCO_RET      Part_IdxCode_pattern_search        ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern, mco_size_t memsize , uint4 code_key_ );
MCO_RET      Part_IdxCode_pattern_next          ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern );
MCO_RET      Part_IdxCode_locate                ( mco_trans_h t, /*OUT*/ mco_cursor_h c, Part * handle);
MCO_RET      Part_IdxCode_find                  ( mco_trans_h t, uint4 code_key_, /*OUT*/ Part *handle_);

/*---------------------------------------------------------------------*/
/* class Part2 methods                                                 */

MCO_RET      Part2_new                             ( mco_trans_h t, /*OUT*/ Part2 *handle );
MCO_RET      Part2_delete                          ( Part2 *handle );
MCO_RET      Part2_delete_all                      ( mco_trans_h t );
MCO_RET      Part2_checkpoint                      ( Part2 *handle );

MCO_RET      Part2_broker_id_get                   ( Part2 *handle, /*OUT*/ int4 * result);
MCO_RET      Part2_broker_id_put                   ( Part2 *handle, int4 value );

MCO_RET      Part2_password_get                    ( Part2 *handle, /*OUT*/ char * dest, uint2 dest_size);
MCO_RET      Part2_password_put                    ( Part2 *handle, const char * src, uint2 len) ;

MCO_RET      Part2_user_id_get                     ( Part2 *handle, /*OUT*/ uint4 * result);
MCO_RET      Part2_user_id_put                     ( Part2 *handle, uint4 value );

MCO_RET      Part2_from_cursor                     ( mco_trans_h t, mco_cursor_h c, /*OUT*/ Part2 *handle );

MCO_RET      Part2_IdxCode_index_cursor            ( mco_trans_h t, /*OUT*/ mco_cursor_h c );
MCO_RET      Part2_IdxCode_search                  ( mco_trans_h t, /*INOUT*/ mco_cursor_h c, MCO_OPCODE op_, uint4 user_id_key_ );
MCO_RET      Part2_IdxCode_compare                 ( mco_trans_h t, mco_cursor_h c, uint4 user_id_key_, /*OUT*/ int *result_ );
MCO_RET      Part2_IdxCode_pattern_size            ( uint4 user_id_key_, /*OUT*/ uint4 *size_);
MCO_RET      Part2_IdxCode_pattern_search          ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern, mco_size_t memsize , uint4 user_id_key_ );
MCO_RET      Part2_IdxCode_pattern_next            ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern );
MCO_RET      Part2_IdxCode_locate                  ( mco_trans_h t, /*OUT*/ mco_cursor_h c, Part2 * handle);
MCO_RET      Part2_IdxCode_find                    ( mco_trans_h t, uint4 user_id_key_, /*OUT*/ Part2 *handle_);

/*---------------------------------------------------------------------*/
/* class User methods                                                  */

MCO_RET      User_new                                 ( mco_trans_h t, /*OUT*/ User *handle );
MCO_RET      User_delete                              ( User *handle );
MCO_RET      User_delete_all                          ( mco_trans_h t );
MCO_RET      User_checkpoint                          ( User *handle );

MCO_RET      User_broker_id_get                       ( User *handle, /*OUT*/ int4 * result);
MCO_RET      User_broker_id_put                       ( User *handle, int4 value );

MCO_RET      User_user_id_get                         ( User *handle, /*OUT*/ uint4 * result);
MCO_RET      User_user_id_put                         ( User *handle, uint4 value );

MCO_RET      User_password_get                        ( User *handle, /*OUT*/ char * dest, uint2 dest_size);
MCO_RET      User_password_put                        ( User *handle, const char * src, uint2 len) ;

MCO_RET      User_from_cursor                         ( mco_trans_h t, mco_cursor_h c, /*OUT*/ User *handle );

MCO_RET      User_idx_user_id_index_cursor            ( mco_trans_h t, /*OUT*/ mco_cursor_h c );
MCO_RET      User_idx_user_id_search                  ( mco_trans_h t, /*INOUT*/ mco_cursor_h c, MCO_OPCODE op_, uint4 user_id_key_ );
MCO_RET      User_idx_user_id_compare                 ( mco_trans_h t, mco_cursor_h c, uint4 user_id_key_, /*OUT*/ int *result_ );
MCO_RET      User_idx_user_id_pattern_size            ( uint4 user_id_key_, /*OUT*/ uint4 *size_);
MCO_RET      User_idx_user_id_pattern_search          ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern, mco_size_t memsize , uint4 user_id_key_ );
MCO_RET      User_idx_user_id_pattern_next            ( mco_trans_h t, mco_cursor_h c, /*INOUT*/ void *allocated_pattern );
MCO_RET      User_idx_user_id_locate                  ( mco_trans_h t, /*OUT*/ mco_cursor_h c, User * handle);
MCO_RET      User_idx_user_id_find                    ( mco_trans_h t, uint4 user_id_key_, /*OUT*/ User *handle_);

/*---------------------------------------------------------------------*/
/* struct Dimensions methods                                           */


MCO_RET      Dimensions_width_get             ( Dimensions *handle, /*OUT*/ float * result);
MCO_RET      Dimensions_width_put             ( Dimensions *handle, float value );

MCO_RET      Dimensions_length_get            ( Dimensions *handle, /*OUT*/ float * result);
MCO_RET      Dimensions_length_put            ( Dimensions *handle, float value );

MCO_RET      Dimensions_height_get            ( Dimensions *handle, /*OUT*/ float * result);
MCO_RET      Dimensions_height_put            ( Dimensions *handle, float value );
MCO_RET      Dimensions_fixed_get             ( Dimensions *handle_, Dimensions_fixed* dst_ );
MCO_RET      Dimensions_fixed_put             ( Dimensions *handle_, Dimensions_fixed const* src_ );
MCO_RET      A_fixed_get                     ( A *handle_, A_fixed* dst_ );
MCO_RET      A_fixed_put                     ( A *handle_, A_fixed const* src_ );
MCO_RET      B_fixed_get                     ( B *handle_, B_fixed* dst_ );
MCO_RET      B_fixed_put                     ( B *handle_, B_fixed const* src_ );
MCO_RET      Part_fixed_get                     ( Part *handle_, Part_fixed* dst_ );
MCO_RET      Part_fixed_put                     ( Part *handle_, Part_fixed const* src_ );
MCO_RET      Part2_fixed_get                       ( Part2 *handle_, Part2_fixed* dst_ );
MCO_RET      Part2_fixed_put                       ( Part2 *handle_, Part2_fixed const* src_ );
MCO_RET      User_fixed_get                           ( User *handle_, User_fixed* dst_ );
MCO_RET      User_fixed_put                           ( User *handle_, User_fixed const* src_ );

#ifdef __cplusplus
#ifdef MCO_CPP_INTERFACE
} // namespace genericdb
#endif
} // extern "C" 
#endif

#endif
