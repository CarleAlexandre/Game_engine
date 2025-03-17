#include <haven/haven_type.h>
#include <cglm/cglm.h>
#include <math.h>

typedef struct haven_flowfield_node_s {
    vec3 flow_direction;
    struct haven_flowfield_node_s *children[8];
    bool is_leaf;
} haven_flowfield_node_t;

typedef struct haven_flowfield_s {
    haven_flowfield_node_t *root;
    float min_cell_size;
    vec3 bounds;
    vec3 origin;
} haven_flowfield_t;

void haven_flowfield_init(haven_flowfield_t *field, vec3 origin, vec3 bounds, float min_cell_size) {
    field->root = calloc(1, sizeof(haven_flowfield_node_t));
    field->root->is_leaf = true;
    glm_vec3_copy(origin, field->origin);
    glm_vec3_copy(bounds, field->bounds);
    field->min_cell_size = min_cell_size;
}

void haven_flowfield_node_free(haven_flowfield_node_t *node) {
    if (!node->is_leaf) {
        for (int i = 0; i < 8; i++) {
            if (node->children[i]) {
                haven_flowfield_node_free(node->children[i]);
            }
        }
    }
    free(node);
}

void haven_flowfield_free(haven_flowfield_t *field) {
    if (field->root) {
        haven_flowfield_node_free(field->root);
    }
}

static void haven_flowfield_subdivide(haven_flowfield_node_t *node) {
    node->is_leaf = false;
    for (int i = 0; i < 8; i++) {
        node->children[i] = calloc(1, sizeof(haven_flowfield_node_t));
        node->children[i]->is_leaf = true;
    }
}

void haven_flowfield_set_flow(haven_flowfield_t *field, vec3 position, vec3 direction, float cell_size) {
    haven_flowfield_node_t *current = field->root;
    vec3 current_pos = {0};
    vec3 current_size = {0};
    glm_vec3_copy(field->bounds, current_size);
    glm_vec3_copy(field->origin, current_pos);
    
    while (cell_size >= field->min_cell_size) {
        if (current->is_leaf) {
            haven_flowfield_subdivide(current);
        }
        
        int octant = 0;
        for (int i = 0; i < 3; i++) {
            float mid = current_pos[i] + current_size[i] * 0.5f;
            if (position[i] >= mid) {
                octant |= (1 << i);
                current_pos[i] = mid;
            }
        }
        
        for (int i = 0; i < 3; i++) {
            current_size[i] *= 0.5f;
        }
        
        current = current->children[octant];
    }
    
    glm_vec3_normalize_to(direction, current->flow_direction);
}

void haven_flowfield_get_flow(haven_flowfield_t *field, vec3 position, vec3 result) {
    haven_flowfield_node_t *current = field->root;
    vec3 current_pos = {0};
    vec3 current_size = {0};
    glm_vec3_copy(field->bounds, current_size);
    glm_vec3_copy(field->origin, current_pos);
    
    while (!current->is_leaf) {
        int octant = 0;
        for (int i = 0; i < 3; i++) {
            float mid = current_pos[i] + current_size[i] * 0.5f;
            if (position[i] >= mid) {
                octant |= (1 << i);
                current_pos[i] = mid;
            }
        }
        
        for (int i = 0; i < 3; i++) {
            current_size[i] *= 0.5f;
        }
        
        current = current->children[octant];
    }
    
    glm_vec3_copy(current->flow_direction, result);
}
