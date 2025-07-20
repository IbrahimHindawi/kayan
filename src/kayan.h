#pragma once
#include <stdio.h>
// #include <intrin.h>
// #include "vec3.h"
#include <cglm/struct.h>
#include <Array.h>
#include <Map.h>

#define max_entity_count 1024

typedef enum Signature Signature;
enum Signature {
    nul = 0,
    positions = 1ULL << 0,
    velocities = 1ULL << 1,
    torque = 1ULL << 2,
    health = 1ULL << 3,
    damage = 1ULL << 4,
    alive = 1ULL << 5,
};

bool queryTest(u64 a, u64 b) {
    return (a & b) == b;
}

// components
//---------------------------------------------------------------------------------------------------
typedef struct MovementComponent MovementComponent;
struct MovementComponent {
    Array_vec4s positions;
    Array_vec4s velocities;
    Array_vec4s torque;
};

MovementComponent movementComponentAlloc(Arena *arena, size_t allocsize) {
    return (MovementComponent) {
        .positions = Array_vec4s_reserve(arena, allocsize),
        .velocities = Array_vec4s_reserve(arena, allocsize),
        .torque = Array_vec4s_reserve(arena, allocsize),
    };
}

typedef struct AttributeComponent AttributeComponent;
struct AttributeComponent {
    Array_f32 health;
    Array_f32 maxhealth;
};

AttributeComponent attributeComponentAlloc(Arena *arena, size_t allocsize) {
    return (AttributeComponent) {
        .health = Array_f32_reserve(arena, allocsize),
        .maxhealth = Array_f32_reserve(arena, allocsize),
    };
}

// archetypes
//---------------------------------------------------------------------------------------------------
typedef struct ArchetypeHeader ArchetypeHeader;
struct ArchetypeHeader {
    Map_u64 offsetmap; // pointer offset map
    u64 signature;
    u64 entity_count;
};

typedef struct A A;
struct A {
    ArchetypeHeader archetypeheader;
    Array_vec4s positions;
    Array_vec4s velocities;
    Array_vec4s torque;
};

typedef struct B B;
struct B {
    ArchetypeHeader archetypeheader;
    Array_vec4s positions;
    Array_vec4s velocities;
};

typedef struct C C;
struct C {
    ArchetypeHeader archetypeheader;
    Array_vec4s positions;
};

typedef struct D D;
struct D {
    ArchetypeHeader archetypeheader;
    Array_vec4s positions;
    Array_vec4s velocities;
    Array_vec2s health; // {health, maxhealth}
    Array_f32 damage;
    Array_i8 alive;
};

void Archetype_test(Arena *arena) {
    printf("kayan begin.\n");
    A a = {};
    B b = {};
    C c = {};
    D d = {};

    a.archetypeheader.signature = positions | velocities | torque;
    a.archetypeheader.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &a.archetypeheader.offsetmap, "positions", offsetof(A, positions));
    Map_u64_set(arena, &a.archetypeheader.offsetmap, "velocities", offsetof(A, velocities));
    Map_u64_set(arena, &a.archetypeheader.offsetmap, "torque", offsetof(A, torque));
    // Array_vec4s_create();
    // alloc entity count
    Array_vec4s_append(arena, &a.positions, (vec4s){0.f});
    Array_vec4s_append(arena, &a.positions, (vec4s){2.f});
    Array_vec4s_append(arena, &a.positions, (vec4s){6.f});
    Array_vec4s_append(arena, &a.velocities, (vec4s){1.f});
    Array_vec4s_append(arena, &a.velocities, (vec4s){1.f});
    Array_vec4s_append(arena, &a.velocities, (vec4s){1.f});

    b.archetypeheader.signature = positions | velocities;
    b.archetypeheader.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &b.archetypeheader.offsetmap, "positions", offsetof(B, positions));
    Map_u64_set(arena, &b.archetypeheader.offsetmap, "velocities", offsetof(B, velocities));
    // alloc entity count
    Array_vec4s_append(arena, &b.positions, (vec4s){2.f});
    Array_vec4s_append(arena, &b.positions, (vec4s){2.f});
    Array_vec4s_append(arena, &b.velocities, (vec4s){8.f});
    Array_vec4s_append(arena, &b.velocities, (vec4s){8.f});

    c.archetypeheader.signature = positions;
    c.archetypeheader.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &c.archetypeheader.offsetmap, "positions", offsetof(C, positions));

    d.archetypeheader.signature = health | damage | alive;
    d.archetypeheader.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &d.archetypeheader.offsetmap, "health", offsetof(D, health));
    Map_u64_set(arena, &d.archetypeheader.offsetmap, "damage", offsetof(D, damage));
    Map_u64_set(arena, &d.archetypeheader.offsetmap, "alive", offsetof(D, alive));
    Map_u64_set(arena, &d.archetypeheader.offsetmap, "positions", offsetof(D, positions));
    Map_u64_set(arena, &d.archetypeheader.offsetmap, "velocities", offsetof(D, velocities));
    // alloc entity count
    Array_vec2s_append(arena, &d.health, (vec2s){100.f, 100.f});
    Array_vec2s_append(arena, &d.health, (vec2s){100.f, 100.f});
    Array_vec2s_append(arena, &d.health, (vec2s){100.f, 100.f});
    Array_i8_append(arena, &d.alive, true);
    Array_i8_append(arena, &d.alive, true);
    Array_i8_append(arena, &d.alive, true);
    Array_f32_append(arena, &d.damage, 25.f);
    Array_f32_append(arena, &d.damage, 25.f);
    Array_f32_append(arena, &d.damage, 25.f);

    // archetype array
    Array_voidptr typeerased = {0};
    Array_voidptr_append(arena, &typeerased, &a);
    Array_voidptr_append(arena, &typeerased, &b);
    Array_voidptr_append(arena, &typeerased, &c);
    Array_voidptr_append(arena, &typeerased, &d);

    for (i32 i = 0; i < typeerased.length; i++) {
        u8 signature = *((u8 *)(typeerased.data[i]));
        if (queryTest(signature, positions | velocities)) {
            printf("signature = %d\n", signature);
        }
    }

    // printf("%p\n", typeerased[0]);
    // printf("%p\n", typeerased[1]);
    // printf("%p\n", typeerased[2]);

    for (i32 i = 0; i < typeerased.length; i++) {
        ArchetypeHeader archetypeheader = *((ArchetypeHeader *)typeerased.data[i]);
        Map_u64 offsetmap = archetypeheader.offsetmap;
        // printf("offsetmap border = %llu, offsetmap length = %llu\n", offsetmap.border, offsetmap.length);
        if (queryTest(archetypeheader.signature, positions | velocities)) {
            Array_vec4s *positions = (Array_vec4s *)((u8 *)typeerased.data[i] + *Map_u64_get(arena, &offsetmap, "positions"));
            Array_vec4s *velocities = (Array_vec4s *)((u8 *)typeerased.data[i] + *Map_u64_get(arena, &offsetmap, "velocities"));
            for (i32 i = 0; i < positions->length; ++i) {
                positions->data[i] = glms_vec4_add(positions->data[i], velocities->data[i]);
                // positions->data[i].x += velocities->data[i].x;
                // positions->data[i].y += velocities->data[i].y;
                // positions->data[i].z += velocities->data[i].z;
                // positions->data[i].w += velocities->data[i].w;
            }
            //---------------------------------------------------------------------------------------------------
            // printf("%p = &a \n", &a);
            // printf("%p = &a.positions\n", &a.positions);
            // printf("%p = &a.velocities\n", &a.velocities);
            // printf("%llu\n", a.positions.length);
            // printf("%llu\n", a.velocities.length);
            // printf("%llu\n", positions->length);
            // printf("%llu\n", velocities->length);
            // printf("%p = &positions\n", positions);
        }
    }
    printf("{%f, %f, %f}\n", a.positions.data[0].x, a.positions.data[0].y, a.positions.data[0].z);
    printf("{%f, %f, %f}\n", a.positions.data[1].x, a.positions.data[1].y, a.positions.data[1].z);
    printf("{%f, %f, %f}\n", a.positions.data[2].x, a.positions.data[2].y, a.positions.data[2].z);
    printf("kayan end.\n");
}
