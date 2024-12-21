#version 330

// Input
// TODO(student): Get vertex attributes from each location
//layout(location = 0) in vec3 v_position;
//layout(location = 1) in vec3 v_normal;
//layout(location = 2) in vec2 v_texCoord;
//layout(location = 3) in vec3 v_color;

//schimb 1 3 
layout(location = 0) in vec3 v_position;
layout(location = 1) in vec3 v_color;
layout(location = 2) in vec2 v_texCoord;
layout(location = 3) in vec3 v_normal;

// Uniform properties
uniform mat4 Model;
uniform mat4 View;
uniform mat4 Projection;
uniform float u_time;

// Output
// TODO(student): Output values to fragment shader
out vec3 frag_color;
out vec3 frag_normal;
out vec2 frag_texCoord;


void main()
{
    // TODO(student): Send output to fragment shader
    //gl_Position = Projection * View * Model * vec4(inPosition, 1.0);

    // TODO(student): Compute gl_Position
    //fragColor = inColor;
    float offset = sin(u_time) * 0.5;
    vec3 animated_position = v_position + vec3(offset, 0.0, 0.0);


    frag_color = v_color * vec3(0.5 + 0.5 * sin(u_time), 0.5 + 0.5 * cos(u_time), 1.0);

    //frag_color = v_color;
    frag_normal = v_normal;
    frag_texCoord = v_texCoord;

    // Calculate position in clip space
    //gl_Position = Projection * View * Model * vec4(v_position, 1.0);
    gl_Position = Projection * View * Model * vec4(animated_position, 1.0);

}
