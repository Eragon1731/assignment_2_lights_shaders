#version 330 core

// This is a sample fragment shader.
out vec4 color;

/* Tutorial*/
uniform mat4 model;

uniform struct Light {
    bool on;
    vec3 position;
    vec3 intensities; //a.k.a the color of the light
} light;

/*
uniform struct Materials {
    vec3 amb;
    vec3 diff;
    vec3 spec;
    float shiny;
} materials;
 */

in vec3 fragNormal;
in vec3 fragVert;

in vec3 Normal;
in vec3 FragPos; 

void main()
{
        //calculate normal in world coordinates
        mat3 normalMatrix = transpose(inverse(mat3(model)));
        vec3 normal = normalize(normalMatrix * fragNormal);
        
        //calculate the location of this fragment (pixel) in world coordinates
        vec3 fragPosition = vec3(model * vec4(fragVert, 1));
        
        //calculate the vector from this pixels surface to the light source
        vec3 surfaceToLight = light.position - fragPosition;
        
        //calculate the cosine of the angle of incidence
        float brightness = dot(normal, surfaceToLight) / (length(surfaceToLight) * length(normal));
        brightness = clamp(brightness, 0, 1);
        
        //calculate final color of the pixel, based on:
        // 1. The angle of incidence: brightness
        // 2. The color/intensities of the light: light.intensities
        // 3. The texture and texture coord: texture(tex, fragTexCoord)
        color = vec4(brightness * light.intensities);
}


