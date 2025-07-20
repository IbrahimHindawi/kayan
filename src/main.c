//---------------------------------------------------------------------------------------------------
// monomorphization codegen limitations:
//---------------------------------------------------------------------------------------------------
// for containers that have value types eg `T`,
// the type must be included before the generated header.
// this is because the container expects to know the type in it's struct.
// Warning: cannot be recursive type
//
// for containers that have pointer types eg `T *`,
// the type can be included before or after the generated header.
// this is because the container has `T` forward declared.
// Warning: can be recursive type
//
// for types that include a container of themselves eg `struct T { Array_T arr; };`
// the type must be included after the generated header.
// this is because the type needs to know the container definition.
// Warning: can be recursive type with `T *` but not `T`
//---------------------------------------------------------------------------------------------------
// primitives
//---------------------------------------------------------------------------------------------------
// haikal@Array:i8:p
// haikal@Array:i32:p
// haikal@Array:f32:p
// haikal@Array:voidptr:p
// haikal@Map:i32:p
// haikal@Map:u64:p
// haikal@Node:i32:p
// haikal@List:i32:p
// haikal@BiNode:i32:p
// haikal@DList:i32:p
// haikal@Queue:i32:p
// haikal@Stack:i32:p
//---------------------------------------------------------------------------------------------------
// structs
//---------------------------------------------------------------------------------------------------
// haikal@Array:List_i32:s
// haikal@Map:Array_i8:s
// haikal@Map:Array_i32:s
//---------------------------------------------------------------------------------------------------
// unions
//---------------------------------------------------------------------------------------------------
// haikal@Array:vec2s:u
// haikal@Array:vec3s:u
// haikal@Array:vec4s:u
// haikal@Map:vec3s:u

#include <stdio.h>
#include <string.h>
#define CORE_IMPL
#include <core.h>
#include <stdalign.h>
#include <cglm/struct.h>

#define SAHA_IMPLEMENTATION
#include "saha.h"

#include <Array.h>
#include <Node.h>
#include <List.h>
#include <BiNode.h>
#include <DList.h>
#include <Stack.h>
#include <Queue.h>

#include <Map.h>

#include "kayan.h"

i32 main(i32 argc, char *argv[]) {
    printf("kayan test begin.\n");
    Arena arena = {};
    arenaInit(&arena);
    kayan_main(&arena);
    printf("kayan test end.\n");
    return 0;
}

#include <Array.c>
#include <BiNode.c>
#include <DList.c>
#include <Map.c>
#include <List.c>
#include <Node.c>
#include <Stack.c>
#include <Queue.c>
