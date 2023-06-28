#include "system.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>

struct SystemManager {
    System registered_systems[MAX_NUMBER_OF_SYSTEMS];
    size_t number_of_registered_systems;
} g_system_manager;

void system_manager_on_update(const double delta_time)
{
    for(size_t i = 0; i < g_system_manager.number_of_registered_systems; ++i) {
        g_system_manager.registered_systems[i].on_update(delta_time);
    }
}

int32_t system_add_entity(System* system, const EntityId entity)
{
    for (size_t i = 0; i < system->number_of_registered_entities; ++i) {
        if(system->registered_entities[i] == entity) {
            errno = ENTITY_ALREADY_REGISTERED_IN_SYSTEM;
            return -1;
        }
    }

    system->registered_entities[system->number_of_registered_entities++] = entity;
}

void system_remove_entity(System* system, const EntityId entity)
{
    int32_t found_index = -1;
    for(size_t i = 0; i < system->number_of_registered_entities; ++i) {
        if(system->registered_entities[i] == entity) {
            found_index = i;
            break;
        }
    }

    if(found_index == -1) {
        return;
    }

    if(found_index == system->number_of_registered_entities - 1) {
        system->registered_entities[--system->number_of_registered_entities] = -1;
    }
    else {
        system->registered_entities[found_index] = system->registered_entities[--system->number_of_registered_entities];
    }
}

int32_t system_manager_init()
{
    g_system_manager.number_of_registered_systems = 0;
}

int32_t system_manager_add_entity(const EntityId entity, const unsigned long entity_signature)
{
    for(size_t i = 0; i < g_system_manager.number_of_registered_systems; ++i) {
        if((g_system_manager.registered_systems[i].signature & entity_signature) > 0) {
            system_add_entity(&g_system_manager.registered_systems[i], entity);
        }
    }

    return 0;
}

int32_t system_manager_remove_entity(const EntityId entity)
{
    for(size_t i = 0; i < g_system_manager.number_of_registered_systems; i++) {
        system_remove_entity(&g_system_manager.registered_systems[i], entity);
    }

    return 0;
}

int32_t system_register(const char* name, void(*update_callback)(), const unsigned long system_signature)
{
    for(size_t i = 0; i < g_system_manager.number_of_registered_systems; ++i) {
        if(strcmp(g_system_manager.registered_systems[i].name, name) == 0) {
            errno = SYSTEM_ALREADY_REGISTERED;
            return -1;
        }
    }

    System system_to_register = {.name = name, .on_update = update_callback, .number_of_registered_entities = 0, .signature = system_signature };
    for(size_t i = 0; i < MAX_ENTITIES_IN_SYSTEM; i++) {
        system_to_register.registered_entities[i] = -1;
    }
    g_system_manager.registered_systems[g_system_manager.number_of_registered_systems++] = system_to_register;

    return 0;
}

// void update_sys(double delta_time) {
//     printf("DT: %f\n", delta_time);
//     for(size_t i = 0; i < 10; ++i) {
//         printf("%ld\n", i);
//     }
// }

// void move_sys_up(double delta_time) {
//     printf("move sys 1.0 + %f\n", delta_time);
// }

// void test_systems()
// {
//     system_manager_init();
//     if(system_register("First system", update_sys, 0b001) == 0) {
//         printf("SUCCESS\n");
//     }
//     if(system_register("mov", move_sys_up, 0b010) == 0) {
//         printf("SUCCESS\n");
//     }
//     // if(system_register("Move system", move_sys_up, 1001) == 0) {
//     //     printf("SUCCESS2\n");
//     // }
//     // if(system_register("Move system", move_sys_up, 10) == 0) {
//     //     printf("SUCCESS3\n");
//     // }
//     // else {
//     //     printf("Faile3\n");
//     // }
//     // system_manager_on_update(0.3);

//     printf("----------------------------------\n");

//     system_manager_add_entity(15, 0b10010);
//     system_manager_add_entity(13, 0b1);
//     system_manager_add_entity(17, 0b1001);
//     system_manager_add_entity(19, 0b1111);

//     for(size_t i = 0; i < 2; ++i){
//         printf("Name: %s\nArray: [",g_system_manager.registered_systems[i].name);
//         for(size_t j = 0; j < g_system_manager.registered_systems[i].number_of_registered_entities; ++j) {
//             printf("%d, ", g_system_manager.registered_systems[i].registered_entities[j]);
//         }
//         printf("]\n");
//     }
//     system_manager_remove_entity(19);
//     system_manager_remove_entity(13);
//     system_manager_remove_entity(17);
//     system_manager_remove_entity(15);
// }