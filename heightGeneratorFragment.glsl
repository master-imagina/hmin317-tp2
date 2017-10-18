#version 330 core
out vec4 FragColor;
in vec2 textureCoord;

uniform float r1;
uniform float r2;
uniform float r3;
uniform float r4;

uniform vec2 resolution = vec2(8192,8192);//Oversampling resolution to "smooth"

// ----------------------------------------------------------------------------
float random (in vec2 st) {
    return fract(sin(dot(st.xy,
                         vec2(12.9898,78.233)))*
        43758.5453123*r1);
}

// ----------------------------------------------------------------------------
vec3 noised( in vec2 x )
{
    vec2 f = fract(x);
    vec2 u = f*f*(3.0-2.0*f);


    vec2 p = floor(x);
    float a = random( (p+vec2(0.5,0.5))/256.0);
    float b = random( (p+vec2(1.5,0.5))/256.0);
    float c = random( (p+vec2(0.5,1.5))/256.0);
    float d = random( (p+vec2(1.5,1.5))/256.0);


        return vec3(a+(b-a)*u.x+(c-a)*u.y+(a-b-c+d)*u.x*u.y,
                                6.0*f*(1.0-f)*(vec2(b-a,c-a)+(a-b-c+d)*u.yx));//tridimensionnel interpolation
}

// ----------------------------------------------------------------------------
#define OCTAVES 20
//Fractional Brownian Motion Derived
float fbm_derived( in vec2 point)
{
    mat2 randomMatrix = mat2(r1,r2,r3,r4);
    float value = -0.5;
    float amplitude = 1.0;
    vec2  d = vec2(0.0);
    for( int i=0; i<OCTAVES; i++ )
    {
        vec3 noiseVector = noised(point);
        d += noiseVector.yz; // increase derivation
        value += amplitude*noiseVector.x/(1.0+dot(d,d)); // compute value using derivation
        amplitude *= 0.4;
        point = randomMatrix*point*5.0;
    }
    return value;
}

// ----------------------------------------------------------------------------
void main() 
{
    vec2 uv = gl_FragCoord.xy/resolution.xy;
    uv.x *= resolution.x/resolution.y;

    vec3 color = vec3(0.0);
    color += fbm_derived(uv*20.0);

    FragColor = vec4(color,1.0);
}
