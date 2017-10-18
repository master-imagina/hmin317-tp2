#version 410 core



layout (triangles, equal_spacing, cw) in;



uniform sampler2D texture;

uniform sampler2D cliffNormal;
uniform sampler2D sandNormal;
uniform sampler2D grassNormal;
uniform sampler2D rockNormal;

uniform sampler2D sandDisp;
uniform sampler2D grassDisp;
uniform sampler2D rockDisp;
uniform sampler2D cliffDisp;


uniform mat4 mvp_matrix;

in TCS_OUT
{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
    float snow;
}tes_in[];

out TES_OUT
{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
    float snow;
}tess_out;

// ----------------------------------------------------------------------------
float interpolate(float v0, float v1, float v2)
{
    return (gl_TessCoord.x) * v0 + (gl_TessCoord.y) * v1 + (gl_TessCoord.z) * v2;
}

// ----------------------------------------------------------------------------
vec2 interpolate(vec2 v0, vec2 v1, vec2 v2)
{
    return vec2(gl_TessCoord.x) * v0 + vec2(gl_TessCoord.y) * v1 + vec2(gl_TessCoord.z) * v2;
}

// ----------------------------------------------------------------------------
vec3 interpolate(vec3 v0, vec3 v1, vec3 v2)
{
    return vec3(gl_TessCoord.x) * v0 + vec3(gl_TessCoord.y) * v1 + vec3(gl_TessCoord.z) * v2;
}

// ----------------------------------------------------------------------------
void main(void){

        //Interpolate incoming data
        tess_out.height = interpolate(tes_in[0].height, tes_in[1].height, tes_in[2].height);
        tess_out.normal = interpolate(tes_in[0].normal, tes_in[1].normal, tes_in[2].normal);
        tess_out.eye_coord = interpolate(tes_in[0].eye_coord, tes_in[1].eye_coord, tes_in[2].eye_coord);
        tess_out.v_texcoord = interpolate(tes_in[0].v_texcoord, tes_in[1].v_texcoord, tes_in[2].v_texcoord);

        //Classic displacement Mapping
        float h=0;
        if(tess_out.height<0.05){
            h = texture2D(sandDisp,tess_out.v_texcoord*15.0).r*0.6;
        }else if(tess_out.height<0.2){
            h = mix(texture2D(sandDisp,tess_out.v_texcoord*15.0).r*0.6,texture2D(grassDisp,tess_out.v_texcoord*40.0).r*0.1,(tess_out.height - 0.05)/(0.15));
        }else if(tess_out.height<0.7){
            h = texture2D(grassDisp,tess_out.v_texcoord*40.0).r*0.1;
        }else if(tess_out.height<0.9){
            h = mix(texture2D(grassDisp,tess_out.v_texcoord*40.0).r*0.1,texture2D(rockDisp,tess_out.v_texcoord*40.0).r*0.5,(tess_out.height - 0.7)/(0.9-0.7));
        }else{
            h = texture2D(rockDisp,tess_out.v_texcoord*40.0).r*0.5;
        }


        float cliffAmout = 1.0f - tess_out.normal.y;
        float blendAmount;

        float cliffMin = 0.4;
        float cliffMax = 0.7;

        // Auto Cliff
        if(cliffAmout < cliffMin)
        {
           blendAmount = cliffAmout / cliffMin;
           h = mix(h, texture2D(cliffDisp,tess_out.v_texcoord*40.0).r*0.8, blendAmount);
        }

        if((cliffAmout < cliffMax) && (cliffAmout >=cliffMin))
        {
           blendAmount = (cliffAmout - cliffMin) * (1.0 / (cliffMax - cliffMin));
           h = mix(texture2D(cliffDisp,tess_out.v_texcoord*40.0).r*0.8, h, blendAmount);   
        }

        if(cliffAmout >= cliffMax)
        {
           h = texture2D(cliffDisp,tess_out.v_texcoord*40.0).r*0.8;
        }
        tess_out.snow = interpolate(tes_in[0].snow, tes_in[1].snow, tes_in[2].snow);
        h += tess_out.snow;

        vec3 p = interpolate(tes_in[0].world_coord, tes_in[1].world_coord, tes_in[2].world_coord);
        p += h * tess_out.normal;
        tess_out.world_coord = p;

        gl_Position = mvp_matrix * vec4(p,1.0);

        tess_out.lightDir = interpolate(tes_in[0].lightDir, tes_in[1].lightDir, tes_in[2].lightDir);
}
