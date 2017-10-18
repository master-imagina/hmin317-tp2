#version 330 core

in vec2 textureCoord;

uniform sampler2D particlesTex;
uniform sampler2D extraTex;
uniform sampler2D heightMap;
uniform vec3 gravityVector;
uniform float speed;
uniform vec3 windVector;
uniform float randomParameter;
uniform float particlesCount;
uniform float particlesFactor;

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

    if(particule.g <= height+(0.1/4.0) || extraData.g==1.0){
        if(extraData.g != 0)
            extraData.r += 0.01;
        //random position
        outParticule = vec3( uv.x + uv.x/particlesCount - random(uv)/particlesCount,1.0, uv.y + uv.y/particlesCount - random(uv)/particlesCount);
        extraData.g = (random(uv)+(1-particlesFactor)>0.5)?1.0:0.0;
    }else{
        extraData.r -= 0.0002;
        //outParticule.w = particule.w + 0.0001;

        //outParticule.rgb = particule.rgb ;// * speed + windVector;
        outParticule.r +=  max(sin(outParticule.g*40)/(400.0)-random(uv),0.0);
        if(extraData.g==0.0)
            outParticule.g -= (random(uv)+0.4)/150.0;
    }

    FragColor = outParticule;
    extraMap = extraData;
}
