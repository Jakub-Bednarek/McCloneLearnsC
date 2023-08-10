#include "ecs_manager.h"

#include "component.h"
#include "system.h"

#include <errno.h>
#include <string.h>
#include <stdbool.h>

struct ECSManager {
    EntityManager entity_manager;
    ComponentManager component_manager;
    SystemManager system_manager;
} g_ecs_manager;

int32_t ecs_initialize()
{
    entity_manager_initialize(&g_ecs_manager.entity_manager);
    component_manager_initialize(&g_ecs_manager.component_manager);
    system_manager_initialize(&g_ecs_manager.system_manager);
    return 0;
}

int32_t ecs_uninitialize()
{
    system_manager_uninitialize(&g_ecs_manager.system_manager);
    component_manager_uninitialize(&g_ecs_manager.component_manager);
    entity_manager_uninitialize(&g_ecs_manager.entity_manager);
    return 0;
}

EntityId ecs_get_free_entity()
{
    int32_t ret_val = entity_get_next_free_id(&g_ecs_manager.entity_manager);
    if( ret_val == -1) {
        printf("Error while getting free entity, error code: %d\n", errno);
    }

    return ret_val;
}

int32_t ecs_remove_entity(const EntityId entity)
{
    int32_t ret_val = entity_free(&g_ecs_manager.entity_manager, entity);
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

int32_t ecs_add_component_to_entity(const char* name, const EntityId entity, void** data_ptr)
{
    Signature component_signature = 0;
    int32_t ret_val = component_add_to_entity(&g_ecs_manager.component_manager, name, entity, data_ptr, &component_signature);
    if(ret_val == -1) {
        printf("Error while adding component to entity, error code: %d\n", errno);
        return ret_val;
    }

    entity_add_to_signature(&g_ecs_manager.entity_manager, entity, component_signature);

    Signature entity_signature = 0;
    entity_get_signature(&g_ecs_manager.entity_manager, entity, &entity_signature);
    system_manager_extend_entity_signature(&g_ecs_manager.system_manager, entity, entity_signature);

    return 0;
}

int32_t ecs_remove_component_from_entity(const char* name, const EntityId entity)
{
    Signature component_signature = 0;
    int32_t ret_val = component_remove_from_entity(&g_ecs_manager.component_manager, name, entity, &component_signature);
    if(ret_val == -1) {
        printf("Error while removing component from entity, error code: %d\n", ret_val);
        return ret_val;
    }

    entity_remove_from_signature(&g_ecs_manager.entity_manager, entity, component_signature);

    Signature entity_signature = 0;
    entity_get_signature(&g_ecs_manager.entity_manager, entity, &entity_signature);
    system_manager_reduce_entity_signature(&g_ecs_manager.system_manager, entity, entity_signature);

    return 0;
}

int32_t ecs_register_system(const char* name, void(*update_func)(double), const char** components, const size_t number_of_components)
{
    Signature signature = 0;
    // SHOULD BE OPTIMIZED
    ComponentManager* component_manager = &g_ecs_manager.component_manager;
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