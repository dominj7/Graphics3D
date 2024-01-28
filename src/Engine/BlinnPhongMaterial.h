#pragma once

#include "AbstractMaterial.h"  
#include <glm/glm.hpp>
#include "ObjectReader/sMesh.h"
#include "texture.h"
#include "mesh_loader.h"
#include "utils.h"

namespace xe {
    class BlinnPhongMaterial : public AbstractMaterial<BlinnPhongMaterial> {
    public:
        // Constructor
        BlinnPhongMaterial(const glm::vec4 &Kd, int use_vertex_colors=false) : Kd_(Kd), use_vertex_colors_(use_vertex_colors), Textura_(0) {};
        
        BlinnPhongMaterial(const glm::vec4 &Kd, int use_vertex_colors, GLuint& Textura) : Kd_(Kd), use_vertex_colors_(use_vertex_colors), Textura_(Textura) {};

        static glm::vec4 colorAmbient;
        static GLint map_Kd_location_;
        void bind() const override;
        void unbind() const override;
        static void init();
        static Material *create_from_mtl(const mtl_material_t &mat, std::string mtl_dir);
        void set_texture(GLuint textura ){Textura_ = textura;};
    

    private:
    
        const glm::vec4 Kd_; 
        int use_vertex_colors_;
        GLuint Textura_ ;
    };

}