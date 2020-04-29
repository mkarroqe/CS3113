
uniform sampler2D diffuse;
varying vec2 texCoordVar;

void main() {
    vec4 color = texture2D(diffuse, texCoordVar);
    
//    // invert
//    gl_FragColor = vec4(1.0 - color.r, 1.0 - color.g, 1.0 - color.b, color.a);
//
//    // greyscale
//    float c = (color.r + color.g + color.b) / 3.0;
//    gl_FragColor = vec4(c, c, c, color.a);
    
    // luminosity (more realistic greyscale)
    vec3 luminance = vec3(dot(vec3(0.2126, 0.7152, 0.0722), color.rbg));
//    gl_FragColor = vec4(luminance, color.a);
    
    // (de)saturation: 2 --> sat, 0.5 --> grey
    vec3 m = mix(luminance, color.rgb, 0.5);
    gl_FragColor = vec4(m, color.a);
    
}
