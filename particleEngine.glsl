#version 330 core

in vec2 textureCoord;

uniform sampler2D particlesTex;
uniform sampler2D extraTex;
uniform sampler2D heightMap;
uniform vec3 gravityVector;
uniform float speed;
uniform vec3 windVector;
uniform float randomParameter;

uniform vec2 resolution;

layout(location = 0) out vec3 FragColor;
layout(location = 1) out vec3 extraMap;

float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123*randomParameter);
}


void main(){

    vec2 uv = gl_FragCoord.xy/resolution.xy;


    vec3 particule = texture2D(particlesTex,textureCoord).rgb;
    vec3 extraData = texture2D(extraTex,textureCoord).rgb;
    float height = texture2D(heightMap,vec2(particule.x,particule.z)).r;

    vec3 outParticule = particule;

    if(particule.g <= height+(0.1/4.0) ){
        extraData.r += 0.01;
        //random position
        outParticule = vec3(random(uv*20),1.0,random(uv));
        extraData.g = random(uv);
    }else{
        extraData.r -= 0.2;
        //outParticule.w = particule.w + 0.0001;

        //outParticule.rgb = particule.rgb ;// * speed + windVector;
        outParticule.g -= 0.01;
    }

    FragColor = outParticule;
    extraMap = extraData;
}
