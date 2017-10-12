#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform mat4 mvp_matrix;
uniform sampler2D texture;

attribute vec4 a_position;
attribute vec2 a_texcoord;

varying vec2 v_texcoord;

//! [0]
void main()
{
    vec4 a_height = vec4(a_position.x, a_position.y, texture2D(texture, a_texcoord).r, 1.0f);
    // Calculate vertex position in screen space
    gl_Position = mvp_matrix * a_height;

    // Pass texture coordinate to fragment shader
    // Value will be automatically interpolated to fragments inside polygon faces
    v_texcoord = a_texcoord;
}
//! [0]
