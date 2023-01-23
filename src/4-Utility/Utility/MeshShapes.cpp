#include "MeshShapes.hpp"

using namespace glm;
using namespace std;

std::shared_ptr<sre::Mesh> fg::utility::MeshShapes::GetBoxShape(float x_size, float y_size, float z_size) {
    // Modified version of Mesh::WithCube 
    // 
    //    v5----- v4
    //   /|      /|
    //  v1------v0|
    //  | |     | |
    //  | |v6---|-|v7
    //  |/      |/
    //  v2------v3
    vec3 p[] = {
        vec3{x_size, y_size, z_size},
        vec3{-x_size, y_size, z_size},
        vec3{-x_size, -y_size, z_size},
        vec3{x_size, -y_size, z_size},

        vec3{x_size, y_size, -z_size},
        vec3{-x_size, y_size, -z_size},
        vec3{-x_size, -y_size, -z_size},
        vec3{x_size, -y_size, -z_size}
    };

    vector<uint32_t> indices({
                            0,1,2, 0,2,3,
                            4,5,6, 4,6,7,
                            8,9,10, 8,10,11,
                            12,13,14, 12, 14,15,
                            16,17,18, 16,18,19,
                            20,21,22, 20,22,23
        });

    vector<vec3> positions({ p[0],p[1],p[2], p[3], // v0-v1-v2-v3
                             p[4],p[0],p[3], p[7], // v4-v0-v3-v7
                             p[5],p[4],p[7], p[6], // v5-v4-v7-v6
                             p[1],p[5],p[6], p[2], // v1-v5-v6-v2
                             p[4],p[5],p[1], p[0], // v1-v5-v6-v2
                             p[3],p[2],p[6], p[7], // v1-v5-v6-v2
        });

    vec4 u[] = {
        vec4(1,1,0,0),
        vec4(0,1,0,0),
        vec4(0,0,0,0),
        vec4(1,0,0,0)
    };

    vector<vec4> uvs({ u[0],u[1],u[2], u[3],
                   u[0],u[1],u[2], u[3],
                   u[0],u[1],u[2], u[3],
                   u[0],u[1],u[2], u[3],
                   u[0],u[1],u[2], u[3],
                   u[0],u[1],u[2], u[3],
        });

    vector<vec3> normals({
                             vec3{0, 0, 1},
                             vec3{0, 0, 1},
                             vec3{0, 0, 1},
                             vec3{0, 0, 1},
                             vec3{1, 0, 0},
                             vec3{1, 0, 0},
                             vec3{1, 0, 0},
                             vec3{1, 0, 0},
                             vec3{0, 0, -1},
                             vec3{0, 0, -1},
                             vec3{0, 0, -1},
                             vec3{0, 0, -1},
                             vec3{-1, 0, 0},
                             vec3{-1, 0, 0},
                             vec3{-1, 0, 0},
                             vec3{-1, 0, 0},
                             vec3{0, 1, 0},
                             vec3{0, 1, 0},
                             vec3{0, 1, 0},
                             vec3{0, 1, 0},
                             vec3{0, -1, 0},
                             vec3{0, -1, 0},
                             vec3{0, -1, 0},
                             vec3{0, -1, 0},
        });

    vector<vec4> tangents({
                             vec4{1, 0,  0,1},
                             vec4{1, 0,  0,1},
                             vec4{1, 0,  0,1},
                             vec4{1, 0,  0,1},
                             vec4{0, 0, -1,1},
                             vec4{0, 0, -1,1},
                             vec4{0, 0, -1,1},
                             vec4{0, 0, -1,1},
                             vec4{-1, 0, 0,1},
                             vec4{-1, 0, 0,1},
                             vec4{-1, 0, 0,1},
                             vec4{-1, 0, 0,1},
                             vec4{0, 0,  1,1},
                             vec4{0, 0,  1,1},
                             vec4{0, 0,  1,1},
                             vec4{0, 0,  1,1},
                             vec4{1, 0,  0,1},
                             vec4{1, 0,  0,1},
                             vec4{1, 0,  0,1},
                             vec4{1, 0,  0,1},
                             vec4{-1, 0, 0,1},
                             vec4{-1, 0, 0,1},
                             vec4{-1, 0, 0,1},
                             vec4{-1, 0, 0,1},
        });

	return 
		sre::Mesh::create()
		.withPositions(positions)
        .withNormals(normals)
        .withUVs(uvs)
        .withTangents(tangents)
        .withIndices(indices)
        .withMeshTopology(sre::MeshTopology::Triangles)
		.build();
}