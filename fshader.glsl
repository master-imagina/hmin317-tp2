#version 410 core


uniform sampler2D texture;
uniform sampler2D grass;
uniform sampler2D sand;
uniform sampler2D rock;
uniform sampler2D cliff;
uniform sampler2D cliffNormal;
uniform sampler2D sandNormal;
uniform sampler2D grassNormal;
uniform sampler2D rockNormal;
uniform sampler2D snowMap;
uniform sampler2D particlesMap;

uniform vec3 ambientColor;
uniform int calendar;

in TES_OUT{
    vec2 v_texcoord;
    vec3 normal;
    vec3 lightDir;
    float height;
    vec3 eye_coord;
    vec3 world_coord;
    float snow;
}fs_in;

out vec4 FragColor;

const vec4 fog_color = vec4(0.2, 0.55, 1.0, 1.0);
const float distanceFog = 50;
const float heightFog = 200;
// ----------------------------------------------------------------------------
//Classic height Fog using extinction and inscattering equation : color * e^(distance*FactorExtinction) + fog_color * (1.0 - e^(distance * FactorInscattering)
vec4 fog(vec4 c)
{
    float z = length(fs_in.eye_coord)/distanceFog;

    float de = 0.025 * smoothstep(0.0, 6.0, heightFog-10.0 - fs_in.world_coord.y);
    float di = 0.045 * (smoothstep(0.0, 12.0, heightFog - fs_in.world_coord.y));

    float extinction   = clamp(exp(-z * de),0,1);
    float inscattering = clamp(exp(-z * di),0,1);

    return c * extinction + fog_color * (1.0 - inscattering);
}

// ----------------------------------------------------------------------------
void main()
{
    // Set fragment color from texture

    vec3 normalColor = normalize(texture2D(sandNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0);




    float diff = max(dot(fs_in.normal, fs_in.lightDir), 0.2) ;

    vec4 albedo = vec4(1.0);

    //Texturing base on heigh
    if(fs_in.height<0.05){
        albedo = texture2D(sand,fs_in.v_texcoord*15.0);
        normalColor = normalize(texture2D(sandNormal,fs_in.v_texcoord*15).rgb * 2.0 - 1.0);
    }else if(fs_in.height<0.2){
        albedo = mix(texture2D(sand,fs_in.v_texcoord*15.0),texture2D(grass,fs_in.v_texcoord*40.0),(fs_in.height - 0.05)/(0.15));
        normalColor = mix(normalize(texture2D(sandNormal,fs_in.v_texcoord*15).rgb * 2.0 - 1.0),normalize(texture2D(grassNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0),(fs_in.height - 0.05)/(0.15));
    }else if(fs_in.height<0.7){
        albedo = texture2D(grass,fs_in.v_texcoord*40.0);
        normalColor = normalize(texture2D(grassNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0);
    }else if(fs_in.height<0.9){
        albedo = mix(texture2D(grass,fs_in.v_texcoord*40.0),texture2D(rock,fs_in.v_texcoord*40.0),(fs_in.height - 0.8));
        normalColor = mix(normalize(texture2D(grassNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0),normalize(texture2D(rockNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0),(fs_in.height - 0.7)/(0.9-0.7));
    }else{
        albedo = texture2D(rock,fs_in.v_texcoord*40.0);
        normalColor = normalize(texture2D(rockNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0);
    }


    float cliffAmout = 1.0f - fs_in.normal.y;
    float blendAmount;

    float cliffMin = 0.4;
    float cliffMax = 0.7;

    // Auto Cliff
    if(cliffAmout < cliffMin)
    {
       blendAmount = cliffAmout / cliffMin;
       albedo = mix(albedo, texture2D(cliff,fs_in.v_texcoord*40.0), blendAmount);
       normalColor = mix(normalColor,normalize(texture2D(cliffNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0),blendAmount);
    }

    if((cliffAmout < cliffMax) && (cliffAmout >=cliffMin))
    {
       blendAmount = (cliffAmout - cliffMin) * (1.0 / (cliffMax - cliffMin));
       albedo = mix(texture2D(cliff,fs_in.v_texcoord*40.0), albedo, blendAmount);
       normalColor = normalize(texture2D(cliffNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0);
       normalColor = mix(normalize(texture2D(cliffNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0),normalColor,blendAmount);
    }

    if(cliffAmout >= cliffMax)
    {
       blendAmount=1.0;
       albedo = texture2D(cliff,fs_in.v_texcoord*40.0);
       normalColor = normalize(texture2D(cliffNormal,fs_in.v_texcoord*40).rgb * 2.0 - 1.0);
    }
    float snowFactor=0;
    if(calendar < 60){
        snowFactor = calendar/40.0;
    }else if (calendar <120){
        snowFactor =(120.0-calendar)/40.0;
    }
    snowFactor *= fs_in.height+0.2;


    float orientation = max(dot(fs_in.normal, vec3(0.0,1.0,0.0)),0.0);
    snowFactor *= orientation;
    snowFactor = fs_in.snow;
    if( snowFactor>0.0 ){
        albedo = mix(albedo,vec4(1.,1.,1.,1.0),snowFactor*2.0);

        normalColor = mix(normalColor,vec3(0,-1,0),min(snowFactor*2.0,1.0));
    }
    float diffnormalMap = max(abs(dot(normalColor, fs_in.lightDir)), 0.0);

    float gamma = 1.2;
    vec3 color = albedo.rgb * diff * diffnormalMap + ambientColor/6.0;

    color = pow(color, vec3(1.0/gamma));


    FragColor = fog(vec4(color,1.0));



}


