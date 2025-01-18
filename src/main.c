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
// haikal@Array:vec3s:u
// haikal@Array:vec4s:u
// haikal@Map:vec3s:u

#include <stdio.h>
#include <string.h>
#define CORE_IMPL
#include <core.h>
#include <stdalign.h>
#include <cglm/struct.h>

#include "Arena.h"

#include <Array.h>
#include <Node.h>
#include <List.h>
#include <BiNode.h>
#include <DList.h>
#include <Stack.h>
#include <Queue.h>

#include <Map.h>

#include "archetype.h"

typedef struct Payload Payload;
struct Payload {
    i32 id;
    i32 mx;
    char *str;
};

typedef struct vec4i8 vec4i8;
struct vec4i8 {
    i8 x;
    i8 y;
    i8 z;
    i8 w; 
};

void Arena_test() {
    Arena arena = {0};
    // arenaInit(&arena, store);

    const i32 len = 4;
    f32 *nums = arenaPushArray(&arena, i32, len);
    for (i32 i = 0; i < len; ++i) {
        nums[i] = (f32)(i + 1);
    }
    for (i32 i = 0; i < len; ++i) {
        printf("%f ", nums[i]);
    }
    printf("\n");

    u8 *ptr = (u8 *)nums;
    for (i32 i = 0; i < sizeof(f32) * len; ++i) {
        printf("%02x ", ptr[i]);
    }
    printf("\n");

    char *str0 = strAlloc(&arena, "this is a te");
    char *str1 = strAlloc(&arena, "st string to");
    char *str2 = strAlloc(&arena, "alloc bytes.");
    printf("%s\n", str0);
    printf("%s\n", str1);
    printf("%s\n", str2);

    strDealloc(&arena, str2);
    str2 = strAlloc(&arena, "fortitude");
    printf("%s\n", str0);
    printf("%s\n", str1);
    printf("%s\n", str2);

    Payload *pld = arenaPushStruct(&arena, Payload);
    pld->id = 0xDEADBEEF;
    pld->mx = 0xCAFEBABE;
    pld->str = "Name0";
    arenaPop(&arena, sizeof(Payload));
    pld = arenaPushStruct(&arena, Payload);
    pld->id = 0xFFFFFFFF;
    pld->mx = 0xFFFFFFFF;
    pld->str = "Name0";
    arenaPop(&arena, sizeof(Payload));

    arenaClear(&arena);

    // vec4i8 *vs = arenaPushArrayZero(&arena, vec4i8, 32);
    const i32 npts = 32;
    vec4i8 *vs = arenaPushArray(&arena, vec4i8, npts);
    for (i32 i = 0; i < npts; ++i) {
        vs[i].x = 0xAA;
        vs[i].y = 0xBB;
        vs[i].z = 0xCC;
        vs[i].w = 0xDD;
    }
    arenaPopArray(&arena, vec4i8, npts);

    // printf("Memory Dump: %d bytes allocated.\n", N);
    // printf("%p: ", store);
    // for (i32 i = 0; i < N; ++i) {
    //     if(i % 8 == 0 && i != 0) {
    //         printf("\n");
    //         printf("%p: ", &store[i]);
    //     }
    //     printf("%02x ", store[i]);
    // }
    // arenaClear(&arena);
}

i32 main(i32 argc, char *argv[]) {
    printf("kayan test begin.\n");
    // Arena_test();
    Archetype_test();
    printf("kayan test end.\n");
    printf("alignment %llu\n", alignof(i32));
    printf("alignment %llu\n", alignof(Array_voidptr));
    printf("alignment %llu\n", alignof(__m128));
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
