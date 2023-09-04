#include "ecs/ecs_manager.h"

#include "ecs/component.h"
#include "ecs/system.h"

#include <errno.h>
#include <string.h>
#include <stdbool.h>
#include <assert.h>

struct ecs_manager_t {
    entity_manager_t* entity_manager;
    component_manager_t component_manager;
    system_manager_t system_manager;
} g_ecs_manager;

int32_t ecs_initialize(size_t max_number_of_entities, size_t max_number_of_components)
{
    entity_manager_alloc(&g_ecs_manager.entity_manager, max_number_of_entities);
    component_manager_initialize(&g_ecs_manager.component_manager);
    system_manager_initialize(&g_ecs_manager.system_manager);
    return 0;
}

int32_t ecs_uninitialize()
{
    system_manager_uninitialize(&g_ecs_manager.system_manager);
    component_manager_uninitialize(&g_ecs_manager.component_manager);
    entity_manager_free(g_ecs_manager.entity_manager);

    return 0;
}

entity_id_t ecs_get_free_entity()
{
    entity_id_t allocated_id = 0;
    assert(entity_get_next_free_id(g_ecs_manager.entity_manager, &allocated_id) == ENTITY_OK);

    return allocated_id;
}

int32_t ecs_remove_entity(const entity_id_t entity)
{
    int32_t ret_val = entity_free(g_ecs_manager.entity_manager, entity);
    if(ret_val == -1) {
        printf("Error while removing entity with id: %d, error code: %d\n", entity, errno);
        return ret_val;
    }

    for(size_t i = 0; i < g_ecs_manager.component_manager.next_free_component_index; ++i) {
        component_remove_from_entity(&g_ecs_manager.component_manager, g_ecs_manager.component_manager.registered_components[i].name, entity, NULL);
    }
    for(size_t i = 0; i < g_ecs_manager.system_manager.number_of_registered_systems; ++i) {
        system_manager_remove_entity(&g_ecs_manager.system_manager, entity);
    }

    return ret_val;
}

int32_t ecs_register_component(const char* name, const size_t size, const size_t number_of_elements)
{
    int32_t ret_val = component_register(&g_ecs_manager.component_manager, name, size, number_of_elements);
    if(ret_val == -1) {
        printf("Error while registering component, error code: %d\n", errno);
    }

    return ret_val;
}

int32_t ecs_add_component_to_entity(const char* name, const entity_id_t entity, void** data_ptr)
{
    signature_t component_signature = 0;
    int32_t ret_val = component_add_to_entity(&g_ecs_manager.component_manager, name, entity, data_ptr, &component_signature);
    if(ret_val == -1) {
        printf("Error while adding component to entity, error code: %d\n", errno);
        return ret_val;
    }

    entity_add_component_signature(g_ecs_manager.entity_manager, entity, component_signature);

    signature_t entity_signature = 0;
    entity_get_signature(g_ecs_manager.entity_manager, entity, &entity_signature);
    system_manager_extend_entity_signature(&g_ecs_manager.system_manager, entity, entity_signature);

    return 0;
}

int32_t ecs_remove_component_from_entity(const char* name, const entity_id_t entity)
{
    signature_t component_signature = 0;
    int32_t ret_val = component_remove_from_entity(&g_ecs_manager.component_manager, name, entity, &component_signature);
    if(ret_val == -1) {
        printf("Error while removing component from entity, error code: %d\n", ret_val);
        return ret_val;
    }

    entity_remove_component_signature(g_ecs_manager.entity_manager, entity, component_signature);

    signature_t entity_signature = 0;
    entity_get_signature(g_ecs_manager.entity_manager, entity, &entity_signature);
    system_manager_reduce_entity_signature(&g_ecs_manager.system_manager, entity, entity_signature);

    return 0;
}

int32_t ecs_register_system(const char* name, void(*update_func)(double), const char** components, const size_t number_of_components)
{
    signature_t signature = 0;
    // SHOULD BE OPTIMIZED
    component_manager_t* component_manager = &g_ecs_manager.component_manager;
    for(size_t i = 0; i < number_of_components; ++i) {
        int32_t component_index = -1;
        for(size_t j = 0; j < component_manager->next_free_component_index; ++j) {
            if(strcmp(component_manager->registered_components[j].name, components[i]) == 0) {
                component_index = j;
                break;
            }
        }

        if(component_index == -1) {
            errno = COMPONENT_NOT_FOUND;
            return -1;
        }

        signature |= component_manager->registered_components[component_index].signature;
    }

    system_register(&g_ecs_manager.system_manager, name, update_func, signature);
}