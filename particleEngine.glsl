#version 330

in vec2 textureCoord;

uniform sampler2D particlesTex;
uniform sampler2D heightMap;
uniform vec3 gravityVector;
uniform float speed;
uniform vec3 windVector;

uniform vec2 resolution;

out vec4 FragColor;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123);
}


void main(){

    vec2 uv = gl_FragCoord.xy/resolution.xy;
    uv.x *= resolution.x/resolution.y;

    vec4 particule = texture2D(particlesTex,textureCoord);

    vec4 outParticule;

    if(particule.w == 0){
        //random position
        outParticule.rb = random(uv);
        outParticule.g = 1.0;
        outParticule.w = 1.0;
    }else{

        outParticule.w = particule.w - 0.1;

        outParticule.rgb = particule.rgb * gravityVector * speed + windVector;
    }

    FragColor = vec4(outParticule);
}
