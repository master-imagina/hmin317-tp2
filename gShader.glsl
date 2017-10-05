#version 430

layout(triangles) in;
layout(triangle_strip, max_vertices = 3) out;



in TES_OUT
{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
}geo_in[3];

out GEO_OUT
{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
}geo_out;



void main() {



    vec3 A = geo_in[2].world_coord - geo_in[0].world_coord;
    vec3 B = geo_in[1].world_coord - geo_in[0].world_coord;
    geo_out.normal = normalize(cross(A, B));


    geo_out.v_texcoord = geo_in[0].v_texcoord;
    geo_out.height = geo_in[0].height;
    gl_Position = gl_in[0].gl_Position; EmitVertex();

    geo_out.v_texcoord = geo_in[1].v_texcoord;
     geo_out.height = geo_in[1].height;
    gl_Position = gl_in[1].gl_Position; EmitVertex();

    geo_out.v_texcoord = geo_in[2].v_texcoord;
     geo_out.height = geo_in[2].height;
    gl_Position = gl_in[2].gl_Position; EmitVertex();

    EndPrimitive();


}
