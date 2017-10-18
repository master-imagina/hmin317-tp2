#version 410 core


uniform mat4 mvp_matrix;


uniform sampler2D texture;
uniform float quantityVertices;
uniform float timer;
uniform sampler2D snowMap;
uniform sampler2D particlesMap;


in vec4 a_position;
in vec2 a_texcoord;
out VS_OUT{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;

    vec3 eye_coord;
    vec3 world_coord;
    float snow;
}vs_out;


//! [0]
void main()
{
    const float heighMultiply = 40.0;
    vec3 omniLightPos = vec3(-2000,2000,-2000.0);
    vs_out.height = texture2D(texture, a_texcoord).r;
    // Calculate vertex position in screen space
    vec4 computePosition = vec4(a_position.x,vs_out.height*heighMultiply,a_position.z,1.0);
    vec4 worldPosition = computePosition;
    vec4 Peye = mvp_matrix * a_position;
    gl_Position = Peye;

    //compute Normal
    ivec3 off = ivec3(-1, 1, 0);
    float left,right,up,down;

    if((a_texcoord-vec2(1.0/quantityVertices, 0.0)).x>=0.0)
        left = texture2D(texture, a_texcoord-vec2(1.0/quantityVertices, 0.0)).r*heighMultiply;
    else
        left = texture2D(texture, a_texcoord).r*heighMultiply;

    if((a_texcoord+vec2(1.0/quantityVertices, 0.0)).x<1.0)
        right = texture2D(texture, a_texcoord+vec2(1.0/quantityVertices, 0.0)).r*heighMultiply;
    else
        right = texture2D(texture, a_texcoord).r*heighMultiply;

    if((a_texcoord+vec2(0.0, 1.0/quantityVertices)).y<1.0)
        up = texture2D(texture, a_texcoord+vec2(0.0, 1.0/quantityVertices)).r*heighMultiply;
    else
        up = texture2D(texture, a_texcoord).r*heighMultiply;

    if((a_texcoord-vec2(0.0, 1.0/quantityVertices)).y>=0.0)
        down = texture2D(texture, a_texcoord-vec2(0.0, 1.0/quantityVertices)).r*heighMultiply;
    else
       down = texture2D(texture, a_texcoord).r*heighMultiply;

    // deduce terrain normal
    vs_out.normal.x = left - right;
    vs_out.normal.y = 2.0;
    vs_out.normal.z = down - up;
    vs_out.normal = normalize(vs_out.normal);

    vs_out.eye_coord = Peye.xyz;
    vs_out.world_coord = worldPosition.xyz;
    vs_out.lightDir = normalize(omniLightPos - worldPosition.xyz);
    vs_out.snow = 0.0;


    vs_out.snow = texture2D(snowMap,a_texcoord).r;


    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    vs_out.v_texcoord = a_texcoord;
}
//! [0]
