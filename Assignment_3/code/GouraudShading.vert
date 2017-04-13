#version 140

// created by Thorsten Thormaehlen for educational purpose
in vec3 inputPosition;
in vec2 inputTexCoord;
in vec3 inputNormal;

uniform mat4 projection, modelview, normalMat;
uniform int mode;

out vec4 forFragColor;

const vec3 lightPos = vec3(2.0, 1.0, 1.0);
const vec3 ambientColor = vec3(0.0, 0.1, 0.1);
const vec3 diffuseColor = vec3(0.0, 0.6, 0.6);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main(){
    gl_Position = projection * modelview * vec4(inputPosition, 1.0);

    // all following gemetric computations are performed in the
    // camera coordinate system (aka eye coordinates)
    vec3 normal = vec3(normalMat * vec4(inputNormal, 0.0));
    vec4 vertPos4 = modelview * vec4(inputPosition, 1.0);
    vec3 vertPos = vec3(vertPos4) / vertPos4.w;
    vec3 lightDir = normalize(lightPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-vertPos);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
       float specAngle = max(dot(reflectDir, viewDir), 0.0);
       specular = pow(specAngle, 4.0);
    }

    forFragColor = vec4(ambientColor+lambertian*diffuseColor + specular*specColor, 1.0);

}
