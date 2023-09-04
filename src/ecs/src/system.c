#include "ecs/system.h"

#include <string.h>
#include <errno.h>
#include <stdio.h>
#include <stdbool.h>

int32_t system_entity_find(System* system, const entity_id_t entity)
{
    for(size_t i = 0; i < system->number_of_registered_entities; ++i) {
        if(system->registered_entities[i] == entity) {
            return i;
        }
    }

    return -1;
}

int32_t system_add_entity(System* system, const entity_id_t entity)
{
    if(system_entity_find(system, entity) != -1) {
        errno = ENTITY_ALREADY_REGISTERED_IN_SYSTEM;
        return -1;
    }

    system->registered_entities[system->number_of_registered_entities++] = entity;
}

int32_t system_remove_entity(System* system, const entity_id_t entity)
{
    int32_t found_index = system_entity_find(system, entity);

    if(found_index == -1) {
        errno = ENTITY_NOT_REGISTERED_IN_SYSTEM;
        return -1;
    }

    if(found_index == system->number_of_registered_entities - 1) {
        system->registered_entities[--system->number_of_registered_entities] = -1;
    }
    else {
        system->registered_entities[found_index] = system->registered_entities[--system->number_of_registered_entities];
    }

    return 0;
}

int32_t system_manager_initialize(system_manager_t* system_manager)
{
    printf("System manager initialize\n");
    system_manager->number_of_registered_systems = 0;
}

int32_t system_manager_uninitialize(system_manager_t* system_manager)
{
    printf("System manager uninitialize\n");
    system_manager->number_of_registered_systems = 0;
}

int32_t system_manager_add_entity(system_manager_t* system_manager, const entity_id_t entity, const signature_t signature)
{
    for(size_t i = 0; i < system_manager->number_of_registered_systems; ++i) {
        if((system_manager->registered_systems[i].signature & signature) > 0) {
            system_add_entity(&system_manager->registered_systems[i], entity);
        }
    }

    return 0;
}

int32_t system_manager_extend_entity_signature(system_manager_t* system_manager, const entity_id_t entity, const signature_t signature)
{
    System* system = NULL;
    bool entity_found = false;

    for(size_t i = 0; i < system_manager->number_of_registered_systems; ++i) {
        system = &system_manager->registered_systems[i];
        if(system->signature & signature == system->signature) {
            system_add_entity(system, entity);
        }
    }

    return 0;
}

int32_t system_manager_reduce_entity_signature(system_manager_t* system_manager, const entity_id_t entity, const signature_t signature)
{
    System* system = NULL;
    bool entity_removed = false;
    for(size_t i = 0; i < system_manager->number_of_registered_systems; ++i) {
        system = &system_manager->registered_systems[i];
        if(system->signature & signature != system->signature) {
            if(system_remove_entity(system, entity) != -1) {
                entity_removed = true;
            }
        }
    }

    return (int32_t)(entity_removed);
}

int32_t system_manager_remove_entity(system_manager_t* system_manager, const entity_id_t entity)
{
    for(size_t i = 0; i < system_manager->number_of_registered_systems; ++i) {
        system_remove_entity(&system_manager->registered_systems[i], entity);
    }

    return 0;
}

int32_t system_register(system_manager_t* system_manager, const char* name, void(*update_callback)(), const signature_t system_signature)
{
    for(size_t i = 0; i < system_manager->number_of_registered_systems; ++i) {
        if(strcmp(system_manager->registered_systems[i].name, name) == 0) {
            errno = SYSTEM_ALREADY_REGISTERED;
            return -1;
        }
    }

    System system_to_register = {.name = name, .on_update = update_callback, .number_of_registered_entities = 0, .signature = system_signature };
    for(size_t i = 0; i < MAX_ENTITIES_IN_SYSTEM; i++) {
        system_to_register.registered_entities[i] = -1;
    }
    system_manager->registered_systems[system_manager->number_of_registered_systems++] = system_to_register;

    return 0;
}

void system_manager_on_update(system_manager_t* system_manager, const double delta_time)
{
    for(size_t i = 0; i < system_manager->number_of_registered_systems; ++i) {
        system_manager->registered_systems[i].on_update(delta_time);
    }
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
//         printf("Name: %s\nArray: [",system_manager->registered_systems[i].name);
//         for(size_t j = 0; j < system_manager->registered_systems[i].number_of_registered_entities; ++j) {
//             printf("%d, ", system_manager->registered_systems[i].registered_entities[j]);
//         }
//         printf("]\n");
//     }
//     system_manager_remove_entity(19);
//     system_manager_remove_entity(13);
//     system_manager_remove_entity(17);
//     system_manager_remove_entity(15);
// }