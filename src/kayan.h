#pragma once
#include <stdio.h>
// #include <intrin.h>
// #include "vec3.h"
#include <cglm/struct.h>
#include <Array.h>
#include <Map.h>

typedef enum Signature Signature;
enum Signature {
    nul = 0b0000000000000000000000000000000000000000000000000000000000000000,
    pos = 0b0000000000000000000000000000000000000000000000000000000000000001,
    vel = 0b0000000000000000000000000000000000000000000000000000000000000010,
    trq = 0b0000000000000000000000000000000000000000000000000000000000000100,
};

bool queryTest(u64 a, u64 b) {
    return (a & b) == b;
}

// components
//---------------------------------------------------------------------------------------------------
typedef struct MovementComponent MovementComponent;
struct MovementComponent {
    Array_vec4s pos;
    Array_vec4s vel;
    Array_vec4s trq;
};

MovementComponent movementComponentAlloc(size_t allocsize) {
    return (MovementComponent) {
        .pos = Array_vec4s_create(allocsize),
        .vel = Array_vec4s_create(allocsize),
        .trq = Array_vec4s_create(allocsize),
    };
}

typedef struct AttributeComponent AttributeComponent;
struct AttributeComponent {
    Array_f32 health;
    Array_f32 maxhealth;
};

AttributeComponent attributeComponentAlloc(size_t allocsize) {
    return (AttributeComponent) {
        .health = Array_f32_create(allocsize),
        .maxhealth = Array_f32_create(allocsize),
    };
}

// archetypes
//---------------------------------------------------------------------------------------------------
typedef struct ArchetypeHeader ArchetypeHeader;
struct ArchetypeHeader {
    Map_u64 offsetmap;
    u64 signature;
};

typedef struct A A;
struct A {
    ArchetypeHeader archetypeheader;
    Array_vec4s pos;
    Array_vec4s vel;
    Array_vec4s trq;
};

typedef struct B B;
struct B {
    ArchetypeHeader archetypeheader;
    Array_vec4s pos;
    Array_vec4s vel;
};

typedef struct C C;
struct C {
    ArchetypeHeader archetypeheader;
    Array_vec4s pos;
};

void Archetype_test() {
    printf("kayan begin.\n");
    A a = {};
    B b = {};
    C c = {};

    a.archetypeheader.signature = pos | vel | trq;
    a.archetypeheader.offsetmap = *Map_u64_create();
    Map_u64_set(&a.archetypeheader.offsetmap, "pos", offsetof(A, pos));
    Map_u64_set(&a.archetypeheader.offsetmap, "vel", offsetof(A, vel));
    Map_u64_set(&a.archetypeheader.offsetmap, "trq", offsetof(A, trq));
    Array_vec4s_append(&a.pos, (vec4s){0.f});
    Array_vec4s_append(&a.pos, (vec4s){2.f});
    Array_vec4s_append(&a.pos, (vec4s){6.f});
    Array_vec4s_append(&a.vel, (vec4s){1.f});
    Array_vec4s_append(&a.vel, (vec4s){1.f});
    Array_vec4s_append(&a.vel, (vec4s){1.f});

    b.archetypeheader.signature = pos | vel;
    b.archetypeheader.offsetmap = *Map_u64_create();
    Map_u64_set(&b.archetypeheader.offsetmap, "pos", offsetof(B, pos));
    Map_u64_set(&b.archetypeheader.offsetmap, "vel", offsetof(B, vel));
    Array_vec4s_append(&b.pos, (vec4s){2.f});
    Array_vec4s_append(&b.pos, (vec4s){2.f});
    Array_vec4s_append(&b.vel, (vec4s){8.f});
    Array_vec4s_append(&b.vel, (vec4s){8.f});

    c.archetypeheader.signature = pos;
    c.archetypeheader.offsetmap = *Map_u64_create();
    Map_u64_set(&c.archetypeheader.offsetmap, "pos", offsetof(C, pos));

    Array_voidptr typeerased = {0};
    Array_voidptr_append(&typeerased, &a);
    Array_voidptr_append(&typeerased, &b);
    Array_voidptr_append(&typeerased, &c);

    for (i32 i = 0; i < typeerased.length; i++) {
        u8 signature = *((u8 *)(typeerased.data[i]));
        if (queryTest(signature, pos | vel)) {
            printf("signature = %d\n", signature);
        }
    }

    // printf("%p\n", typeerased[0]);
    // printf("%p\n", typeerased[1]);
    // printf("%p\n", typeerased[2]);

    for (i32 i = 0; i < typeerased.length; i++) {
        ArchetypeHeader archetypeheader = *((ArchetypeHeader *)typeerased.data[i]);
        u64 signature = archetypeheader.signature;
        Map_u64 offsetmap = archetypeheader.offsetmap;
        // printf("offsetmap border = %llu, offsetmap length = %llu\n", offsetmap.border, offsetmap.length);
        if (queryTest(signature, pos | vel)) {
            u64 posoff = *Map_u64_get(&offsetmap, "pos");
            Array_vec4s *pos = (Array_vec4s *)((u8 *)typeerased.data[i] + posoff);
            u64 veloff = *Map_u64_get(&offsetmap, "vel");
            Array_vec4s *vel = (Array_vec4s *)((u8 *)typeerased.data[i] + veloff);
            for (i32 i = 0; i < pos->length; ++i) {
                pos->data[i] = glms_vec4_add(pos->data[i], vel->data[i]);
                // pos->data[i].x += vel->data[i].x;
                // pos->data[i].y += vel->data[i].y;
                // pos->data[i].z += vel->data[i].z;
                // pos->data[i].w += vel->data[i].w;
            }
            //---------------------------------------------------------------------------------------------------
            // printf("%p = &a \n", &a);
            // printf("%p = &a.pos\n", &a.pos);
            // printf("%p = &a.vel\n", &a.vel);
            // printf("%llu\n", a.pos.length);
            // printf("%llu\n", a.vel.length);
            // printf("%llu\n", pos->length);
            // printf("%llu\n", vel->length);
            // printf("%p = &pos\n", pos);
        }
    }
    printf("{%f, %f, %f}\n", a.pos.data[0].x, a.pos.data[0].y, a.pos.data[0].z);
    printf("{%f, %f, %f}\n", a.pos.data[1].x, a.pos.data[1].y, a.pos.data[1].z);
    printf("{%f, %f, %f}\n", a.pos.data[2].x, a.pos.data[2].y, a.pos.data[2].z);
    printf("kayan end.\n");
}
