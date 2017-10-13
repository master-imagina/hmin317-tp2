#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D texture;
uniform bool enableTexture;
uniform vec4 color;

varying vec2 v_texcoord;
//! [0]
void main()
{
    // Set fragment color from texture
    if(enableTexture){
        if(color == vec4(1,1,1,1))
            gl_FragColor = texture2D(texture, v_texcoord);
        else
            gl_FragColor = texture2D(texture, v_texcoord) + color;
    }
    else{
        gl_FragColor = color;
    }
}
//! [0]

