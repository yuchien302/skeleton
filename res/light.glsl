struct directional
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 direction;
};

void shade_directional(directional light, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, vec3 vertex, vec3 normal, float shininess)
{
    vec3 eye_direction = -vertex;
    float eye_distance = length(eye_direction);
    eye_direction /= eye_distance;
    
    vec3 half_vector = normalize(light.direction + eye_direction);
    
    float normal_dot_light_direction = max(0.0, dot(normal, light.direction));
    float normal_dot_half_vector = max(0.0, dot(normal, half_vector));
    
    float power_factor = 0.0;
    if (normal_dot_light_direction > 0.0)
        power_factor = pow(normal_dot_half_vector, shininess);
    
    ambient += light.ambient;
    diffuse += light.diffuse*normal_dot_light_direction;
    specular += light.specular*power_factor;
}

struct point
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 attenuation;
    
    vec3 position;
};

void shade_point(point light, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, vec3 vertex, vec3 normal, float shininess)
{    
    vec3 toLight = light.position - vertex;
	float d = length(toLight);
	toLight = toLight/d;
	vec3 toEye = normalize(-vertex);
	normal = normalize(normal);
	float diff_cosine = max(0.0, dot(normal, toLight));
	float spec_base = max(0.0, dot(normal, normalize(toLight+toEye)));
	float spec_pf;

	if(diff_cosine == 0.0)
		spec_pf = 0.0;
	else
		spec_pf = pow(spec_base, shininess);

	float decay = 1.0 / (light.attenuation[0] + light.attenuation[1]*d + light.attenuation[2]*d*d);

	ambient += light.ambient * decay;
	diffuse += light.diffuse * diff_cosine * decay;
	specular += light.specular * spec_pf * decay;
}

struct spot
{
    vec3 ambient;
    vec3 diffuse;
    vec3 specular;
    
    vec3 attenuation;
    float cutoff;
    float exponent;
    
    vec3 position;
    vec3 direction;
};

void shade_spot(spot light, inout vec3 ambient, inout vec3 diffuse, inout vec3 specular, vec3 vertex, vec3 normal, float shininess)
{
    vec3 light_direction = light.position - vertex;
    float light_distance = length(light_direction);
    light_direction /= light_distance;
    
    vec3 eye_direction = -vertex;
    float eye_distance = length(eye_direction);
    eye_direction /= eye_distance;
    
    float att = 1.0/(light.attenuation[0] + light.attenuation[1]*light_distance + light.attenuation[2]*light_distance*light_distance);
    float spotdot = dot(-light_direction, light.direction);
    
    float spotatt = 0.0;
    if (spotdot >= light.cutoff)
        spotatt = pow(spotdot, light.exponent);
    
    att *= spotatt;
    
    vec3 half_vector = normalize(light_direction + normalize(-vertex));
    float normal_dot_light_direction = max(0.0, dot(normal, light_direction));
    float normal_dot_half_vector = max(0.0, dot(normal, half_vector));
    
    float power_factor = 0.0;
    if (normal_dot_light_direction > 0.0)
        power_factor = pow(normal_dot_half_vector, shininess);
    
    ambient += light.ambient*att;
    diffuse += light.diffuse*normal_dot_light_direction*att;
    specular += light.specular*power_factor*att;
}

uniform directional dlights[4];
uniform int num_dlights;

uniform point plights[4];
uniform int num_plights;

uniform spot slights[4];
uniform int num_slights;

vec3 lighting(vec3 emission, vec3 ambient, vec3 diffuse, vec3 specular, float shininess, vec3 vertex, vec3 normal)
{
    vec3 light_ambient = vec3(0.0, 0.0, 0.0);
    vec3 light_diffuse = vec3(0.0, 0.0, 0.0);
    vec3 light_specular = vec3(0.0, 0.0, 0.0);
    
    for (int j = 0; j < num_dlights; j++)
        shade_directional(dlights[j], light_ambient, light_diffuse, light_specular, vertex, normal, shininess);
    
    for (int j = 0; j < num_plights; j++)
        shade_point(plights[j], light_ambient, light_diffuse, light_specular, vertex, normal, shininess);
    
    for (int j = 0; j < num_slights; j++)
        shade_spot(slights[j], light_ambient, light_diffuse, light_specular, vertex, normal, shininess);
    
    return clamp(emission + ambient*light_ambient + diffuse*light_diffuse + specular*light_specular, 0.0, 1.0);
}