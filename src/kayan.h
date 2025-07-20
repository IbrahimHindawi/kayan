#pragma once
#include "Array_f32.h"
#include "Array_i8.h"
#include "Array_vec2s.h"
#include "Array_vec4s.h"
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

bool kayan_query(u64 a, u64 b) {
    return (a & b) == b;
}

u64 kayan_entity_add(u64 *entity_data) {
    // add bounds checking?
    u64 old = *entity_data;
    *entity_data += 1;
    return old;
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
    ArchetypeHeader archetype_header;
    Array_vec4s positions;
    Array_vec4s velocities;
    Array_vec4s torque;
};

typedef struct B B;
struct B {
    ArchetypeHeader archetype_header;
    Array_vec4s positions;
    Array_vec4s velocities;
};

typedef struct C C;
struct C {
    ArchetypeHeader archetype_header;
    Array_vec4s positions;
};

typedef struct D D;
struct D {
    ArchetypeHeader archetype_header;
    Array_vec4s positions;
    Array_vec4s velocities;
    Array_vec2s health; // {health, maxhealth}
    Array_f32 damage;
    Array_i8 alive;
};

void kayan_main(Arena *arena) {
    printf("kayan begin.\n");

    u64 entity_id = 0;
    A a = {};
    entity_id = a.archetype_header.entity_count;
    a.archetype_header.signature = positions | velocities | torque;
    a.archetype_header.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &a.archetype_header.offsetmap, "positions", offsetof(A, positions));
    Map_u64_set(arena, &a.archetype_header.offsetmap, "velocities", offsetof(A, velocities));
    Map_u64_set(arena, &a.archetype_header.offsetmap, "torque", offsetof(A, torque));
    a.positions = Array_vec4s_reserve(arena, max_entity_count);
    a.velocities = Array_vec4s_reserve(arena, max_entity_count);
    a.torque = Array_vec4s_reserve(arena, max_entity_count);
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &a.positions, (vec4s){0.f});
    Array_vec4s_append(arena, &a.velocities, (vec4s){0.f});
    Array_vec4s_append(arena, &a.torque, (vec4s){0.f});
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &a.positions, (vec4s){0.f, 1.f, 0.f});
    Array_vec4s_append(arena, &a.velocities, (vec4s){0.f, 1.f, 0.f});
    Array_vec4s_append(arena, &a.torque, (vec4s){0.f, .1f, 0.f});
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &a.positions, (vec4s){0.f, 1.f, 0.f});
    Array_vec4s_append(arena, &a.velocities, (vec4s){0.f, 1.f, 0.f});
    Array_vec4s_append(arena, &a.torque, (vec4s){0.f, .1f, 0.f});

    B b = {};
    entity_id = a.archetype_header.entity_count;
    b.archetype_header.signature = positions | velocities;
    b.archetype_header.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &b.archetype_header.offsetmap, "positions", offsetof(B, positions));
    Map_u64_set(arena, &b.archetype_header.offsetmap, "velocities", offsetof(B, velocities));
    b.positions = Array_vec4s_reserve(arena, max_entity_count);
    b.velocities = Array_vec4s_reserve(arena, max_entity_count);
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &b.positions, (vec4s){0.f, 1.f, 0.f});
    Array_vec4s_append(arena, &b.velocities, (vec4s){0.f, 1.f, 0.f});
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &b.positions, (vec4s){0.f, 1.f, 0.f});
    Array_vec4s_append(arena, &b.velocities, (vec4s){0.f, 1.f, 0.f});

    C c = {};
    c.archetype_header.signature = positions;
    c.archetype_header.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &c.archetype_header.offsetmap, "positions", offsetof(C, positions));
    c.positions = Array_vec4s_reserve(arena, max_entity_count);

    D d = {};
    d.archetype_header.signature = health | damage | alive;
    d.archetype_header.offsetmap = *Map_u64_create(arena);
    Map_u64_set(arena, &d.archetype_header.offsetmap, "health", offsetof(D, health));
    Map_u64_set(arena, &d.archetype_header.offsetmap, "damage", offsetof(D, damage));
    Map_u64_set(arena, &d.archetype_header.offsetmap, "alive", offsetof(D, alive));
    Map_u64_set(arena, &d.archetype_header.offsetmap, "positions", offsetof(D, positions));
    Map_u64_set(arena, &d.archetype_header.offsetmap, "velocities", offsetof(D, velocities));
    d.positions = Array_vec4s_reserve(arena, max_entity_count); 
    d.velocities = Array_vec4s_reserve(arena, max_entity_count); 
    d.health = Array_vec2s_reserve(arena, max_entity_count); 
    d.damage = Array_f32_reserve(arena, max_entity_count); 
    d.alive = Array_i8_reserve(arena, max_entity_count); 
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &d.positions, (vec4s){0.f});
    Array_vec4s_append(arena, &d.velocities, (vec4s){0.f});
    Array_vec2s_append(arena, &d.health, (vec2s){100.f, 100.f});
    Array_i8_append(arena, &d.alive, true);
    Array_f32_append(arena, &d.damage, 25.f);
    entity_id = kayan_entity_add(&a.archetype_header.entity_count);
    Array_vec4s_append(arena, &d.positions, (vec4s){1.f});
    Array_vec4s_append(arena, &d.velocities, (vec4s){1.f});
    Array_vec2s_append(arena, &d.health, (vec2s){100.f, 100.f});
    Array_i8_append(arena, &d.alive, true);
    Array_f32_append(arena, &d.damage, 25.f);

    // archetype array
    Array_voidptr type_erased = {0};
    Array_voidptr_append(arena, &type_erased, &a);
    Array_voidptr_append(arena, &type_erased, &b);
    Array_voidptr_append(arena, &type_erased, &c);
    Array_voidptr_append(arena, &type_erased, &d);

    for (i32 i = 0; i < type_erased.length; i++) {
        ArchetypeHeader archetype_header = *((ArchetypeHeader *)type_erased.data[i]);
        if (kayan_query(archetype_header.signature, positions | velocities)) {
            printf("signature = %llu\n", archetype_header.signature);
        }
    }

    for (i32 i = 0; i < type_erased.length; i++) {
        ArchetypeHeader archetype_header = *((ArchetypeHeader *)type_erased.data[i]);
        Map_u64 offsetmap = archetype_header.offsetmap;
        if (kayan_query(archetype_header.signature, positions | velocities)) {
            Array_vec4s *positions = (Array_vec4s *)((u8 *)type_erased.data[i] + *Map_u64_get(arena, &offsetmap, "positions"));
            Array_vec4s *velocities = (Array_vec4s *)((u8 *)type_erased.data[i] + *Map_u64_get(arena, &offsetmap, "velocities"));
            for (i32 i = 0; i < positions->length; ++i) {
                positions->data[i] = glms_vec4_add(positions->data[i], velocities->data[i]);
            }
        }
    }
    printf("kayan end.\n");
}
