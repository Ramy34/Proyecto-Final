#version 330 core
out vec4 FragColor;

struct Light {
    vec3 position;  
    vec3 direction;
    float cutOff;
    float outerCutOff;
  
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
	
    float constant;
    float linear;
    float quadratic;
};

in vec3 FragPos;  
in vec3 Normal;  
in vec2 TexCoords;
  
uniform vec3 viewPos;
uniform sampler2D material_diffuse;
uniform sampler2D material_specular;
uniform float material_shininess;
uniform Light light;

void main()
{
    vec3 lightDir = normalize(light.position - FragPos);
    
    // check if lighting is inside the spotlight cone
    float theta = dot(lightDir, normalize(-light.direction)); 
    
    if(theta > light.cutOff) // remember that we're working with angles as cosines instead of degrees so a '>' is used.
    {    
        // ambient
        vec3 ambient = light.ambient * texture(material_diffuse, TexCoords).rgb;
        
        // diffuse 
        vec3 norm = normalize(Normal);
        float diff = max(dot(norm, lightDir), 0.0);
        vec3 diffuse = light.diffuse * diff * texture(material_diffuse, TexCoords).rgb;  
        
        // specular
        vec3 viewDir = normalize(viewPos - FragPos);
        vec3 reflectDir = reflect(-lightDir, norm);  
        float spec = pow(max(dot(viewDir, reflectDir), 0.0), material_shininess);
        vec3 specular = light.specular * spec * texture(material_specular, TexCoords).rgb;  
        
        // attenuation
        float distance    = length(light.position - FragPos);
        float attenuation = 1.0 / (light.constant + light.linear * distance + light.quadratic * (distance * distance));    

        // ambient  *= attenuation; // remove attenuation from ambient, as otherwise at large distances the light would be darker inside than outside the spotlight due the ambient term in the else branche
        diffuse   *= attenuation;
        specular *= attenuation;   
            
        //vec3 result = ambient + diffuse + specular;
        //FragColor = vec4(result, 1.0);
        vec4 result = vec4(ambient + diffuse + specular,texture(material_diffuse, TexCoords).a) ;
        if(result.a < 0.1)
            discard;
        FragColor = result;
    }
    else 
    {
        // else, use ambient light so scene isn't completely dark outside the spotlight.
        //FragColor = vec4(light.ambient * texture(material_diffuse, TexCoords).rgb, 1.0);
        vec4 result = vec4(light.ambient * texture(material_diffuse, TexCoords).rgb, texture(material_diffuse, TexCoords).a);
        if(result.a < 0.1)
            discard;
        FragColor = result;
    }
} 