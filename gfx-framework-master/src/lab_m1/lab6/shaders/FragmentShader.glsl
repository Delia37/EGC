#version 330

// Input
// TODO(student): Get values from vertex shader
in vec3 frag_color;
in vec3 frag_normal;
// Output
layout(location = 0) out vec4 out_color;


void main()
{
    // TODO(student): Write pixel out color
    //out_color = vec4(1);
    vec3 color = 0.5 * (normalize(frag_normal) + 1.0);
    //out_color = vec4(color, 1.0);
    out_color = vec4(frag_color, 1.0);


}
