#version 330

// Input
in vec3 world_position;
in vec3 world_normal;

// Uniforms for light properties
uniform vec3 light_direction;
uniform vec3 light_position;
uniform vec3 second_light_position;
uniform vec3 eye_position;

uniform float material_kd;
uniform float material_ks;
uniform int material_shininess;

uniform vec3 object_color;
uniform int type_of_light;
uniform float cut_off_angle;

// Output
layout(location = 0) out vec4 out_color;

void main()
{
    vec3 N = normalize(world_normal);
    vec3 V = normalize(eye_position - world_position);

    vec3 L1 = normalize(light_position - world_position);
    vec3 H1 = normalize(L1 + V);

    float ambient_light1 = 0.25;
    float diffuse_light1 = material_kd * max(dot(N, L1), 0.0);
    float specular_light1 = 0.0;

    if (diffuse_light1 > 0.0)
    {
        specular_light1 = material_ks * pow(max(dot(N, H1), 0.0), material_shininess);
    }

    float light1 = 0.0;

    if (type_of_light == 1)
    {
        float cut_off_rad = radians(cut_off_angle);
        float spot_light = dot(-L1, light_direction);
        float spot_light_limit = cos(cut_off_rad);

        if (spot_light > spot_light_limit)
        {
            float linear_att = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
            float light_att_factor = pow(linear_att, 2.0);

            float d	= distance(light_position, world_position);
	    float attenuation_factor = 1 / (d * d);

            light1 = ambient_light1 + attenuation_factor * light_att_factor * (diffuse_light1 + specular_light1);
        }
        else
        {
            light1 = ambient_light1;
        }
    }
    else
    {
        float d	= distance(light_position, world_position);
	    float attenuation_factor = 1 / (d * d);

        light1 = ambient_light1 + attenuation_factor * (diffuse_light1 + specular_light1);
    }


    vec3 L2 = normalize(second_light_position - world_position);
    vec3 H2 = normalize(L2 + V);

    float ambient_light2 = 0.25;
    float diffuse_light2 = material_kd * max(dot(N, L2), 0.0);
    float specular_light2 = 0.0;

    if (diffuse_light2 > 0.0)
    {
        specular_light2 = material_ks * pow(max(dot(N, H2), 0.0), material_shininess);
    }
    //float d1 = distance(second_light_position, world_position);

	//float attenuation_factor2 = 1 / (d1 * d1);
    //float light2 = ambient_light2 + attenuation_factor2 * (diffuse_light2 + specular_light2);
    float light2 = 0.0;

    if (type_of_light == 1)
    {
        float cut_off_rad = radians(cut_off_angle);
        float spot_light = dot(-L2, light_direction);
        float spot_light_limit = cos(cut_off_rad);

        if (spot_light > spot_light_limit)
        {
            float linear_att = (spot_light - spot_light_limit) / (1.0 - spot_light_limit);
            float light_att_factor = pow(linear_att, 2.0);

            float d	= distance(second_light_position, world_position);
	    float attenuation_factor = 1 / (d * d);

            light2 = ambient_light2 + attenuation_factor * light_att_factor * (diffuse_light2 + specular_light2);
        }
        else
        {
            light2 = ambient_light2;
        }
    }
    else
    {
        float d	= distance(second_light_position, world_position);
	    float attenuation_factor = 1 / (d * d);

        light2 = ambient_light2 + attenuation_factor * (diffuse_light2 + specular_light2);
    }


    //float total_light = light1 + light2;
    float total_light = light1 + light2 - ambient_light1;

    vec3 colour = object_color * total_light;

    out_color = vec4(colour, 1.0);
}
