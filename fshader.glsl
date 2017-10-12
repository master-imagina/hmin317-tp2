#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform sampler2D sand;
uniform sampler2D rock;

varying vec2 v_texcoord;
varying float h;

//! [0]
void main()
{
    float rockH = 0.3f;
    float sandH = 0.20f;
    // Set fragment color from texture
    if(h > rockH) {
        gl_FragColor = texture2D(rock, v_texcoord * 5);
    } else if (h > sandH && h < rockH ){
        gl_FragColor = mix(texture2D(sand, v_texcoord * 5),texture2D(rock, v_texcoord * 5), (h - sandH) / (rockH - sandH));
    } else {
        gl_FragColor = texture2D(sand, v_texcoord * 5);
    }
}
//! [0]

