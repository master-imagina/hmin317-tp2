#ifdef GL_ES
// Set default precision to medium
precision mediump int;
precision mediump float;
#endif

uniform float minHeight;            // Terrain minimum height
uniform float maxHeight;            // Terrain maximum height

varying float vertexHeight;

void main()
{
    float a = 255. / maxHeight;
    float b = (255. * minHeight) / (maxHeight - minHeight);

    float grayscaleValue = floor(a * vertexHeight + b) / 255.;

    gl_FragColor = vec4(grayscaleValue, grayscaleValue, grayscaleValue, 1.0);
}
