#version 140
   
flat in vec3 normalInterp;
flat in vec3 vertPos;

flat out vec4 outputColor;


const vec3 lightPos = vec3(2.0,1.0,1.0);
const vec3 ambientColor = vec3(0.0, 0.1, 0.1);
const vec3 diffuseColor = vec3(0.0, 0.6, 0.6);
const vec3 specColor = vec3(1.0, 1.0, 1.0);

void main() {
    vec3 normal = normalize(normalInterp);
    vec3 lightDir = normalize(lightPos - vertPos);
    vec3 reflectDir = reflect(-lightDir, normal);
    vec3 viewDir = normalize(-vertPos);

    float lambertian = max(dot(lightDir,normal), 0.0);
    float specular = 0.0;

    if(lambertian > 0.0) {
       float specAngle = max(dot(reflectDir, viewDir), 0.0);
       specular = pow(specAngle, 4.0);
    }
    outputColor = vec4(ambientColor +
                      lambertian*diffuseColor +
                      specular*specColor, 1.0);

}
