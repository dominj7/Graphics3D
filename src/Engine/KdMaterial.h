#pragma once

#include "AbstractMaterial.h"  
#include <glm/glm.hpp>

namespace xe {
    class KdMaterial : public AbstractMaterial<KdMaterial> {
    public:
        // Constructor
        KdMaterial(const glm::vec4 &Kd, int use_vertex_colors=false) : Kd_(Kd), use_vertex_colors_(use_vertex_colors) {};

    
        void bind() const override;
        void unbind() const override;
        static void init();


    private:
        const glm::vec4 Kd_;  // Diffuse reflection color
        static int material_uniform_buffer_;  // Static variable for material buffer
        int use_vertex_colors_;
    };

}