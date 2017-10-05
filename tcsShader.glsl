#version 410 core

layout (vertices = 3) out;

in VS_OUT{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
}tcs_in[];

out TCS_OUT
{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
}tcs_out[];



uniform mat4 mvp_matrix;

// ----------------------------------------------------------------------------
void main(void){
        if(gl_InvocationID == 0){ //No need to calculate tessellation level for every invocation
                vec4 p0 = mvp_matrix * vec4(tcs_in[0].world_coord,1.0);
                vec4 p1 = mvp_matrix * vec4(tcs_in[1].world_coord,1.0);
                vec4 p2 = mvp_matrix * vec4(tcs_in[2].world_coord,1.0);

                p0 /= p0.w;
                p1 /= p1.w;
                p2 /= p2.w;



                if(p0.z <= 0.0 || p1.z <= 0.0 || p2.z <= 0.0){
                        gl_TessLevelOuter[0] = 0.0;
                        gl_TessLevelOuter[1] = 0.0;
                        gl_TessLevelOuter[2] = 0.0;
                }else
                {
                        float t = 8.0;
                        float l0 = length(p1.xy - p2.xy) * t + 1.0; //Get size of edge to deduct level of tesselation
                        float l1 = length(p2.xy - p0.xy) * t + 1.0;
                        float l2 = length(p0.xy - p1.xy) * t + 1.0;
                        if(l0>=3.0)
                            l0 = t;
                        else if(l0>=2.0)
                            l0 = t/2.0;
                        if(l1>=3.0)
                            l1 = t;
                        else if(l1>=2.0)
                            l1 = t/2.0;
                        if(l2>=3.0)
                            l2 = t;
                        else if(l2>=2.0)
                            l2 = t/2.0;


                        gl_TessLevelOuter[0] = l0;
                        gl_TessLevelOuter[1] = l1;
                        gl_TessLevelOuter[2] = l2;
                        //gl_TessLevelOuter[3] = l3;

                        gl_TessLevelInner[0] = min(min(l0,l1),l2);

                }
        }

        //Pass different attribute to next stage
        tcs_out[gl_InvocationID].v_texcoord = tcs_in[gl_InvocationID].v_texcoord;
        tcs_out[gl_InvocationID].normal = tcs_in[gl_InvocationID].normal;
        tcs_out[gl_InvocationID].lightDir = tcs_in[gl_InvocationID].lightDir;
        tcs_out[gl_InvocationID].height = tcs_in[gl_InvocationID].height;
        tcs_out[gl_InvocationID].eye_coord = tcs_in[gl_InvocationID].eye_coord;
        tcs_out[gl_InvocationID].world_coord = tcs_in[gl_InvocationID].world_coord;
}
